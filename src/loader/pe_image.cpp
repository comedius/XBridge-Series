#include "pe_image_internal.hpp"

#include <algorithm>
#include <charconv>
#include <limits>
#include <string>
#include <utility>

namespace xbridge::loader {
namespace {

using ByteSpan = std::span<const std::uint8_t>;

template <typename T>
[[nodiscard]] T min_nonzero(T primary, T alias) noexcept {
    if (alias == T{}) {
        return primary;
    }
    return primary < alias ? primary : alias;
}

template <typename T>
[[nodiscard]] T min_value(T first, T second) noexcept {
    return first < second ? first : second;
}

[[nodiscard]] bool is_power_of_two(std::uint32_t value) noexcept {
    return value != 0U && (value & (value - 1U)) == 0U;
}

[[nodiscard]] bool checked_add_u64(std::uint64_t left, std::uint64_t right,
                                   std::uint64_t& result) noexcept {
    if (right > (std::numeric_limits<std::uint64_t>::max)() - left) {
        return false;
    }
    result = left + right;
    return true;
}

class Reader {
public:
    explicit Reader(ByteSpan bytes) noexcept : bytes_(bytes) {}

    [[nodiscard]] bool has(std::size_t offset, std::size_t length) const noexcept {
        return offset <= bytes_.size() && length <= bytes_.size() - offset;
    }

    [[nodiscard]] bool u8(std::size_t offset, std::uint8_t& value) const noexcept {
        if (!has(offset, 1U)) {
            return false;
        }
        value = bytes_[offset];
        return true;
    }

    [[nodiscard]] bool u16(std::size_t offset, std::uint16_t& value) const noexcept {
        if (!has(offset, 2U)) {
            return false;
        }
        value = static_cast<std::uint16_t>(bytes_[offset]) |
                static_cast<std::uint16_t>(static_cast<std::uint16_t>(bytes_[offset + 1U]) << 8U);
        return true;
    }

    [[nodiscard]] bool u32(std::size_t offset, std::uint32_t& value) const noexcept {
        if (!has(offset, 4U)) {
            return false;
        }
        value = static_cast<std::uint32_t>(bytes_[offset]) |
                (static_cast<std::uint32_t>(bytes_[offset + 1U]) << 8U) |
                (static_cast<std::uint32_t>(bytes_[offset + 2U]) << 16U) |
                (static_cast<std::uint32_t>(bytes_[offset + 3U]) << 24U);
        return true;
    }

    [[nodiscard]] bool u64(std::size_t offset, std::uint64_t& value) const noexcept {
        std::uint32_t low = 0U;
        std::uint32_t high = 0U;
        if (!u32(offset, low) || !u32(offset + 4U, high)) {
            return false;
        }
        value = static_cast<std::uint64_t>(low) |
                (static_cast<std::uint64_t>(high) << 32U);
        return true;
    }

    [[nodiscard]] std::size_t size() const noexcept { return bytes_.size(); }

private:
    ByteSpan bytes_;
};

[[nodiscard]] std::string section_name(const Reader& reader, std::size_t offset) {
    std::string name;
    for (std::size_t i = 0U; i < 8U; ++i) {
        std::uint8_t byte = 0U;
        if (!reader.u8(offset + i, byte) || byte == 0U) {
            break;
        }
        if (byte < 0x20U || byte > 0x7eU) {
            name.push_back('?');
        } else {
            name.push_back(static_cast<char>(byte));
        }
    }
    return name;
}

[[nodiscard]] bool read_c_string(const Reader& reader, std::size_t offset,
                                 std::size_t max_length, std::string& value) noexcept {
    if (!reader.has(offset, 1U)) {
        return false;
    }
    value.clear();
    for (std::size_t i = 0U; i < max_length; ++i) {
        std::uint8_t byte = 0U;
        if (!reader.u8(offset + i, byte)) {
            return false;
        }
        if (byte == 0U) {
            return !value.empty();
        }
        if (byte < 0x20U || byte > 0x7eU) {
            return false;
        }
        value.push_back(static_cast<char>(byte));
    }
    return false;
}

[[nodiscard]] bool map_rva_to_offset(std::uint32_t rva,
                                     const PeImageInfo& info,
                                     std::size_t& offset) noexcept {
    if (rva < info.size_of_headers && static_cast<std::size_t>(rva) <= info.size_of_headers) {
        offset = static_cast<std::size_t>(rva);
        return true;
    }
    for (const PeSectionInfo& section : info.sections) {
        const std::uint64_t span = section.virtual_size != 0U
                                       ? static_cast<std::uint64_t>(section.virtual_size)
                                       : static_cast<std::uint64_t>(section.raw_size);
        const std::uint64_t begin = section.virtual_address;
        if (span == 0U || static_cast<std::uint64_t>(rva) < begin) {
            continue;
        }
        const std::uint64_t delta = static_cast<std::uint64_t>(rva) - begin;
        if (delta >= span || delta >= section.raw_size) {
            continue;
        }
        std::uint64_t file_offset = 0U;
        if (!checked_add_u64(section.raw_offset, delta, file_offset) ||
            file_offset > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)())) {
            return false;
        }
        offset = static_cast<std::size_t>(file_offset);
        return true;
    }
    return false;
}

[[nodiscard]] const char* relocation_type_name(std::uint16_t type) noexcept {
    switch (type) {
    case 0U: return "ABSOLUTE";
    case 10U: return "DIR64";
    default: return "unsupported";
    }
}

[[nodiscard]] std::string relocation_type_label(std::uint16_t type) {
    return std::to_string(type) + " (" + relocation_type_name(type) + ")";
}

void add_unique(std::vector<std::string>& values, const std::string& value) {
    if (std::find(values.begin(), values.end(), value) == values.end()) {
        values.push_back(value);
    }
}

void parse_imports(const Reader& reader,
                   const PeImageInfo& info,
                   std::uint32_t directory_rva,
                   std::uint32_t directory_size,
                   std::size_t max_names,
                   PeImageInfo& result) noexcept {
    if (directory_rva == 0U || directory_size < 20U) {
        return;
    }
    std::size_t offset = 0U;
    if (!map_rva_to_offset(directory_rva, info, offset)) {
        return;
    }
    const std::size_t available = std::min<std::size_t>(
        directory_size, reader.size() > offset ? reader.size() - offset : 0U);
    std::size_t used = 0U;
    while (used + 20U <= available && result.dependencies.size() < max_names) {
        std::uint32_t name_rva = 0U;
        if (!reader.u32(offset + used + 12U, name_rva)) {
            return;
        }
        if (name_rva == 0U) {
            return;
        }
        std::size_t name_offset = 0U;
        std::string name;
        if (map_rva_to_offset(name_rva, info, name_offset) &&
            read_c_string(reader, name_offset, max_names, name)) {
            add_unique(result.imports, name);
            add_unique(result.dependencies, name);
        }
        used += 20U;
    }
}

void parse_exports(const Reader& reader,
                   const PeImageInfo& info,
                   std::uint32_t directory_rva,
                   std::uint32_t directory_size,
                   std::size_t max_names,
                   PeImageInfo& result) noexcept {
    if (directory_rva == 0U || directory_size < 40U) {
        return;
    }
    std::size_t offset = 0U;
    if (!map_rva_to_offset(directory_rva, info, offset) || !reader.has(offset, 40U)) {
        return;
    }
    std::uint32_t name_rva = 0U;
    std::uint32_t name_count = 0U;
    std::uint32_t names_rva = 0U;
    if (!reader.u32(offset + 12U, name_rva) || !reader.u32(offset + 24U, name_count) ||
        !reader.u32(offset + 32U, names_rva)) {
        return;
    }
    std::size_t dll_offset = 0U;
    std::string dll_name;
    if (map_rva_to_offset(name_rva, info, dll_offset) &&
        read_c_string(reader, dll_offset, max_names, dll_name)) {
        add_unique(result.exports, "export_dll:" + dll_name);
    }
    const std::uint32_t limit = std::min<std::uint32_t>(name_count,
                                                        static_cast<std::uint32_t>(max_names));
    for (std::uint32_t i = 0U; i < limit; ++i) {
        std::uint64_t entry_rva_wide = 0U;
        std::size_t table_offset = 0U;
        if (!checked_add_u64(names_rva, static_cast<std::uint64_t>(i) * 4U, entry_rva_wide) ||
            entry_rva_wide > static_cast<std::uint64_t>((std::numeric_limits<std::uint32_t>::max)()) ||
            !map_rva_to_offset(static_cast<std::uint32_t>(entry_rva_wide), info, table_offset)) {
            continue;
        }
        std::uint32_t string_rva = 0U;
        if (!reader.u32(table_offset, string_rva)) {
            continue;
        }
        std::size_t string_offset = 0U;
        std::string export_name;
        if (map_rva_to_offset(string_rva, info, string_offset) &&
            read_c_string(reader, string_offset, max_names, export_name)) {
            add_unique(result.exports, export_name);
        }
    }
}

LoadError parse_relocations(const Reader& reader,
                            const PeImageInfo& info,
                            std::uint32_t directory_rva,
                            std::uint32_t directory_size,
                            const LoaderLimits& limits,
                            std::size_t max_names,
                            PeImageInfo& result) noexcept {
    if (directory_rva == 0U && directory_size == 0U) {
        return LoadError::None;
    }
    if (directory_rva == 0U || directory_size < 8U) {
        return LoadError::InvalidHeader;
    }
    std::size_t offset = 0U;
    if (!map_rva_to_offset(directory_rva, info, offset)) {
        return LoadError::InvalidRange;
    }
    if (directory_size > reader.size() - offset) {
        return LoadError::TruncatedData;
    }

    std::size_t consumed = 0U;
    while (consumed < directory_size) {
        if (directory_size - consumed < 8U) {
            return LoadError::TruncatedData;
        }
        std::uint32_t page_rva = 0U;
        std::uint32_t block_size = 0U;
        if (!reader.u32(offset + consumed, page_rva) ||
            !reader.u32(offset + consumed + 4U, block_size)) {
            return LoadError::TruncatedData;
        }
        if (block_size < 8U || block_size > directory_size - consumed ||
            ((block_size - 8U) % 2U) != 0U) {
            return LoadError::InvalidHeader;
        }
        const std::size_t entry_count = (block_size - 8U) / 2U;
        if (entry_count > limits.max_relocations - result.relocations.size()) {
            return LoadError::LimitExceeded;
        }
        for (std::size_t i = 0U; i < entry_count; ++i) {
            std::uint16_t entry = 0U;
            if (!reader.u16(offset + consumed + 8U + i * 2U, entry)) {
                return LoadError::TruncatedData;
            }
            const std::uint16_t type = static_cast<std::uint16_t>(entry >> 12U);
            const std::uint16_t low = static_cast<std::uint16_t>(entry & 0x0FFFU);
            std::uint64_t target = 0U;
            if (!checked_add_u64(page_rva, low, target) || target >= info.size_of_image) {
                return LoadError::RangeError;
            }
            if (type == 10U) {
                std::uint64_t extent_end = 0U;
                if (!checked_add_u64(target, 8U, extent_end) || extent_end > info.size_of_image) {
                    return LoadError::RangeError;
                }
            }
            PeRelocationInfo relocation{};
            relocation.page_rva = page_rva;
            relocation.offset = low;
            relocation.type = type;
            relocation.target_rva = static_cast<std::uint32_t>(target);
            relocation.relocation_page_rva = page_rva;
            relocation.relocation_offset = low;
            result.relocations.push_back(relocation);
            if (result.relocation_types.size() < max_names) {
                add_unique(result.relocation_types, relocation_type_label(type));
            }
        }
        consumed += block_size;
    }
    return LoadError::None;
}

} // namespace

namespace detail {

ParsedImage parse_synthetic_pe(ByteSpan bytes, const LoadOptions& options) noexcept {
    ParsedImage parsed{};
    const Reader reader(bytes);
    const LoaderLimits& input_limits = options.limits;
    LoaderLimits limits{};
    limits.max_image_bytes = min_nonzero(input_limits.max_image_bytes, input_limits.max_bytes);
    limits.max_image_bytes = min_nonzero(limits.max_image_bytes, input_limits.max_file_bytes);
    limits.max_image_bytes = min_nonzero(limits.max_image_bytes, input_limits.max_image_size);
    limits.max_section_bytes = min_nonzero(input_limits.max_section_bytes,
                                           static_cast<std::size_t>(input_limits.max_virtual_size));
    limits.max_sections = min_nonzero(input_limits.max_sections, input_limits.max_section_count);
    limits.max_relocations = min_nonzero(input_limits.max_relocations,
                                         input_limits.max_relocation_entries);
    limits.max_virtual_address = input_limits.max_virtual_address;
    limits.max_physical_address = input_limits.max_physical_address;

    auto fail = [&parsed](LoadError error) {
        parsed.error = error;
        return parsed;
    };

    if (bytes.size() > limits.max_image_bytes) {
        return fail(LoadError::LimitExceeded);
    }
    if (!reader.has(0U, 64U)) {
        return fail(LoadError::TruncatedData);
    }
    if (bytes[0U] != 'M' || bytes[1U] != 'Z') {
        return fail(LoadError::InvalidHeader);
    }

    std::uint32_t pe_offset = 0U;
    if (!reader.u32(0x3CU, pe_offset) || pe_offset > reader.size() ||
        reader.size() - pe_offset < 24U) {
        return fail(LoadError::TruncatedData);
    }
    if (pe_offset > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)()) - 24U) {
        return fail(LoadError::Overflow);
    }
    const std::size_t coff = static_cast<std::size_t>(pe_offset) + 4U;
    if (bytes[pe_offset] != 'P' || bytes[pe_offset + 1U] != 'E' ||
        bytes[pe_offset + 2U] != 0U || bytes[pe_offset + 3U] != 0U) {
        return fail(LoadError::InvalidHeader);
    }

    std::uint16_t machine = 0U;
    std::uint16_t section_count = 0U;
    std::uint16_t optional_size = 0U;
    if (!reader.u16(coff, machine) || !reader.u16(coff + 2U, section_count) ||
        !reader.u16(coff + 16U, optional_size)) {
        return fail(LoadError::TruncatedData);
    }
    if (machine != 0x8664U) {
        return fail(LoadError::UnsupportedImage);
    }
    if (section_count == 0U || section_count > limits.max_sections) {
        return fail(LoadError::LimitExceeded);
    }
    if (optional_size < 112U) {
        return fail(LoadError::UnsupportedImage);
    }

    const std::size_t optional = static_cast<std::size_t>(pe_offset) + 24U;
    if (optional > reader.size() || optional_size > reader.size() - optional) {
        return fail(LoadError::TruncatedData);
    }
    std::uint16_t magic = 0U;
    std::uint64_t image_base = 0U;
    std::uint32_t section_alignment = 0U;
    std::uint32_t file_alignment = 0U;
    std::uint32_t size_of_image = 0U;
    std::uint32_t size_of_headers = 0U;
    std::uint32_t entry_rva = 0U;
    std::uint32_t directory_count = 0U;
    if (!reader.u16(optional, magic) || !reader.u64(optional + 24U, image_base) ||
        !reader.u32(optional + 32U, section_alignment) ||
        !reader.u32(optional + 36U, file_alignment) ||
        !reader.u32(optional + 16U, entry_rva) ||
        !reader.u32(optional + 56U, size_of_image) ||
        !reader.u32(optional + 60U, size_of_headers) ||
        !reader.u32(optional + 108U, directory_count)) {
        return fail(LoadError::TruncatedData);
    }
    if (magic != 0x020BU) {
        return fail(LoadError::UnsupportedImage);
    }
    if (!is_power_of_two(section_alignment) || !is_power_of_two(file_alignment) ||
        file_alignment > section_alignment || size_of_image == 0U ||
        entry_rva >= size_of_image || size_of_headers == 0U ||
        size_of_headers > bytes.size()) {
        return fail(LoadError::InvalidHeader);
    }

    std::uint64_t effective_base = image_base;
    const std::uint64_t image_base_override = options.image_base_override != 0U
                                                  ? options.image_base_override
                                                  : (options.image_base != 0U
                                                         ? options.image_base
                                                         : options.base_address);
    if (image_base_override != 0U) {
        effective_base = image_base_override;
    }
    std::uint64_t image_end = 0U;
    if (!checked_add_u64(effective_base, size_of_image, image_end) ||
        image_end > limits.max_virtual_address) {
        return fail(LoadError::Overflow);
    }

    PeImageInfo info{};
    info.machine = machine;
    info.magic = magic;
    info.image_base = image_base;
    info.entry_point_rva = entry_rva;
    info.entry_point = effective_base + entry_rva;
    info.size_of_image = size_of_image;
    info.size_of_headers = size_of_headers;
    info.section_alignment = section_alignment;
    info.file_alignment = file_alignment;

    const std::size_t section_table = optional + optional_size;
    if (section_table > reader.size() ||
        static_cast<std::uint64_t>(section_count) * 40U > reader.size() - section_table) {
        return fail(LoadError::TruncatedData);
    }

    std::vector<std::pair<std::uint64_t, std::uint64_t>> virtual_spans;
    for (std::uint16_t i = 0U; i < section_count; ++i) {
        const std::size_t at = section_table + static_cast<std::size_t>(i) * 40U;
        PeSectionInfo section{};
        section.name = section_name(reader, at);
        if (!reader.u32(at + 8U, section.virtual_size) ||
            !reader.u32(at + 12U, section.virtual_address) ||
            !reader.u32(at + 16U, section.raw_size) ||
            !reader.u32(at + 20U, section.raw_offset) ||
            !reader.u32(at + 36U, section.characteristics)) {
            return fail(LoadError::TruncatedData);
        }
        section.size_of_raw_data = section.raw_size;
        section.pointer_to_raw_data = section.raw_offset;

        if (section.virtual_address % section_alignment != 0U) {
            return fail(LoadError::AlignmentError);
        }
        std::uint64_t raw_end = 0U;
        if (!checked_add_u64(section.raw_offset, section.raw_size, raw_end) ||
            raw_end > reader.size()) {
            return fail(LoadError::RangeError);
        }
        if (section.raw_size > 0U && section.raw_offset % file_alignment != 0U) {
            return fail(LoadError::AlignmentError);
        }
        const std::uint64_t mapped_size = section.virtual_size != 0U
                                              ? section.virtual_size
                                              : section.raw_size;
        if (mapped_size > limits.max_section_bytes) {
            return fail(LoadError::LimitExceeded);
        }
        std::uint64_t section_end = 0U;
        if (!checked_add_u64(section.virtual_address, mapped_size, section_end) ||
            section_end > size_of_image) {
            return fail(LoadError::RangeError);
        }
        if (mapped_size != 0U) {
            for (const auto& span : virtual_spans) {
                if (section.virtual_address < span.second && span.first < section_end) {
                    return fail(LoadError::Overlap);
                }
            }
            virtual_spans.emplace_back(section.virtual_address, section_end);
        }
        info.sections.push_back(section);
    }

    std::uint32_t import_rva = 0U;
    std::uint32_t import_size = 0U;
    std::uint32_t export_rva = 0U;
    std::uint32_t export_size = 0U;
    std::uint32_t reloc_rva = 0U;
    std::uint32_t reloc_size = 0U;
    const std::size_t directory_base = optional + 112U;
    if (directory_count > 0U) {
        if (reader.has(directory_base, static_cast<std::size_t>(directory_count) * 8U)) {
            if (directory_count > 0U) {
                reader.u32(directory_base, export_rva);
                reader.u32(directory_base + 4U, export_size);
            }
            if (directory_count > 1U) {
                reader.u32(directory_base + 8U, import_rva);
                reader.u32(directory_base + 12U, import_size);
            }
            if (directory_count > 6U) {
                reader.u32(directory_base + 5U * 8U, reloc_rva);
                reader.u32(directory_base + 5U * 8U + 4U, reloc_size);
            }
        }
    }

    const std::size_t max_names = 256U;
    parse_imports(reader, info, import_rva, import_size, max_names, info);
    parse_exports(reader, info, export_rva, export_size, max_names, info);
    const LoadError reloc_error = parse_relocations(reader, info, reloc_rva, reloc_size,
                                                     limits, max_names, info);
    if (reloc_error != LoadError::None) {
        return fail(reloc_error);
    }

    info.relocation_entries = info.relocations;
    std::sort(info.relocation_types.begin(), info.relocation_types.end());
    info.relocation_types.erase(
        std::unique(info.relocation_types.begin(), info.relocation_types.end()),
        info.relocation_types.end());
    std::sort(info.imports.begin(), info.imports.end());
    info.imports.erase(std::unique(info.imports.begin(), info.imports.end()), info.imports.end());
    std::sort(info.dependencies.begin(), info.dependencies.end());
    info.dependencies.erase(std::unique(info.dependencies.begin(), info.dependencies.end()),
                            info.dependencies.end());
    std::sort(info.exports.begin(), info.exports.end());
    info.exports.erase(std::unique(info.exports.begin(), info.exports.end()), info.exports.end());

    parsed.info = info;
    try {
        parsed.bytes.assign(bytes.begin(), bytes.end());
    } catch (...) {
        return fail(LoadError::LimitExceeded);
    }
    return parsed;
}

} // namespace detail

bool can_transition(LoaderState from, LoaderState to) noexcept {
    switch (from) {
    case LoaderState::Unloaded: return to == LoaderState::Validated || to == LoaderState::Failed;
    case LoaderState::Validated: return to == LoaderState::Mapped || to == LoaderState::Failed ||
                                        to == LoaderState::Unloaded;
    case LoaderState::Mapped: return to == LoaderState::Relocated || to == LoaderState::Failed ||
                                     to == LoaderState::Unloaded;
    case LoaderState::Relocated: return to == LoaderState::Ready || to == LoaderState::Failed ||
                                        to == LoaderState::Unloaded;
    case LoaderState::Ready: return to == LoaderState::Unloaded || to == LoaderState::Failed;
    case LoaderState::Failed: return to == LoaderState::Unloaded;
    }
    return false;
}

const char* to_string(LoaderState state) noexcept {
    switch (state) {
    case LoaderState::Unloaded: return "Unloaded";
    case LoaderState::Validated: return "Validated";
    case LoaderState::Mapped: return "Mapped";
    case LoaderState::Relocated: return "Relocated";
    case LoaderState::Ready: return "Ready";
    case LoaderState::Failed: return "Failed";
    }
    return "Unknown";
}

const char* to_string(LoadError error) noexcept {
    switch (error) {
    case LoadError::None: return "None";
    case LoadError::InvalidState: return "InvalidState";
    case LoadError::InvalidHeader: return "InvalidHeader";
    case LoadError::UnsupportedImage: return "UnsupportedImage";
    case LoadError::TruncatedData: return "TruncatedData";
    case LoadError::LimitExceeded: return "LimitExceeded";
    case LoadError::Overflow: return "Overflow";
    case LoadError::AlignmentError: return "AlignmentError";
    case LoadError::RangeError: return "RangeError";
    case LoadError::Overlap: return "Overlap";
    case LoadError::InvalidEntryPoint: return "InvalidEntryPoint";
    case LoadError::MemoryFailure: return "MemoryFailure";
    case LoadError::UnsupportedRelocation: return "UnsupportedRelocation";
    case LoadError::InvalidRelocation: return "InvalidRelocation";
    case LoadError::PhysicalBackingInaccessible: return "PhysicalBackingInaccessible";
    }
    return "Unknown";
}

} // namespace xbridge::loader
