#pragma once

#include "xbridge/cpu/x64_cpu_state.hpp"
#include "xbridge/cpu/x64_instruction_decoder.hpp"
#include "xbridge/memory/guest_virtual_memory.hpp"
#include "xbridge/memory/memory.hpp"

#include <optional>
#include <span>

namespace xbridge::cpu {

// Status der Instruktionsausführung.
enum class X64ExecuteStatus {
    Ok,
    MemoryAccessError
};

// Ergebnis der Ausführung über IGuestVirtualMemory. Der Status bleibt mit dem
// bestehenden X64ExecuteStatus vergleichbar; Diagnostik wird nur bei
// Speicherfehlern mitgeliefert.
struct X64ExecutionResult {
    X64ExecuteStatus status{X64ExecuteStatus::Ok};
    std::optional<memory::MemoryResult> memory_result{};

    friend constexpr bool operator==(const X64ExecutionResult& result, X64ExecuteStatus status) noexcept {
        return result.status == status;
    }
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

// Führt genau eine dokumentierte x64-Instruktion aus Roh-Bytes aus.
// Komposition aus decode_x64_instruction(bytes) und execute_instruction(...);
// Decoder und Interpreter bleiben sauber getrennt. Kein RIP-basiertes Fetch.
[[nodiscard]] X64ExecuteStatus step_x64_instruction(
    std::span<const std::uint8_t> bytes,
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept;

// Holt höchstens 15 Bytes ab state.rip aus GuestMemory und übergibt genau diese
// Bytes an step_x64_instruction. Das Fenster ist nur ein lokales Lese-Limit,
// keine RIP-relative Guest-Adressierung; der Status wird unverändert zurückgegeben.
[[nodiscard]] X64ExecuteStatus step_x64_at_rip(
    X64CpuState& state,
    memory::GuestMemory& memory) noexcept;

// Additive Ausführungspfade über die virtuelle Speicher-Schnittstelle.
// Die bestehenden GuestMemory-Funktionen bleiben unverändert erhalten.
[[nodiscard]] X64ExecutionResult execute_instruction_via(
    const DecodedInstruction& instruction,
    X64CpuState& state,
    memory::IGuestVirtualMemory& memory) noexcept;

[[nodiscard]] X64ExecutionResult step_x64_instruction_via(
    std::span<const std::uint8_t> bytes,
    X64CpuState& state,
    memory::IGuestVirtualMemory& memory) noexcept;

[[nodiscard]] X64ExecutionResult step_x64_at_rip_via(
    X64CpuState& state,
    memory::IGuestVirtualMemory& memory) noexcept;

} // namespace xbridge::cpu
