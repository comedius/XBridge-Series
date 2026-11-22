#pragma once

#include "xbridge/cpu/x64_cpu_state.hpp"

#include <cstddef>
#include <cstdint>
#include <span>

namespace xbridge::cpu {

// Status des Dekodierergebnisses.
// Ok:             erfolgreich dekodiert.
// EmptyInput:     keine Bytes vorhanden.
// UnknownOpcode:  Opcode nicht unterstützt.
// InvalidEncoding:// ungültige Präfix- oder Encodings-Kombination.
// TruncatedInput: Eingabe zu kurz für vollständige Instruktion.
enum class X64DecodeStatus : std::uint8_t {
    Ok,
    EmptyInput,
    UnknownOpcode,
    InvalidEncoding,
    TruncatedInput
};

// REX-Präfix-Felder (Byte 0x40-0x4F, Muster 0100WRXB).
struct X64RexPrefix {
    bool present = false;
    bool w = false; // 64-bit operand size
    bool r = false; // Extended ModR/M reg field
    bool x = false; // Extended SIB index field (unused in this decoder)
    bool b = false; // Extended ModR/M r/m or opcode register
};

// Operandengröße der dekodierten Instruktion.
enum class X64OperandSize : std::uint8_t {
    Bits8,
    Bits16,
    Bits32,
    Bits64
};

// Register-Referenz, zusammengesetzt aus Basis-Register und REX-Erweiterung.
struct X64RegisterRef {
    X64GeneralRegister base = X64GeneralRegister::Rax;
    bool extended = false; // true wenn REX.B bzw. REX.R gesetzt ist
};

// ModR/M Byte Analyse.
struct X64ModRM {
    bool present = false;
    std::uint8_t mod = 0; // 0-3 (Adressierungsmodus)
    std::uint8_t reg = 0; // 0-7 (Register oder Opcode-Erweiterung)
    std::uint8_t rm = 0;  // 0-7 (Operand oder Speicheradresse)
};

// Strukturierte Darstellung einer dekodierten x64-Instruktion.
// Der Decoder ist state-los und verändert keinen CPU-Zustand.
struct DecodedInstruction {
    X64DecodeStatus status = X64DecodeStatus::Ok;
    std::uint8_t opcode = 0x00;           // roher primärer Opcode-Byte
    std::size_t length = 0;               // Gesamtlänge in Bytes
    X64OperandSize operand_size = X64OperandSize::Bits32;
    X64RexPrefix rex;
    X64RegisterRef dest_reg;              // Ziel-Register (für MOV)
    X64RegisterRef src_reg;               // Quell-Register (für MOV)
    bool memory_operand = false;          // true wenn Operand im Speicher liegt
    std::int32_t displacement = 0;        // Speicher-Displacement
    std::uint64_t immediate = 0;          // Immediate-Wert
    std::size_t immediate_size = 0;       // Immediate-Größe in Bytes (0, 1, 2, 4, 8)
};

// Dekodiert die erste x64-Instruktion aus dem gegebenen Byte-Span.
// Unterstützte Instruktionen: NOP (0x90), MOV (Register/Immediate, Register/Register,
// Register<->Speicher) mit REX-Präfixen.
// Bei Fehlern wird ein definierter Status zurückgegeben, keine Exceptions.
[[nodiscard]] DecodedInstruction decode_x64_instruction(
    std::span<const std::uint8_t> bytes) noexcept;

} // namespace xbridge::cpu
