#pragma once

#include "xbridge/loader/pe_image.hpp"
#include "xbridge/memory/guest_address_space.hpp"
#include "xbridge/memory/guest_memory_result.hpp"
#include "xbridge/memory/guest_physical_memory.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace xbridge::loader {

// State machine for bounded synthetic PE images. There is no execution entry
// point: prepare() only records that mapping and relocations completed.
class ExecutableLoader {
public:
    ExecutableLoader(memory::GuestAddressSpace& address_space,
                     memory::GuestPhysicalMemory& physical_memory) noexcept;
    ExecutableLoader(memory::GuestPhysicalMemory& physical_memory,
                     memory::GuestAddressSpace& address_space) noexcept;

    [[nodiscard]] LoaderState state() const noexcept { return state_; }
    [[nodiscard]] LoaderState loader_state() const noexcept { return state_; }
    [[nodiscard]] LoadError last_error() const noexcept { return last_error_; }
    [[nodiscard]] const PeImageInfo& image_info() const noexcept { return image_info_; }
    [[nodiscard]] const LoadOptions& options() const noexcept { return options_; }
    [[nodiscard]] std::uint64_t image_base() const noexcept { return effective_base_; }
    [[nodiscard]] std::uint64_t entry_point() const noexcept { return image_info_.entry_point; }

    [[nodiscard]] const std::vector<VirtualMapRecord>& virtual_mappings() const noexcept {
        return virtual_maps_;
    }
    [[nodiscard]] const std::vector<PhysicalRegionRecord>& physical_regions() const noexcept {
        return physical_regions_;
    }
    [[nodiscard]] const std::vector<PhysicalRegionRecord>& inaccessible_physical_regions()
        const noexcept {
        return inaccessible_regions_;
    }
    [[nodiscard]] memory::MemoryResult first_memory_result() const noexcept {
        return first_memory_result_;
    }

    [[nodiscard]] LoadError validate(std::span<const std::uint8_t> bytes,
                                     const LoadOptions& options = {}) noexcept;
    [[nodiscard]] LoadError map() noexcept;
    [[nodiscard]] LoadError relocate() noexcept;
    [[nodiscard]] LoadError prepare() noexcept;
    [[nodiscard]] LoadError unload() noexcept;
    [[nodiscard]] LoadError reset() noexcept { return unload(); }

private:
    [[nodiscard]] LoadError fail(LoadError error) noexcept;
    [[nodiscard]] LoadError fail_memory(const memory::MemoryResult& result) noexcept;
    [[nodiscard]] bool find_mapped_range(std::uint64_t virtual_base, std::uint64_t length,
                                         VirtualMapRecord& record) const noexcept;
    void rollback_mappings(const memory::MemoryResult* cause) noexcept;

    memory::GuestAddressSpace& address_space_;
    memory::GuestPhysicalMemory& physical_memory_;
    LoaderState state_{LoaderState::Unloaded};
    LoadError last_error_{LoadError::None};
    LoadOptions options_{};
    PeImageInfo image_info_{};
    std::vector<std::uint8_t> image_bytes_{};
    std::vector<VirtualMapRecord> virtual_maps_{};
    std::vector<PhysicalRegionRecord> physical_regions_{};
    std::vector<PhysicalRegionRecord> inaccessible_regions_{};
    memory::MemoryResult first_memory_result_{};
    std::uint64_t effective_base_{0U};
    std::uint64_t pe_image_base_{0U};
};

} // namespace xbridge::loader
