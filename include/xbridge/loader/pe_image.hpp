#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

// Read-only metadata for bounded synthetic PE32+/AMD64 test images. This is a
// loader input description only. Recording an entry point never executes it and
// makes no claim about Xbox compatibility.
namespace xbridge::loader {

enum class LoaderState { Unloaded, Validated, Mapped, Relocated, Ready, Failed };

enum class LoadError : unsigned {
    None = 0U,
    InvalidState,
    InvalidHeader,
    UnsupportedImage,
    TruncatedData,
    LimitExceeded,
    Overflow,
    AlignmentError,
    RangeError,
    Overlap,
    InvalidEntryPoint,
    MemoryFailure,
    UnsupportedRelocation,
    InvalidRelocation,
    PhysicalBackingInaccessible,

    // Stable aliases retained for callers using the longer diagnostic names.
    InvalidAlignment = AlignmentError,
    InvalidRange = RangeError,
    UnsupportedFormat = UnsupportedImage,
    Truncated = TruncatedData,
    OutOfMemory = LimitExceeded,
    UnknownRelocation = UnsupportedRelocation,
    InvalidRelocationType = UnsupportedRelocation
};

// Loader bounds are implementation limits, not Xbox or Windows constants.
// Alias fields of zero mean "not set"; effective limits use the minimum of all
// explicitly set values.
struct LoaderLimits {
    std::size_t max_image_bytes{16U * 1024U * 1024U};
    std::size_t max_section_bytes{64U * 1024U * 1024U};
    std::size_t max_sections{96U};
    std::size_t max_relocations{65536U};
    std::uint64_t max_virtual_address{0x00007FFFFFFFFFFFull};
    std::uint64_t max_physical_address{0x00007FFFFFFFFFFFull};

    std::size_t max_bytes{0U};
    std::size_t max_file_bytes{0U};
    std::size_t max_image_size{0U};
    std::size_t max_section_count{0U};
    std::size_t max_relocation_entries{0U};
    std::uint64_t max_virtual_size{0U};
};

struct LoadOptions {
    LoaderLimits limits{};
    // Zero means "use the PE ImageBase". A nonzero value is the effective base;
    // the DIR64 delta is override minus the PE ImageBase.
    std::uint64_t image_base_override{0U};
    std::uint64_t image_base{0U};      // alias override
    std::uint64_t base_address{0U};    // alias override
};

inline constexpr std::uint32_t kSectionMemRead = 0x40000000U;
inline constexpr std::uint32_t kSectionMemWrite = 0x80000000U;
inline constexpr std::uint32_t kSectionMemExecute = 0x20000000U;

struct PeSectionInfo {
    std::string name;
    std::uint32_t virtual_address{0U};
    std::uint32_t virtual_size{0U};
    std::uint32_t raw_size{0U};
    std::uint32_t raw_offset{0U};
    std::uint32_t characteristics{0U};

    // Aliases filled with the same values for direct PE-field spelling.
    std::uint32_t size_of_raw_data{0U};
    std::uint32_t pointer_to_raw_data{0U};
};

struct PeRelocationInfo {
    std::uint32_t page_rva{0U};
    std::uint16_t offset{0U};
    std::uint16_t type{0U};
    std::uint32_t target_rva{0U};

    // Aliases filled with the same values for direct PE-field spelling.
    std::uint32_t relocation_page_rva{0U};
    std::uint16_t relocation_offset{0U};
};

struct PeImageInfo {
    std::uint16_t machine{0U};
    std::uint16_t magic{0U};
    std::uint64_t image_base{0U};
    std::uint32_t entry_point_rva{0U};
    std::uint64_t entry_point{0U};
    std::uint32_t size_of_image{0U};
    std::uint32_t size_of_headers{0U};
    std::uint32_t section_alignment{0U};
    std::uint32_t file_alignment{0U};

    std::vector<PeSectionInfo> sections;
    std::vector<PeRelocationInfo> relocations;
    std::vector<PeRelocationInfo> relocation_entries; // same values as relocations
    std::vector<std::string> imports;
    std::vector<std::string> dependencies;
    std::vector<std::string> exports;
    std::vector<std::string> relocation_types;
};

struct VirtualMapRecord {
    std::uint64_t virtual_base{0U};
    std::uint64_t physical_base{0U};
    std::uint64_t length{0U};
};

struct PhysicalRegionRecord {
    std::uint64_t base{0U};
    std::uint64_t length{0U};
    bool inaccessible{false};
};

[[nodiscard]] bool can_transition(LoaderState from, LoaderState to) noexcept;
[[nodiscard]] const char* to_string(LoaderState state) noexcept;
[[nodiscard]] const char* to_string(LoadError error) noexcept;

} // namespace xbridge::loader
