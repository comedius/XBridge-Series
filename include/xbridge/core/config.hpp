#pragma once
#include <string>
#include <string_view>

namespace xbridge {

enum class ProfileKind { SeriesX };

struct HardwareProfile {
    ProfileKind kind;
    std::string_view id;
    std::string_view display_name;
    unsigned cpu_cores;
    unsigned logical_threads;
    unsigned memory_gib;
    double gpu_tflops;
};

const HardwareProfile& profile_for(ProfileKind kind);
bool parse_profile(std::string_view value, ProfileKind& out);
std::string_view profile_name(ProfileKind kind);

} // namespace xbridge
