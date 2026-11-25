#include "xbridge/memory/guest_physical_memory.hpp"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <iterator>
#include <limits>
#include <utility>

namespace xbridge::memory {
namespace {

// Internal commitment granularity. This is not a guest-visible page size and
// makes no claim about console memory layout.
constexpr std::uint64_t kChunkSize = 4096U;

[[nodiscard]] bool get_region_end(const GuestPhysicalRegion& region, std::uint64_t& end) noexcept {
    return checked_range_end(region.base.value, region.length, end);
}

[[nodiscard]] GuestPhysicalRegion empty_region() noexcept {
    return GuestPhysicalRegion{GuestPhysicalAddress{}, 0U, RegionKind::Reserved, MemoryPermissions{}};
}

[[nodiscard]] bool span_length(std::size_t size, std::uint64_t& length) noexcept {
    const auto wide_size = static_cast<std::uintmax_t>(size);
    if (wide_size > static_cast<std::uintmax_t>((std::numeric_limits<std::uint64_t>::max)()))
        return false;
    length = static_cast<std::uint64_t>(wide_size);
    return true;
}

void ensure_chunks(
    std::map<std::uint64_t, std::vector<std::uint8_t>>& chunks,
    std::uint64_t offset,
    std::uint64_t length) {
    while (length > 0U) {
        const std::uint64_t index = offset / kChunkSize;
        const std::uint64_t within = offset % kChunkSize;
        auto& chunk = chunks[index];
        if (chunk.empty())
            chunk.assign(static_cast<std::size_t>(kChunkSize), std::uint8_t{0U});

        const std::uint64_t take = (std::min)(length, kChunkSize - within);
        offset += take;
        length -= take;
    }
}

} // namespace

const GuestPhysicalMemory::Region* GuestPhysicalMemory::find_region(std::uint64_t address) const noexcept {
    auto it = regions_.upper_bound(address);
    if (it == regions_.begin())
        return nullptr;
    --it;

    std::uint64_t end = 0U;
    if (!get_region_end(it->second.descriptor, end) || address >= end)
        return nullptr;
    return &it->second;
}

MemoryResult GuestPhysicalMemory::add_region(
    GuestPhysicalAddress base,
    std::uint64_t length,
    RegionKind kind,
    MemoryPermissions permissions) {
    const MemoryAccess access = MemoryAccess::Write;
    const MemoryAccessContext context = MemoryAccessContext::Map;

    if (length == 0U)
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, base.value, length);

    std::uint64_t end = 0U;
    if (!checked_range_end(base.value, length, end))
        return MemoryResult::failure(MemoryFault::AddressOverflow, access, context, base.value, length);

    try {
        auto next = regions_.lower_bound(base.value);
        if (next != regions_.end()) {
            std::uint64_t next_end = 0U;
            if (get_region_end(next->second.descriptor, next_end) && base.value < next_end)
                return MemoryResult::failure(MemoryFault::InvalidRange, access, context, base.value, length);
        }
        if (next != regions_.begin()) {
            auto previous = std::prev(next);
            std::uint64_t previous_end = 0U;
            if (get_region_end(previous->second.descriptor, previous_end) && previous_end > base.value)
                return MemoryResult::failure(MemoryFault::InvalidRange, access, context, base.value, length);
        }

        Region region{};
        region.descriptor = GuestPhysicalRegion{base, length, kind, permissions};
        regions_.emplace(base.value, std::move(region));
    } catch (const std::exception&) {
        return MemoryResult::failure(MemoryFault::ResourceExhausted, access, context, base.value, length);
    }

    return MemoryResult::success(access, context, base.value, length);
}

MemoryResult GuestPhysicalMemory::read(
    GuestPhysicalAddress address,
    std::span<std::uint8_t> destination,
    MemoryAccessContext context) noexcept {
    const MemoryAccess access = MemoryAccess::Read;

    std::uint64_t length = 0U;
    if (!span_length(destination.size(), length))
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, 0U);
    if (length == 0U)
        return MemoryResult::success(access, context, address.value, 0U, 0U);

    std::uint64_t end = 0U;
    if (!checked_range_end(address.value, length, end))
        return MemoryResult::failure(MemoryFault::AddressOverflow, access, context, address.value, length);

    const Region* region = find_region(address.value);
    if (region == nullptr || region->descriptor.kind == RegionKind::Reserved)
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);
    if (region->descriptor.kind == RegionKind::Device)
        return MemoryResult::failure(MemoryFault::DeviceAccessRequired, access, context, address.value, length);

    std::uint64_t region_end = 0U;
    if (!get_region_end(region->descriptor, region_end) || end > region_end)
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, length);
    // A fetch context reads bytes authorized by the region's execute bit;
    // ordinary data reads use the read bit.
    const bool allowed = context == MemoryAccessContext::Fetch
                             ? region->descriptor.permissions.execute
                             : region->descriptor.permissions.read;
    if (!allowed)
        return MemoryResult::failure(MemoryFault::AccessDenied, access, context, address.value, length);

    auto owned = regions_.find(region->descriptor.base.value);
    if (owned == regions_.end())
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);

    try {
        // Commit every needed chunk before touching the destination so failures
        // cannot produce a partially delivered read.
        ensure_chunks(owned->second.chunks, address.value - region->descriptor.base.value, length);

        std::uint64_t offset = address.value - region->descriptor.base.value;
        std::size_t copied = 0U;
        while (copied < destination.size()) {
            const std::uint64_t index = offset / kChunkSize;
            const std::uint64_t within = offset % kChunkSize;
            const auto& chunk = owned->second.chunks.find(index)->second;
            const std::uint64_t take_u64 = (std::min)(
                static_cast<std::uint64_t>(destination.size() - copied), kChunkSize - within);
            const auto take = static_cast<std::size_t>(take_u64);
            for (std::size_t i = 0U; i < take; ++i)
                destination[copied + i] = chunk[static_cast<std::size_t>(within) + i];
            offset += take_u64;
            copied += take;
        }
    } catch (const std::exception&) {
        return MemoryResult::failure(MemoryFault::ResourceExhausted, access, context, address.value, length);
    }

    return MemoryResult::success(access, context, address.value, length, length);
}

MemoryResult GuestPhysicalMemory::write(
    GuestPhysicalAddress address,
    std::span<const std::uint8_t> source,
    MemoryAccessContext context) noexcept {
    const MemoryAccess access = MemoryAccess::Write;

    std::uint64_t length = 0U;
    if (!span_length(source.size(), length))
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, 0U);
    if (length == 0U)
        return MemoryResult::success(access, context, address.value, 0U, 0U);

    std::uint64_t end = 0U;
    if (!checked_range_end(address.value, length, end))
        return MemoryResult::failure(MemoryFault::AddressOverflow, access, context, address.value, length);

    const Region* region = find_region(address.value);
    if (region == nullptr || region->descriptor.kind == RegionKind::Reserved)
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);
    if (region->descriptor.kind == RegionKind::Device)
        return MemoryResult::failure(MemoryFault::DeviceAccessRequired, access, context, address.value, length);

    std::uint64_t region_end = 0U;
    if (!get_region_end(region->descriptor, region_end) || end > region_end)
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, length);
    if (!region->descriptor.permissions.write)
        return MemoryResult::failure(MemoryFault::AccessDenied, access, context, address.value, length);

    auto owned = regions_.find(region->descriptor.base.value);
    if (owned == regions_.end())
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);

    try {
        // All backing checks and commitments precede the first byte write.
        ensure_chunks(owned->second.chunks, address.value - region->descriptor.base.value, length);

        std::uint64_t offset = address.value - region->descriptor.base.value;
        std::size_t written = 0U;
        while (written < source.size()) {
            const std::uint64_t index = offset / kChunkSize;
            const std::uint64_t within = offset % kChunkSize;
            auto& chunk = owned->second.chunks.find(index)->second;
            const std::uint64_t take_u64 = (std::min)(
                static_cast<std::uint64_t>(source.size() - written), kChunkSize - within);
            const auto take = static_cast<std::size_t>(take_u64);
            for (std::size_t i = 0U; i < take; ++i)
                chunk[static_cast<std::size_t>(within) + i] = source[written + i];
            offset += take_u64;
            written += take;
        }
    } catch (const std::exception&) {
        return MemoryResult::failure(MemoryFault::ResourceExhausted, access, context, address.value, length);
    }

    return MemoryResult::success(access, context, address.value, length, length);
}

GuestPhysicalRegion GuestPhysicalMemory::region_at(GuestPhysicalAddress address) const noexcept {
    const Region* region = find_region(address.value);
    return region == nullptr ? empty_region() : region->descriptor;
}

bool GuestPhysicalMemory::region_covers(GuestPhysicalAddress base, std::uint64_t length) const noexcept {
    std::uint64_t end = 0U;
    if (!checked_range_end(base.value, length, end))
        return false;

    const Region* region = find_region(base.value);
    if (region == nullptr)
        return false;

    std::uint64_t region_end = 0U;
    return get_region_end(region->descriptor, region_end) && end <= region_end;
}

} // namespace xbridge::memory
