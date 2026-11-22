#include "xbridge/cpu/x64_instruction_interpreter.hpp"

#include <stdexcept>

namespace xbridge::cpu {

[[nodiscard]] X64ExecuteStatus execute_instruction(
    const DecodedInstruction& instruction,
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept
{
    switch (instruction.status) {
    case X64DecodeStatus::EmptyInput:
    case X64DecodeStatus::UnknownOpcode:
    case X64DecodeStatus::InvalidEncoding:
    case X64DecodeStatus::TruncatedInput:
        // Nicht ausführbare Instruktionen werden nicht ausgeführt.
        return X64ExecuteStatus::Ok;
    case X64DecodeStatus::Ok:
        break;
    }

    const auto operand_size = instruction.operand_size;

    // --- NOP ---
    if (instruction.opcode == 0x90) {
        state.rip += instruction.length;
        return X64ExecuteStatus::Ok;
    }

    // --- MOV r64/r32, imm64/imm32 ---
    if (instruction.opcode >= 0xB8 && instruction.opcode <= 0xBF) {
        auto value = instruction.immediate;
        if (operand_size == X64OperandSize::Bits32) {
            value = static_cast<std::uint32_t>(value);
        }
        state.gpr(instruction.dest_reg.base) = value;
        state.rip += instruction.length;
        return X64ExecuteStatus::Ok;
    }

    // --- MOV mit ModR/M (0x88, 0x89, 0x8A, 0x8B) ---
    if (instruction.opcode == 0x88 || instruction.opcode == 0x89 ||
        instruction.opcode == 0x8A || instruction.opcode == 0x8B) {
        // 8-bit Varianten werden nicht unterstützt.
        if (operand_size == X64OperandSize::Bits8) {
            return X64ExecuteStatus::Ok;
        }

        if (instruction.memory_operand) {
            // Speicheradresse kommt aus dem rm-Register (dest_reg bei 0x89, src_reg bei 0x8B)
            const auto address_reg =
                (instruction.opcode == 0x89) ? instruction.dest_reg : instruction.src_reg;
            const auto address = static_cast<std::uint32_t>(state.gpr(address_reg.base));
            if (instruction.opcode == 0x89) {
                // MOV r/m, r: Speicher = Register
                const auto value =
                    (operand_size == X64OperandSize::Bits64)
                        ? state.gpr(instruction.src_reg.base)
                        : static_cast<std::uint32_t>(state.gpr(instruction.src_reg.base));
                try {
                    if (operand_size == X64OperandSize::Bits64) {
                        // 64-bit Speicherzugang als 8x write8 (little-endian)
                        for (int i = 0; i < 8; ++i) {
                            memory.write8(static_cast<std::uint32_t>(address + i),
                                          static_cast<std::uint8_t>((value >> (8 * i)) & 0xFF));
                        }
                    } else {
                        memory.write32(address, static_cast<std::uint32_t>(value));
                    }
                } catch (const std::out_of_range&) {
                    return X64ExecuteStatus::MemoryAccessError;
                }
            } else {
                // MOV r, r/m: Register = Speicher
                try {
                    std::uint64_t value = 0;
                    if (operand_size == X64OperandSize::Bits64) {
                        for (int i = 0; i < 8; ++i) {
                            value |= static_cast<std::uint64_t>(
                                         memory.read8(static_cast<std::uint32_t>(address + i)))
                                     << (8 * i);
                        }
                    } else {
                        value = memory.read32(address);
                    }
                    state.gpr(instruction.dest_reg.base) = value;
                } catch (const std::out_of_range&) {
                    return X64ExecuteStatus::MemoryAccessError;
                }
            }
            state.rip += instruction.length;
            return X64ExecuteStatus::Ok;
        }

        // Register-zu-Register
        if (instruction.opcode == 0x89) {
            // MOV r/m, r: dest = rm, src = reg
            const auto value =
                (operand_size == X64OperandSize::Bits64)
                    ? state.gpr(instruction.src_reg.base)
                    : static_cast<std::uint32_t>(state.gpr(instruction.src_reg.base));
            state.gpr(instruction.dest_reg.base) = value;
        } else {
            // MOV r, r/m: dest = reg, src = rm
            const auto value =
                (operand_size == X64OperandSize::Bits64)
                    ? state.gpr(instruction.src_reg.base)
                    : static_cast<std::uint32_t>(state.gpr(instruction.src_reg.base));
            state.gpr(instruction.dest_reg.base) = value;
        }
        state.rip += instruction.length;
        return X64ExecuteStatus::Ok;
    }

    state.rip += instruction.length;
    return X64ExecuteStatus::Ok;
}

} // namespace xbridge::cpu
