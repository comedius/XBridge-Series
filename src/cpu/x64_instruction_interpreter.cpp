#include "xbridge/cpu/x64_instruction_interpreter.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>

namespace xbridge::cpu {

[[nodiscard]] bool is_address_range_valid(
    std::uint64_t address,
    std::size_t length,
    const memory::GuestMemory& memory) noexcept
{
    if (address > static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max())) {
        return false;
    }
    const auto addr32 = static_cast<std::uint32_t>(address);
    if (static_cast<std::size_t>(addr32) > memory.size()) {
        return false;
    }
    if (memory.size() - static_cast<std::size_t>(addr32) < length) {
        return false;
    }
    return true;
}

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
            const auto address = state.gpr(address_reg.base);
            if (instruction.opcode == 0x89) {
                // MOV r/m, r: Speicher = Register
                const auto value =
                    (operand_size == X64OperandSize::Bits64)
                        ? state.gpr(instruction.src_reg.base)
                        : static_cast<std::uint32_t>(state.gpr(instruction.src_reg.base));
                const auto write_length =
                    (operand_size == X64OperandSize::Bits64) ? 8ULL : 4ULL;
                if (!is_address_range_valid(address, write_length, memory)) {
                    return X64ExecuteStatus::MemoryAccessError;
                }
                try {
                    if (operand_size == X64OperandSize::Bits64) {
                        // 64-bit Speicherzugang: vollständigen Bereich validieren,
                        // dann 8x write8 (little-endian). Keine Teiländerung bei Fehler.
                        for (int i = 0; i < 8; ++i) {
                            memory.write8(static_cast<std::uint32_t>(address + static_cast<std::uint64_t>(i)),
                                          static_cast<std::uint8_t>((value >> (8 * i)) & 0xFF));
                        }
                    } else {
                        memory.write32(static_cast<std::uint32_t>(address), static_cast<std::uint32_t>(value));
                    }
                } catch (const std::out_of_range&) {
                    return X64ExecuteStatus::MemoryAccessError;
                }
            } else {
                // MOV r, r/m: Register = Speicher
                const auto read_length =
                    (operand_size == X64OperandSize::Bits64) ? 8ULL : 4ULL;
                if (!is_address_range_valid(address, read_length, memory)) {
                    return X64ExecuteStatus::MemoryAccessError;
                }
                try {
                    std::uint64_t value = 0;
                    if (operand_size == X64OperandSize::Bits64) {
                        for (int i = 0; i < 8; ++i) {
                            value |= static_cast<std::uint64_t>(
                                         memory.read8(static_cast<std::uint32_t>(address + static_cast<std::uint64_t>(i))))
                                     << (8 * i);
                        }
                    } else {
                        value = memory.read32(static_cast<std::uint32_t>(address));
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

[[nodiscard]] X64ExecuteStatus step_x64_instruction(
    std::span<const std::uint8_t> bytes,
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept
{
    const auto decoded = decode_x64_instruction(bytes);
    return execute_instruction(decoded, state, memory);
}

[[nodiscard]] X64ExecuteStatus step_x64_at_rip(
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept
{
    constexpr auto fetch_capacity = std::size_t{15};

    if (state.rip > static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max())) {
        return X64ExecuteStatus::MemoryAccessError;
    }

    const auto rip_offset = static_cast<std::size_t>(state.rip);
    if (rip_offset >= memory.size()) {
        return X64ExecuteStatus::MemoryAccessError;
    }

    const auto count = std::min(fetch_capacity, memory.size() - rip_offset);
    std::array<std::uint8_t, fetch_capacity> buffer{};

    try {
        const auto rip = static_cast<std::uint32_t>(state.rip);
        for (std::size_t i = 0; i < count; ++i) {
            buffer[i] = memory.read8(rip + static_cast<std::uint32_t>(i));
        }
    } catch (const std::out_of_range&) {
        return X64ExecuteStatus::MemoryAccessError;
    }

    return step_x64_instruction(std::span<const std::uint8_t>(buffer.data(), count), state, memory);
}

} // namespace xbridge::cpu
