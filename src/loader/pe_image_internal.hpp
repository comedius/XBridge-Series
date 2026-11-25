#pragma once

#include "xbridge/loader/pe_image.hpp"

#include <cstdint>
#include <span>
#include <vector>

namespace xbridge::loader::detail {

// Loader-private bounded parser. It never includes or modifies the analyzer
// parser and exposes only value types to ExecutableLoader.
struct ParsedImage {
    LoadError error{LoadError::None};
    PeImageInfo info{};
    std::vector<std::uint8_t> bytes{};
};

[[nodiscard]] ParsedImage parse_synthetic_pe(std::span<const std::uint8_t> bytes,
                                             const LoadOptions& options) noexcept;

} // namespace xbridge::loader::detail
