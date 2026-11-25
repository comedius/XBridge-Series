#pragma once

#include "xbridge/execution/cpu_feature_detector.hpp"
#include "xbridge/execution/guest_execution_context.hpp"

#include <cstdint>

namespace xbridge::execution {

// Only compiled-in, trusted, synthetic routine IDs are accepted. There is no
// public path for caller-supplied function pointers, byte buffers or RVAs.
enum class NativeRoutineId {
    Nop,
    MovImm32,
    MovRegToReg,
    AddImm32,
    LoadU64ViaMemory,
    StoreU64ViaMemory,
    RequestService,
    Avx2Gate,
    Bmi1Gate,
    Bmi2Gate,
    FmaGate,

    // Spelling aliases used by callers that describe the same fixed routines.
    LoadViaMemory = LoadU64ViaMemory,
    StoreViaMemory = StoreU64ViaMemory,
    ServiceTransition = RequestService,
    Avx2Probe = Avx2Gate,
    Bmi1Probe = Bmi1Gate,
    Bmi2Probe = Bmi2Gate,
    FmaProbe = FmaGate
};

// InvalidContext and TransitionError are reserved diagnostic codes for state
// validation in a future seam; the synthetic routines below do not emit them.
enum class NativeExecutionError {
    None,
    UnsupportedFeature,
    UnknownRoutine,
    InvalidContext,
    MemoryAccessError,
    TransitionError
};

struct NativeExecutionResult {
    NativeExecutionError error{NativeExecutionError::None};
    GuestRegisterState registers{};
    GuestHostTransition transition{};
    std::uint64_t native_calls{0};
};

// Immediate encoded in MovImm32, matching the interpreter's MOV EAX imm32
// example. It zero-extends into gpr[0].
inline constexpr std::uint32_t kMovImm32Immediate = 0x12345678U;

// Immediate encoded in AddImm32. 32-bit unsigned addition wraps and the result
// zero-extends into gpr[0].
inline constexpr std::uint32_t kAddImm32Immediate = 0x0000000AU;

class NativeExecutionBackend {
public:
    explicit NativeExecutionBackend(const ICpuFeatureDetector& detector) noexcept;

    [[nodiscard]] bool can_execute(NativeRoutineId routine) const noexcept;
    [[nodiscard]] NativeExecutionResult execute(NativeRoutineId routine,
                                                GuestExecutionContext& context) noexcept;

private:
    // Snapshot taken at construction; later detector changes are invisible.
    CpuFeatureSet features_{};
};

enum class ExecutionBackendPreference { Interpreter, Native, Automatic };
enum class ExecutionBackendKind { Interpreter, Native };

using BackendPreference = ExecutionBackendPreference;
using BackendKind = ExecutionBackendKind;

// Fallback is decided before a routine starts. No operation switches paths in
// the middle of an execution.
class ExecutionBackendSelector {
public:
    explicit ExecutionBackendSelector(const ICpuFeatureDetector& detector) noexcept;

    [[nodiscard]] ExecutionBackendKind select(NativeRoutineId routine,
                                              ExecutionBackendPreference preference) const noexcept;

private:
    NativeExecutionBackend backend_;
};

[[nodiscard]] const char* to_string(NativeRoutineId routine) noexcept;
[[nodiscard]] const char* to_string(NativeExecutionError error) noexcept;
[[nodiscard]] const char* to_string(ExecutionBackendPreference preference) noexcept;
[[nodiscard]] const char* to_string(ExecutionBackendKind kind) noexcept;

} // namespace xbridge::execution
