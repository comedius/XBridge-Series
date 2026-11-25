#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>

namespace xbridge::memory {

enum class MemoryAccess : std::uint8_t { Read, Write, Execute };

enum class MemoryAccessContext : std::uint8_t {
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

enum class RegionKind : std::uint8_t { Normal, Device, Reserved };

struct MemoryPermissions {
    bool read{false};
    bool write{false};
    bool execute{false};
};

// Value-type diagnostic result. No field points into mutable memory-system state.
struct MemoryResult {
    bool ok{false};
    MemoryFault fault{MemoryFault::Unsupported};
    MemoryAccess access{MemoryAccess::Read};
    MemoryAccessContext context{MemoryAccessContext::Data};
    std::uint64_t address{};
    std::uint64_t requested_length{};
    std::uint64_t completed_bytes{};
    std::optional<std::uint64_t> translated_address{};
    std::optional<std::uint64_t> region_base{};

    explicit operator bool() const noexcept { return ok; }

    [[nodiscard]] static MemoryResult success(
        MemoryAccess access = MemoryAccess::Read,
        MemoryAccessContext context = MemoryAccessContext::Data,
        std::uint64_t address = 0U,
        std::uint64_t requested_length = 0U,
        std::uint64_t completed_bytes = 0U,
        std::optional<std::uint64_t> translated_address = std::nullopt,
        std::optional<std::uint64_t> region_base = std::nullopt) noexcept {
        MemoryResult result{};
        result.ok = true;
        result.fault = MemoryFault::None;
        result.access = access;
        result.context = context;
        result.address = address;
        result.requested_length = requested_length;
        result.completed_bytes = (std::min)(completed_bytes, requested_length);
        result.translated_address = translated_address;
        result.region_base = region_base;
        return result;
    }

    [[nodiscard]] static MemoryResult failure(
        MemoryFault fault = MemoryFault::Unsupported,
        MemoryAccess access = MemoryAccess::Read,
        MemoryAccessContext context = MemoryAccessContext::Data,
        std::uint64_t address = 0U,
        std::uint64_t requested_length = 0U) noexcept {
        MemoryResult result{};
        result.ok = false;
        // "None" cannot describe a failure; keep the ok/fault invariant intact.
        result.fault = fault == MemoryFault::None ? MemoryFault::Unsupported : fault;
        result.access = access;
        result.context = context;
        result.address = address;
        result.requested_length = requested_length;
        result.completed_bytes = 0U;
        return result;
    }
};

} // namespace xbridge::memory
