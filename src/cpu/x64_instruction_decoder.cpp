#include "xbridge/cpu/x64_instruction_decoder.hpp"

#include <cstdint>
#include <span>

namespace xbridge::cpu {

namespace {

constexpr std::uint8_t kNopOpcode = 0x90;
constexpr std::uint8_t kMovRm8R8 = 0x88;
constexpr std::uint8_t kMovRm32R32 = 0x89;
constexpr std::uint8_t kMovR8Rm8 = 0x8A;
constexpr std::uint8_t kMovR32Rm32 = 0x8B;
constexpr std::uint8_t kMovR32Imm32Base = 0xB8;

constexpr std::uint8_t kRexPrefixBase = 0x40;

[[nodiscard]] bool is_rex_prefix(std::uint8_t byte) noexcept {
    return (byte & 0xF0) == kRexPrefixBase;
}

[[nodiscard]] X64RexPrefix parse_rex_prefix(std::uint8_t byte) noexcept {
    X64RexPrefix rex{};
    rex.present = true;
    rex.w = (byte & 0x08) != 0;
    rex.r = (byte & 0x04) != 0;
    rex.x = (byte & 0x02) != 0;
    rex.b = (byte & 0x01) != 0;
    return rex;
}

[[nodiscard]] X64GeneralRegister register_from_index(std::uint8_t index) noexcept {
    return static_cast<X64GeneralRegister>(index & 0x0F);
}

[[nodiscard]] X64RegisterRef make_register_ref(X64GeneralRegister base, bool extended) noexcept {
    X64RegisterRef ref{};
    ref.base = base;
    ref.extended = extended;
    return ref;
}

void parse_modrm(std::uint8_t byte, X64ModRM& modrm) noexcept {
    modrm.present = true;
    modrm.mod = (byte >> 6) & 0x03;
    modrm.reg = (byte >> 3) & 0x07;
    modrm.rm = byte & 0x07;
}
// Einzige unterstützte Instruktion im Sprint-8-Umfang.
constexpr std::uint8_t kNopOpcode = 0x90;

} // namespace

[[nodiscard]] DecodedInstruction decode_x64_instruction(
    std::span<const std::uint8_t> bytes) noexcept
{
    DecodedInstruction result{};

    if (bytes.empty()) {
        result.status = X64DecodeStatus::EmptyInput;
        return result;
    }

    std::size_t offset = 0;

    // --- REX-Präfixe parsen (können mehrfach vorkommen, nur letztes zählt) ---
    X64RexPrefix rex{};
    while (offset < bytes.size() && is_rex_prefix(bytes[offset])) {
        rex = parse_rex_prefix(bytes[offset]);
        ++offset;
    }

    // --- Primärer Opcode ---
    if (offset >= bytes.size()) {
        result.status = X64DecodeStatus::TruncatedInput;
        return result;
    }

    const std::uint8_t opcode = bytes[offset];
    result.opcode = opcode;
    result.rex = rex;

    // --- Operandengröße bestimmen ---
    result.operand_size = rex.w ? X64OperandSize::Bits64 : X64OperandSize::Bits32;

    // --- NOP ---
    if (opcode == kNopOpcode) {
        result.status = X64DecodeStatus::Ok;
        result.length = offset + 1;
        return result;
    }

    // --- MOV r64/r32, imm64/imm32 (0xB8 + reg) ---
    if (opcode >= kMovR32Imm32Base && opcode <= (kMovR32Imm32Base | 0x07)) {
        const std::uint8_t reg_low = opcode & 0x07;
        const auto reg_base = register_from_index(reg_low | (rex.b ? 8 : 0));
        result.dest_reg = make_register_ref(reg_base, rex.b);

        const std::size_t imm_size = (result.operand_size == X64OperandSize::Bits64) ? 8 : 4;
        if (offset + 1 + imm_size > bytes.size()) {
            result.status = X64DecodeStatus::TruncatedInput;
            return result;
        }

        std::uint64_t imm = 0;
        for (std::size_t i = 0; i < imm_size; ++i) {
            imm |= static_cast<std::uint64_t>(bytes[offset + 1 + i]) << (8 * i);
        }
        result.immediate = imm;
        result.immediate_size = imm_size;
        result.status = X64DecodeStatus::Ok;
        result.length = offset + 1 + imm_size;
        return result;
    }

    // --- MOV mit ModR/M (0x88, 0x89, 0x8A, 0x8B) ---
    if (opcode == kMovRm8R8 || opcode == kMovRm32R32 ||
        opcode == kMovR8Rm8 || opcode == kMovR32Rm32) {
        if (offset + 1 >= bytes.size()) {
            result.status = X64DecodeStatus::TruncatedInput;
            return result;
        }

        X64ModRM modrm{};
        parse_modrm(bytes[offset + 1], modrm);

        // REX-Erweiterte Register-Indizes
        const auto reg_base = register_from_index(modrm.reg | (rex.r ? 8 : 0));
        const auto rm_base = register_from_index(modrm.rm | (rex.b ? 8 : 0));

        // 8-bit Varianten werden nicht unterstützt (keine REX, kein Operand-Size-Override)
        if (opcode == kMovRm8R8 || opcode == kMovR8Rm8) {
            result.status = X64DecodeStatus::UnknownOpcode;
            return result;
        }

        // Register-zu-Register (mod == 3 / 0b11)
        if (modrm.mod == 0b11) {
            if (opcode == kMovRm32R32) {
                // MOV r/m, r: dest = rm, src = reg
                result.dest_reg = make_register_ref(rm_base, rex.b);
                result.src_reg = make_register_ref(reg_base, rex.r);
            } else {
                // MOV r, r/m: dest = reg, src = rm
                result.dest_reg = make_register_ref(reg_base, rex.r);
                result.src_reg = make_register_ref(rm_base, rex.b);
            }
            result.memory_operand = false;
            result.status = X64DecodeStatus::Ok;
            result.length = offset + 2;
            return result;
        }

        // Speicheroperand (mod != 3)
        // Unterstützte einfache Adressierung ohne SIB und ohne Displacement:
        // mod == 0b00 mit rm in {000-011, 110-111} (einfache Register-Indirektadressierung)
        // mod == 0b00 mit rm == 100 benötigt ein SIB-Byte → nicht unterstützt
        // mod == 0b00 mit rm == 101 benötigt RIP-relative Displacement → nicht unterstützt
        if (modrm.mod == 0b00) {
            if (modrm.rm == 0b100) {
                // SIB-Byte erforderlich
                result.status = X64DecodeStatus::InvalidEncoding;
                return result;
            }
            if (modrm.rm == 0b101) {
                // RIP-relative Adressierung mit Displacement erforderlich
                result.status = X64DecodeStatus::InvalidEncoding;
                return result;
            }
            // rm in {000-011, 110-111}: einfache Register-Indirektadressierung
            result.memory_operand = true;
            result.displacement = 0;
            if (opcode == kMovRm32R32) {
                // MOV r/m, r: dest = memory (rm-Register als Adresse), src = reg
                result.dest_reg = make_register_ref(rm_base, rex.b);
                result.src_reg = make_register_ref(reg_base, rex.r);
            } else {
                // MOV r, r/m: dest = reg, src = memory (rm-Register als Adresse)
                result.dest_reg = make_register_ref(reg_base, rex.r);
                result.src_reg = make_register_ref(rm_base, rex.b);
            }
            result.status = X64DecodeStatus::Ok;
            result.length = offset + 2;
            return result;
        }

        // Alle anderen ModR/M-Kombinationen (mod != 0 mit Displacement, SIB, etc.) nicht unterstützt
        result.status = X64DecodeStatus::InvalidEncoding;
        return result;
    }

    // --- Unbekannter Opcode ---
    result.status = X64DecodeStatus::UnknownOpcode;
    result.length = offset + 1;
    return result;
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
