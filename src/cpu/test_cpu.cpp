#include "xbridge/cpu/test_cpu.hpp"
#include <stdexcept>

namespace xbridge::cpu {

TestCpu::TestCpu(memory::GuestMemory& memory) : memory_(memory) {}

void TestCpu::load_program(const std::vector<std::uint8_t>& program) {
    state_ = {};
    memory_.load(0, program);
}

std::uint8_t TestCpu::fetch8() {
    const auto value = memory_.read8(state_.instruction_pointer);
    ++state_.instruction_pointer;
    return value;
}

std::uint32_t TestCpu::fetch32() {
    const auto b0 = fetch8();
    const auto b1 = fetch8();
    const auto b2 = fetch8();
    const auto b3 = fetch8();
    return static_cast<std::uint32_t>(b0)
        | (static_cast<std::uint32_t>(b1) << 8)
        | (static_cast<std::uint32_t>(b2) << 16)
        | (static_cast<std::uint32_t>(b3) << 24);
}

void TestCpu::run(std::uint64_t instruction_limit) {
    std::uint64_t executed = 0;
    while (!state_.halted) {
        if (executed++ >= instruction_limit)
            throw std::runtime_error("TestCpu instruction limit exceeded");

        const auto opcode = static_cast<Opcode>(fetch8());
        switch (opcode) {
        case Opcode::LoadImm: {
            const auto reg = fetch8();
            const auto value = fetch32();
            if (reg >= 4) throw std::runtime_error("Invalid register index");
            state_.registers[reg] = value;
            break;
        }
        case Opcode::AddImm: {
            const auto reg = fetch8();
            const auto value = fetch32();
            if (reg >= 4) throw std::runtime_error("Invalid register index");
            state_.registers[reg] += value;
            break;
        }
        case Opcode::Store: {
            const auto reg = fetch8();
            const auto address = fetch32();
            if (reg >= 4) throw std::runtime_error("Invalid register index");
            memory_.write32(address, state_.registers[reg]);
            break;
        }
        case Opcode::Halt:
            state_.halted = true;
            break;
        default:
            throw std::runtime_error("Unknown TestCpu opcode");
        }
    }
}

const CpuState& TestCpu::state() const noexcept { return state_; }

} // namespace xbridge::cpu
