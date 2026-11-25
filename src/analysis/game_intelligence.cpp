#include "xbridge/analysis/game_intelligence.hpp"

#include "pe_coff_reader.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

namespace xbridge::analysis {
namespace {

std::string path_to_utf8(const std::filesystem::path& path) {
    const std::u8string text = path.u8string();
    return std::string(reinterpret_cast<const char*>(text.data()), text.size());
}

std::string ascii_lower(std::string_view text) {
    std::string result;
    result.reserve(text.size());
    for (unsigned char c : text) {
        result.push_back(c >= 'A' && c <= 'Z' ? static_cast<char>(c + 32) : static_cast<char>(c));
    }
    return result;
}

bool ascii_space(unsigned char c) noexcept {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// Looks for attribute=<quoted value> in a manifest without matching a suffix
// of a longer attribute name.
bool manifest_has_value(std::string_view text, std::string_view attribute,
                        std::string_view value) {
    const std::string lowered_text = ascii_lower(text);
    const std::string lowered_attribute = ascii_lower(attribute);
    if (lowered_attribute.empty()) {
        return false;
    }
    std::size_t from = 0;
    while (from < lowered_text.size()) {
        const std::size_t found = lowered_text.find(lowered_attribute, from);
        if (found == std::string::npos) {
            return false;
        }
        const bool boundary = found == 0 ||
            !((lowered_text[found - 1] >= 'a' && lowered_text[found - 1] <= 'z') ||
              (lowered_text[found - 1] >= '0' && lowered_text[found - 1] <= '9') ||
              lowered_text[found - 1] == '_' || lowered_text[found - 1] == '-');
        std::size_t at = found + lowered_attribute.size();
        while (at < text.size() && ascii_space(static_cast<unsigned char>(text[at]))) {
            ++at;
        }
        if (boundary && at < text.size() && text[at] == '=') {
            ++at;
            while (at < text.size() && ascii_space(static_cast<unsigned char>(text[at]))) {
                ++at;
            }
            if (at < text.size() && (text[at] == '"' || text[at] == '\'')) {
                const char quote = text[at++];
                const std::size_t begin = at;
                while (at < text.size() && text[at] != quote && text[at] != '\n' &&
                       text[at] != '\r') {
                    ++at;
                }
                if (at < text.size() && text[at] == quote &&
                    text.substr(begin, at - begin) == value) {
                    return true;
                }
            }
        }
        from = found + 1;
    }
    return false;
}

bool is_container_extension(std::string_view extension) noexcept {
    return extension == ".xvc" || extension == ".msixvc";
}

bool is_shader_extension(std::string_view extension) noexcept {
    return extension == ".dxil" || extension == ".cso" || extension == ".hlsl" ||
           extension == ".fx" || extension == ".shader";
}

void add_component_candidates(AnalysisEntry& entry) {
    // Candidates only: these names are not an approved architecture mapping.
    entry.component_mapping.push_back(
        "candidate: bounded_pe_coff_reader (read-only header and field metadata)");
    entry.component_mapping.push_back(
        "candidate: indicator_classifier (evidence labels only)");
    entry.component_mapping.push_back(
        "candidate: bounded_directory_enumerator (import/export/reloc/debug/resource)");
    entry.component_mapping.push_back(
        "candidate: read_only_json_reporter");
}

void finalize_entry(AnalysisEntry& entry) {
    auto sort_unique = [](std::vector<std::string>& values) {
        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
    };
    sort_unique(entry.platform_indicators);
    sort_unique(entry.pe_fields);
    sort_unique(entry.imports);
    sort_unique(entry.exports);
    sort_unique(entry.relocation_types);
    sort_unique(entry.debug_types);
    sort_unique(entry.dependencies);
    sort_unique(entry.resource_kinds);
    sort_unique(entry.uncertainties);
    sort_unique(entry.component_mapping);
}

void apply_pe_facts(AnalysisEntry& entry, const detail::PeFacts& facts) {
    entry.file_type = "pe";
    entry.architecture = facts.architecture.empty() ? "pe_unknown" : facts.architecture;
    entry.pe_fields = facts.fields;
    entry.imports = facts.imports;
    entry.exports = facts.exports;
    entry.relocation_types = facts.relocation_types;
    entry.debug_types = facts.debug_types;
    entry.dependencies = facts.dependencies;
    entry.resource_kinds = facts.resource_kinds;
    entry.timestamp = facts.timestamp.empty() ? "unknown (timestamp not readable)" : facts.timestamp;
    entry.uncertainties.insert(entry.uncertainties.end(), facts.uncertainties.begin(),
                               facts.uncertainties.end());
    if (facts.amd64_pe32plus) {
        entry.platform_indicators.push_back(
            "pe_machine=0x8664+magic=0x20b (AMD64/PE32+ indicator only; not compatibility or executability)");
    }
    if (facts.subsystem_xbox) {
        entry.platform_indicators.push_back(
            "pe_subsystem=14 (IMAGE_SUBSYSTEM_XBOX indicator only; retail use unknown)");
    }
    entry.uncertainties.push_back(
        "PE32+ metadata does not establish Windows compatibility or direct executability");
    entry.status = facts.truncated ? DetectionStatus::Unknown
                                   : (facts.amd64_pe32plus || facts.subsystem_xbox
                                          ? DetectionStatus::Indicator
                                          : DetectionStatus::Confirmed);
}

void apply_non_pe_facts(AnalysisEntry& entry, const std::span<const std::uint8_t> bytes,
                        const std::filesystem::path& path, std::string_view extension,
                        const detail::PeFacts& facts) {
    entry.timestamp = facts.truncated ? "unknown (header truncated)"
                                      : "not_applicable (not a PE/COFF image)";
    entry.pe_fields.push_back(facts.truncated
                                  ? "pe_coff=unknown (truncated DOS/PE header)"
                                  : "pe_coff=not_applicable (file is not a PE/COFF image)");
    entry.uncertainties.insert(entry.uncertainties.end(), facts.uncertainties.begin(),
                               facts.uncertainties.end());

    bool weak_indicator = false;
    if (is_container_extension(extension)) {
        entry.file_type = extension == ".xvc" ? "xvc_container" : "msixvc_container";
        entry.platform_indicators.push_back(
            "container_extension=" + std::string(extension) +
            " (weak package-container indicator only)");
        entry.uncertainties.push_back(
            "container payload internals unknown; package content was not parsed");
        weak_indicator = true;
    } else if (extension == ".dds") {
        entry.file_type = "dds";
        entry.resource_kinds.push_back("extension=.dds (weak DirectX resource indicator only)");
        entry.platform_indicators.push_back(
            "weak_resource_extension=.dds (DirectX resource hint only; no Xbox format claim)");
        entry.uncertainties.push_back(
            "extension-based resource classification is weak, may produce false positives, "
            "and makes no Xbox format claim");
        weak_indicator = true;
    } else if (is_shader_extension(extension)) {
        entry.file_type = "shader";
        entry.resource_kinds.push_back(
            "extension=" + std::string(extension) + " (weak shader indicator only)");
        entry.platform_indicators.push_back(
            "weak_shader_extension=" + std::string(extension) +
            " (public DirectX hint only; no Xbox format claim)");
        entry.uncertainties.push_back(
            "extension-based shader classification is weak, may produce false positives, "
            "and makes no Xbox format claim");
        weak_indicator = true;
    } else {
        entry.file_type = "data";
    }

    const std::string filename = ascii_lower(path_to_utf8(path.filename()));
    if (filename == "microsoftgame.config" && !bytes.empty()) {
        const std::string_view text(reinterpret_cast<const char*>(bytes.data()), bytes.size());
        const bool scarlett = manifest_has_value(text, "TargetDeviceFamily", "Scarlett");
        const bool x64 = manifest_has_value(text, "Architecture", "x64");
        if (scarlett) {
            entry.platform_indicators.push_back(
                "gdk_target_device_family=Scarlett (manifest indicator only)");
            weak_indicator = true;
        }
        if (x64) {
            entry.platform_indicators.push_back(
                "gdk_architecture=x64 (manifest indicator only)");
            weak_indicator = true;
        }
        if (scarlett || x64) {
            entry.file_type = "gdk_manifest";
            entry.uncertainties.push_back(
                "manifest values are declarative configuration; not verified platform behavior");
        } else {
            entry.uncertainties.push_back(
                "no Scarlett/x64 GDK manifest values observed in the scanned prefix");
        }
    }

    entry.status = facts.truncated ? DetectionStatus::Unknown
                                   : (weak_indicator ? DetectionStatus::Indicator
                                                     : DetectionStatus::NotApplicable);
}

struct FileRecord {
    std::filesystem::path path;
    std::string relative;
    std::uintmax_t size{0};
    bool size_available{false};
    bool is_symlink{false};
};

AnalysisEntry analyze_one_file(const FileRecord& record, const AnalysisOptions& options) {
    AnalysisEntry entry;
    entry.relative_path = record.relative;
    entry.size_bytes = record.size;
    entry.status = DetectionStatus::Unknown;
    add_component_candidates(entry);

    if (record.is_symlink) {
        entry.file_type = "symlink";
        entry.timestamp = "not_applicable (symbolic link; target not inspected)";
        entry.pe_fields.push_back("pe_coff=not_applicable (symbolic link target not read)");
        entry.uncertainties.push_back("symbolic link target was not inspected");
        finalize_entry(entry);
        return entry;
    }
    if (!record.size_available) {
        entry.uncertainties.push_back("file size was unavailable from the directory entry");
    }

    const std::uintmax_t stream_limit =
        static_cast<std::uintmax_t>(std::numeric_limits<std::streamsize>::max());
    const std::uintmax_t cap = std::min(options.max_scan_bytes, stream_limit);
    if (record.size_available && record.size > cap) {
        entry.uncertainties.push_back("file exceeds max_scan_bytes; only a prefix was inspected");
    }
    if (cap == 0) {
        entry.file_type = "unknown";
        entry.timestamp = "unknown (no file content inspected)";
        entry.uncertainties.push_back("max_scan_bytes is zero; no file content was inspected");
        finalize_entry(entry);
        return entry;
    }

    std::uintmax_t wanted = record.size_available ? std::min(record.size, cap)
                                                  : std::min<std::uintmax_t>(cap, 65536u);
    if (wanted > static_cast<std::uintmax_t>(std::numeric_limits<std::size_t>::max())) {
        wanted = static_cast<std::uintmax_t>(std::numeric_limits<std::size_t>::max());
    }

    std::ifstream stream(record.path, std::ios::binary);
    if (!stream.is_open()) {
        entry.file_type = "unknown";
        entry.timestamp = "unknown (file unreadable)";
        entry.pe_fields.push_back("pe_coff=unknown (file unreadable)");
        entry.uncertainties.push_back("file could not be opened for reading");
        finalize_entry(entry);
        return entry;
    }

    std::vector<std::uint8_t> bytes;
    bytes.resize(static_cast<std::size_t>(wanted));
    std::streamsize got = 0;
    if (wanted > 0) {
        stream.read(reinterpret_cast<char*>(bytes.data()),
                    static_cast<std::streamsize>(wanted));
        got = stream.gcount();
        if (got < static_cast<std::streamsize>(wanted)) {
            if (stream.bad()) {
                entry.uncertainties.push_back("file read failed; content is incomplete");
            } else {
                entry.uncertainties.push_back("short read; file may have changed during the scan");
            }
        }
        bytes.resize(static_cast<std::size_t>(got));
    }

    const std::string extension = ascii_lower(path_to_utf8(record.path.extension()));
    detail::PeFacts facts;
    if (!bytes.empty()) {
        facts = detail::parse_pe_image(bytes, options);
    }

    if (facts.is_pe) {
        apply_pe_facts(entry, facts);
    } else {
        apply_non_pe_facts(entry, bytes, record.path, extension, facts);
    }
    finalize_entry(entry);
    return entry;
}

} // namespace

std::string_view status_name(DetectionStatus status) noexcept {
    switch (status) {
        case DetectionStatus::Confirmed: return "confirmed";
        case DetectionStatus::Indicator: return "indicator";
        case DetectionStatus::Unknown: return "unknown";
        case DetectionStatus::NotApplicable: return "not_applicable";
    }
    return "unknown";
}

namespace {

void append_json_string(std::string& out, std::string_view value) {
    static constexpr char kHex[] = "0123456789abcdef";
    out.push_back('"');
    for (unsigned char c : value) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (c < 0x20) {
                    out += "\\u00";
                    out.push_back(kHex[c >> 4]);
                    out.push_back(kHex[c & 0x0f]);
                } else {
                    out.push_back(static_cast<char>(c));
                }
                break;
        }
    }
    out.push_back('"');
}

void append_json_array(std::string& out, std::string_view name,
                       const std::vector<std::string>& values, std::size_t indent) {
    out.append(indent, ' ');
    append_json_string(out, name);
    out += ": ";
    if (values.empty()) {
        out += "[]\n";
        return;
    }
    out += "[\n";
    for (std::size_t i = 0; i < values.size(); ++i) {
        out.append(indent + 2, ' ');
        append_json_string(out, values[i]);
        if (i + 1 < values.size()) {
            out.push_back(',');
        }
        out.push_back('\n');
    }
    out.append(indent, ' ');
    out += "]\n";
}

void append_json_entry(std::string& out, const AnalysisEntry& entry, bool last) {
    out += "    {\n";
    out += "      \"relative_path\": ";
    append_json_string(out, entry.relative_path);
    out += ",\n";
    out += "      \"size_bytes\": " + std::to_string(entry.size_bytes) + ",\n";
    out += "      \"file_type\": ";
    append_json_string(out, entry.file_type);
    out += ",\n";
    out += "      \"architecture\": ";
    append_json_string(out, entry.architecture);
    out += ",\n";
    append_json_array(out, "platform_indicators", entry.platform_indicators, 6);
    out += ",\n";
    append_json_array(out, "pe_fields", entry.pe_fields, 6);
    out += ",\n";
    append_json_array(out, "imports", entry.imports, 6);
    out += ",\n";
    append_json_array(out, "exports", entry.exports, 6);
    out += ",\n";
    append_json_array(out, "relocation_types", entry.relocation_types, 6);
    out += ",\n";
    append_json_array(out, "debug_types", entry.debug_types, 6);
    out += ",\n";
    append_json_array(out, "dependencies", entry.dependencies, 6);
    out += ",\n";
    append_json_array(out, "resource_kinds", entry.resource_kinds, 6);
    out += ",\n";
    out += "      \"timestamp\": ";
    append_json_string(out, entry.timestamp);
    out += ",\n";
    append_json_array(out, "uncertainties", entry.uncertainties, 6);
    out += ",\n";
    append_json_array(out, "component_mapping", entry.component_mapping, 6);
    out += ",\n";
    out += "      \"status\": ";
    append_json_string(out, status_name(entry.status));
    out += "\n    }";
    if (!last) {
        out.push_back(',');
    }
    out.push_back('\n');
}

AnalysisReport analyze_directory_impl(const std::filesystem::path& directory,
                                      const AnalysisOptions& options,
                                      std::error_code& error) {
    AnalysisReport report;
    report.source_directory = path_to_utf8(directory);
    error.clear();

    std::error_code ec;
    const std::filesystem::file_status directory_status =
        std::filesystem::status(directory, ec);
    if (ec) {
        error = ec;
        return report;
    }
    if (!std::filesystem::exists(directory_status)) {
        error = std::make_error_code(std::errc::no_such_file_or_directory);
        return report;
    }
    if (!std::filesystem::is_directory(directory_status)) {
        error = std::make_error_code(std::errc::not_a_directory);
        return report;
    }

    std::vector<FileRecord> records;
    std::error_code iteration_error;
    // directory_options::none: directory symlinks are reported but never followed.
    std::filesystem::recursive_directory_iterator iterator(
        directory, std::filesystem::directory_options::none, iteration_error);
    if (iteration_error) {
        error = iteration_error;
        return report;
    }
    const std::filesystem::recursive_directory_iterator end;
    bool entry_cap_reached = false;
    while (iterator != end) {
        const std::filesystem::directory_entry& item = *iterator;
        std::error_code entry_error;
        const bool is_symlink = item.is_symlink(entry_error);
        if (entry_error) {
            report.notes.push_back("entry type unavailable for " + path_to_utf8(item.path()) +
                                   ": " + entry_error.message());
            entry_error.clear();
        }
        const bool is_directory = item.is_directory(entry_error);
        if (entry_error) {
            report.notes.push_back("entry type unavailable for " + path_to_utf8(item.path()) +
                                   ": " + entry_error.message());
            entry_error.clear();
        }
        if (!is_directory || is_symlink) {
            if (records.size() >= options.max_entries) {
                entry_cap_reached = true;
                break;
            }
            FileRecord record;
            record.path = item.path();
            record.relative = path_to_utf8(item.path().lexically_relative(directory));
            if (record.relative.empty()) {
                record.relative = path_to_utf8(item.path().filename());
            }
            record.is_symlink = is_symlink;
            record.size = item.file_size(entry_error);
            if (entry_error) {
                entry_error.clear();
                record.size = 0;
            } else {
                record.size_available = true;
            }
            records.push_back(std::move(record));
        }
        iterator.increment(iteration_error);
        if (iteration_error) {
            report.notes.push_back("directory traversal stopped: " + iteration_error.message());
            break;
        }
    }
    if (entry_cap_reached) {
        report.notes.push_back("entry cap reached at " +
                               std::to_string(options.max_entries) +
                               "; remaining entries were not inventoried");
    }

    std::sort(records.begin(), records.end(), [](const FileRecord& left,
                                                 const FileRecord& right) {
        return left.relative < right.relative;
    });
    report.entries.reserve(records.size());
    for (const FileRecord& record : records) {
        report.entries.push_back(analyze_one_file(record, options));
    }
    report.notes.push_back("inventory sorted by relative path; source files were not modified");
    std::sort(report.notes.begin(), report.notes.end());
    report.notes.erase(std::unique(report.notes.begin(), report.notes.end()),
                       report.notes.end());
    return report;
}

} // namespace

std::string AnalysisReport::to_json() const {
    std::string out;
    out += "{\n";
    out += "  \"source_directory\": ";
    append_json_string(out, source_directory);
    out += ",\n";
    out += "  \"entries\": ";
    if (entries.empty()) {
        out += "[]";
    } else {
        out += "[\n";
        for (std::size_t i = 0; i < entries.size(); ++i) {
            append_json_entry(out, entries[i], i + 1 == entries.size());
        }
        out += "  ]";
    }
    out += ",\n";
    out += "  \"notes\": ";
    if (notes.empty()) {
        out += "[]\n";
    } else {
        out += "[\n";
        for (std::size_t i = 0; i < notes.size(); ++i) {
            out += "    ";
            append_json_string(out, notes[i]);
            if (i + 1 < notes.size()) {
                out.push_back(',');
            }
            out.push_back('\n');
        }
        out += "  ]\n";
    }
    out += "}\n";
    return out;
}

AnalysisReport analyze_directory(const std::filesystem::path& directory,
                                 const AnalysisOptions& options,
                                 std::error_code& error) noexcept {
    AnalysisReport report;
    try {
        report = analyze_directory_impl(directory, options, error);
    } catch (...) {
        if (!error) {
            error = std::make_error_code(std::errc::not_enough_memory);
        }
    }
    return report;
}

AnalysisReport analyze_directory(const std::filesystem::path& directory,
                                 std::error_code& error) noexcept {
    return analyze_directory(directory, AnalysisOptions{}, error);
}

} // namespace xbridge::analysis
