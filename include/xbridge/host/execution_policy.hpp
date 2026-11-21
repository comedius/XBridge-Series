#pragma once
#include "xbridge/host/capabilities.hpp"
#include <string>

namespace xbridge::host {

enum class GraphicsBackend { Vulkan, Direct3D12, Software, Unavailable };
enum class CpuBackend { Jit, Interpreter };
enum class GraphicsPreference { Automatic, Vulkan, Direct3D12, Software };
enum class CpuPreference { Automatic, Jit, Interpreter };
enum class PerformanceProfile { Low, Balanced, High, Custom };

struct ExecutionSettings {
    GraphicsPreference graphics{GraphicsPreference::Automatic};
    CpuPreference cpu{CpuPreference::Automatic};
    PerformanceProfile profile{PerformanceProfile::Balanced};
    bool allow_software_fallback{true};
};

struct ExecutionPlan {
    GraphicsBackend graphics{GraphicsBackend::Unavailable};
    CpuBackend cpu{CpuBackend::Interpreter};
    PerformanceProfile profile{PerformanceProfile::Balanced};
    bool graphics_accelerated{false};
    bool fallback_used{false};
    std::string report;
};

// Pure, deterministic policy. Platform probing and backend initialization live elsewhere.
ExecutionPlan select_execution_plan(const HostCapabilities& host,
                                    const ExecutionSettings& settings);

} // namespace xbridge::host
