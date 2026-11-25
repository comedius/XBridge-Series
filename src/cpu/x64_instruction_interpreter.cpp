#include "xbridge/cpu/x64_instruction_interpreter.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace xbridge::cpu {
namespace {

constexpr std::size_t kMaxFetchLength = 15U;
constexpr std::size_t kMaxDataAccessLength = 8U;

[[nodiscard]] X64ExecutionResult memory_error(const memory::MemoryResult& result) noexcept {
    X64ExecutionResult outcome{};
    outcome.status = X64ExecuteStatus::MemoryAccessError;
    outcome.memory_result = result;
    return outcome;
}

[[nodiscard]] std::size_t data_access_length(X64OperandSize operand_size) noexcept {
    return operand_size == X64OperandSize::Bits64 ? 8U : 4U;
}

[[nodiscard]] X64ExecutionResult execute_memory_mov_via(
    const DecodedInstruction& instruction,
    X64CpuState& state,
    memory::IGuestVirtualMemory& memory) noexcept
{
    const auto address_reg =
        (instruction.opcode == 0x89) ? instruction.dest_reg : instruction.src_reg;
    const memory::GuestVirtualAddress address{state.gpr(address_reg.base)};
    const auto length = data_access_length(instruction.operand_size);

    if (instruction.opcode == 0x89) {
        // MOV r/m, r: einen lokalen Little-Endian-Puffer vollständig füllen und
        // erst nach vollständig erfolgreichem Schreibzugriff RIP fortschreiben.
        const auto value = (instruction.operand_size == X64OperandSize::Bits64)
                               ? state.gpr(instruction.src_reg.base)
                               : static_cast<std::uint32_t>(state.gpr(instruction.src_reg.base));
        std::array<std::uint8_t, kMaxDataAccessLength> buffer{};
        for (std::size_t index = 0U; index < length; ++index) {
            buffer[index] = static_cast<std::uint8_t>((value >> (8U * index)) & 0xFFU);
        }

        const auto result =
            memory.write(address, std::span<const std::uint8_t>(buffer.data(), length));
        if (!result.ok || result.completed_bytes != static_cast<std::uint64_t>(length)) {
            return memory_error(result);
        }
    } else {
        // MOV r, r/m: der Ziel-GPR wird erst nach vollständig erfolgreichem
        // Lesevorgang verändert.
        std::array<std::uint8_t, kMaxDataAccessLength> buffer{};
        const auto result = memory.read(address, std::span<std::uint8_t>(buffer.data(), length));
        if (!result.ok || result.completed_bytes != static_cast<std::uint64_t>(length)) {
            return memory_error(result);
        }

        std::uint64_t value = 0U;
        for (std::size_t index = 0U; index < length; ++index) {
            value |= static_cast<std::uint64_t>(buffer[index]) << (8U * index);
        }
        state.gpr(instruction.dest_reg.base) = value;
    }

    state.rip += instruction.length;
    return {};
}

} // namespace

[[nodiscard]] X64ExecutionResult execute_instruction_via(
    const DecodedInstruction& instruction,
    X64CpuState& state,
    memory::IGuestVirtualMemory& memory) noexcept
{
    switch (instruction.status) {
    case X64DecodeStatus::EmptyInput:
    case X64DecodeStatus::UnknownOpcode:
    case X64DecodeStatus::InvalidEncoding:
    case X64DecodeStatus::TruncatedInput:
        // Nicht ausführbare Instruktionen bleiben No-Ops.
        return {};
    case X64DecodeStatus::Ok:
        break;
    }

    const auto operand_size = instruction.operand_size;

    // --- NOP ---
    if (instruction.opcode == 0x90) {
        state.rip += instruction.length;
        return {};
    }

    // --- MOV r64/r32, imm64/imm32 ---
    if (instruction.opcode >= 0xB8 && instruction.opcode <= 0xBF) {
        auto value = instruction.immediate;
        if (operand_size == X64OperandSize::Bits32) {
            value = static_cast<std::uint32_t>(value);
        }
        state.gpr(instruction.dest_reg.base) = value;
        state.rip += instruction.length;
        return {};
    }

    // --- MOV mit ModR/M (0x88, 0x89, 0x8A, 0x8B) ---
    if (instruction.opcode == 0x88 || instruction.opcode == 0x89 ||
        instruction.opcode == 0x8A || instruction.opcode == 0x8B) {
        // 8-bit Varianten werden nicht unterstützt.
        if (operand_size == X64OperandSize::Bits8) {
            return {};
        }

        if (instruction.memory_operand) {
            return execute_memory_mov_via(instruction, state, memory);
        }

        // Register-zu-Register: wie im Legacy-Pfad wird der Quellwert (bei 32 Bit
        // auf uint32_t gekürzt) in das Zielregister geschrieben.
        const auto value = (operand_size == X64OperandSize::Bits64)
                               ? state.gpr(instruction.src_reg.base)
                               : static_cast<std::uint32_t>(state.gpr(instruction.src_reg.base));
        state.gpr(instruction.dest_reg.base) = value;
        state.rip += instruction.length;
        return {};
    }

    state.rip += instruction.length;
    return {};
}

[[nodiscard]] X64ExecutionResult step_x64_instruction_via(
    std::span<const std::uint8_t> bytes,
    X64CpuState& state,
    memory::IGuestVirtualMemory& memory) noexcept
{
    const auto decoded = decode_x64_instruction(bytes);
    return execute_instruction_via(decoded, state, memory);
}

[[nodiscard]] X64ExecutionResult step_x64_at_rip_via(
    X64CpuState& state,
    memory::IGuestVirtualMemory& memory) noexcept
{
    std::array<std::uint8_t, kMaxFetchLength> buffer{};
    const memory::GuestVirtualAddress rip_address{state.rip};
    std::optional<memory::MemoryResult> last_retry_failure{};

    for (auto attempted = kMaxFetchLength; attempted > 0U; --attempted) {
        buffer.fill(std::uint8_t{0U});
        const auto result =
            memory.fetch(rip_address, std::span<std::uint8_t>(buffer.data(), attempted));

        if (!result.ok) {
            last_retry_failure = result;
            if (result.fault == memory::MemoryFault::Unmapped ||
                result.fault == memory::MemoryFault::InvalidRange) {
                continue;
            }
            return memory_error(result);
        }
        if (result.completed_bytes != static_cast<std::uint64_t>(attempted)) {
            return memory_error(result);
        }

        const auto decoded =
            decode_x64_instruction(std::span<const std::uint8_t>(buffer.data(), attempted));
        return execute_instruction_via(decoded, state, memory);
    }

    if (last_retry_failure.has_value()) {
        return memory_error(*last_retry_failure);
    }

    X64ExecutionResult outcome{};
    outcome.status = X64ExecuteStatus::MemoryAccessError;
    return outcome;
}

[[nodiscard]] X64ExecuteStatus execute_instruction(
    const DecodedInstruction& instruction,
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept
{
    memory::GuestMemoryView view(memory);
    return execute_instruction_via(instruction, state, view).status;
}

[[nodiscard]] X64ExecuteStatus step_x64_instruction(
    std::span<const std::uint8_t> bytes,
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept
{
    memory::GuestMemoryView view(memory);
    return step_x64_instruction_via(bytes, state, view).status;
}

[[nodiscard]] X64ExecuteStatus step_x64_at_rip(
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept
{
    memory::GuestMemoryView view(memory);
    return step_x64_at_rip_via(state, view).status;
}

} // namespace xbridge::cpu
