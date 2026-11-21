#include "xbridge/core/config.hpp"
#include "xbridge/core/runtime.hpp"
#include "xbridge/cpu/test_cpu.hpp"
#include "xbridge/memory/memory.hpp"
#include "xbridge/host/execution_policy.hpp"
#include <iostream>
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

template <typename Exception, typename F>
void check_throws_as(F&& fn, const char* message) {
    try {
        fn();
        check(false, message);
    } catch (const Exception&) {
        check(true, message);
    } catch (const std::exception&) {
        check(false, message);
    }
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
    memory.write32(16, 0x12345678);
    check(memory.read32(16) == 0x12345678, "32-bit guest memory round trip");
    check_throws_as<std::out_of_range>(
        [&] { memory.read32(254); }, "reject out-of-range memory read");

    memory::GuestMemory boundary_memory(16);
    boundary_memory.write8(15, 0xA5);
    check(boundary_memory.read8(15) == 0xA5,
          "allow byte access at the last valid address");
    boundary_memory.write32(12, 0x12345678);
    check(boundary_memory.read32(12) == 0x12345678,
          "allow 32-bit access ending at the last valid address");
    check_throws_as<std::out_of_range>(
        [&] { boundary_memory.read8(16); }, "reject read past the last valid address");
    check_throws_as<std::out_of_range>(
        [&] { boundary_memory.write8(16, 0); }, "reject write past the last valid address");
    check_throws_as<std::out_of_range>(
        [&] { boundary_memory.read32(13); }, "reject partially out-of-range 32-bit read");
    check_throws_as<std::out_of_range>(
        [&] { boundary_memory.write32(13, 0); }, "reject partially out-of-range 32-bit write");

    memory::GuestMemory empty_memory(0);
    empty_memory.load(0, {});
    check(empty_memory.size() == 0, "allow an empty load into zero-sized memory");
    check_throws_as<std::out_of_range>(
        [&] { empty_memory.read8(0); }, "reject read from zero-sized memory");
    check_throws_as<std::out_of_range>(
        [&] { empty_memory.write8(0, 0); }, "reject write to zero-sized memory");

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

    memory::GuestMemory invalid_register_memory(6);
    cpu::TestCpu invalid_register_cpu(invalid_register_memory);
    invalid_register_cpu.load_program({0x01, 0x04, 0x2A, 0x00, 0x00, 0x00});
    check_throws_as<std::runtime_error>(
        [&] { invalid_register_cpu.run(); }, "reject invalid TestCpu register");

    memory::GuestMemory invalid_add_register_memory(6);
    cpu::TestCpu invalid_add_register_cpu(invalid_add_register_memory);
    invalid_add_register_cpu.load_program({0x02, 0x04, 0x2A, 0x00, 0x00, 0x00});
    check_throws_as<std::runtime_error>(
        [&] { invalid_add_register_cpu.run(); }, "reject invalid register in ADD_IMM");

    memory::GuestMemory invalid_store_register_memory(6);
    cpu::TestCpu invalid_store_register_cpu(invalid_store_register_memory);
    invalid_store_register_cpu.load_program({0x03, 0x04, 0x10, 0x00, 0x00, 0x00});
    check_throws_as<std::runtime_error>(
        [&] { invalid_store_register_cpu.run(); }, "reject invalid register in STORE");

    memory::GuestMemory unknown_opcode_memory(1);
    cpu::TestCpu unknown_opcode_cpu(unknown_opcode_memory);
    unknown_opcode_cpu.load_program({0xFE});
    check_throws_as<std::runtime_error>(
        [&] { unknown_opcode_cpu.run(); }, "reject unknown TestCpu opcode");

    memory::GuestMemory truncated_program_memory(3);
    cpu::TestCpu truncated_program_cpu(truncated_program_memory);
    truncated_program_cpu.load_program({0x01, 0x00, 0x2A});
    check_throws_as<std::out_of_range>(
        [&] { truncated_program_cpu.run(); }, "reject truncated TestCpu instruction");

    memory::GuestMemory instruction_limit_memory(6);
    cpu::TestCpu instruction_limit_cpu(instruction_limit_memory);
    instruction_limit_cpu.load_program({0x01, 0x00, 0x01, 0x00, 0x00, 0x00});
    check_throws_as<std::runtime_error>(
        [&] { instruction_limit_cpu.run(1); }, "stop TestCpu at its instruction limit");

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
