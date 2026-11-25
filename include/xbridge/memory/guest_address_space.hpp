#pragma once

#include "xbridge/memory/guest_address.hpp"
#include "xbridge/memory/guest_memory_result.hpp"
#include "xbridge/memory/guest_physical_memory.hpp"
#include "xbridge/memory/guest_region.hpp"

#include <cstdint>
#include <span>
#include <vector>

namespace xbridge::memory {

struct DeviceAccessRequest {
    GuestPhysicalAddress region_base{};
    std::uint64_t offset{};
    MemoryAccess access{MemoryAccess::Read};
    MemoryAccessContext context{MemoryAccessContext::DeviceDispatch};
    std::span<std::uint8_t> data{};
};

class IDeviceDispatcher {
public:
    virtual ~IDeviceDispatcher() = default;
    virtual MemoryResult dispatch(const DeviceAccessRequest& request) noexcept = 0;
};

// Software mapping table and single routing point for guest virtual accesses.
// It does not model or claim any console-specific page-table format.
class GuestAddressSpace {
public:
    explicit GuestAddressSpace(GuestPhysicalMemory& physical) noexcept;

    void set_device_dispatcher(IDeviceDispatcher* dispatcher) noexcept;

    MemoryResult map(
        GuestVirtualAddress virtual_base,
        GuestPhysicalAddress physical_base,
        std::uint64_t length,
        MemoryPermissions permissions) noexcept;

    MemoryResult unmap(GuestVirtualAddress virtual_base, std::uint64_t length) noexcept;

    MemoryResult translate(
        GuestVirtualAddress address,
        std::uint64_t length,
        MemoryAccess access,
        MemoryAccessContext context,
        GuestPhysicalAddress& translated) const noexcept;

    MemoryResult fetch(GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept;
    MemoryResult read(GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept;
    MemoryResult write(GuestVirtualAddress address, std::span<const std::uint8_t> source) noexcept;

private:
    GuestPhysicalMemory& physical_;
    IDeviceDispatcher* dispatcher_{nullptr};
    std::vector<GuestVirtualMapping> mappings_{};

    [[nodiscard]] MemoryResult dispatch_device_read(
        const GuestPhysicalRegion& region,
        GuestPhysicalAddress physical_address,
        std::uint64_t virtual_address,
        std::span<std::uint8_t> destination) noexcept;

    [[nodiscard]] MemoryResult dispatch_device_write(
        const GuestPhysicalRegion& region,
        GuestPhysicalAddress physical_address,
        std::uint64_t virtual_address,
        std::span<const std::uint8_t> source) noexcept;
};

} // namespace xbridge::memory
