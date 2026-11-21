#include "xbridge/core/config.hpp"
#include "xbridge/core/runtime.hpp"
#include "xbridge/cpu/test_cpu.hpp"
#include "xbridge/memory/memory.hpp"
#include "xbridge/host/execution_policy.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
int failures = 0;

void check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << "\n";
        ++failures;
    } else {
        std::cout << "[PASS] " << message << "\n";
    }
}

template <typename ExpectedException, typename F>
void check_throws(F&& fn, const char* message) {
    try { fn(); }
    catch (const ExpectedException&) { check(true, message); return; }
    catch (const std::exception&) { check(false, message); return; }
    check(false, message);
}

template <typename F>
void check_does_not_throw(F&& fn, const char* message) {
    try { fn(); check(true, message); }
    catch (const std::exception&) { check(false, message); }
}
}

int main() {
    using namespace xbridge;

    ProfileKind parsed{};
    check(parse_profile("series_x", parsed) && parsed == ProfileKind::SeriesX,
          "parse Series X profile");
    check(!parse_profile("series_s", parsed),
          "Series S is not a supported target");
    check(!parse_profile("unknown", parsed), "reject unknown profile");

    host::HostCapabilities capable_host{};
    capable_host.logical_threads = 16;
    capable_host.available_ram_mb = 8192;
    capable_host.dedicated_vram_mb = 8192;
    capable_host.cpu_jit_available = true;
    capable_host.vulkan_available = true;
    capable_host.direct3d12_available = true;
    const auto accelerated = host::select_execution_plan(capable_host, {});
    check(accelerated.graphics == host::GraphicsBackend::Vulkan &&
          accelerated.cpu == host::CpuBackend::Jit && accelerated.graphics_accelerated,
          "automatic policy selects supported acceleration");

    host::HostCapabilities limited_host{};
    limited_host.software_renderer_available = true;
    const auto fallback = host::select_execution_plan(limited_host, {});
    check(fallback.graphics == host::GraphicsBackend::Software && fallback.fallback_used,
          "automatic policy uses software fallback when acceleration is unavailable");

    host::ExecutionSettings no_fallback{};
    no_fallback.allow_software_fallback = false;
    const auto unavailable = host::select_execution_plan(limited_host, no_fallback);
    check(unavailable.graphics == host::GraphicsBackend::Unavailable,
          "policy reports unavailable backend when fallback is disabled");

    host::ExecutionSettings manual{};
    manual.graphics = host::GraphicsPreference::Direct3D12;
    manual.cpu = host::CpuPreference::Interpreter;
    const auto overridden = host::select_execution_plan(capable_host, manual);
    check(overridden.graphics == host::GraphicsBackend::Direct3D12 &&
          overridden.cpu == host::CpuBackend::Interpreter,
          "manual backend preferences override automatic defaults");

    memory::GuestMemory memory(256);
    check(memory.read8(0) == 0, "guest memory is zero initialized");
    memory.write32(16, 0x12345678);
    check(memory.read32(16) == 0x12345678, "32-bit guest memory round trip");
    check(memory.read8(16) == 0x78 && memory.read8(19) == 0x12,
          "32-bit guest memory accesses are little-endian");
    memory.write32(17, 0xA1B2C3D4);
    check(memory.read32(17) == 0xA1B2C3D4, "32-bit accesses support unaligned addresses");
    memory.write8(255, 0x5A);
    check(memory.read8(255) == 0x5A, "read and write the last valid guest address");
    memory.write32(252, 0x89ABCDEF);
    check(memory.read32(252) == 0x89ABCDEF, "32-bit access ending at memory boundary succeeds");
    check_throws<std::out_of_range>([&] { memory.read8(256); },
                                    "reject read at one-past-end address");
    check_throws<std::out_of_range>([&] { memory.write8(256, 0); },
                                    "reject write at one-past-end address");
    check_throws<std::out_of_range>([&] { memory.read32(253); },
                                    "reject 32-bit read crossing memory boundary");
    check_throws<std::out_of_range>([&] { memory.write32(253, 0); },
                                    "reject 32-bit write crossing memory boundary");

    const std::vector<std::uint8_t> tail{0xAB, 0xCD};
    memory.load(254, tail);
    check(memory.read8(254) == 0xAB && memory.read8(255) == 0xCD,
          "load succeeds when data ends at memory boundary");
    const std::vector<std::uint8_t> empty_data;
    check_does_not_throw([&] { memory.load(256, empty_data); },
                         "empty load at one-past-end is a no-op");
    check_throws<std::out_of_range>([&] { memory.load(257, empty_data); },
                                    "reject empty load beyond one-past-end");
    check_throws<std::out_of_range>([&] { memory.load(255, tail); },
                                    "reject load crossing memory boundary");

    memory::GuestMemory empty_memory(0);
    check_throws<std::out_of_range>([&] { empty_memory.read8(0); },
                                    "reject read from zero-sized memory");
    check_throws<std::out_of_range>([&] { empty_memory.write8(0, 0); },
                                    "reject write to zero-sized memory");
    check_does_not_throw([&] { empty_memory.load(0, empty_data); },
                         "empty load into zero-sized memory is a no-op");
    check_throws<std::out_of_range>(
        [&] { memory.read32(std::numeric_limits<std::uint32_t>::max() - 1U); },
        "reject high guest address without wrapping");

    const auto address_space_size =
        static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max()) + 1U;
    if (std::numeric_limits<std::size_t>::max() > address_space_size) {
        check_throws<std::length_error>(
            [&] { memory::GuestMemory oversized(static_cast<std::size_t>(address_space_size + 1U)); },
            "reject memory size beyond 32-bit guest address space");
    }

    // r0 = 7; r0 += 5; store r0 at address 64; halt
    const std::vector<std::uint8_t> program{
        0x01, 0x00, 0x07, 0x00, 0x00, 0x00,
        0x02, 0x00, 0x05, 0x00, 0x00, 0x00,
        0x03, 0x00, 0x40, 0x00, 0x00, 0x00,
        0xFF
    };
    memory::GuestMemory cpu_memory(256);
    cpu::TestCpu cpu(cpu_memory);
    cpu.load_program(program);
    cpu.run();
    check(cpu_memory.read32(64) == 12, "TestCpu executes synthetic program");

    std::ostringstream output;
    Runtime runtime(ProfileKind::SeriesX);
    check(runtime.run_demo(output) == 0, "runtime demo succeeds");
    check(output.str().find("not Xbox emulation") != std::string::npos,
          "runtime clearly identifies scaffold status");

    if (failures != 0) {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}
