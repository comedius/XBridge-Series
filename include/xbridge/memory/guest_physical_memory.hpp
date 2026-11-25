#pragma once

#include "xbridge/memory/guest_address.hpp"
#include "xbridge/memory/guest_memory_result.hpp"
#include "xbridge/memory/guest_region.hpp"

#include <cstdint>
#include <map>
#include <span>
#include <vector>

namespace xbridge::memory {

// Owns generic guest physical regions. Normal regions commit backing bytes only
// when touched; Device and Reserved regions have no byte backing.
class GuestPhysicalMemory {
public:
    MemoryResult add_region(
        GuestPhysicalAddress base,
        std::uint64_t length,
        RegionKind kind,
        MemoryPermissions permissions);

    MemoryResult read(
        GuestPhysicalAddress address,
        std::span<std::uint8_t> destination,
        MemoryAccessContext context) noexcept;

    MemoryResult write(
        GuestPhysicalAddress address,
        std::span<const std::uint8_t> source,
        MemoryAccessContext context) noexcept;

    [[nodiscard]] GuestPhysicalRegion region_at(GuestPhysicalAddress address) const noexcept;
    [[nodiscard]] bool region_covers(GuestPhysicalAddress base, std::uint64_t length) const noexcept;

private:
    struct Region {
        GuestPhysicalRegion descriptor{};
        // Internal commitment chunks, not a guest-visible page-table concept.
        std::map<std::uint64_t, std::vector<std::uint8_t>> chunks{};
    };

    std::map<std::uint64_t, Region> regions_{};

    [[nodiscard]] const Region* find_region(std::uint64_t address) const noexcept;
};

} // namespace xbridge::memory
