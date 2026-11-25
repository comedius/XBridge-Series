#pragma once

#include "xbridge/memory/guest_address.hpp"
#include "xbridge/memory/guest_memory_result.hpp"

#include <cstdint>

namespace xbridge::memory {

struct GuestPhysicalRegion {
    GuestPhysicalAddress base{};
    std::uint64_t length{};
    RegionKind kind{RegionKind::Reserved};
    MemoryPermissions permissions{};
};

struct GuestVirtualMapping {
    GuestVirtualAddress base{};
    GuestPhysicalAddress physical_base{};
    std::uint64_t length{};
    MemoryPermissions permissions{};
};

} // namespace xbridge::memory
