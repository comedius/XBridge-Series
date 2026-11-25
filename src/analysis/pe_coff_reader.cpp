#include "pe_coff_reader.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <utility>

namespace xbridge::analysis::detail {
namespace {

constexpr std::uint16_t kMagicPe32 = 0x010b;
constexpr std::uint16_t kMagicPe32Plus = 0x020b;
constexpr std::uint16_t kMachineAmd64 = 0x8664;
constexpr std::uint16_t kSubsystemXbox = 14;
constexpr std::size_t kMaxDataDirectories = 16;
constexpr std::size_t kMaxSections = 96;
constexpr unsigned kMaxResourceDepth = 3;

struct Reader {
    std::span<const std::uint8_t> bytes;

    [[nodiscard]] bool has(std::size_t offset, std::size_t length) const noexcept {
        return offset <= bytes.size() && length <= bytes.size() - offset;
    }
    [[nodiscard]] std::uint8_t u8(std::size_t offset) const noexcept {
        return bytes[offset];
    }
    [[nodiscard]] std::uint16_t u16(std::size_t offset) const noexcept {
        return static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(u8(offset)) |
            static_cast<std::uint16_t>(static_cast<std::uint16_t>(u8(offset + 1)) << 8));
    }
    [[nodiscard]] std::uint32_t u32(std::size_t offset) const noexcept {
        return static_cast<std::uint32_t>(u8(offset)) |
               (static_cast<std::uint32_t>(u8(offset + 1)) << 8) |
               (static_cast<std::uint32_t>(u8(offset + 2)) << 16) |
               (static_cast<std::uint32_t>(u8(offset + 3)) << 24);
    }
    [[nodiscard]] std::uint64_t u64(std::size_t offset) const noexcept {
        return static_cast<std::uint64_t>(u32(offset)) |
               (static_cast<std::uint64_t>(u32(offset + 4)) << 32);
    }
};

std::string hex_value(std::uint64_t value, unsigned digits) {
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string text;
    text.reserve(static_cast<std::size_t>(digits) + 2);
    text.push_back('0');
    text.push_back('x');
    for (int i = static_cast<int>(digits) - 1; i >= 0; --i) {
        text.push_back(kDigits[(value >> (static_cast<unsigned>(i) * 4)) & 0xfu]);
    }
    return text;
}

std::string dec_value(std::uint64_t value) {
    return std::to_string(value);
}

void finalize(std::vector<std::string>& values) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
}

void cap_vector(std::vector<std::string>& values, std::size_t limit,
                const char* note, PeFacts& facts) {
    if (limit == 0) {
        values.clear();
        return;
    }
    if (values.size() > limit) {
        values.resize(limit);
        facts.uncertainties.push_back(note);
    }
}

struct Section {
    std::uint32_t virtual_size{0};
    std::uint32_t virtual_address{0};
    std::uint32_t raw_size{0};
    std::uint32_t raw_offset{0};
};

struct DataDirectory {
    std::uint32_t rva{0};
    std::uint32_t size{0};
};

struct Region {
    std::size_t offset{0};
    std::size_t size{0};

    [[nodiscard]] std::size_t end() const noexcept { return offset + size; }
    [[nodiscard]] bool contains(std::size_t at, std::size_t length) const noexcept {
        return at >= offset && at <= end() && length <= end() - at;
    }
};

// Maps an RVA to a bounded contiguous file range. Section raw data is the
// authority; the header area is only a fallback. Arithmetic is widened before
// addition so malformed values cannot wrap.
bool map_rva_extent(const Reader& reader, const std::vector<Section>& sections,
                    std::uint32_t headers_size, std::uint32_t rva,
                    std::size_t& offset, std::size_t& available,
                    bool& ambiguous) noexcept {
    offset = 0;
    available = 0;
    ambiguous = false;

    std::size_t best_offset = 0;
    std::size_t best_available = 0;
    std::size_t matches = 0;
    for (const Section& section : sections) {
        const std::uint64_t virtual_span = section.virtual_size != 0
                                               ? static_cast<std::uint64_t>(section.virtual_size)
                                               : static_cast<std::uint64_t>(section.raw_size);
        if (virtual_span == 0 || static_cast<std::uint64_t>(rva) < section.virtual_address) {
            continue;
        }
        const std::uint64_t delta = static_cast<std::uint64_t>(rva) - section.virtual_address;
        if (delta >= virtual_span || delta >= section.raw_size) {
            continue;
        }
        const std::uint64_t file_offset = static_cast<std::uint64_t>(section.raw_offset) + delta;
        if (file_offset >= reader.bytes.size()) {
            continue;
        }
        const std::uint64_t section_available = static_cast<std::uint64_t>(section.raw_size) - delta;
        const std::uint64_t file_available =
            static_cast<std::uint64_t>(reader.bytes.size()) - file_offset;
        const std::uint64_t usable = std::min(section_available, file_available);
        if (usable == 0) {
            continue;
        }
        if (matches == 0) {
            best_offset = static_cast<std::size_t>(file_offset);
            best_available = static_cast<std::size_t>(usable);
        }
        ++matches;
    }

    if (matches == 0 && static_cast<std::uint64_t>(rva) < headers_size &&
        static_cast<std::uint64_t>(rva) < reader.bytes.size()) {
        const std::uint64_t header_available =
            std::min<std::uint64_t>(static_cast<std::uint64_t>(headers_size) - rva,
                                    reader.bytes.size() - rva);
        if (header_available > 0) {
            best_offset = static_cast<std::size_t>(rva);
            best_available = static_cast<std::size_t>(header_available);
            matches = 1;
        }
    }

    ambiguous = matches > 1;
    if (matches == 0) {
        return false;
    }
    offset = best_offset;
    available = best_available;
    return true;
}

bool map_rva(const Reader& reader, const std::vector<Section>& sections,
             std::uint32_t headers_size, std::uint32_t rva, std::size_t length,
             std::size_t& offset, bool& ambiguous) noexcept {
    std::size_t available = 0;
    if (!map_rva_extent(reader, sections, headers_size, rva, offset, available, ambiguous)) {
        return false;
    }
    return available >= length;
}

// Reads a NUL-terminated ASCII string inside a bounded region. A missing
// terminator is reported rather than silently accepted.
bool read_bounded_ascii(const Reader& reader, std::size_t offset, std::size_t region_end,
                        std::size_t max_bytes, std::string& out, bool& terminated,
                        bool& sanitized) {
    out.clear();
    terminated = false;
    sanitized = false;
    if (region_end > reader.bytes.size()) {
        region_end = reader.bytes.size();
    }
    if (offset >= region_end || max_bytes == 0) {
        return false;
    }
    const std::uintmax_t limit =
        std::min<std::uintmax_t>(region_end, static_cast<std::uintmax_t>(offset) + max_bytes);
    for (std::size_t i = offset; i < static_cast<std::size_t>(limit); ++i) {
        const unsigned char c = reader.bytes[i];
        if (c == 0) {
            terminated = true;
            return true;
        }
        if (c >= 0x20 && c <= 0x7e) {
            out.push_back(static_cast<char>(c));
        } else {
            out.push_back('?');
            sanitized = true;
        }
    }
    return !out.empty();
}

void note_ambiguous(bool ambiguous, const char* what, PeFacts& facts) {
    if (ambiguous) {
        facts.uncertainties.push_back(std::string(what) +
                                      ": overlapping sections; first mapping used");
    }
}

bool directory_region(const Reader& reader, const std::vector<Section>& sections,
                      std::uint32_t headers_size, const DataDirectory& directory,
                      Region& region, PeFacts& facts, const char* name) {
    if (directory.rva == 0 || directory.size == 0) {
        return false;
    }
    std::size_t offset = 0;
    bool ambiguous = false;
    if (!map_rva(reader, sections, headers_size, directory.rva, 1, offset, ambiguous)) {
        facts.uncertainties.push_back(std::string(name) + " directory RVA is not mappable");
        return false;
    }
    note_ambiguous(ambiguous, name, facts);
    region.offset = offset;
    region.size = static_cast<std::size_t>(std::min<std::uint64_t>(
        directory.size, static_cast<std::uint64_t>(reader.bytes.size() - offset)));
    if (region.size < directory.size) {
        facts.uncertainties.push_back(std::string(name) +
                                      " directory is truncated by the scanned prefix");
    }
    return region.size > 0;
}

void parse_import_thunks(const Reader& reader, const std::vector<Section>& sections,
                         std::uint32_t headers_size, std::uint32_t table_rva,
                         const std::string& dll, const AnalysisOptions& options,
                         PeFacts& facts) {
    std::size_t offset = 0;
    std::size_t available = 0;
    bool ambiguous = false;
    if (!map_rva_extent(reader, sections, headers_size, table_rva, offset, available, ambiguous)) {
        facts.uncertainties.push_back("import thunk RVA " + hex_value(table_rva, 8) +
                                      " is not mappable");
        return;
    }
    note_ambiguous(ambiguous, "import thunk", facts);

    const std::size_t width = facts.magic == kMagicPe32Plus ? 8u : 4u;
    if (width > available) {
        facts.uncertainties.push_back("import thunk table is too small for one entry");
        return;
    }
    const std::size_t count = std::min<std::size_t>(options.max_names, available / width);
    bool terminated = false;
    for (std::size_t i = 0; i < count; ++i) {
        const std::size_t at = offset + i * width;
        const std::uint64_t entry = width == 8 ? reader.u64(at) : reader.u32(at);
        if (entry == 0) {
            terminated = true;
            break;
        }
        const bool by_ordinal = width == 8
                                    ? (entry & (1ull << 63)) != 0
                                    : (entry & 0x80000000ull) != 0;
        if (by_ordinal) {
            facts.imports.push_back(dll + "!#" + dec_value(entry & 0xffffu));
            continue;
        }
        const std::uint32_t hint_name_rva = static_cast<std::uint32_t>(entry & 0x7fffffffu);
        std::size_t name_offset = 0;
        std::size_t name_available = 0;
        if (!map_rva_extent(reader, sections, headers_size, hint_name_rva,
                            name_offset, name_available, ambiguous)) {
            facts.uncertainties.push_back("import hint/name RVA " +
                                          hex_value(hint_name_rva, 8) + " is not mappable");
            continue;
        }
        if (name_available <= 2) {
            facts.uncertainties.push_back("import hint/name entry is truncated");
            continue;
        }
        std::string name;
        bool name_terminated = false;
        bool sanitized = false;
        const bool read = read_bounded_ascii(
            reader, name_offset + 2, name_offset + name_available,
            options.max_string_bytes, name, name_terminated, sanitized);
        if (!read) {
            facts.uncertainties.push_back("import name string is empty or out of bounds");
            continue;
        }
        if (!name_terminated) {
            facts.uncertainties.push_back("import name string has no terminator within its bound");
        }
        if (sanitized) {
            facts.uncertainties.push_back("import name contains non-printable bytes");
        }
        facts.imports.push_back(dll + "!" + name);
    }
    if (!terminated && count > 0) {
        facts.uncertainties.push_back("import thunk table has no zero terminator within its bound");
    }
    if (count == options.max_names && options.max_names > 0) {
        facts.uncertainties.push_back("import thunk name cap reached");
    }
}

void parse_imports(const Reader& reader, const std::vector<Section>& sections,
                   std::uint32_t headers_size, const DataDirectory& directory,
                   const AnalysisOptions& options, PeFacts& facts) {
    Region region;
    if (!directory_region(reader, sections, headers_size, directory, region, facts, "import")) {
        return;
    }
    if (region.size < 20) {
        facts.uncertainties.push_back("import directory is too small for one descriptor");
        return;
    }

    bool terminated = false;
    bool capped = false;
    std::size_t descriptors = 0;
    for (std::size_t pos = region.offset; region.contains(pos, 20); pos += 20) {
        if (descriptors >= options.max_names) {
            capped = true;
            break;
        }
        ++descriptors;
        const std::uint32_t lookup = reader.u32(pos);
        const std::uint32_t stamp = reader.u32(pos + 4);
        const std::uint32_t chain = reader.u32(pos + 8);
        const std::uint32_t name_rva = reader.u32(pos + 12);
        const std::uint32_t thunk_rva = reader.u32(pos + 16);
        if (lookup == 0 && stamp == 0 && chain == 0 && name_rva == 0 && thunk_rva == 0) {
            terminated = true;
            break;
        }

        bool name_terminated = false;
        bool sanitized = false;
        std::string dll;
        if (name_rva != 0) {
            std::size_t name_offset = 0;
            bool ambiguous = false;
            if (map_rva(reader, sections, headers_size, name_rva, 1, name_offset, ambiguous)) {
                read_bounded_ascii(reader, name_offset, reader.bytes.size(),
                                   options.max_string_bytes, dll, name_terminated, sanitized);
                if (!name_terminated) {
                    facts.uncertainties.push_back("import DLL name has no terminator within its bound");
                }
                if (sanitized) {
                    facts.uncertainties.push_back("import DLL name contains non-printable bytes");
                }
            } else {
                facts.uncertainties.push_back("import DLL name RVA is not mappable");
            }
        }
        if (dll.empty()) {
            dll = "<unnamed_dll>";
            facts.uncertainties.push_back("import descriptor has no readable DLL name");
        }
        facts.imports.push_back(dll);
        facts.dependencies.push_back(dll);

        const std::uint32_t table_rva = lookup != 0 ? lookup : thunk_rva;
        if (table_rva == 0) {
            facts.uncertainties.push_back("import descriptor has no usable thunk table RVA");
        } else {
            parse_import_thunks(reader, sections, headers_size, table_rva,
                                dll, options, facts);
        }
    }
    if (!terminated && !capped && descriptors > 0) {
        facts.uncertainties.push_back(
            "import descriptor list has no null terminator within the directory");
    }
    if (capped) {
        facts.uncertainties.push_back("import descriptor cap reached");
    }
}

void parse_exports(const Reader& reader, const std::vector<Section>& sections,
                   std::uint32_t headers_size, const DataDirectory& directory,
                   const AnalysisOptions& options, PeFacts& facts) {
    Region region;
    if (!directory_region(reader, sections, headers_size, directory, region, facts, "export")) {
        return;
    }
    if (region.size < 40) {
        facts.uncertainties.push_back("export directory is too small for its 40-byte header");
        return;
    }

    const std::size_t base = region.offset;
    const std::uint32_t name_rva = reader.u32(base + 12);
    const std::uint32_t ordinal_base = reader.u32(base + 16);
    const std::uint32_t function_count = reader.u32(base + 20);
    const std::uint32_t name_count = reader.u32(base + 24);
    const std::uint32_t address_table_rva = reader.u32(base + 28);
    const std::uint32_t name_pointer_rva = reader.u32(base + 32);
    const std::uint32_t ordinal_table_rva = reader.u32(base + 36);
    facts.fields.push_back("export_directory=rva=" + hex_value(directory.rva, 8) +
                           ",size=" + dec_value(directory.size));
    facts.fields.push_back("export_name_count=" + dec_value(name_count));

    if (name_rva != 0) {
        std::size_t name_offset = 0;
        bool ambiguous = false;
        if (map_rva(reader, sections, headers_size, name_rva, 1, name_offset, ambiguous)) {
            std::string dll;
            bool terminated = false;
            bool sanitized = false;
            if (read_bounded_ascii(reader, name_offset, reader.bytes.size(),
                                   options.max_string_bytes, dll, terminated, sanitized)) {
                facts.exports.push_back("export_dll:" + dll);
                if (!terminated || sanitized) {
                    facts.uncertainties.push_back("export DLL name is unterminated or non-printable");
                }
            }
        } else {
            facts.uncertainties.push_back("export DLL name RVA is not mappable");
        }
    }

    const std::size_t declared_names = std::min<std::size_t>(
        std::min<std::size_t>(name_count, options.max_names),
        static_cast<std::size_t>(directory.size) / 4);
    std::size_t name_offset = 0;
    std::size_t name_available = 0;
    std::size_t ordinal_offset = 0;
    std::size_t ordinal_available = 0;
    bool ambiguous = false;
    const bool names_mapped = name_pointer_rva != 0 &&
        map_rva_extent(reader, sections, headers_size, name_pointer_rva,
                       name_offset, name_available, ambiguous);
    const bool ordinals_mapped = ordinal_table_rva != 0 &&
        map_rva_extent(reader, sections, headers_size, ordinal_table_rva,
                       ordinal_offset, ordinal_available, ambiguous);
    if (!names_mapped) {
        facts.uncertainties.push_back("export name pointer table is absent or not mappable");
    }
    if (!ordinals_mapped && declared_names > 0) {
        facts.uncertainties.push_back("export ordinal table is absent or not mappable");
    }
    const std::size_t names_by_bytes = names_mapped ? name_available / 4 : 0;
    const std::size_t count = std::min(declared_names, names_by_bytes);
    if (declared_names > count) {
        facts.uncertainties.push_back("export name pointer table is truncated or capped");
    }
    for (std::size_t i = 0; i < count; ++i) {
        const std::uint32_t string_rva = reader.u32(name_offset + i * 4);
        std::string name;
        bool terminated = false;
        bool sanitized = false;
        if (string_rva != 0) {
            std::size_t string_offset = 0;
            if (map_rva(reader, sections, headers_size, string_rva, 1,
                        string_offset, ambiguous)) {
                read_bounded_ascii(reader, string_offset, reader.bytes.size(),
                                   options.max_string_bytes, name, terminated, sanitized);
                if (!terminated || sanitized) {
                    facts.uncertainties.push_back("export name is unterminated or non-printable");
                }
            } else {
                facts.uncertainties.push_back("export name RVA is not mappable");
            }
        }
        if (name.empty()) {
            facts.uncertainties.push_back("export name pointer resolves to an empty string");
            continue;
        }
        std::uint64_t ordinal = 0;
        if (ordinals_mapped && i * 2 + 2 <= ordinal_available) {
            ordinal = static_cast<std::uint64_t>(reader.u16(ordinal_offset + i * 2));
        }
        facts.exports.push_back(name + " (ordinal " +
                                dec_value(static_cast<std::uint64_t>(ordinal_base) + ordinal) + ")");
    }

    if (address_table_rva != 0 && function_count > 0) {
        std::size_t eat_offset = 0;
        std::size_t eat_available = 0;
        if (map_rva_extent(reader, sections, headers_size, address_table_rva,
                           eat_offset, eat_available, ambiguous)) {
            const std::size_t eat_count = std::min<std::size_t>(
                std::min<std::size_t>(function_count, options.max_names), eat_available / 4);
            const std::uint64_t export_begin = directory.rva;
            const std::uint64_t export_end = export_begin + directory.size;
            for (std::size_t i = 0; i < eat_count; ++i) {
                const std::uint32_t target = reader.u32(eat_offset + i * 4);
                if (target == 0) {
                    continue;
                }
                if (static_cast<std::uint64_t>(target) >= export_begin &&
                    static_cast<std::uint64_t>(target) < export_end) {
                    std::size_t forwarder_offset = 0;
                    if (map_rva(reader, sections, headers_size, target, 1,
                                forwarder_offset, ambiguous)) {
                        std::string forwarder;
                        bool terminated = false;
                        bool sanitized = false;
                        const bool read = read_bounded_ascii(
                            reader, forwarder_offset, region.end(), options.max_string_bytes,
                            forwarder, terminated, sanitized);
                        if (read) {
                            facts.exports.push_back("forwarder:" + forwarder);
                            if (!terminated || sanitized) {
                                facts.uncertainties.push_back(
                                    "export forwarder string is unterminated or non-printable");
                            }
                        } else {
                            facts.uncertainties.push_back("export forwarder string is out of bounds");
                        }
                    }
                }
            }
        } else {
            facts.uncertainties.push_back("export address table is not mappable");
        }
    }
}

const char* relocation_type_name(std::uint16_t type) noexcept {
    switch (type) {
        case 0: return "ABSOLUTE";
        case 1: return "HIGH";
        case 2: return "LOW";
        case 3: return "HIGHLOW";
        case 4: return "HIGHADJ";
        case 9: return "MIPS_JMPADDR16";
        case 10: return "DIR64";
        default: return "UNKNOWN";
    }
}

void parse_relocations(const Reader& reader, const std::vector<Section>& sections,
                       std::uint32_t headers_size, const DataDirectory& directory,
                       const AnalysisOptions& options, PeFacts& facts) {
    Region region;
    if (!directory_region(reader, sections, headers_size, directory, region, facts,
                          "base relocation")) {
        return;
    }
    if (region.size < 8) {
        facts.uncertainties.push_back("base relocation directory is too small for one block");
        return;
    }

    std::size_t pos = region.offset;
    std::size_t blocks = 0;
    while (region.contains(pos, 8)) {
        if (blocks >= options.max_names) {
            facts.uncertainties.push_back("base relocation block cap reached");
            break;
        }
        ++blocks;
        const std::uint32_t block_size = reader.u32(pos + 4);
        if (block_size < 8 || (block_size % 2) != 0) {
            facts.uncertainties.push_back("base relocation block size is invalid");
            break;
        }
        if (!region.contains(pos, block_size)) {
            facts.uncertainties.push_back("base relocation block extends outside the directory");
            break;
        }
        const std::size_t entries = (block_size - 8) / 2;
        for (std::size_t i = 0; i < entries; ++i) {
            const std::uint16_t word = reader.u16(pos + 8 + i * 2);
            const std::uint16_t type = static_cast<std::uint16_t>(word >> 12);
            facts.relocation_types.push_back(dec_value(type) + " (" +
                                             relocation_type_name(type) + ")");
            if (type == 4 && i + 1 < entries) {
                ++i; // HIGHADJ consumes two entries.
            }
        }
        pos += block_size;
    }
    if (region.contains(pos, 1)) {
        facts.uncertainties.push_back("base relocation directory has an incomplete trailing block");
    }
    if (blocks == 0) {
        facts.uncertainties.push_back("base relocation directory contains no complete block");
    }
}

const char* debug_type_name(std::uint32_t type) noexcept {
    switch (type) {
        case 1: return "COFF";
        case 2: return "CODEVIEW";
        case 3: return "FPO";
        case 4: return "MISC";
        case 5: return "EXCEPTION";
        case 6: return "FIXUP";
        case 7: return "OMAP_TO_SRC";
        case 8: return "OMAP_FROM_SRC";
        case 9: return "BORLAND";
        case 10: return "RESERVED10";
        case 11: return "CLSID";
        case 12: return "VC_FEATURE";
        case 13: return "POGO";
        case 14: return "ILTCG";
        case 15: return "MPX";
        case 16: return "REPRO";
        case 20: return "EX_DLLCHARACTERISTICS";
        default: return "UNKNOWN";
    }
}

void parse_debug(const Reader& reader, const std::vector<Section>& sections,
                 std::uint32_t headers_size, const DataDirectory& directory,
                 const AnalysisOptions& options, PeFacts& facts) {
    Region region;
    if (!directory_region(reader, sections, headers_size, directory, region, facts, "debug")) {
        return;
    }
    if (region.size < 28) {
        facts.uncertainties.push_back("debug directory is too small for one descriptor");
        return;
    }
    if (region.size % 28 != 0) {
        facts.uncertainties.push_back("debug directory size is not a multiple of 28 bytes");
    }
    const std::size_t declared = region.size / 28;
    const std::size_t count = std::min(declared, options.max_names);
    if (declared > count) {
        facts.uncertainties.push_back("debug descriptor cap reached");
    }
    for (std::size_t i = 0; i < count; ++i) {
        const std::size_t at = region.offset + i * 28;
        const std::uint32_t type = reader.u32(at + 12);
        const std::uint32_t address_of_raw_data = reader.u32(at + 20);
        const std::uint32_t pointer_to_raw_data = reader.u32(at + 24);
        facts.debug_types.push_back(dec_value(type) + " (" + debug_type_name(type) + ")");
        if (address_of_raw_data == 0) {
            facts.uncertainties.push_back("debug descriptor has AddressOfRawData = 0");
        }
        if (pointer_to_raw_data != 0 &&
            static_cast<std::uint64_t>(pointer_to_raw_data) > reader.bytes.size()) {
            facts.uncertainties.push_back("debug PointerToRawData lies beyond the scanned prefix");
        }
        if (type == 16) {
            facts.uncertainties.push_back(
                "IMAGE_DEBUG_TYPE_REPRO may be hash-derived; timestamp is not a build date");
        }
    }
}

bool decode_resource_name(const Reader& reader, const std::vector<Section>& sections,
                          std::uint32_t headers_size, std::uint32_t root_rva,
                          std::uint32_t name_offset, std::size_t max_string_bytes,
                          std::string& out, bool& sanitized) {
    out.clear();
    sanitized = false;
    const std::uint64_t target = static_cast<std::uint64_t>(root_rva) + name_offset;
    if (target > 0xffffffffull) {
        return false;
    }
    std::size_t offset = 0;
    std::size_t available = 0;
    bool ambiguous = false;
    if (!map_rva_extent(reader, sections, headers_size, static_cast<std::uint32_t>(target),
                        offset, available, ambiguous)) {
        return false;
    }
    if (available < 2) {
        return false;
    }
    const std::uint32_t units = reader.u16(offset);
    const std::size_t byte_count = static_cast<std::size_t>(units) * 2;
    // The loop consumes whole UTF-16 code units. Normalize to an even byte limit
    // before the bounds guard so the final u16 read cannot exceed the mapping.
    const std::size_t limit = (std::min(byte_count, max_string_bytes) / 2U) * 2U;
    if (limit > available - 2) {
        return false;
    }
    for (std::size_t i = 0; i < limit; i += 2) {
        const std::uint16_t unit = reader.u16(offset + 2 + i);
        if (unit >= 0x20 && unit <= 0x7e) {
            out.push_back(static_cast<char>(unit));
        } else if (unit != 0) {
            out.push_back('?');
            sanitized = true;
        }
    }
    return !out.empty();
}

void walk_resources(const Reader& reader, const std::vector<Section>& sections,
                    std::uint32_t headers_size, std::uint32_t root_rva,
                    std::uint32_t relative_offset, unsigned depth,
                    std::size_t& nodes, const AnalysisOptions& options,
                    PeFacts& facts) {
    if (depth > kMaxResourceDepth || nodes >= options.max_names) {
        return;
    }
    ++nodes;
    const std::uint64_t target = static_cast<std::uint64_t>(root_rva) + relative_offset;
    if (target > 0xffffffffull) {
        facts.uncertainties.push_back("resource directory RVA overflow");
        return;
    }
    std::size_t offset = 0;
    std::size_t available = 0;
    bool ambiguous = false;
    if (!map_rva_extent(reader, sections, headers_size, static_cast<std::uint32_t>(target),
                        offset, available, ambiguous)) {
        facts.uncertainties.push_back("resource directory RVA is not mappable");
        return;
    }
    if (available < 16) {
        facts.uncertainties.push_back("resource directory header is truncated");
        return;
    }
    const std::size_t declared = static_cast<std::size_t>(reader.u16(offset + 12)) +
                                 static_cast<std::size_t>(reader.u16(offset + 14));
    const std::size_t by_bytes = (available - 16) / 8;
    const std::size_t count = std::min(std::min(declared, options.max_names), by_bytes);
    if (declared > by_bytes) {
        facts.uncertainties.push_back("resource entry table is truncated by its section");
    }
    if (declared > options.max_names && options.max_names > 0) {
        facts.uncertainties.push_back("resource entry cap reached");
    }

    for (std::size_t i = 0; i < count; ++i) {
        const std::size_t at = offset + 16 + i * 8;
        const std::uint32_t name_field = reader.u32(at);
        const std::uint32_t data_field = reader.u32(at + 4);
        if (depth == 0) {
            if ((name_field & 0x80000000u) != 0) {
                std::string name;
                bool sanitized = false;
                if (decode_resource_name(reader, sections, headers_size, root_rva,
                                         name_field & 0x7fffffffu, options.max_string_bytes,
                                         name, sanitized)) {
                    facts.resource_kinds.push_back("resource_type=name:" + name);
                    if (sanitized) {
                        facts.uncertainties.push_back(
                            "resource type name contains non-ASCII code units");
                    }
                } else {
                    facts.uncertainties.push_back("resource type name is unreadable");
                }
            } else {
                facts.resource_kinds.push_back("resource_type=id:" + dec_value(name_field));
            }
        }
        if ((data_field & 0x80000000u) != 0) {
            if (depth < kMaxResourceDepth) {
                walk_resources(reader, sections, headers_size, root_rva,
                               data_field & 0x7fffffffu, depth + 1, nodes, options, facts);
            } else {
                facts.uncertainties.push_back("resource tree is deeper than four levels");
            }
            continue;
        }
        const std::uint64_t data_target = static_cast<std::uint64_t>(root_rva) + data_field;
        if (data_target > 0xffffffffull) {
            facts.uncertainties.push_back("resource data entry RVA overflow");
            continue;
        }
        std::size_t data_offset = 0;
        std::size_t data_available = 0;
        if (!map_rva_extent(reader, sections, headers_size,
                            static_cast<std::uint32_t>(data_target),
                            data_offset, data_available, ambiguous) ||
            data_available < 8) {
            facts.uncertainties.push_back("resource data entry is unmappable or truncated");
            continue;
        }
        const std::uint32_t data_rva = reader.u32(data_offset);
        const std::uint32_t data_size = reader.u32(data_offset + 4);
        facts.resource_kinds.push_back("resource_data_size=" + dec_value(data_size));
        if (data_rva != 0) {
            std::size_t mapped = 0;
            bool data_ambiguous = false;
            if (!map_rva(reader, sections, headers_size, data_rva, 1,
                         mapped, data_ambiguous)) {
                facts.uncertainties.push_back("resource data RVA is not mappable");
            }
        }
    }
}

void parse_resources(const Reader& reader, const std::vector<Section>& sections,
                     std::uint32_t headers_size, const DataDirectory& directory,
                     const AnalysisOptions& options, PeFacts& facts) {
    if (directory.rva == 0 || directory.size == 0) {
        return;
    }
    std::size_t nodes = 0;
    walk_resources(reader, sections, headers_size, directory.rva, 0, 0,
                   nodes, options, facts);
}

void parse_certificate(const Reader& reader, const DataDirectory& directory,
                       PeFacts& facts) {
    if (directory.rva == 0 && directory.size == 0) {
        return;
    }
    // The certificate table's first field is a FILE OFFSET, not an RVA.
    facts.fields.push_back("certificate_table=file_offset=" + hex_value(directory.rva, 8) +
                           ",size=" + dec_value(directory.size) +
                           ",rva_mapping=not_applicable");
    if (static_cast<std::uint64_t>(directory.rva) + directory.size > reader.bytes.size()) {
        facts.uncertainties.push_back("certificate table extends beyond the scanned prefix");
    }
    facts.uncertainties.push_back(
        "certificate payload not parsed; no signature, key or DRM processing");
}

void note_dll_characteristics(std::uint16_t flags, PeFacts& facts) {
    std::string text = hex_value(flags, 4);
    text.push_back('[');
    bool first = true;
    const std::array<std::pair<std::uint16_t, const char*>, 7> named{{
        {std::uint16_t{0x0020}, "HIGH_ENTROPY_VA"}, {std::uint16_t{0x0040}, "DYNAMIC_BASE"},
        {std::uint16_t{0x0080}, "FORCE_INTEGRITY"}, {std::uint16_t{0x0100}, "NX_COMPAT"},
        {std::uint16_t{0x0400}, "NO_SEH"}, {std::uint16_t{0x1000}, "APPCONTAINER"},
        {std::uint16_t{0x4000}, "GUARD_CF"}}};
    for (const auto& entry : named) {
        if ((flags & entry.first) != 0) {
            if (!first) {
                text.push_back(',');
            }
            text += entry.second;
            first = false;
        }
    }
    text.push_back(']');
    facts.fields.push_back("dll_characteristics=" + text);
    facts.uncertainties.push_back(
        "DllCharacteristics are loader assertions, not proof of host compatibility");
}

void parse_optional_header(const Reader& reader, std::size_t optional_offset,
                           std::size_t optional_available,
                           std::vector<DataDirectory>& directories,
                           std::uint32_t& headers_size, PeFacts& facts) {
    const auto has_opt = [optional_available](std::size_t at, std::size_t length) noexcept {
        return at <= optional_available && length <= optional_available - at;
    };

    if (optional_available < 2) {
        facts.uncertainties.push_back("optional header is too small for Magic");
        return;
    }
    facts.magic = reader.u16(optional_offset);
    if (facts.magic == kMagicPe32Plus) {
        facts.fields.push_back("optional_magic=0x020b (PE32+)");
    } else if (facts.magic == kMagicPe32) {
        facts.fields.push_back("optional_magic=0x010b (PE32)");
    } else {
        facts.fields.push_back("optional_magic=" + hex_value(facts.magic, 4) + " (unsupported)");
        facts.uncertainties.push_back("unsupported optional-header Magic; remaining fields skipped");
        return;
    }

    if (has_opt(16, 4)) {
        facts.fields.push_back("entry_point_rva=" +
                               hex_value(reader.u32(optional_offset + 16), 8));
    } else {
        facts.uncertainties.push_back("optional header truncated before AddressOfEntryPoint");
    }
    if (facts.magic == kMagicPe32Plus) {
        if (has_opt(24, 8)) {
            facts.fields.push_back("image_base=" + hex_value(reader.u64(optional_offset + 24), 16));
        } else {
            facts.uncertainties.push_back("optional header truncated before ImageBase");
        }
    } else if (has_opt(28, 4)) {
        facts.fields.push_back("image_base=" + hex_value(reader.u32(optional_offset + 28), 8));
    } else {
        facts.uncertainties.push_back("optional header truncated before ImageBase");
    }
    if (has_opt(60, 4)) {
        headers_size = reader.u32(optional_offset + 60);
        facts.fields.push_back("size_of_headers=" + hex_value(headers_size, 8));
    }
    if (has_opt(68, 2)) {
        facts.subsystem = reader.u16(optional_offset + 68);
        if (facts.subsystem == kSubsystemXbox) {
            facts.subsystem_xbox = true;
            facts.fields.push_back("subsystem=14 (IMAGE_SUBSYSTEM_XBOX indicator only)");
        } else {
            facts.fields.push_back("subsystem=" + dec_value(facts.subsystem));
        }
    } else {
        facts.uncertainties.push_back("optional header truncated before Subsystem");
    }
    if (has_opt(70, 2)) {
        note_dll_characteristics(reader.u16(optional_offset + 70), facts);
    }

    const std::size_t count_offset = facts.magic == kMagicPe32Plus ? 108 : 92;
    const std::size_t directories_offset = facts.magic == kMagicPe32Plus ? 112 : 96;
    std::uint32_t declared_directories = 0;
    if (has_opt(count_offset, 4)) {
        declared_directories = reader.u32(optional_offset + count_offset);
        facts.fields.push_back("number_of_rva_and_sizes=" + dec_value(declared_directories));
    } else {
        facts.uncertainties.push_back("optional header truncated before NumberOfRvaAndSizes");
    }
    if (declared_directories > kMaxDataDirectories) {
        facts.uncertainties.push_back("NumberOfRvaAndSizes exceeds 16; capped");
    }
    const std::size_t by_header = directories_offset <= optional_available
                                      ? (optional_available - directories_offset) / 8
                                      : 0;
    if (declared_directories > by_header && by_header < kMaxDataDirectories) {
        facts.uncertainties.push_back("data directory table is truncated by the optional header");
    }
    const std::size_t count = std::min<std::size_t>(
        std::min<std::size_t>(declared_directories, kMaxDataDirectories), by_header);
    for (std::size_t i = 0; i < count; ++i) {
        const std::size_t at = optional_offset + directories_offset + i * 8;
        directories[i].rva = reader.u32(at);
        directories[i].size = reader.u32(at + 4);
        if (directories[i].rva != 0 || directories[i].size != 0) {
            facts.fields.push_back("data_directory[" + dec_value(i) + "]=rva=" +
                                   hex_value(directories[i].rva, 8) + ",size=" +
                                   dec_value(directories[i].size));
        }
    }
    facts.fields.push_back("data_directory_indices=0:export,1:import,2:resource,3:exception,"
                           "4:certificate(file_offset),5:basereloc,6:debug,12:iat,13:delayimport");
    if (declared_directories > count) {
        facts.uncertainties.push_back("data directory table was capped by the optional header");
    }
}

void parse_sections(const Reader& reader, std::size_t section_table,
                    std::uint16_t declared_sections, std::vector<Section>& sections,
                    PeFacts& facts) {
    std::size_t count = declared_sections;
    if (count > kMaxSections) {
        facts.uncertainties.push_back("NumberOfSections exceeds 96; capped");
        count = kMaxSections;
    }
    if (section_table > reader.bytes.size()) {
        facts.uncertainties.push_back("section table offset lies beyond the scanned prefix");
        return;
    }
    const std::size_t by_bytes = (reader.bytes.size() - section_table) / 40;
    if (count > by_bytes) {
        facts.uncertainties.push_back("section table is truncated by the scanned prefix");
        count = by_bytes;
    }
    for (std::size_t i = 0; i < count; ++i) {
        const std::size_t at = section_table + i * 40;
        Section section;
        std::string name;
        for (std::size_t j = 0; j < 8; ++j) {
            const unsigned char c = reader.u8(at + j);
            if (c == 0) {
                break;
            }
            name.push_back(c >= 0x20 && c <= 0x7e ? static_cast<char>(c) : '?');
        }
        if (name.empty()) {
            name = "unnamed";
        }
        section.virtual_size = reader.u32(at + 8);
        section.virtual_address = reader.u32(at + 12);
        section.raw_size = reader.u32(at + 16);
        section.raw_offset = reader.u32(at + 20);
        sections.push_back(section);
        facts.fields.push_back("section[" + dec_value(i) + "]=name=" + name +
                               ",va=" + hex_value(section.virtual_address, 8) +
                               ",virtual_size=" + hex_value(section.virtual_size, 8) +
                               ",raw_offset=" + hex_value(section.raw_offset, 8) +
                               ",raw_size=" + hex_value(section.raw_size, 8));
        if (static_cast<std::uint64_t>(section.raw_offset) + section.raw_size >
            reader.bytes.size()) {
            facts.uncertainties.push_back("section " + name +
                                          " raw range exceeds the scanned prefix");
        }
    }
}

void parse_impl(const Reader& reader, const AnalysisOptions& options, PeFacts& facts) {
    if (!reader.has(0, 2) || reader.u8(0) != 'M' || reader.u8(1) != 'Z') {
        return; // Not a DOS/PE image; field-level NotApplicable is decided by the caller.
    }
    if (!reader.has(0x3c, 4)) {
        facts.truncated = true;
        facts.uncertainties.push_back("DOS header truncated before e_lfanew at 0x3c");
        return;
    }
    const std::uint32_t pe_offset = reader.u32(0x3c);
    if (!reader.has(pe_offset, 24)) {
        facts.truncated = true;
        facts.uncertainties.push_back("PE header offset e_lfanew lies beyond the scanned prefix");
        return;
    }
    if (reader.u8(pe_offset) != 'P' || reader.u8(pe_offset + 1) != 'E' ||
        reader.u8(pe_offset + 2) != 0 || reader.u8(pe_offset + 3) != 0) {
        return; // DOS image without a PE\0\0 signature.
    }
    facts.is_pe = true;

    const std::size_t coff = static_cast<std::size_t>(pe_offset) + 4;
    facts.machine = reader.u16(coff);
    const std::uint16_t section_count = reader.u16(coff + 2);
    const std::uint32_t timestamp = reader.u32(coff + 4);
    const std::uint16_t optional_size = reader.u16(coff + 16);
    const std::uint16_t characteristics = reader.u16(coff + 18);
    facts.fields.push_back("size_of_optional_header=" + dec_value(optional_size));
    facts.fields.push_back("machine=" + hex_value(facts.machine, 4) +
                           (facts.machine == kMachineAmd64
                                ? " (IMAGE_FILE_MACHINE_AMD64, x64 indicator only)"
                                : ""));
    facts.fields.push_back("number_of_sections=" + dec_value(section_count));
    facts.fields.push_back("characteristics=" + hex_value(characteristics, 4));

    if (timestamp == 0 || timestamp == 0xffffffffu) {
        facts.timestamp = hex_value(timestamp, 8) +
                          " (not a date; placeholder or reproducibility value)";
        facts.fields.push_back("time_date_stamp=" + facts.timestamp);
        facts.uncertainties.push_back(
            "TimeDateStamp is 0 or 0xFFFFFFFF; it is not a date and not a build date");
    } else {
        facts.timestamp = hex_value(timestamp, 8) +
                          " (raw metadata field; date meaning unverified)";
        facts.fields.push_back("time_date_stamp=" + facts.timestamp);
        facts.uncertainties.push_back(
            "TimeDateStamp is an untrusted metadata field; not presented as a build date");
    }

    const std::size_t optional_offset = coff + 20;
    if (optional_size < 2 || !reader.has(optional_offset, 2)) {
        facts.truncated = true;
        facts.uncertainties.push_back("optional header is absent or truncated");
        return;
    }
    std::size_t optional_available = optional_size;
    if (!reader.has(optional_offset, optional_size)) {
        optional_available = reader.bytes.size() - optional_offset;
        facts.truncated = true;
        facts.uncertainties.push_back("optional header is truncated by the scanned prefix");
    }
    facts.magic = reader.u16(optional_offset);

    std::vector<Section> sections;
    std::vector<DataDirectory> directories(kMaxDataDirectories);
    std::uint32_t headers_size = 0;
    parse_optional_header(reader, optional_offset, optional_available,
                          directories, headers_size, facts);

    if (facts.magic == kMagicPe32Plus && facts.machine == kMachineAmd64) {
        facts.amd64_pe32plus = true;
        facts.architecture = "x64_amd64_pe32plus";
        facts.fields.push_back(
            "machine_magic_agreement=amd64_pe32plus (indicator only; not compatibility or executability)");
    } else if (facts.magic == kMagicPe32 && facts.machine == kMachineAmd64) {
        facts.architecture = "amd64_pe32_mismatch";
        facts.uncertainties.push_back("Machine AMD64 disagrees with PE32 Magic; indicator set is inconsistent");
    } else if (facts.magic == kMagicPe32Plus) {
        facts.architecture = "pe32plus_machine_" + hex_value(facts.machine, 4);
        facts.uncertainties.push_back("PE32+ Magic without AMD64 Machine; indicator set is inconsistent");
    } else if (facts.magic == kMagicPe32) {
        facts.architecture = "pe32_machine_" + hex_value(facts.machine, 4);
    }
    if (facts.subsystem_xbox) {
        facts.fields.push_back(
            "subsystem_14_retail_usage=unknown (indicator only, no platform verdict)");
    }

    // The section table starts after the declared optional header.
    const std::size_t section_table = optional_offset + optional_size;
    parse_sections(reader, section_table, section_count, sections, facts);

    parse_exports(reader, sections, headers_size, directories[0], options, facts);
    parse_imports(reader, sections, headers_size, directories[1], options, facts);
    parse_resources(reader, sections, headers_size, directories[2], options, facts);
    parse_certificate(reader, directories[4], facts);
    parse_relocations(reader, sections, headers_size, directories[5], options, facts);
    parse_debug(reader, sections, headers_size, directories[6], options, facts);

    cap_vector(facts.imports, options.max_names, "import name cap reached", facts);
    cap_vector(facts.exports, options.max_names, "export name cap reached", facts);
    cap_vector(facts.relocation_types, options.max_names, "relocation type cap reached", facts);
    cap_vector(facts.debug_types, options.max_names, "debug type cap reached", facts);
    cap_vector(facts.dependencies, options.max_names, "dependency cap reached", facts);
    cap_vector(facts.resource_kinds, options.max_names, "resource kind cap reached", facts);
    cap_vector(facts.fields, options.max_names, "PE field cap reached", facts);
    cap_vector(facts.uncertainties, options.max_names, "uncertainty cap reached", facts);
    finalize(facts.imports);
    finalize(facts.exports);
    finalize(facts.relocation_types);
    finalize(facts.debug_types);
    finalize(facts.dependencies);
    finalize(facts.resource_kinds);
    finalize(facts.fields);
    finalize(facts.uncertainties);
}

} // namespace

PeFacts parse_pe_image(std::span<const std::uint8_t> bytes,
                       const AnalysisOptions& options) {
    PeFacts facts;
    parse_impl(Reader{bytes}, options, facts);
    return facts;
}

} // namespace xbridge::analysis::detail
