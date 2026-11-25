#pragma once

#include "xbridge/analysis/game_intelligence.hpp"

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace xbridge::analysis::detail {

// Internal, read-only PE/COFF facts. Every string vector is sorted and deduplicated
// before it is copied into the public AnalysisEntry. This type is not public API.
struct PeFacts {
    bool is_pe{false};
    bool truncated{false};
    bool amd64_pe32plus{false};
    bool subsystem_xbox{false};
    std::uint16_t machine{0};
    std::uint16_t magic{0};
    std::uint16_t subsystem{0};
    std::string architecture;
    std::string timestamp;
    std::vector<std::string> uncertainties;
    std::vector<std::string> fields;
    std::vector<std::string> imports;
    std::vector<std::string> exports;
    std::vector<std::string> relocation_types;
    std::vector<std::string> debug_types;
    std::vector<std::string> dependencies;
    std::vector<std::string> resource_kinds;
};

// Parses only the already-read prefix. Malformed, truncated or inconsistent
// data produces uncertainties instead of exceptions or guessed conclusions.
// The caller is responsible for guarding allocation failures.
[[nodiscard]] PeFacts parse_pe_image(std::span<const std::uint8_t> bytes,
                                     const AnalysisOptions& options);

} // namespace xbridge::analysis::detail
