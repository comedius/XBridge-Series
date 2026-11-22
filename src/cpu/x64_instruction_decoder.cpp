#include "xbridge/cpu/x64_instruction_decoder.hpp"

namespace xbridge::cpu {

namespace {

// Einzige unterstützte Instruktion im Sprint-8-Umfang.
constexpr std::uint8_t kNopOpcode = 0x90;

} // namespace

[[nodiscard]] DecodedInstruction decode_x64_instruction(
    std::span<const std::uint8_t> bytes) noexcept
{
    if (bytes.empty()) {
        return DecodedInstruction{
            .status = X64DecodeStatus::EmptyInput,
            .opcode = 0x00,
            .length = 0
        };
    }

    const std::uint8_t opcode = bytes[0];

    if (opcode == kNopOpcode) {
        return DecodedInstruction{
            .status = X64DecodeStatus::Ok,
            .opcode = opcode,
            .length = 1
        };
    }

    return DecodedInstruction{
        .status = X64DecodeStatus::UnknownOpcode,
        .opcode = opcode,
        .length = 0
    };
}

} // namespace xbridge::cpu
