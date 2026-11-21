#include "xbridge/core/config.hpp"

namespace xbridge {

const HardwareProfile& profile_for(ProfileKind) {
    // Metadata is illustrative and deliberately not a cycle-accurate hardware model.
    static constexpr HardwareProfile series_x{
        ProfileKind::SeriesX, "series_x", "Xbox Series X (target profile)",
        8, 16, 16, 12.15
    };
    return series_x;
}

bool parse_profile(std::string_view value, ProfileKind& out) {
    if (value == "series_x") { out = ProfileKind::SeriesX; return true; }
    return false;
}

std::string_view profile_name(ProfileKind kind) {
    return profile_for(kind).id;
}

} // namespace xbridge
