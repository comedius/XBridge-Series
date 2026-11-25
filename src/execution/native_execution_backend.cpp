#include "xbridge/execution/native_execution_backend.hpp"

#include "xbridge/memory/guest_address.hpp"

#include <array>

namespace xbridge::execution {
namespace {

constexpr std::uint64_t kU64Width = 8U;

void store_le_u64(std::array<std::uint8_t, 8>& bytes, std::uint64_t value) noexcept {
    for (std::size_t index = 0U; index < bytes.size(); ++index) {
        bytes[index] = static_cast<std::uint8_t>((value >> (index * 8U)) & 0xFFULL);
    }
}

[[nodiscard]] std::uint64_t load_le_u64(const std::array<std::uint8_t, 8>& bytes) noexcept {
    std::uint64_t value = 0U;
    for (std::size_t index = 0U; index < bytes.size(); ++index) {
        value |= static_cast<std::uint64_t>(bytes[index]) << (index * 8U);
    }
    return value;
}

[[nodiscard]] bool resolve_stack_address(memory::GuestVirtualAddress base,
                                         std::uint64_t offset,
                                         memory::GuestVirtualAddress& address) noexcept {
    std::uint64_t begin = 0U;
    std::uint64_t end = 0U;
    if (!memory::checked_range_end(base.value, offset, begin) ||
        !memory::checked_range_end(begin, kU64Width, end)) {
        return false;
    }
    address = memory::GuestVirtualAddress{begin};
    return true;
}

NativeExecutionError run_nop(GuestExecutionContext&) noexcept {
    return NativeExecutionError::None;
}

NativeExecutionError run_mov_imm32(GuestExecutionContext& context) noexcept {
    auto& registers = context.registers();
    registers.gprs[0] = static_cast<std::uint32_t>(kMovImm32Immediate);
    return NativeExecutionError::None;
}

NativeExecutionError run_mov_reg_to_reg(GuestExecutionContext& context) noexcept {
    auto& registers = context.registers();
    registers.gprs[0] = registers.gprs[1];
    return NativeExecutionError::None;
}

NativeExecutionError run_add_imm32(GuestExecutionContext& context) noexcept {
    auto& registers = context.registers();
    const auto sum = static_cast<std::uint32_t>(registers.gprs[0]) + kAddImm32Immediate;
    registers.gprs[0] = static_cast<std::uint64_t>(sum);
    return NativeExecutionError::None;
}

NativeExecutionError run_load_u64(GuestExecutionContext& context) noexcept {
    const auto& registers = context.registers();
    std::array<std::uint8_t, 8> bytes{};
    const memory::GuestVirtualAddress address{registers.gprs[1]};
    if (!context.memory().read(address, bytes)) {
        return NativeExecutionError::MemoryAccessError;
    }
    // Commit the destination only after a complete successful read.
    const std::uint64_t value = load_le_u64(bytes);
    context.registers().gprs[0] = value;
    return NativeExecutionError::None;
}

NativeExecutionError run_store_u64(GuestExecutionContext& context) noexcept {
    const auto& registers = context.registers();
    std::array<std::uint8_t, 8> bytes{};
    store_le_u64(bytes, registers.gprs[0]);
    const memory::GuestVirtualAddress address{registers.gprs[1]};
    if (!context.memory().write(address, bytes)) {
        return NativeExecutionError::MemoryAccessError;
    }
    return NativeExecutionError::None;
}

NativeExecutionError run_request_service(GuestExecutionContext& context) noexcept {
    const auto& registers = context.registers();
    context.set_transition(GuestHostTransition{
        GuestHostTransitionKind::ServiceRequest, registers.gprs[2], registers.gprs[3]});
    return NativeExecutionError::None;
}

// The four gate routines are gating metadata for this synthetic proof only.
// They use plain C++ arithmetic and never emit AVX2, BMI or FMA instructions.
NativeExecutionError run_avx2_gate(GuestExecutionContext& context) noexcept {
    auto& registers = context.registers();
    registers.gprs[0] ^= registers.gprs[1];
    return NativeExecutionError::None;
}

NativeExecutionError run_bmi1_gate(GuestExecutionContext& context) noexcept {
    auto& registers = context.registers();
    registers.gprs[0] &= ~registers.gprs[1];
    return NativeExecutionError::None;
}

NativeExecutionError run_bmi2_gate(GuestExecutionContext& context) noexcept {
    auto& registers = context.registers();
    registers.gprs[0] |= registers.gprs[1];
    return NativeExecutionError::None;
}

NativeExecutionError run_fma_gate(GuestExecutionContext& context) noexcept {
    auto& registers = context.registers();
    registers.gprs[0] += registers.gprs[1];
    return NativeExecutionError::None;
}

[[nodiscard]] bool is_known_routine(NativeRoutineId routine) noexcept {
    switch (routine) {
    case NativeRoutineId::Nop:
    case NativeRoutineId::MovImm32:
    case NativeRoutineId::MovRegToReg:
    case NativeRoutineId::AddImm32:
    case NativeRoutineId::LoadU64ViaMemory:
    case NativeRoutineId::StoreU64ViaMemory:
    case NativeRoutineId::RequestService:
    case NativeRoutineId::Avx2Gate:
    case NativeRoutineId::Bmi1Gate:
    case NativeRoutineId::Bmi2Gate:
    case NativeRoutineId::FmaGate:
        return true;
    }
    return false;
}

} // namespace

bool GuestExecutionContext::push_u64(std::uint64_t value) noexcept {
    if (stack_.pointer_offset > stack_.size || stack_.size - stack_.pointer_offset < kU64Width) {
        return false;
    }
    memory::GuestVirtualAddress address{};
    if (!resolve_stack_address(stack_.base, stack_.pointer_offset, address)) {
        return false;
    }
    std::array<std::uint8_t, 8> bytes{};
    store_le_u64(bytes, value);
    if (!memory_.write(address, bytes)) {
        return false;
    }
    stack_.pointer_offset += kU64Width;
    return true;
}

bool GuestExecutionContext::pop_u64(std::uint64_t& value) noexcept {
    if (stack_.pointer_offset < kU64Width || stack_.pointer_offset > stack_.size) {
        return false;
    }
    const std::uint64_t read_offset = stack_.pointer_offset - kU64Width;
    memory::GuestVirtualAddress address{};
    if (!resolve_stack_address(stack_.base, read_offset, address)) {
        return false;
    }
    std::array<std::uint8_t, 8> bytes{};
    if (!memory_.read(address, bytes)) {
        return false;
    }
    value = load_le_u64(bytes);
    stack_.pointer_offset = read_offset;
    return true;
}

NativeExecutionBackend::NativeExecutionBackend(const ICpuFeatureDetector& detector) noexcept
    : features_(detector.detect()) {}

bool NativeExecutionBackend::can_execute(NativeRoutineId routine) const noexcept {
    switch (routine) {
    case NativeRoutineId::Nop:
    case NativeRoutineId::MovImm32:
    case NativeRoutineId::MovRegToReg:
    case NativeRoutineId::AddImm32:
    case NativeRoutineId::LoadU64ViaMemory:
    case NativeRoutineId::StoreU64ViaMemory:
    case NativeRoutineId::RequestService:
        return features_.baseline_x64;
    case NativeRoutineId::Avx2Gate:
        return features_.has(CpuFeature::Avx2);
    case NativeRoutineId::Bmi1Gate:
        return features_.bmi1;
    case NativeRoutineId::Bmi2Gate:
        return features_.bmi2;
    case NativeRoutineId::FmaGate:
        return features_.fma;
    }
    return false;
}

NativeExecutionResult NativeExecutionBackend::execute(NativeRoutineId routine,
                                                      GuestExecutionContext& context) noexcept {
    NativeExecutionResult result{};
    result.registers = context.registers();
    result.transition = context.last_transition();

    // The feature/existence gate runs before any routine mutation. There is no
    // mid-routine fallback: a rejected routine is never partly executed.
    if (!can_execute(routine)) {
        result.error = is_known_routine(routine) ? NativeExecutionError::UnsupportedFeature
                                                 : NativeExecutionError::UnknownRoutine;
        return result;
    }

    NativeExecutionError error = NativeExecutionError::UnknownRoutine;
    switch (routine) {
    case NativeRoutineId::Nop: error = run_nop(context); break;
    case NativeRoutineId::MovImm32: error = run_mov_imm32(context); break;
    case NativeRoutineId::MovRegToReg: error = run_mov_reg_to_reg(context); break;
    case NativeRoutineId::AddImm32: error = run_add_imm32(context); break;
    case NativeRoutineId::LoadU64ViaMemory: error = run_load_u64(context); break;
    case NativeRoutineId::StoreU64ViaMemory: error = run_store_u64(context); break;
    case NativeRoutineId::RequestService: error = run_request_service(context); break;
    case NativeRoutineId::Avx2Gate: error = run_avx2_gate(context); break;
    case NativeRoutineId::Bmi1Gate: error = run_bmi1_gate(context); break;
    case NativeRoutineId::Bmi2Gate: error = run_bmi2_gate(context); break;
    case NativeRoutineId::FmaGate: error = run_fma_gate(context); break;
    default: error = NativeExecutionError::UnknownRoutine; break;
    }

    result.error = error;
    result.registers = context.registers();
    result.transition = context.last_transition();
    if (error == NativeExecutionError::None) {
        result.native_calls = 1U;
    }
    return result;
}

ExecutionBackendSelector::ExecutionBackendSelector(const ICpuFeatureDetector& detector) noexcept
    : backend_(detector) {}

ExecutionBackendKind ExecutionBackendSelector::select(
    NativeRoutineId routine, ExecutionBackendPreference preference) const noexcept {
    switch (preference) {
    case ExecutionBackendPreference::Interpreter:
        return ExecutionBackendKind::Interpreter;
    case ExecutionBackendPreference::Native:
        return ExecutionBackendKind::Native;
    case ExecutionBackendPreference::Automatic:
        return backend_.can_execute(routine) ? ExecutionBackendKind::Native
                                             : ExecutionBackendKind::Interpreter;
    }
    return ExecutionBackendKind::Interpreter;
}

const char* to_string(NativeRoutineId routine) noexcept {
    switch (routine) {
    case NativeRoutineId::Nop: return "Nop";
    case NativeRoutineId::MovImm32: return "MovImm32";
    case NativeRoutineId::MovRegToReg: return "MovRegToReg";
    case NativeRoutineId::AddImm32: return "AddImm32";
    case NativeRoutineId::LoadU64ViaMemory: return "LoadU64ViaMemory";
    case NativeRoutineId::StoreU64ViaMemory: return "StoreU64ViaMemory";
    case NativeRoutineId::RequestService: return "RequestService";
    case NativeRoutineId::Avx2Gate: return "Avx2Gate";
    case NativeRoutineId::Bmi1Gate: return "Bmi1Gate";
    case NativeRoutineId::Bmi2Gate: return "Bmi2Gate";
    case NativeRoutineId::FmaGate: return "FmaGate";
    }
    return "Unknown";
}

const char* to_string(NativeExecutionError error) noexcept {
    switch (error) {
    case NativeExecutionError::None: return "None";
    case NativeExecutionError::UnsupportedFeature: return "UnsupportedFeature";
    case NativeExecutionError::UnknownRoutine: return "UnknownRoutine";
    case NativeExecutionError::InvalidContext: return "InvalidContext";
    case NativeExecutionError::MemoryAccessError: return "MemoryAccessError";
    case NativeExecutionError::TransitionError: return "TransitionError";
    }
    return "Unknown";
}

const char* to_string(ExecutionBackendPreference preference) noexcept {
    switch (preference) {
    case ExecutionBackendPreference::Interpreter: return "Interpreter";
    case ExecutionBackendPreference::Native: return "Native";
    case ExecutionBackendPreference::Automatic: return "Automatic";
    }
    return "Unknown";
}

const char* to_string(ExecutionBackendKind kind) noexcept {
    switch (kind) {
    case ExecutionBackendKind::Interpreter: return "Interpreter";
    case ExecutionBackendKind::Native: return "Native";
    }
    return "Unknown";
}

const char* to_string(GuestHostTransitionKind kind) noexcept {
    switch (kind) {
    case GuestHostTransitionKind::ServiceRequest: return "ServiceRequest";
    case GuestHostTransitionKind::NormalExit: return "NormalExit";
    case GuestHostTransitionKind::FaultExit: return "FaultExit";
    }
    return "Unknown";
}

} // namespace xbridge::execution
