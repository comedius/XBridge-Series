#include "xbridge/host/execution_policy.hpp"
#include <sstream>

namespace xbridge::host {
namespace {
const char* graphics_name(GraphicsBackend backend) {
    switch (backend) {
    case GraphicsBackend::Vulkan: return "Vulkan";
    case GraphicsBackend::Direct3D12: return "Direct3D 12";
    case GraphicsBackend::Software: return "software renderer";
    case GraphicsBackend::Unavailable: return "unavailable";
    }
    return "unavailable";
}
}

ExecutionPlan select_execution_plan(const HostCapabilities& host,
                                    const ExecutionSettings& settings) {
    ExecutionPlan plan;
    plan.profile = settings.profile;

    // A forced preference is honored when supported. Automatic mode prefers
    // Vulkan, then D3D12; software is a correctness/fallback path, not a promise
    // of acceptable frame rates.
    switch (settings.graphics) {
    case GraphicsPreference::Vulkan:
        if (host.vulkan_available) plan.graphics = GraphicsBackend::Vulkan;
        break;
    case GraphicsPreference::Direct3D12:
        if (host.direct3d12_available) plan.graphics = GraphicsBackend::Direct3D12;
        break;
    case GraphicsPreference::Software:
        if (host.software_renderer_available) plan.graphics = GraphicsBackend::Software;
        break;
    case GraphicsPreference::Automatic:
        if (host.vulkan_available) plan.graphics = GraphicsBackend::Vulkan;
        else if (host.direct3d12_available) plan.graphics = GraphicsBackend::Direct3D12;
        else if (settings.allow_software_fallback && host.software_renderer_available)
            plan.graphics = GraphicsBackend::Software;
        break;
    }

    if (plan.graphics == GraphicsBackend::Unavailable && settings.allow_software_fallback &&
        host.software_renderer_available) {
        plan.graphics = GraphicsBackend::Software;
        plan.fallback_used = true;
    }

    plan.graphics_accelerated = plan.graphics == GraphicsBackend::Vulkan ||
                                plan.graphics == GraphicsBackend::Direct3D12;
    if (plan.graphics == GraphicsBackend::Software) plan.fallback_used = true;

    // Interpreter is the conservative reference path. JIT is selected only
    // when explicitly requested or available under automatic policy.
    plan.cpu = (settings.cpu == CpuPreference::Interpreter || !host.cpu_jit_available)
                   ? CpuBackend::Interpreter : CpuBackend::Jit;

    std::ostringstream report;
    report << "Graphics: " << graphics_name(plan.graphics)
           << "; CPU: " << (plan.cpu == CpuBackend::Jit ? "JIT" : "interpreter")
           << "; profile: ";
    switch (plan.profile) {
    case PerformanceProfile::Low: report << "low"; break;
    case PerformanceProfile::Balanced: report << "balanced"; break;
    case PerformanceProfile::High: report << "high"; break;
    case PerformanceProfile::Custom: report << "custom"; break;
    }
    if (plan.fallback_used) report << "; software fallback active (performance may be limited)";
    if (plan.graphics == GraphicsBackend::Unavailable)
        report << "; no supported graphics backend; initialization must fail clearly";
    report << ". Host capabilities are not guest hardware semantics.";
    plan.report = report.str();
    return plan;
}

} // namespace xbridge::host
