#pragma once
#include <cstdint>
#include <string>

namespace xbridge::host {

// Immutable facts supplied by a platform-specific detector or deterministic tests.
// This structure describes the host PC, never the emulated Xbox Series X.
struct HostCapabilities {
    std::uint32_t logical_threads{1};
    std::uint64_t available_ram_mb{0};
    std::uint64_t dedicated_vram_mb{0};
    bool cpu_jit_available{false};
    bool vulkan_available{false};
    bool direct3d12_available{false};
    bool software_renderer_available{true};
    std::string gpu_name{"unknown"};
    std::string driver_version{"unknown"};
};

} // namespace xbridge::host
