#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace xbridge::cpu {

// Status des Dekodierergebnisses.
// Ok:        ein unterstützter Opcode wurde erfolgreich dekodiert.
// EmptyInput: die Eingabe enthielt keine Bytes.
// UnknownOpcode: der erste Byte ist kein bekannter (unterstützter) Opcode.
enum class X64DecodeStatus : std::uint8_t {
    Ok,
    EmptyInput,
    UnknownOpcode
};

// Strukturierte Darstellung einer dekodierten x64-Instruktion.
// Der Decoder ist state-los und verändert keinen CPU-Zustand.
// Im Sprint-8-Umfang wird ausschließlich NOP (0x90) unterstützt.
struct DecodedInstruction {
    X64DecodeStatus status;
    std::uint8_t opcode;     // roher Opcode-Byte (nur bei Ok / UnknownOpcode aussagekräftig)
    std::size_t length;      // Instruktionslänge in Bytes (nur bei Ok aussagekräftig)
};

// Dekodiert die erste x64-Instruktion aus dem gegebenen Byte-Span.
// Unterstützt aktuell nur NOP (0x90).
// Bei leerer Eingabe wird Status::EmptyInput zurückgegeben.
// Bei unbekanntem Opcode wird Status::UnknownOpcode zurückgegeben.
// Es werden keine Exceptions für unbekannte Opcodes geworfen.
[[nodiscard]] DecodedInstruction decode_x64_instruction(
    std::span<const std::uint8_t> bytes) noexcept;

} // namespace xbridge::cpu
