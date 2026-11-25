# Guest Address Space and Memory-System Design

**Status:** proposed architecture baseline; not yet implemented or hardware-validated<br>
**Target:** Xbox Series X guest model<br>
**Language:** C++20<br>
**Scope:** memory contracts shared by CPU, GPU, and storage/I/O components<br>
**Evidence note:** all interface signatures below are a proposal. They describe a target contract, not current product behavior.

## 1. Purpose

The current `GuestMemory` is a useful bounded test fixture, but it is not an adequate long-term memory architecture. It represents one contiguous byte vector addressed by 32-bit offsets. The x64 interpreter currently depends directly on that type and narrows guest addresses to 32 bits (`observed`: `include/xbridge/memory/memory.hpp:8-20`, `src/cpu/x64_instruction_interpreter.cpp:10-26,161-188`).

This document defines the next architectural direction without claiming undocumented Xbox memory-management details as facts.

The implementation must distinguish:

1. guest virtual addresses;
2. guest physical addresses;
3. host virtual pointers and host allocations.

These address domains must not be implicitly interchangeable.

`documented` — the current `GuestMemory` contract remains generic and unchanged in this package (`docs/ARCHITECTURE.md:30-38`, `docs/DECISIONS.md:38-41`). This document does not make it an Xbox virtual-memory model.

## 2. Evidence and confidence

| Item | Status | Design consequence |
|---|---|---|
| Xbox Series X has 16 GB GDDR6 unified memory with documented bandwidth partitions | documented at platform-overview level | Model a fixed guest memory budget and bandwidth classes as configuration/data, not as host RAM equivalence |
| Exact guest virtual address layout and page-table behavior | unknown to this project | Keep translation policy replaceable; do not invent a console page-table format |
| Exact physical allocation, reservation, aliasing and residency semantics | unknown to this project | Implement generic region and mapping primitives first |
| Host machine memory and GPU memory differ from guest memory | architectural requirement | Guest capacity must not be silently reduced to host available RAM |
| CPU, GPU and I/O need controlled access to guest resources | architectural requirement | Expose access through memory-system interfaces, not direct host pointers |
| Device/MMIO addresses, interrupt behavior, DMA behavior and CPUID properties | unknown to this project | Define only a dispatch boundary; do not populate concrete device maps or guest-visible device semantics |

## 3. Address types

Use strong types or wrappers, not aliases alone, so accidental cross-domain use is rejected by the compiler.

Conceptual types:

- `GuestVirtualAddress`: address issued by a guest CPU context.
- `GuestPhysicalAddress`: address in the emulated physical address space.
- Host pointers remain implementation details of backing stores and are never exposed as guest addresses.

All address arithmetic must detect overflow. Range checks use half-open intervals `[base, end)` and must avoid computing an overflowing end before validation. A 64-bit guest address must never be silently truncated to 32 bits.

## 4. Layered architecture

### 4.1 GuestAddressSpace

Owns guest virtual mappings and translation policy. It accepts a virtual address, access type, and execution context/address-space identifier where applicable, and returns a translated guest physical address or a structured fault.

The initial implementation may use a simple software mapping table. It must not pretend that this table reproduces Xbox hardware page tables.

`GuestAddressSpace` is the single routing point for guest virtual accesses. It checks virtual permissions, resolves the corresponding physical region, and forwards Normal-region accesses to `GuestPhysicalMemory` or Device-region accesses to a dispatcher. CPU, GPU and storage clients must not bypass this routing with raw host pointers.

### 4.2 GuestPhysicalMemory

Owns physical regions and their backing-store associations. It supports region lookup, bounded reads/writes, region attributes, and controlled mapping/unmapping.

A region descriptor should contain at least:

- guest physical base and length;
- region kind (RAM, device/MMIO, reserved, or unmapped);
- access attributes;
- backing-store identity/offset where applicable;
- optional memory/bandwidth class metadata.

Region kinds are architectural categories, not claims about exact Xbox physical ranges. Normal regions are backed by a `BackingStore`. Reserved and unmapped regions reject ordinary accesses with structured faults.

### 4.3 BackingStore

Owns host allocations or sparse committed storage. It is responsible for host allocation lifetime and bounds. It must not determine guest-visible address validity by exposing host pointer arithmetic.

Avoid allocating the entire guest memory budget eagerly. Sparse or region-backed commitment is preferable, provided guest-visible semantics remain deterministic. Host allocation failure is a structured resource fault; it must not silently reduce guest capacity.

### 4.4 Device regions and dispatch

`documented` — device/MMIO addresses and behavior are `unknown` for this project. This document defines only a boundary, not a device map.

A region marked `Device` must never be treated as flat RAM. Ordinary virtual translation may resolve it, but the access is then reported as `MemoryFault::DeviceAccessRequired` unless the caller explicitly requested device dispatch through the memory-system interface. The CPU must not dispatch devices directly.

The dispatch contract is:

- `GuestAddressSpace` routes a Device-region access to an injected `IDeviceDispatcher`.
- The dispatcher receives a value-type request whose `region_base` plus `offset` identify the region and offset, whose access kind and `AccessContext` are explicit, and whose `data.size()` is the requested length; no host pointer is included.
- The dispatcher returns a structured `MemoryResult` whose `completed_bytes` reports transferred bytes and whose `fault` carries failure diagnostics.
- The dispatcher must not expose host pointers, concrete MMIO addresses, interrupt behavior, DMA behavior, or CPUID properties.
- An unbound or unsupported device returns `DeviceAccessRequired` or `Unsupported` without side effects.
- A single access must not span Normal and Device regions. Bulk operations touching a Device region are rejected before dispatch, because device side effects are not assumed to be rollbackable.

This boundary allows later evidence-backed device packages to add real semantics without changing CPU or normal-memory interfaces.

### 4.5 Access path

CPU, GPU and storage clients use explicit access interfaces. CPU virtual accesses pass through translation and permission checks. GPU and DMA-like operations use explicitly specified guest address domains and access policies. No component should bypass validation by retaining raw host pointers.

## 5. Access contract

Define structured results rather than using one generic boolean or silently truncating addresses.

### 5.1 Outcomes and diagnostics

At minimum distinguish:

- success;
- unmapped address;
- access denied;
- address overflow;
- invalid range;
- alignment fault where required by the caller/operation;
- backing-store allocation or host-resource failure;
- device/MMIO access requiring device dispatch;
- unsupported operation.

A `MemoryResult` must carry enough diagnostics to identify the failing request without exposing host implementation details:

- success flag and fault code (`ok`, `fault`);
- access kind and access context (`access`, `context`);
- first failing address, or request base for a successful no-op (`address`); this is virtual for virtual-domain APIs and physical for physical-domain APIs;
- requested length and completed byte count (`requested_length`, `completed_bytes`);
- optional translated guest physical address (`translated_address`);
- optional region base in the same domain as `address` (`region_base`).

The baseline has no separate canonical-detail field; callers use `fault`, `address`, `translated_address` and `region_base` for diagnostics.

Invariants:

- `ok == (fault == MemoryFault::None)`;
- `completed_bytes <= requested_length`;
- for ordinary RAM failures, `completed_bytes == 0` and guest memory is unchanged;
- translated-address and region diagnostics are unset when translation or region lookup failed;
- diagnostics are value types and must not contain pointers into mutable region containers.

Do not force every access to require alignment: unaligned behavior is instruction- and device-specific. The access operation or caller contract must state the requirement.

### 5.2 Execute/fetch access

`AccessKind::Execute` means instruction fetch only. Execute does not imply read or write, and read/write do not imply execute. A write to an executable region is still a write-permission question; a data read from an executable region is still a read-permission question.

A fetch is a contiguous instruction-byte read from a `GuestVirtualAddress` with these rules:

1. length must be at least 1 and at most the current local fetch window (currently 15 bytes; this is an implementation limit, not an x64 architectural constant);
2. the complete half-open range must be checked with overflow-safe 64-bit arithmetic;
3. every covered virtual mapping must allow `Execute`;
4. the resolved physical region must allow the fetch read and must be a Normal region in the baseline;
5. the complete byte sequence is delivered or no bytes are delivered — there is no partial fetch;
6. decode outcomes remain CPU statuses and are not memory faults.

Failure order for a fetch is: invalid/empty or oversized length → address overflow → unmapped range → execute permission denial → physical read denial or Device region → range crossing a region boundary.

### 5.3 Atomicity and bulk operations

Ordinary scalar reads/writes and CPU operand accesses are all-or-nothing. Range, permission, translation, region and backing checks happen before any byte write or destination-register commit.

Bulk accesses are all-or-nothing by default:

- a rejected bulk access writes zero bytes, leaves the destination buffer unchanged and reports `completed_bytes == 0`;
- a successful bulk access reports the exact byte count;
- an access spanning multiple virtual mappings or mixing Normal and Device regions is rejected before any mutation or dispatch;
- no optional partial-completion mode is introduced in the baseline.

This matches the existing scaffold pattern where the full 64-bit store range is validated before the eight byte writes (`observed`: `src/cpu/x64_instruction_interpreter.cpp:82-97`) without making Xbox-specific claims.

Mapping and unmapping operations are atomic: validation precedes insertion or removal, and failure leaves mappings, regions and backing associations unchanged.

The first implementation is single-threaded per address space. Concurrent CPU/GPU/I/O writes require a separate synchronization contract before they are allowed.

### 5.4 Edge-case and failure matrix

The baseline contract is:

- **Zero-length data access:** succeeds as a no-op, performs no translation, permission, region or device check, mutates nothing, and reports `requested_length == 0`, `completed_bytes == 0`.
- **Zero-length fetch:** invalid; instruction fetch requires at least one byte.
- **Maximum address:** `UINT64_MAX + 0` is only valid as a zero-length data no-op; any positive length starting at or crossing `UINT64_MAX` is an address-overflow fault.
- **Overflow:** compute range validity without forming an overflowing end value; overflow faults before any translation, permission check or mutation.
- **Empty ranges:** `map(length == 0)` fails. A zero-length physical region is not a mappable region.
- **Overlaps:** half-open virtual regions must not overlap. Touching ranges such as `[a, b)` and `[b, c)` are allowed. A request spanning two mappings is rejected; it is not silently split.
- **Aliases:** backing aliases are unsupported in the baseline. Two virtual mappings writing the same backing bytes require a later owner/ADR decision, explicit ownership rules and tests.
- **Unknown unmap:** unmapping a base that is not mapped, or a length that does not match an existing mapping, fails without mutation.
- **Map/unmap failure:** leaves the previous state unchanged and returns structured diagnostics.
- **No narrowing:** no `uint64_t` guest address is converted to `uint32_t` or `size_t` without a proven bounds check.

## 6. Permissions and mappings

Represent read, write and execute permissions independently. Permission checks occur at the layer that owns the address domain:

- virtual mapping permissions during translation;
- physical region/device permissions during physical access.

A mapping must declare all three permissions explicitly. Missing permission is denial, not implicit allowance. Zero-length mappings are rejected. Overlapping virtual mappings are rejected. Aliasing is unsupported in the baseline.

Mapping operations must define overlap behavior, replacement behavior, zero-length behavior, and atomic failure semantics. Reject ambiguous overlapping mappings unless an explicit aliasing feature is introduced with tests and ownership rules.

Aliasing, copy-on-write, page granularity, caching attributes, and exact console mapping rules remain future extensions until supported by reliable evidence.

## 7. CPU integration

The x64 interpreter must no longer reject every guest address above `UINT32_MAX` or narrow addresses to `uint32_t`.

Replace direct dependency on concrete `GuestMemory` with an interface appropriate for CPU data accesses and instruction fetch. A virtual CPU access should specify:

- address;
- width/length;
- read/write/execute intent;
- current address-space context if introduced.

### 7.1 Fetch, data access and fault propagation

The future address-space-bound CPU must call explicit memory operations conceptually equivalent to `fetch_instruction`, `read_data` and `write_data`. It must not call concrete `GuestMemory` for guest virtual accesses.

- **Fetch failure:** no bytes are delivered, no instruction is decoded or retired, RIP is unchanged, and GPRs/RFLAGS are unchanged.
- **Data-access failure during execute:** the destination register, RFLAGS, RIP and guest memory are unchanged; no device side effect is visible.
- **Success:** RIP advances by the decoded instruction length only after all memory effects succeed. `RIP + length` overflow is checked before mutation and faults if it occurs.
- **Run loop:** already completed instructions remain committed; the failing step is not counted and execution stops on the first error.
- **Decode errors:** remain CPU execution statuses, not memory faults.
- **Legacy compatibility:** existing `TestCpu` partial-fetch/no-rollback behavior and current x64 status values are preserved (`documented`: `docs/ARCHITECTURE.md:185-210`, `docs/ARCHITECTURE.md:153-183`). Any structured diagnostic carried into a future execution result must be additive.

The instruction decoder remains independent of memory. Instruction fetch belongs to the execution/memory integration layer.

## 8. GPU and storage integration

GPU resource access and storage transfers must use the same guest physical-memory ownership model, but must not be forced through CPU virtual translation unless their defined semantics require it.

The design should leave room for:

- GPU virtual address spaces;
- resource heaps and residency;
- command-processor reads;
- DMA-like transfers;
- asynchronous I/O completion;
- decompression into guest memory;
- synchronization and ownership of in-flight buffers.

These are extension points, not currently implemented capabilities. No concrete guest physical range, GPU page-table format or DMA behavior is defined here.

## 9. Concurrency and determinism

The first implementation should favour deterministic correctness over premature lock-free optimization.

Define ownership and synchronization before allowing concurrent CPU/GPU/I/O writes. Do not expose references or spans whose lifetime can outlive a mapping or backing-store allocation. Any future parallel access model must specify atomicity, ordering, and visibility.

## 10. Suggested C++ interface shape

The following is illustrative, not a committed API. It is the target shape for a separate reviewed implementation package; no header is added by this document.

```cpp
enum class AccessKind : std::uint8_t { Read, Write, Execute };

enum class AccessContext : std::uint8_t {
    Fetch,
    Data,
    Bulk,
    Map,
    Unmap,
    DeviceDispatch
};

enum class MemoryFault : std::uint8_t {
    None,
    Unmapped,
    AccessDenied,
    AddressOverflow,
    InvalidRange,
    Alignment,
    ResourceExhausted,
    DeviceAccessRequired,
    Unsupported
};

struct GuestVirtualAddress {
    std::uint64_t value{};
};

struct GuestPhysicalAddress {
    std::uint64_t value{};
};

struct MemoryResult {
    bool ok{false};
    MemoryFault fault{MemoryFault::Unsupported};
    AccessKind access{AccessKind::Read};
    AccessContext context{AccessContext::Data};
    std::uint64_t address{};                      // request domain: virtual or physical
    std::uint64_t requested_length{};
    std::uint64_t completed_bytes{};
    std::optional<std::uint64_t> translated_address{};
    std::optional<std::uint64_t> region_base{};   // same domain as address
};

struct DeviceAccessRequest {
    GuestPhysicalAddress region_base{};           // region identity
    std::uint64_t offset{};
    AccessKind access{AccessKind::Read};
    AccessContext context{AccessContext::DeviceDispatch};
    std::span<std::byte> data{};                  // data.size() is the requested length
};

class IDeviceDispatcher {
public:
    virtual ~IDeviceDispatcher() = default;
    virtual MemoryResult dispatch(const DeviceAccessRequest& request) noexcept = 0;
};

class IGuestPhysicalMemory {
public:
    virtual ~IGuestPhysicalMemory() = default;
    virtual MemoryResult read(
        GuestPhysicalAddress address,
        std::span<std::byte> destination) const noexcept = 0;
    virtual MemoryResult write(
        GuestPhysicalAddress address,
        std::span<const std::byte> source) noexcept = 0;
};

class IGuestAddressSpace {
public:
    virtual ~IGuestAddressSpace() = default;
    virtual MemoryResult translate(
        GuestVirtualAddress address,
        AccessContext context,
        AccessKind access,
        GuestPhysicalAddress& result) const noexcept = 0;
    virtual MemoryResult fetch_instruction(
        GuestVirtualAddress address,
        std::span<std::byte> destination) const noexcept = 0;
    virtual MemoryResult read_data(
        GuestVirtualAddress address,
        std::span<std::byte> destination) const noexcept = 0;
    virtual MemoryResult write_data(
        GuestVirtualAddress address,
        std::span<const std::byte> source) noexcept = 0;
};
```

The concrete API must be reconciled with existing project conventions and exception policy before implementation.

## 11. Migration sequence

1. Introduce strong guest virtual/physical address types and checked arithmetic helpers.
2. Introduce region descriptors and a physical-memory interface while retaining `GuestMemory` as a compatibility/test adapter.
3. Add a deterministic region-backed physical-memory implementation.
4. Add a replaceable software guest-address translation table with explicit mapping and permission tests.
5. Add the structured diagnostic result and the device-dispatch boundary.
6. Adapt CPU fetch and data accesses to the interfaces without changing supported instruction semantics.
7. Replace silent unsupported-instruction success with explicit execution statuses in a separate CPU package.
8. Add GPU/storage clients only after physical-memory contracts are stable.
9. Add concurrency, residency and bandwidth models as separately testable packages.

## 12. Required verification

Future implementation packages must cover:

- strong type separation at compile time where practical;
- zero-length data access success and zero-length fetch rejection;
- `UINT64_MAX + 1` overflow and absence of 64-to-32 truncation;
- exact region boundaries and requests spanning two mappings;
- overlapping map rejection and unchanged state after failed map/unmap;
- unknown unmap and empty-region rejection;
- backing alias rejection in the baseline;
- read/write/execute permission independence;
- executable fetch success, execute denial, data-read denial on an executable region, and Device-region fetch rejection;
- Normal versus Device routing, unbound device fault, and no mixed Normal/Device bulk access;
- all `MemoryResult` fields (`ok`, `fault`, `access`, `context`, `address`, `requested_length`, `completed_bytes`, `translated_address`, `region_base`), the `ok`/`fault` invariant and unset optional fields before translation;
- failed scalar store leaves destination register and bytes unchanged;
- failed bulk write writes zero bytes and leaves the destination unchanged;
- backing-store exhaustion reported as a structured fault;
- virtual-to-physical translation;
- CPU fetch and data-access fault propagation with unchanged RIP/GPRs/RFLAGS;
- `RIP + length` overflow before mutation;
- preservation of the existing `GuestMemory`, `TestCpu` and x64 regression suites.

## 13. Explicit non-claims

This design does not establish Xbox-specific virtual address layouts, page-table formats, physical region addresses, cache coherency rules, MMIO maps, GPU page-table semantics, or exact DMA behavior. Those remain documented/observed/hypothesis/unknown research items until reliable evidence exists.

It does not implement `GuestAddressSpace`, `GuestPhysicalMemory`, `BackingStore`, `IDeviceDispatcher`, structured `MemoryResult`, or a new CPU memory binding. No public header, source, test, CMake file, ADR or license is changed by this document.

The goal is to create an extensible memory subsystem capable of representing those properties later without embedding unsupported assumptions in the core.
