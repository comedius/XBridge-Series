#include "xbridge/loader/executable_loader.hpp"

#include "pe_image_internal.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <utility>

namespace xbridge::loader {
namespace {

using memory::GuestPhysicalAddress;
using memory::GuestVirtualAddress;
using memory::MemoryAccessContext;
using memory::MemoryResult;

[[nodiscard]] memory::MemoryPermissions permissions_from_characteristics(
    std::uint32_t characteristics) noexcept {
    return memory::MemoryPermissions{
        (characteristics & kSectionMemRead) != 0U,
        (characteristics & kSectionMemWrite) != 0U,
        (characteristics & kSectionMemExecute) != 0U};
}

[[nodiscard]] bool checked_add_u64(std::uint64_t left, std::uint64_t right,
                                   std::uint64_t& result) noexcept {
    if (right > (std::numeric_limits<std::uint64_t>::max)() - left) {
        return false;
    }
    result = left + right;
    return true;
}

[[nodiscard]] std::uint64_t section_mapped_size(const PeSectionInfo& section) noexcept {
    return section.virtual_size != 0U ? section.virtual_size : section.raw_size;
}

void store_le_u64(std::array<std::uint8_t, 8U>& bytes, std::uint64_t value) noexcept {
    for (std::size_t i = 0U; i < bytes.size(); ++i) {
        bytes[i] = static_cast<std::uint8_t>((value >> (i * 8U)) & 0xFFU);
    }
}

[[nodiscard]] std::uint64_t load_le_u64(const std::array<std::uint8_t, 8U>& bytes) noexcept {
    std::uint64_t value = 0U;
    for (std::size_t i = 0U; i < bytes.size(); ++i) {
        value |= static_cast<std::uint64_t>(bytes[i]) << (i * 8U);
    }
    return value;
}

[[nodiscard]] bool apply_delta(std::uint64_t value, std::int64_t delta,
                               std::uint64_t& result) noexcept {
    if (delta >= 0) {
        return checked_add_u64(value, static_cast<std::uint64_t>(delta), result);
    }
    const std::uint64_t magnitude = static_cast<std::uint64_t>(-(delta + 1U)) + 1U;
    if (value < magnitude) {
        return false;
    }
    result = value - magnitude;
    return true;
}

} // namespace

ExecutableLoader::ExecutableLoader(memory::GuestAddressSpace& address_space,
                                   memory::GuestPhysicalMemory& physical_memory) noexcept
    : address_space_(address_space), physical_memory_(physical_memory) {}

ExecutableLoader::ExecutableLoader(memory::GuestPhysicalMemory& physical_memory,
                                   memory::GuestAddressSpace& address_space) noexcept
    : address_space_(address_space), physical_memory_(physical_memory) {}

LoadError ExecutableLoader::fail(LoadError error) noexcept {
    last_error_ = error;
    state_ = LoaderState::Failed;
    return error;
}

LoadError ExecutableLoader::fail_memory(const MemoryResult& result) noexcept {
    rollback_mappings(&result);
    return fail(LoadError::MemoryFailure);
}

void ExecutableLoader::rollback_mappings(const MemoryResult* cause) noexcept {
    if (cause != nullptr) {
        first_memory_result_ = *cause;
    }
    for (auto it = virtual_maps_.rbegin(); it != virtual_maps_.rend(); ++it) {
        const MemoryResult result = address_space_.unmap(
            GuestVirtualAddress{it->virtual_base}, it->length);
        if (!result.ok && !first_memory_result_.ok) {
            first_memory_result_ = result;
        }
    }
    virtual_maps_.clear();

    for (PhysicalRegionRecord& region : physical_regions_) {
        region.inaccessible = true;
        if (std::find_if(inaccessible_regions_.begin(), inaccessible_regions_.end(),
                         [&region](const PhysicalRegionRecord& existing) {
                             return existing.base == region.base &&
                                    existing.length == region.length;
                         }) == inaccessible_regions_.end()) {
            inaccessible_regions_.push_back(region);
        }
    }
}

bool ExecutableLoader::find_mapped_range(std::uint64_t virtual_base, std::uint64_t length,
                                          VirtualMapRecord& record) const noexcept {
    for (const VirtualMapRecord& candidate : virtual_maps_) {
        if (virtual_base < candidate.virtual_base) {
            continue;
        }
        const std::uint64_t offset = virtual_base - candidate.virtual_base;
        if (offset <= candidate.length && length <= candidate.length - offset) {
            record = candidate;
            return true;
        }
    }
    return false;
}

LoadError ExecutableLoader::validate(std::span<const std::uint8_t> bytes,
                                     const LoadOptions& options) noexcept {
    if (state_ != LoaderState::Unloaded) {
        return fail(LoadError::InvalidState);
    }
    try {
        detail::ParsedImage parsed = detail::parse_synthetic_pe(bytes, options);
        if (parsed.error != LoadError::None) {
            return fail(parsed.error);
        }
        image_info_ = parsed.info;
        image_bytes_ = std::move(parsed.bytes);
        options_ = options;
        pe_image_base_ = parsed.info.image_base;
        effective_base_ = parsed.info.entry_point >= parsed.info.entry_point_rva
                              ? parsed.info.entry_point - parsed.info.entry_point_rva
                              : parsed.info.image_base;
        state_ = LoaderState::Validated;
        last_error_ = LoadError::None;
        return LoadError::None;
    } catch (...) {
        return fail(LoadError::LimitExceeded);
    }
}

LoadError ExecutableLoader::map() noexcept {
    if (state_ != LoaderState::Validated) {
        return fail(LoadError::InvalidState);
    }

    try {
        const std::uint32_t size_of_image = image_info_.size_of_image;
        std::uint64_t image_end = 0U;
        if (!checked_add_u64(effective_base_, size_of_image, image_end) ||
            image_end > options_.limits.max_virtual_address) {
            return fail(LoadError::Overflow);
        }

        bool entry_mapped = false;
        for (const PeSectionInfo& section : image_info_.sections) {
            const std::uint32_t characteristics = section.characteristics;
            if ((characteristics & kSectionMemExecute) == 0U ||
                image_info_.entry_point_rva < section.virtual_address) {
                continue;
            }
            const std::uint64_t delta = image_info_.entry_point_rva - section.virtual_address;
            if (delta < section_mapped_size(section)) {
                entry_mapped = true;
                break;
            }
        }
        if (!entry_mapped) {
            return fail(LoadError::InvalidEntryPoint);
        }

        std::uint64_t next_physical = effective_base_;
        if (next_physical > options_.limits.max_physical_address) {
            return fail(LoadError::Overflow);
        }

        for (const PeSectionInfo& section : image_info_.sections) {
            const std::uint64_t length = section_mapped_size(section);
            if (length == 0U) {
                continue;
            }
            if (length > options_.limits.max_section_bytes) {
                return fail_memory(MemoryResult::failure(
                    memory::MemoryFault::ResourceExhausted, memory::MemoryAccess::Write,
                    MemoryAccessContext::Map, effective_base_ + section.virtual_address, length));
            }

            std::uint64_t virtual_base = 0U;
            if (!checked_add_u64(effective_base_, section.virtual_address, virtual_base)) {
                rollback_mappings(nullptr);
                return fail(LoadError::Overflow);
            }
            std::uint64_t physical_end = 0U;
            if (!checked_add_u64(next_physical, length, physical_end) ||
                physical_end > options_.limits.max_physical_address) {
                rollback_mappings(nullptr);
                return fail(LoadError::Overflow);
            }

            const MemoryResult added = physical_memory_.add_region(
                GuestPhysicalAddress{next_physical}, length, memory::RegionKind::Normal,
                memory::MemoryPermissions{true, true, false});
            if (!added.ok) {
                return fail_memory(added);
            }
            physical_regions_.push_back(
                PhysicalRegionRecord{next_physical, length, false});

            const std::uint64_t raw_size = std::min<std::uint64_t>(section.raw_size, length);
            if (raw_size > 0U) {
                if (section.raw_offset > image_bytes_.size() ||
                    raw_size > image_bytes_.size() - section.raw_offset) {
                    return fail_memory(MemoryResult::failure(
                        memory::MemoryFault::InvalidRange, memory::MemoryAccess::Write,
                        MemoryAccessContext::Map, virtual_base, raw_size));
                }
                const std::span<const std::uint8_t> source(
                    image_bytes_.data() + section.raw_offset,
                    static_cast<std::size_t>(raw_size));
                const MemoryResult written = physical_memory_.write(
                    GuestPhysicalAddress{next_physical}, source, MemoryAccessContext::Bulk);
                if (!written.ok) {
                    return fail_memory(written);
                }
            }

            const memory::MemoryPermissions permissions =
                permissions_from_characteristics(section.characteristics);
            const MemoryResult mapped = address_space_.map(
                GuestVirtualAddress{virtual_base}, GuestPhysicalAddress{next_physical}, length,
                permissions);
            if (!mapped.ok) {
                return fail_memory(mapped);
            }
            virtual_maps_.push_back(VirtualMapRecord{virtual_base, next_physical, length});
            next_physical = physical_end;
        }

        state_ = LoaderState::Mapped;
        last_error_ = LoadError::None;
        return LoadError::None;
    } catch (...) {
        return fail_memory(MemoryResult::failure(memory::MemoryFault::ResourceExhausted,
                                                  memory::MemoryAccess::Write,
                                                  MemoryAccessContext::Map));
    }
}

LoadError ExecutableLoader::relocate() noexcept {
    if (state_ != LoaderState::Mapped) {
        return fail(LoadError::InvalidState);
    }

    const std::uint64_t override_base = options_.image_base_override != 0U
                                            ? options_.image_base_override
                                            : (options_.image_base != 0U ? options_.image_base
                                                                         : options_.base_address);
    const std::int64_t delta = override_base == 0U
                                   ? 0
                                   : static_cast<std::int64_t>(override_base) -
                                         static_cast<std::int64_t>(pe_image_base_);

    struct PendingWrite {
        std::uint64_t address{0U};
        std::array<std::uint8_t, 8U> bytes{};
    };
    std::vector<PendingWrite> writes;
    try {
        writes.reserve(image_info_.relocations.size());

        // Phase one: validate every relocation and compute every value without
        // writing any target. An unsupported type aborts before mutation.
        for (const PeRelocationInfo& relocation : image_info_.relocations) {
            if (relocation.type == 0U) {
                continue;
            }
            if (relocation.type != 10U) {
                return fail(LoadError::UnsupportedRelocation);
            }
            std::uint64_t target = 0U;
            if (!checked_add_u64(effective_base_, relocation.target_rva, target)) {
                return fail(LoadError::Overflow);
            }
            VirtualMapRecord record{};
            if (!find_mapped_range(target, 8U, record)) {
                return fail(LoadError::InvalidRelocation);
            }
            std::array<std::uint8_t, 8U> bytes{};
            const MemoryResult read = address_space_.read(
                GuestVirtualAddress{target}, std::span<std::uint8_t>(bytes.data(), bytes.size()));
            if (!read.ok) {
                return fail_memory(read);
            }
            std::uint64_t updated = 0U;
            if (!apply_delta(load_le_u64(bytes), delta, updated)) {
                return fail(LoadError::Overflow);
            }
            store_le_u64(bytes, updated);
            writes.push_back(PendingWrite{target, bytes});
        }

        // Phase two: all entries are known-good, so commit the prepared values.
        for (const PendingWrite& write : writes) {
            const MemoryResult result = address_space_.write(
                GuestVirtualAddress{write.address},
                std::span<const std::uint8_t>(write.bytes.data(), write.bytes.size()));
            if (!result.ok) {
                return fail_memory(result);
            }
        }
    } catch (...) {
        return fail_memory(MemoryResult::failure(memory::MemoryFault::ResourceExhausted,
                                                  memory::MemoryAccess::Write,
                                                  MemoryAccessContext::Bulk));
    }

    state_ = LoaderState::Relocated;
    last_error_ = LoadError::None;
    return LoadError::None;
}

LoadError ExecutableLoader::prepare() noexcept {
    if (state_ != LoaderState::Relocated) {
        return fail(LoadError::InvalidState);
    }
    VirtualMapRecord record{};
    if (!find_mapped_range(image_info_.entry_point, 1U, record)) {
        return fail(LoadError::InvalidEntryPoint);
    }
    state_ = LoaderState::Ready;
    last_error_ = LoadError::None;
    return LoadError::None;
}

LoadError ExecutableLoader::unload() noexcept {
    if (state_ == LoaderState::Unloaded) {
        return LoadError::None;
    }
    bool have_unmap_failure = false;
    MemoryResult first_failure{};
    for (auto it = virtual_maps_.rbegin(); it != virtual_maps_.rend(); ++it) {
        const MemoryResult result =
            address_space_.unmap(GuestVirtualAddress{it->virtual_base}, it->length);
        if (!result.ok && !have_unmap_failure) {
            first_failure = result;
            have_unmap_failure = true;
        }
    }
    virtual_maps_.clear();
    for (PhysicalRegionRecord& region : physical_regions_) {
        region.inaccessible = true;
        if (std::find_if(inaccessible_regions_.begin(), inaccessible_regions_.end(),
                         [&region](const PhysicalRegionRecord& existing) {
                             return existing.base == region.base &&
                                    existing.length == region.length;
                         }) == inaccessible_regions_.end()) {
            inaccessible_regions_.push_back(region);
        }
    }
    physical_regions_.clear();
    image_bytes_.clear();
    effective_base_ = 0U;
    pe_image_base_ = 0U;

    if (have_unmap_failure) {
        first_memory_result_ = first_failure;
        return fail(LoadError::MemoryFailure);
    }
    state_ = LoaderState::Unloaded;
    last_error_ = LoadError::None;
    return LoadError::None;
}

} // namespace xbridge::loader
