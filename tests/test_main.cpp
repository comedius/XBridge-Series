#include "xbridge/core/config.hpp"
#include "xbridge/core/runtime.hpp"
#include "xbridge/cpu/test_cpu.hpp"
#include "xbridge/cpu/x64_cpu_state.hpp"
#include "xbridge/memory/memory.hpp"
#include "xbridge/host/execution_policy.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <type_traits>
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

bool registers_are_zero(const xbridge::cpu::CpuState& state) {
    return state.registers[0] == 0 && state.registers[1] == 0 &&
           state.registers[2] == 0 && state.registers[3] == 0;
}

bool state_is_reset(const xbridge::cpu::CpuState& state) {
    return registers_are_zero(state) && state.instruction_pointer == 0 && !state.halted;
}

void test_x64_cpu_state() {
    using namespace xbridge::cpu;

    X64CpuState state;
    const X64CpuState& const_state = state;
    static_assert(std::is_same_v<
                  decltype(state.gpr(X64GeneralRegister::Rax)), std::uint64_t&>);
    static_assert(std::is_same_v<
                  decltype(const_state.gpr(X64GeneralRegister::Rax)),
                  const std::uint64_t&>);

    bool all_registers_are_zero = true;
    for (const auto value : state.general_purpose_registers) {
        all_registers_are_zero = all_registers_are_zero && value == 0;
    }
    check(state.general_purpose_registers.size() == 16 &&
          all_registers_are_zero && state.rip == 0 && state.rflags == 0,
          "X64CpuState default initializes all storage to zero");

    state.gpr(X64GeneralRegister::Rax) = 0x1122334455667788;
    state.gpr(X64GeneralRegister::Rcx) = 0x8877665544332211;
    state.gpr(X64GeneralRegister::R8) = 0xFEDCBA9876543210;
    check(state.gpr(X64GeneralRegister::Rax) == 0x1122334455667788 &&
          state.gpr(X64GeneralRegister::Rcx) == 0x8877665544332211 &&
          state.gpr(X64GeneralRegister::R8) == 0xFEDCBA9876543210,
          "X64CpuState stores independently indexed general registers");

    state.rip = 0x123456789ABCDEF0;
    state.rflags = 0x0000000000000A95;
    check(state.rip == 0x123456789ABCDEF0 &&
          state.rflags == 0x0000000000000A95,
          "X64CpuState stores RIP and RFLAGS independently");

    state.gpr(X64GeneralRegister::R15) = 0xA5A5A5A55A5A5A5A;
    check(state.gpr(X64GeneralRegister::R15) == 0xA5A5A5A55A5A5A5A,
          "X64CpuState accesses the final R15 register slot");
    check(const_state.gpr(X64GeneralRegister::R15) == 0xA5A5A5A55A5A5A5A,
          "const X64CpuState access reads the selected register");

    const auto invalid_register = static_cast<X64GeneralRegister>(16);
    check_throws<std::out_of_range>(
        [&] { (void)state.gpr(invalid_register); },
        "mutable X64CpuState register access rejects an invalid index");
    check_throws<std::out_of_range>(
        [&] { (void)const_state.gpr(invalid_register); },
        "const X64CpuState register access rejects an invalid index");
}

void test_cpu_execution_hardening() {
    using namespace xbridge;

    memory::GuestMemory last_byte_memory(1);
    cpu::TestCpu last_byte_cpu(last_byte_memory);
    last_byte_cpu.load_program({0xFF});
    last_byte_cpu.run(1);
    check(last_byte_cpu.state().instruction_pointer == 1 && last_byte_cpu.state().halted,
          "TestCpu executes Halt at the last valid guest address");
    check_does_not_throw([&] { last_byte_cpu.run(0); },
                         "run after Halt is a no-op even with a zero instruction limit");
    check(last_byte_cpu.state().instruction_pointer == 1 && last_byte_cpu.state().halted,
          "run after Halt preserves the halted CPU state");

    const auto check_invalid_register = [](const std::vector<std::uint8_t>& program,
                                           const char* error_message,
                                           const char* state_message) {
        memory::GuestMemory memory(program.size());
        cpu::TestCpu test_cpu(memory);
        test_cpu.load_program(program);
        check_throws<std::runtime_error>([&] { test_cpu.run(); }, error_message);
        check(test_cpu.state().instruction_pointer == program.size() &&
              registers_are_zero(test_cpu.state()) && !test_cpu.state().halted,
              state_message);
    };
    check_invalid_register({0x01, 0x04, 0x01, 0x00, 0x00, 0x00},
                           "LoadImm rejects an invalid register",
                           "LoadImm failure preserves registers and records consumed operands");
    check_invalid_register({0x02, 0x04, 0x01, 0x00, 0x00, 0x00},
                           "AddImm rejects an invalid register",
                           "AddImm failure preserves registers and records consumed operands");
    check_invalid_register({0x03, 0x04, 0x00, 0x00, 0x00, 0x00},
                           "Store rejects an invalid register",
                           "Store register failure preserves registers and records consumed operands");

    const auto check_truncated_program = [](const std::vector<std::uint8_t>& program,
                                            std::uint32_t expected_ip,
                                            const char* error_message,
                                            const char* state_message) {
        memory::GuestMemory memory(program.size());
        cpu::TestCpu test_cpu(memory);
        test_cpu.load_program(program);
        check_throws<std::out_of_range>([&] { test_cpu.run(); }, error_message);
        check(test_cpu.state().instruction_pointer == expected_ip &&
              registers_are_zero(test_cpu.state()) && !test_cpu.state().halted,
              state_message);
    };
    check_truncated_program({0x01}, 1,
                            "LoadImm rejects a missing register operand",
                            "missing register leaves the IP at the program boundary");
    check_truncated_program({0x01, 0x00, 0x34, 0x12}, 4,
                            "LoadImm rejects a truncated immediate",
                            "truncated LoadImm leaves the IP and registers consistent");
    check_truncated_program({0x02, 0x00, 0x34}, 3,
                            "AddImm rejects a truncated immediate",
                            "truncated AddImm leaves the IP and registers consistent");
    check_truncated_program({0x03, 0x00, 0x0C, 0x00, 0x00}, 5,
                            "Store rejects a truncated address operand",
                            "truncated Store leaves the IP and registers consistent");

    memory::GuestMemory unknown_memory(2);
    cpu::TestCpu unknown_cpu(unknown_memory);
    unknown_cpu.load_program({0x7E, 0xFF});
    check_throws<std::runtime_error>([&] { unknown_cpu.run(); },
                                    "unknown opcode throws a runtime error");
    check(unknown_cpu.state().instruction_pointer == 1 &&
          registers_are_zero(unknown_cpu.state()) && !unknown_cpu.state().halted,
          "unknown opcode leaves a resumable state after its fetched byte");
    check_does_not_throw([&] { unknown_cpu.run(); },
                         "run after unknown-opcode error resumes from the current IP");
    check(unknown_cpu.state().instruction_pointer == 2 && unknown_cpu.state().halted,
          "resumed execution reaches the following Halt");

    const std::vector<std::uint8_t> limit_program{0x01, 0x00, 0x07, 0x00, 0x00, 0x00, 0xFF};
    memory::GuestMemory zero_limit_memory(limit_program.size());
    cpu::TestCpu zero_limit_cpu(zero_limit_memory);
    zero_limit_cpu.load_program(limit_program);
    check_throws<std::runtime_error>([&] { zero_limit_cpu.run(0); },
                                    "zero instruction limit fails before execution");
    check(state_is_reset(zero_limit_cpu.state()),
          "zero instruction limit preserves the initial CPU state");

    memory::GuestMemory normal_limit_memory(limit_program.size());
    cpu::TestCpu normal_limit_cpu(normal_limit_memory);
    normal_limit_cpu.load_program(limit_program);
    check_does_not_throw([&] { normal_limit_cpu.run(10); },
                         "normal instruction limit allows the program to halt");
    check(normal_limit_cpu.state().registers[0] == 7 &&
          normal_limit_cpu.state().instruction_pointer == limit_program.size() &&
          normal_limit_cpu.state().halted,
          "normal instruction limit preserves completed instruction state");

    memory::GuestMemory exact_limit_memory(limit_program.size());
    cpu::TestCpu exact_limit_cpu(exact_limit_memory);
    exact_limit_cpu.load_program(limit_program);
    check_does_not_throw([&] { exact_limit_cpu.run(2); },
                         "exact instruction limit allows the final Halt");
    check(exact_limit_cpu.state().halted,
          "exact instruction limit finishes in a halted state");

    memory::GuestMemory exceeded_limit_memory(limit_program.size());
    cpu::TestCpu exceeded_limit_cpu(exceeded_limit_memory);
    exceeded_limit_cpu.load_program(limit_program);
    check_throws<std::runtime_error>([&] { exceeded_limit_cpu.run(1); },
                                     "instruction limit rejects an additional instruction");
    check(exceeded_limit_cpu.state().registers[0] == 7 &&
          exceeded_limit_cpu.state().instruction_pointer == 6 &&
          !exceeded_limit_cpu.state().halted,
          "instruction-limit error preserves completed work and the next IP");
    check_does_not_throw([&] { exceeded_limit_cpu.run(1); },
                         "run after instruction-limit error resumes at the next instruction");
    check(exceeded_limit_cpu.state().halted &&
          exceeded_limit_cpu.state().instruction_pointer == limit_program.size(),
          "resumed instruction-limit run reaches Halt");

    const std::vector<std::uint8_t> store_to_end{
        0x01, 0x00, 0xD4, 0xC3, 0xB2, 0xA1,
        0x03, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xFF
    };
    memory::GuestMemory end_store_memory(32);
    cpu::TestCpu end_store_cpu(end_store_memory);
    end_store_cpu.load_program(store_to_end);
    check_does_not_throw([&] { end_store_cpu.run(); },
                         "CPU Store ending at the last valid guest byte succeeds");
    check(end_store_memory.read32(28) == 0xA1B2C3D4,
          "CPU Store writes the final valid guest word");

    const std::vector<std::uint8_t> unaligned_store{
        0x01, 0x00, 0xD4, 0xC3, 0xB2, 0xA1,
        0x03, 0x00, 0x11, 0x00, 0x00, 0x00, 0xFF
    };
    memory::GuestMemory unaligned_memory(32);
    cpu::TestCpu unaligned_cpu(unaligned_memory);
    unaligned_cpu.load_program(unaligned_store);
    check_does_not_throw([&] { unaligned_cpu.run(); },
                         "CPU Store supports the GuestMemory unaligned access contract");
    check(unaligned_memory.read32(17) == 0xA1B2C3D4,
          "unaligned CPU Store preserves the written value");

    const std::vector<std::uint8_t> failing_store{
        0x01, 0x00, 0xD4, 0xC3, 0xB2, 0xA1,
        0x03, 0x00, 0x0E, 0x00, 0x00, 0x00
    };
    memory::GuestMemory failing_store_memory(16);
    cpu::TestCpu failing_store_cpu(failing_store_memory);
    failing_store_cpu.load_program(failing_store);
    check_throws<std::out_of_range>([&] { failing_store_cpu.run(); },
                                    "out-of-range CPU Store propagates a memory exception");
    check(failing_store_cpu.state().registers[0] == 0xA1B2C3D4 &&
          failing_store_cpu.state().instruction_pointer == failing_store.size() &&
          !failing_store_cpu.state().halted &&
          failing_store_memory.read8(14) == 0 && failing_store_memory.read8(15) == 0,
          "failed CPU Store preserves registers, IP, and guest memory");
    check_throws<std::out_of_range>([&] { failing_store_cpu.run(); },
                                    "run after failed Store cannot fetch beyond the program");
    check(failing_store_cpu.state().instruction_pointer == failing_store.size() &&
          failing_store_cpu.state().registers[0] == 0xA1B2C3D4 &&
          !failing_store_cpu.state().halted,
          "repeated fetch failure leaves CPU state unchanged");
}

void test_cpu_program_lifecycle() {
    using namespace xbridge;

    memory::GuestMemory memory(16);
    cpu::TestCpu test_cpu(memory);
    check(state_is_reset(test_cpu.state()), "TestCpu starts with zeroed registers, IP, and halt state");

    const std::vector<std::uint8_t> previous_program{
        0x01, 0x02, 0x2A, 0x00, 0x00, 0x00,
        0x01, 0x01, 0x63, 0x00, 0x00, 0x00,
        0xFF
    };
    test_cpu.load_program(previous_program);
    check(state_is_reset(test_cpu.state()), "load_program resets the initial CPU state");
    test_cpu.run();
    check(test_cpu.state().registers[1] == 99 &&
          test_cpu.state().registers[2] == 42 &&
          test_cpu.state().halted,
          "previous program establishes state for reload regression");

    const std::vector<std::uint8_t> shorter_program{0x01, 0x00, 0x05, 0x00, 0x00, 0x00};
    test_cpu.load_program(shorter_program);
    check(state_is_reset(test_cpu.state()),
          "reloading after Halt resets all registers, IP, and halt state");
    check(memory.read8(6) == 0x01,
          "load_program does not clear guest bytes beyond the new program");
    check_throws<std::out_of_range>([&] { test_cpu.run(); },
                                    "short program cannot execute stale trailing instructions");
    check(test_cpu.state().instruction_pointer == shorter_program.size() &&
          test_cpu.state().registers[0] == 5 &&
          test_cpu.state().registers[1] == 0 &&
          test_cpu.state().registers[2] == 0 &&
          !test_cpu.state().halted,
          "program-boundary failure preserves new program state without stale execution");

    const std::vector<std::uint8_t> empty_program;
    test_cpu.load_program(empty_program);
    check(state_is_reset(test_cpu.state()), "loading an empty program resets CPU state");
    check(memory.read8(0) == 0x01,
          "loading an empty program leaves prior GuestMemory bytes unchanged");
    check_throws<std::out_of_range>([&] { test_cpu.run(); },
                                    "running an empty program rejects fetch outside its extent");
    check(state_is_reset(test_cpu.state()),
          "empty-program fetch failure preserves reset state");

    memory::GuestMemory small_memory(1);
    cpu::TestCpu small_cpu(small_memory);
    small_cpu.load_program({0xFF});
    small_cpu.run();
    check(small_cpu.state().halted, "small memory instance runs a fitting program");
    check_throws<std::out_of_range>([&] { small_cpu.load_program({0xFF, 0xFF}); },
                                    "load_program rejects programs larger than GuestMemory");
    check(state_is_reset(small_cpu.state()),
          "failed program load leaves the CPU state reset");
    check(small_memory.read8(0) == 0xFF,
          "failed program load leaves GuestMemory unchanged");
    check_throws<std::out_of_range>([&] { small_cpu.run(); },
                                    "run after failed load does not execute the old program");
    small_cpu.load_program({0xFF});
    small_cpu.run();
    check(small_cpu.state().instruction_pointer == 1 && small_cpu.state().halted,
          "TestCpu loads and halts a valid program after a failed load");
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

    test_x64_cpu_state();
    test_cpu_execution_hardening();
    test_cpu_program_lifecycle();

    host::HostCapabilities capable_host{};
    capable_host.logical_threads = 16;
    capable_host.available_ram_mb = 8192;
    capable_host.dedicated_vram_mb = 8192;
    capable_host.cpu_jit_available = true;
    capable_host.vulkan_available = true;
    capable_host.direct3d12_available = true;
    const auto accelerated = host::select_execution_plan(capable_host, {});
    check(accelerated.graphics == host::GraphicsBackend::Vulkan &&
          accelerated.graphics_accelerated,
          "automatic policy selects supported graphics acceleration");
    check(accelerated.cpu == host::CpuBackend::Interpreter,
          "automatic CPU selection uses interpreter when JIT is available");

    host::HostCapabilities limited_host{};
    limited_host.software_renderer_available = true;
    const auto fallback = host::select_execution_plan(limited_host, {});
    check(fallback.graphics == host::GraphicsBackend::Software && fallback.fallback_used,
          "automatic policy uses software fallback when acceleration is unavailable");
    check(fallback.cpu == host::CpuBackend::Interpreter,
          "automatic CPU selection uses interpreter when JIT is unavailable");

    host::ExecutionSettings no_fallback{};
    no_fallback.allow_software_fallback = false;
    const auto unavailable = host::select_execution_plan(limited_host, no_fallback);
    check(unavailable.graphics == host::GraphicsBackend::Unavailable,
          "policy reports unavailable backend when fallback is disabled");

    host::ExecutionSettings manual{};
    manual.graphics = host::GraphicsPreference::Direct3D12;
    manual.cpu = host::CpuPreference::Interpreter;
    const auto overridden = host::select_execution_plan(capable_host, manual);
    check(overridden.graphics == host::GraphicsBackend::Direct3D12,
          "manual graphics preference overrides automatic selection");
    check(overridden.cpu == host::CpuBackend::Interpreter,
          "explicit interpreter preference selects interpreter");

    host::ExecutionSettings jit_requested{};
    jit_requested.cpu = host::CpuPreference::Jit;
    const auto jit_available = host::select_execution_plan(capable_host, jit_requested);
    check(jit_available.cpu == host::CpuBackend::Jit,
          "explicit JIT preference selects JIT when available");
    const auto jit_unavailable = host::select_execution_plan(limited_host, jit_requested);
    check(jit_unavailable.cpu == host::CpuBackend::Interpreter,
          "explicit JIT preference falls back to interpreter when unavailable");
    check(jit_unavailable.report.find("JIT requested but unavailable") != std::string::npos,
          "CPU report explains unavailable requested JIT");

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
