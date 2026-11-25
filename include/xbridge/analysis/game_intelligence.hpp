#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

// Read-only, local analysis of a directory tree. No source file is ever opened
// for writing, moved, extracted, uploaded or executed. The analyzer reports
// metadata it can actually observe; interpretation remains explicitly labeled.
namespace xbridge::analysis {

// Evidence labels attached to an analysis result.
// Confirmed:    directly observed bytes/fields or file presence/size.
// Indicator:   interpretation of observed data, e.g. x64 AMD64 machine type
//              together with PE32+ optional-header Magic, subsystem 14, GDK
//              manifest values or a weak extension-based hint. An indicator is
//              not a compatibility, executability or platform verdict. In
//              particular, PE32+ never proves Windows compatibility or direct
//              executability of a file from a game container.
// Unknown:      unreadable, encrypted, truncated, internally inconsistent, or
//              otherwise lacking an authoritative basis for a conclusion.
// NotApplicable: a requested field is absent because the file is not a
//              PE/COFF image; entry status uses this label for a readable
//              non-PE file with no indicator. File presence and size remain
//              confirmed facts recorded in relative_path and size_bytes.
enum class DetectionStatus { Confirmed, Indicator, Unknown, NotApplicable };

[[nodiscard]] std::string_view status_name(DetectionStatus status) noexcept;
// Returns: "confirmed", "indicator", "unknown", "not_applicable".

struct AnalysisEntry {
    std::string relative_path;                    // UTF-8, relative, '/' or native separator
    std::uintmax_t size_bytes{0};                 // observed size; 0 when unavailable
    std::string file_type;                        // "pe", "gdk_manifest", "xvc_container",
                                                  // "msixvc_container", "dds", "shader",
                                                  // "symlink", "unknown", "data"
    std::string architecture;                     // "x64_amd64_pe32plus", "pe32", etc.;
                                                  // never a compatibility or executability claim
    std::vector<std::string> platform_indicators; // labeled hints, never platform verdicts
    std::vector<std::string> pe_fields;           // raw/labeled PE/COFF field observations;
                                                  // includes "not_applicable" for non-PE files
    std::vector<std::string> imports;             // "dll" and "dll!name" / "dll!#ordinal"
    std::vector<std::string> exports;             // exported names and forwarders
    std::vector<std::string> relocation_types;    // bounded base-relocation type labels
    std::vector<std::string> debug_types;         // bounded debug directory type labels
    std::vector<std::string> dependencies;        // deduplicated DLL names from imports
    std::vector<std::string> resource_kinds;      // PE resource type/data labels and weak hints
    std::string timestamp;                        // raw field label; never presented as a build date
    std::vector<std::string> uncertainties;       // explicit failure/limitation descriptions
    std::vector<std::string> component_mapping;   // candidate component names only; not approved architecture
    // Additive, evidence-labeled linkage metadata. These are indicators and
    // requirements derived only from already-observed fields; they never claim
    // compatibility, executability or readiness.
    std::vector<std::string> runtime_service_candidates;
    std::vector<std::string> loader_requirements;
    std::vector<std::string> unsupported_or_unknown_structures;
    DetectionStatus status{DetectionStatus::Unknown};
};

struct AnalysisOptions {
    std::uintmax_t max_scan_bytes{16ULL * 1024ULL * 1024ULL}; // per-file read cap
    std::size_t max_entries{10000};
    std::size_t max_names{256};
    std::size_t max_string_bytes{256};
};

struct AnalysisReport {
    std::string source_directory; // UTF-8 spelling of the requested directory
    std::vector<AnalysisEntry> entries; // sorted by relative_path
    std::vector<std::string> notes;     // scan-level caps and traversal limitations
    [[nodiscard]] std::string to_json() const; // deterministic, escaped, no third-party library
};

// No default path: the caller must supply a directory. The error_code form
// reports invalid/missing directories and fatal scan failures; per-file problems
// stay inside AnalysisEntry::uncertainties. Both overloads are noexcept: no
// filesystem, stream or allocation exception escapes.
[[nodiscard]] AnalysisReport analyze_directory(
    const std::filesystem::path& directory, std::error_code& error) noexcept;
[[nodiscard]] AnalysisReport analyze_directory(
    const std::filesystem::path& directory, const AnalysisOptions& options,
    std::error_code& error) noexcept;

} // namespace xbridge::analysis
