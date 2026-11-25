#include "xbridge/memory/guest_address_space.hpp"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <limits>
#include <vector>

namespace xbridge::memory {
namespace {

// Local fetch window only; this is an implementation limit, not an x64 constant.
constexpr std::uint64_t kMaxFetchLength = 15U;

[[nodiscard]] bool span_length(std::size_t size, std::uint64_t& length) noexcept {
    const auto wide_size = static_cast<std::uintmax_t>(size);
    if (wide_size > static_cast<std::uintmax_t>((std::numeric_limits<std::uint64_t>::max)()))
        return false;
    length = static_cast<std::uint64_t>(wide_size);
    return true;
}

[[nodiscard]] bool mapping_virtual_end(const GuestVirtualMapping& mapping, std::uint64_t& end) noexcept {
    return checked_range_end(mapping.base.value, mapping.length, end);
}

[[nodiscard]] bool mapping_physical_end(const GuestVirtualMapping& mapping, std::uint64_t& end) noexcept {
    return checked_range_end(mapping.physical_base.value, mapping.length, end);
}

[[nodiscard]] bool mapping_allows(MemoryPermissions permissions, MemoryAccess access) noexcept {
    switch (access) {
    case MemoryAccess::Read:
        return permissions.read;
    case MemoryAccess::Write:
        return permissions.write;
    case MemoryAccess::Execute:
        return permissions.execute;
    }
    return false;
}

[[nodiscard]] bool region_allows(const GuestPhysicalRegion& region, MemoryAccess access) noexcept {
    switch (access) {
    case MemoryAccess::Read:
        return region.permissions.read;
    case MemoryAccess::Write:
        return region.permissions.write;
    case MemoryAccess::Execute:
        // Fetch requires execute permission at both layers; it never inherits
        // read or write permission at either layer.
        return region.permissions.execute;
    }
    return false;
}

[[nodiscard]] MemoryResult rebase_result(
    const MemoryResult& source,
    MemoryAccess access,
    MemoryAccessContext context,
    std::uint64_t virtual_address,
    std::uint64_t requested_length,
    std::uint64_t physical_address,
    std::uint64_t region_base) noexcept {
    if (!source.ok)
        return MemoryResult::failure(source.fault, access, context, virtual_address, requested_length);
    return MemoryResult::success(
        access, context, virtual_address, requested_length, source.completed_bytes,
        physical_address, region_base);
}

[[nodiscard]] MemoryFault dispatch_fault(const MemoryResult& dispatched) noexcept {
    return dispatched.fault == MemoryFault::None ? MemoryFault::Unsupported : dispatched.fault;
}

} // namespace

GuestAddressSpace::GuestAddressSpace(GuestPhysicalMemory& physical) noexcept : physical_(physical) {}

void GuestAddressSpace::set_device_dispatcher(IDeviceDispatcher* dispatcher) noexcept {
    dispatcher_ = dispatcher;
}

MemoryResult GuestAddressSpace::map(
    GuestVirtualAddress virtual_base,
    GuestPhysicalAddress physical_base,
    std::uint64_t length,
    MemoryPermissions permissions) noexcept {
    const MemoryAccess access = MemoryAccess::Write;
    const MemoryAccessContext context = MemoryAccessContext::Map;

    if (length == 0U)
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, virtual_base.value, length);

    std::uint64_t virtual_end = 0U;
    std::uint64_t physical_end = 0U;
    if (!checked_range_end(virtual_base.value, length, virtual_end) ||
        !checked_range_end(physical_base.value, length, physical_end)) {
        return MemoryResult::failure(MemoryFault::AddressOverflow, access, context, virtual_base.value, length);
    }

    // Coverage is checked first so a range crossing a region boundary reports
    // Unmapped even if it also touches an existing mapping.
    if (!physical_.region_covers(physical_base, length))
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, virtual_base.value, length);

    try {
        for (const auto& mapping : mappings_) {
            std::uint64_t existing_virtual_end = 0U;
            std::uint64_t existing_physical_end = 0U;
            if (!mapping_virtual_end(mapping, existing_virtual_end) ||
                !mapping_physical_end(mapping, existing_physical_end)) {
                return MemoryResult::failure(MemoryFault::InvalidRange, access, context, virtual_base.value, length);
            }
            if (mapping.base.value < virtual_end && virtual_base.value < existing_virtual_end)
                return MemoryResult::failure(MemoryFault::InvalidRange, access, context, virtual_base.value, length);
            if (mapping.physical_base.value < physical_end && physical_base.value < existing_physical_end)
                return MemoryResult::failure(MemoryFault::InvalidRange, access, context, virtual_base.value, length);
        }

        mappings_.push_back(GuestVirtualMapping{virtual_base, physical_base, length, permissions});
    } catch (const std::exception&) {
        return MemoryResult::failure(MemoryFault::ResourceExhausted, access, context, virtual_base.value, length);
    }

    return MemoryResult::success(access, context, virtual_base.value, length);
}

MemoryResult GuestAddressSpace::unmap(GuestVirtualAddress virtual_base, std::uint64_t length) noexcept {
    const MemoryAccess access = MemoryAccess::Write;
    const MemoryAccessContext context = MemoryAccessContext::Unmap;

    if (length == 0U)
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, virtual_base.value, length);

    std::uint64_t virtual_end = 0U;
    if (!checked_range_end(virtual_base.value, length, virtual_end))
        return MemoryResult::failure(MemoryFault::AddressOverflow, access, context, virtual_base.value, length);
    (void)virtual_end;

    const auto match = std::find_if(
        mappings_.begin(), mappings_.end(),
        [&](const GuestVirtualMapping& mapping) {
            return mapping.base.value == virtual_base.value && mapping.length == length;
        });
    if (match == mappings_.end())
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, virtual_base.value, length);

    mappings_.erase(match);
    return MemoryResult::success(access, context, virtual_base.value, length);
}

MemoryResult GuestAddressSpace::translate(
    GuestVirtualAddress address,
    std::uint64_t length,
    MemoryAccess access,
    MemoryAccessContext context,
    GuestPhysicalAddress& translated) const noexcept {
    if (length == 0U)
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, 0U);

    std::uint64_t end = 0U;
    if (!checked_range_end(address.value, length, end))
        return MemoryResult::failure(MemoryFault::AddressOverflow, access, context, address.value, length);

    const GuestVirtualMapping* mapping = nullptr;
    for (const auto& candidate : mappings_) {
        std::uint64_t mapping_end = 0U;
        if (!mapping_virtual_end(candidate, mapping_end))
            continue;
        if (address.value >= candidate.base.value && end <= mapping_end) {
            mapping = &candidate;
            break;
        }
    }
    if (mapping == nullptr)
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);
    if (!mapping_allows(mapping->permissions, access))
        return MemoryResult::failure(MemoryFault::AccessDenied, access, context, address.value, length);

    const std::uint64_t offset = address.value - mapping->base.value;
    const std::uint64_t physical_value = mapping->physical_base.value + offset;
    std::uint64_t physical_end = 0U;
    if (!checked_range_end(physical_value, length, physical_end))
        return MemoryResult::failure(MemoryFault::AddressOverflow, access, context, address.value, length);

    const GuestPhysicalAddress physical{physical_value};
    const GuestPhysicalRegion region = physical_.region_at(physical);
    std::uint64_t region_end = 0U;
    if (!checked_range_end(region.base.value, region.length, region_end) ||
        physical_value < region.base.value || physical_end > region_end ||
        region.kind == RegionKind::Reserved) {
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);
    }
    if (!region_allows(region, access))
        return MemoryResult::failure(MemoryFault::AccessDenied, access, context, address.value, length);

    translated = physical;
    return MemoryResult::success(
        access, context, address.value, length, length, physical_value, region.base.value);
}

MemoryResult GuestAddressSpace::fetch(GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept {
    const MemoryAccess access = MemoryAccess::Execute;
    const MemoryAccessContext context = MemoryAccessContext::Fetch;

    std::uint64_t length = 0U;
    if (!span_length(destination.size(), length))
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, 0U);
    if (length == 0U || length > kMaxFetchLength)
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, length);

    GuestPhysicalAddress physical{};
    const MemoryResult translation = translate(address, length, access, context, physical);
    if (!translation)
        return translation;

    const GuestPhysicalRegion region = physical_.region_at(physical);
    if (region.kind == RegionKind::Device)
        return MemoryResult::failure(MemoryFault::DeviceAccessRequired, access, context, address.value, length);
    if (region.kind == RegionKind::Reserved)
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);

    const MemoryResult physical_read = physical_.read(physical, destination, context);
    return rebase_result(
        physical_read, access, context, address.value, length, physical.value, region.base.value);
}

MemoryResult GuestAddressSpace::read(GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept {
    const MemoryAccess access = MemoryAccess::Read;
    const MemoryAccessContext context = MemoryAccessContext::Data;

    std::uint64_t length = 0U;
    if (!span_length(destination.size(), length))
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, 0U);
    if (length == 0U)
        return MemoryResult::success(access, context, address.value, 0U, 0U);

    GuestPhysicalAddress physical{};
    const MemoryResult translation = translate(address, length, access, context, physical);
    if (!translation)
        return translation;

    const GuestPhysicalRegion region = physical_.region_at(physical);
    if (region.kind == RegionKind::Device)
        return dispatch_device_read(region, physical, address.value, destination);
    if (region.kind == RegionKind::Reserved)
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);

    const MemoryResult physical_read = physical_.read(physical, destination, context);
    return rebase_result(
        physical_read, access, context, address.value, length, physical.value, region.base.value);
}

MemoryResult GuestAddressSpace::write(GuestVirtualAddress address, std::span<const std::uint8_t> source) noexcept {
    const MemoryAccess access = MemoryAccess::Write;
    const MemoryAccessContext context = MemoryAccessContext::Data;

    std::uint64_t length = 0U;
    if (!span_length(source.size(), length))
        return MemoryResult::failure(MemoryFault::InvalidRange, access, context, address.value, 0U);
    if (length == 0U)
        return MemoryResult::success(access, context, address.value, 0U, 0U);

    GuestPhysicalAddress physical{};
    const MemoryResult translation = translate(address, length, access, context, physical);
    if (!translation)
        return translation;

    const GuestPhysicalRegion region = physical_.region_at(physical);
    if (region.kind == RegionKind::Device)
        return dispatch_device_write(region, physical, address.value, source);
    if (region.kind == RegionKind::Reserved)
        return MemoryResult::failure(MemoryFault::Unmapped, access, context, address.value, length);

    const MemoryResult physical_write = physical_.write(physical, source, context);
    return rebase_result(
        physical_write, access, context, address.value, length, physical.value, region.base.value);
}

MemoryResult GuestAddressSpace::dispatch_device_read(
    const GuestPhysicalRegion& region,
    GuestPhysicalAddress physical_address,
    std::uint64_t virtual_address,
    std::span<std::uint8_t> destination) noexcept {
    const MemoryAccess access = MemoryAccess::Read;
    const MemoryAccessContext context = MemoryAccessContext::DeviceDispatch;
    const std::uint64_t length = static_cast<std::uint64_t>(destination.size());

    if (dispatcher_ == nullptr)
        return MemoryResult::failure(MemoryFault::DeviceAccessRequired, access, context, virtual_address, length);

    std::vector<std::uint8_t> buffer;
    try {
        buffer.assign(destination.size(), std::uint8_t{0U});
    } catch (const std::exception&) {
        return MemoryResult::failure(MemoryFault::ResourceExhausted, access, context, virtual_address, length);
    }

    const DeviceAccessRequest request{
        region.base,
        physical_address.value - region.base.value,
        access,
        context,
        std::span<std::uint8_t>(buffer.data(), buffer.size())};
    const MemoryResult dispatched = dispatcher_->dispatch(request);
    if (!dispatched.ok || dispatched.fault != MemoryFault::None || dispatched.completed_bytes != length)
        return MemoryResult::failure(dispatch_fault(dispatched), access, context, virtual_address, length);

    for (std::size_t i = 0U; i < destination.size(); ++i)
        destination[i] = buffer[i];
    return MemoryResult::success(
        access, context, virtual_address, length, length,
        physical_address.value, region.base.value);
}

MemoryResult GuestAddressSpace::dispatch_device_write(
    const GuestPhysicalRegion& region,
    GuestPhysicalAddress physical_address,
    std::uint64_t virtual_address,
    std::span<const std::uint8_t> source) noexcept {
    const MemoryAccess access = MemoryAccess::Write;
    const MemoryAccessContext context = MemoryAccessContext::DeviceDispatch;
    const std::uint64_t length = static_cast<std::uint64_t>(source.size());

    if (dispatcher_ == nullptr)
        return MemoryResult::failure(MemoryFault::DeviceAccessRequired, access, context, virtual_address, length);

    std::vector<std::uint8_t> buffer;
    try {
        buffer.assign(source.begin(), source.end());
    } catch (const std::exception&) {
        return MemoryResult::failure(MemoryFault::ResourceExhausted, access, context, virtual_address, length);
    }

    const DeviceAccessRequest request{
        region.base,
        physical_address.value - region.base.value,
        access,
        context,
        std::span<std::uint8_t>(buffer.data(), buffer.size())};
    const MemoryResult dispatched = dispatcher_->dispatch(request);
    if (!dispatched.ok || dispatched.fault != MemoryFault::None || dispatched.completed_bytes != length)
        return MemoryResult::failure(dispatch_fault(dispatched), access, context, virtual_address, length);

    return MemoryResult::success(
        access, context, virtual_address, length, length,
        physical_address.value, region.base.value);
}

} // namespace xbridge::memory
