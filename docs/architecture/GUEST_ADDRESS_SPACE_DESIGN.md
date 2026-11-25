# Guest Address Space and Memory-System Design

**Status:** proposed architecture baseline; not yet implemented or hardware-validated  
**Target:** Xbox Series X guest model  
**Language:** C++20  
**Scope:** memory contracts shared by CPU, GPU, and storage/I/O components

## 1. Purpose

The current `GuestMemory` is a useful bounded test fixture, but it is not an adequate long-term memory architecture. It represents one contiguous byte vector addressed by 32-bit offsets. The x64 interpreter currently depends directly on that type and narrows guest addresses to 32 bits.

This document defines the next architectural direction without claiming undocumented Xbox memory-management details as facts.

The implementation must distinguish:

1. guest virtual addresses;
2. guest physical addresses;
3. host virtual pointers and host allocations.

These address domains must not be implicitly interchangeable.

## 2. Evidence and confidence

| Item | Status | Design consequence |
|---|---|---|
| Xbox Series X has 16 GB GDDR6 unified memory with documented bandwidth partitions | documented at platform-overview level | Model a fixed guest memory budget and bandwidth classes as configuration/data, not as host RAM equivalence |
| Exact guest virtual address layout and page-table behavior | unknown to this project | Keep translation policy replaceable; do not invent a console page-table format |
| Exact physical allocation, reservation, aliasing and residency semantics | unknown to this project | Implement generic region and mapping primitives first |
| Host machine memory and GPU memory differ from guest memory | architectural requirement | Guest capacity must not be silently reduced to host available RAM |
| CPU, GPU and I/O need controlled access to guest resources | architectural requirement | Expose access through memory-system interfaces, not direct host pointers |

## 3. Address types

Use strong types or wrappers, not aliases alone, so accidental cross-domain use is rejected by the compiler.

Conceptual types:

- `GuestVirtualAddress`: address issued by a guest CPU context.
- `GuestPhysicalAddress`: address in the emulated physical address space.
- `GuestAddress`: optional tagged address for APIs that explicitly accept more than one domain.
- Host pointers remain implementation details of backing stores and are never exposed as guest addresses.

All address arithmetic must detect overflow. Range checks use half-open intervals `[base, end)` and must avoid computing an overflowing end before validation.

## 4. Layered architecture

### 4.1 GuestAddressSpace

Owns guest virtual mappings and translation policy. It accepts a virtual address, access type, and execution context/address-space identifier where applicable, and returns a translated guest physical address or a structured fault.

The initial implementation may use a simple software mapping table. It must not pretend that this table reproduces Xbox hardware page tables.

### 4.2 GuestPhysicalMemory

Owns physical regions and their backing-store associations. It supports region lookup, bounded reads/writes, region attributes, and controlled mapping/unmapping.

A region descriptor should contain at least:

- guest physical base and length;
- region kind (RAM, device/MMIO, reserved, or unmapped);
- access attributes;
- backing-store identity/offset where applicable;
- optional memory/bandwidth class metadata.

Region kinds are architectural categories, not claims about exact Xbox physical ranges.

### 4.3 BackingStore

Owns host allocations or sparse committed storage. It is responsible for host allocation lifetime and bounds. It must not determine guest-visible address validity by exposing host pointer arithmetic.

Avoid allocating the entire guest memory budget eagerly. Sparse or region-backed commitment is preferable, provided guest-visible semantics remain deterministic.

### 4.4 Access path

CPU, GPU and storage clients use explicit access interfaces. CPU virtual accesses pass through translation and permission checks. GPU and DMA-like operations use explicitly specified guest address domains and access policies. No component should bypass validation by retaining raw host pointers.

## 5. Access contract

Define structured results rather than using one generic boolean or silently truncating addresses.

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

Do not force every access to require alignment: unaligned behavior is instruction- and device-specific. The access operation or caller contract must state the requirement.

A failed operation must not partially modify guest memory unless the operation explicitly models partial device/I/O completion. Ordinary scalar CPU stores should validate the complete range before changing bytes.

## 6. Permissions and mappings

Represent read, write and execute permissions independently. Permission checks occur at the layer that owns the address domain:

- virtual mapping permissions during translation;
- physical region/device permissions during physical access.

Mapping operations must define overlap behavior, replacement behavior, zero-length behavior, and atomic failure semantics. Reject ambiguous overlapping mappings unless an explicit aliasing feature is introduced with tests and ownership rules.

Aliasing, copy-on-write, page granularity, caching attributes, and exact console mapping rules remain future extensions until supported by reliable evidence.

## 7. CPU integration

The x64 interpreter must no longer reject every guest address above `UINT32_MAX` or narrow addresses to `uint32_t`.

Replace direct dependency on concrete `GuestMemory` with an interface appropriate for CPU data accesses and instruction fetch. A virtual CPU access should specify:

- address;
- width/length;
- read/write/execute intent;
- current address-space context if introduced.

The memory result must be propagated into an explicit CPU execution/fault status. A memory fault must not advance RIP or partially update architectural state unless the instruction's documented semantics require otherwise.

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

These are extension points, not currently implemented capabilities.

## 9. Concurrency and determinism

The first implementation should favour deterministic correctness over premature lock-free optimization.

Define ownership and synchronization before allowing concurrent CPU/GPU/I/O writes. Do not expose references or spans whose lifetime can outlive a mapping or backing-store allocation. Any future parallel access model must specify atomicity, ordering, and visibility.

## 10. Suggested C++ interface shape

The following is illustrative, not a committed API:

```cpp
struct GuestVirtualAddress {
    std::uint64_t value{};
};

struct GuestPhysicalAddress {
    std::uint64_t value{};
};

enum class MemoryAccess : std::uint8_t {
    Read,
    Write,
    Execute
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

struct MemoryResult {
    MemoryFault fault{MemoryFault::None};
    [[nodiscard]] explicit operator bool() const noexcept {
        return fault == MemoryFault::None;
    }
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
        MemoryAccess access,
        GuestPhysicalAddress& result) const noexcept = 0;
};
```

The concrete API must be reconciled with existing project conventions and exception policy before implementation.

## 11. Migration sequence

1. Introduce strong guest virtual/physical address types and checked arithmetic helpers.
2. Introduce region descriptors and a physical-memory interface while retaining `GuestMemory` as a compatibility/test adapter.
3. Add a deterministic region-backed physical-memory implementation.
4. Add a replaceable software guest-address translation table with explicit mapping and permission tests.
5. Adapt CPU fetch and data accesses to the interfaces without changing supported instruction semantics.
6. Replace silent unsupported-instruction success with explicit execution statuses in a separate CPU package.
7. Add GPU/storage clients only after physical-memory contracts are stable.
8. Add concurrency, residency and bandwidth models as separately testable packages.

## 12. Required verification

Tests must cover:

- strong type separation at compile time where practical;
- zero-length and maximum-address ranges;
- overflow-safe range checks;
- exact region boundaries;
- overlapping map rejection;
- map/unmap and permission transitions;
- read/write/execute access checks;
- unmapped and device regions;
- no partial scalar writes on failure;
- backing-store exhaustion;
- virtual-to-physical translation;
- CPU fetch and data-access fault propagation;
- preservation of the existing `GuestMemory` regression suite.

## 13. Explicit non-claims

This design does not establish Xbox-specific virtual address layouts, page-table formats, physical region addresses, cache coherency rules, MMIO maps, GPU page-table semantics, or exact DMA behavior. Those remain documented/observed/hypothesis/unknown research items until reliable evidence exists.

The goal is to create an extensible memory subsystem capable of representing those properties later without embedding unsupported assumptions in the core.
