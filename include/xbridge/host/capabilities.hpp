#pragma once
#include <cstdint>
#include <string>
#include <utility>

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

// Read-only seam between capability discovery and policy. Implementations expose
// immutable host facts; policy code performs no platform probing or initialization.
class ICapabilityProvider {
public:
    virtual ~ICapabilityProvider() = default;

    // Returned by const reference so the noexcept contract is safe with the
    // std::string members of HostCapabilities (no throwing copy is made).
    [[nodiscard]] virtual const HostCapabilities& capabilities() const noexcept = 0;
};

// Deterministic provider backed by a stored copy of immutable facts. Reads no
// operating-system state; repeated calls return the same stored object.
class FixedCapabilityProvider final : public ICapabilityProvider {
public:
    // Not noexcept: constructing the stored copy can allocate through std::string.
    explicit FixedCapabilityProvider(HostCapabilities capabilities)
        : capabilities_(std::move(capabilities)) {}

    [[nodiscard]] const HostCapabilities& capabilities() const noexcept override {
        return capabilities_;
    }

private:
    const HostCapabilities capabilities_;
};

} // namespace xbridge::host
