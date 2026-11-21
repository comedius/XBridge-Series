#pragma once
#include "xbridge/memory/memory.hpp"
#include <cstdint>
#include <vector>

namespace xbridge::cpu {

// Synthetic test ISA for validating emulator-engine interfaces.
// This is NOT x86-64 and does NOT execute Xbox code.
enum class Opcode : std::uint8_t {
    LoadImm = 0x01, // [opcode][register][u32 little-endian]
    AddImm  = 0x02, // [opcode][register][u32 little-endian]
    Store   = 0x03, // [opcode][register][u32 address]
    Halt    = 0xFF
};

struct CpuState {
    std::uint32_t registers[4]{};
    std::uint32_t instruction_pointer{0};
    bool halted{false};
};

class TestCpu {
public:
    explicit TestCpu(memory::GuestMemory& memory);
    void load_program(const std::vector<std::uint8_t>& program);
    void run(std::uint64_t instruction_limit = 10000);
    const CpuState& state() const noexcept;

private:
    std::uint8_t fetch8();
    std::uint32_t fetch32();
    memory::GuestMemory& memory_;
    CpuState state_{};
};

} // namespace xbridge::cpu
