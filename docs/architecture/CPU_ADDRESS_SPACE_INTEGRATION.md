# CPU / GuestAddressSpace integration design

This document is **development infrastructure**. It records an approved integration
design for the current interpreter baseline; it is not an accepted ADR and does not
claim Xbox hardware behavior.

Status: `proposed design, approved for implementation by XB-NEXT-DEVELOPMENT-02`.
`ADR-0012` remains open (`docs/DECISIONS.md` ends at ADR-0011).

## 1. Goal and boundary

`documented` — The owner requested that the existing x64 interpreter be able to use
`GuestAddressSpace` for instruction fetch, data reads and data writes while keeping
`GuestMemory` as a compatibility layer. The two strands of
`XB-NEXT-DEVELOPMENT-02` are independent: game-file analysis findings do not change
this CPU/memory contract automatically.

`documented` — Constraints: Xbox Series X is the only target; C++20; interpreter-first;
no JIT; no new dependency; no proprietary firmware, keys, assets or DRM work; no
unsupported compatibility or performance claim. Existing `GuestMemory`, `TestCpu`,
`ExecutionPolicy`, interpreter signatures/statuses, presets, ADRs and `LICENSE` are
preserved.

`observed` — `GuestAddressSpace` already exposes `fetch`, `read` and `write` with
`GuestVirtualAddress` and structured `MemoryResult` values
(`include/xbridge/memory/guest_address_space.hpp:51-53`). The interpreter currently
accepts `memory::GuestMemory&` only
(`include/xbridge/cpu/x64_instruction_interpreter.hpp:23-41`).

## 2. Proposed public seam

`hypothesis` — Add one header-only file
`include/xbridge/memory/guest_virtual_memory.hpp` containing:

```cpp
namespace xbridge::memory {

class IGuestVirtualMemory {
public:
    virtual ~IGuestVirtualMemory() = default;
    [[nodiscard]] virtual MemoryResult fetch(
        GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept = 0;
    [[nodiscard]] virtual MemoryResult read(
        GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept = 0;
    [[nodiscard]] virtual MemoryResult write(
        GuestVirtualAddress address, std::span<const std::uint8_t> source) noexcept = 0;
};

class GuestAddressSpaceView final : public IGuestVirtualMemory { /* wraps a reference */ };
class GuestMemoryView final : public IGuestVirtualMemory { /* wraps a reference */ };

} // namespace xbridge::memory
```

`hypothesis` — Use composition instead of changing the `GuestAddressSpace` class
layout: `GuestAddressSpaceView` holds `GuestAddressSpace&` and forwards to its three
existing methods. `GuestMemoryView` holds `GuestMemory&`, checks the documented 32-bit
range contract, and translates `std::out_of_range` / `std::length_error` into a
`MemoryResult` fault. Neither adapter owns or copies guest memory.

`hypothesis` — Add distinct interpreter entry points; do not overload existing names:

```cpp
struct X64ExecutionResult {
    X64ExecuteStatus status{X64ExecuteStatus::Ok};
    std::optional<memory::MemoryResult> memory_result{};
    friend constexpr bool operator==(const X64ExecutionResult&, X64ExecuteStatus) noexcept;
};

X64ExecutionResult execute_instruction_via(
    const DecodedInstruction&, X64CpuState&, memory::IGuestVirtualMemory&) noexcept;
X64ExecutionResult step_x64_instruction_via(
    std::span<const std::uint8_t>, X64CpuState&, memory::IGuestVirtualMemory&) noexcept;
X64ExecutionResult step_x64_at_rip_via(
    X64CpuState&, memory::IGuestVirtualMemory&) noexcept;
```

`observed` — Distinct `_via` names avoid overload ambiguity. The current functions
remain in place and become thin wrappers that construct `GuestMemoryView`, call the
new path and return only `status`. Every existing comparison against
`X64ExecuteStatus::Ok` or `MemoryAccessError` therefore keeps its current meaning.
No new `X64ExecuteStatus` enumerator is introduced.

`hypothesis` — `memory_result` is present when an interface call fails or reports a
short completion; it is empty for successful or non-memory instructions. This preserves
the original `MemoryFault` without changing the old status type.

## 3. Access behavior

### Instruction fetch

1. Use `state.rip` directly as `GuestVirtualAddress{state.rip}`; no CPU-side 64→32
   narrowing in the new path.
2. Bound the window to at most 15 bytes, matching the current local fetch limit
   (`src/cpu/x64_instruction_interpreter.cpp:165-188`).
3. Fill a local buffer; decode only after a complete successful result.
4. Retry with decreasing lengths only for `MemoryFault::Unmapped` or
   `MemoryFault::InvalidRange`, so a mapping shorter than 15 bytes can still execute a
   short instruction. All other faults return immediately. At most 15 attempts occur.
5. `AccessDenied`, `AddressOverflow`, `DeviceAccessRequired`, `ResourceExhausted`,
   `Alignment` and `Unsupported` are never masked by retry.
6. Fetch from a Device region is already rejected before dispatch
   (`src/memory/guest_address_space.cpp:218-222`), so retries have no device side
   effect.

`observed` — `GuestAddressSpace::fetch` checks length 1..15, overflow, mapping
containment, mapping execute permission, region coverage/Reserved, region execute
permission, then Device rejection and physical read
(`src/memory/guest_address_space.cpp:153-227`). Execute is checked independently at
both layers, and data reads do not inherit execute.

### Data reads and writes

1. Derive the 64-bit effective address from the addressing GPR and use length 4 or 8
   for the supported MOV memory forms.
2. Call `IGuestVirtualMemory::read` / `write` once with a local little-endian buffer.
3. Commit the destination GPR and advance RIP only after complete success; on write,
   advance RIP only after the interface reports success.
4. Treat any short completion as failure with no register or guest-memory commit.

`observed` — Mapping/region permission checks, overflow checks and physical translation
are already implemented in `GuestAddressSpace::read`/`write`
(`src/memory/guest_address_space.cpp:229-279`). `GuestPhysicalMemory` validates and
commits before copying bytes, and dispatch uses temporary buffers
(`src/memory/guest_physical_memory.cpp:140-220`,
`src/memory/guest_address_space.cpp:281-349`).

### Fault mapping

`hypothesis` — All interface failures map to the existing
`X64ExecuteStatus::MemoryAccessError`. No new status enumerator is added. The original
`MemoryResult` remains available through `X64ExecutionResult::memory_result` for
callers that need the precise `MemoryFault`, address and access context.

`observed` — Current RIP wrap behavior is preserved in both paths; introducing a new
RIP-overflow fault would change documented existing semantics and is out of scope.

## 4. Compatibility decisions

- `GuestMemory`, `TestCpu` and `ExecutionPolicy` are not edited.
- Existing interpreter signatures and return types are not edited.
- No new CMake source is required if the interface and adapters stay header-only;
  `CMakeLists.txt` is not part of this implementation package.
- CPU-to-device dispatch is not authorized in this package. Tests use Normal regions
  or leave the dispatcher unbound; device side effects are not assumed rollbackable.
- The `GuestMemoryView` path must reproduce the legacy 32-bit guard and bounded fetch
  behavior so old call sites keep byte-for-byte equivalent results.

## 5. Required test matrix

| ID | Case | Expected result |
|---|---|---|
| T1 | Execute-only mapping and region above 4 GiB; NOP, MOV imm32, NOP through `step_x64_at_rip_via`. | `Ok`, RIP `+1/+5/+1`, register set, no data-read permission needed. |
| T2 | RW mapping/region above 4 GiB; MOV store/load through `execute_instruction_via`. | Correct little-endian bytes and register, RIP advanced once per success. |
| T3 | Execute permission missing at mapping layer and separately at region layer. | `MemoryAccessError`, `AccessDenied`, no state change. |
| T4 | Execute but no read/write permission; CPU data access. | `MemoryAccessError`, `AccessDenied`, no partial write. |
| T5 | `UINT64_MAX`, unmapped gap, spanning mappings, address above 32-bit. | `MemoryAccessError`; diagnostics `AddressOverflow`/`Unmapped`; no truncation or mutation. |
| T6 | Mapping of length 1, 4, 14 and 15 containing a NOP. | Bounded retry succeeds or fails with the exact fault; never more than 15 bytes. |
| T7 | Test-local fake `IGuestVirtualMemory` returning every `MemoryFault`. | All map to `MemoryAccessError`; original `MemoryResult` preserved; no CPU mutation. |
| T8 | Legacy parity through old functions and new `_via` + `GuestMemoryView`. | Identical status, RIP, GPRs, RFLAGS and bytes for success, out-of-bounds and >32-bit cases. |
| T9 | All existing decoder, interpreter, TestCpu and memory tests. | Preserved unchanged; single CTest target passes. |

## 6. Risks and residual items

`hypothesis` — Adapter fidelity is the main risk; T8 is mandatory. Retry masking is
mitigated by restricting retries to `Unmapped`/`InvalidRange`. Device side effects are
excluded by scope.

`documented` — No additional owner decision is required for this design. Any later
change to an existing signature, ADR-0012 acceptance, RIP-overflow semantics or CPU
device dispatch requires a separate owner decision.
