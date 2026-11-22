#pragma once

#include "xbridge/cpu/x64_cpu_state.hpp"
#include "xbridge/cpu/x64_instruction_decoder.hpp"
#include "xbridge/memory/memory.hpp"

namespace xbridge::cpu {

// Status der Instruktionsausführung.
enum class X64ExecuteStatus {
    Ok,
    MemoryAccessError
};

// Führt eine dekodierte x64-Instruktion aus.
// Der Decoder und der Interpreter sind sauber getrennt:
//   - Der Decoder produziert eine DecodedInstruction aus Roh-Bytes.
//   - Der Interpreter wendet die Instruktion auf X64CpuState und GuestMemory an.
// RIP wird nach erfolgreicher Ausführung um die Instruktionslänge vorangetrieben.
// Bei Speicherzugriffsfehlern wird kein CPU-Zustand oder Speicher verändert.
[[nodiscard]] X64ExecuteStatus execute_instruction(
    const DecodedInstruction& instruction,
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept;

} // namespace xbridge::cpu
