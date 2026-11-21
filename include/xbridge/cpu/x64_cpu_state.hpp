#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace xbridge::cpu {

// These values identify storage slots for the 64-bit general-purpose registers.
enum class X64GeneralRegister : std::size_t {
    Rax,
    Rcx,
    Rdx,
    Rbx,
    Rsp,
    Rbp,
    Rsi,
    Rdi,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15
};

// This is only a zero-initialized storage container for general x86-64/AMD64 state.
// It defines no instruction semantics, register-alias rules, CPU reset semantics,
// SIMD/FPU state, or Xbox-specific properties.
struct X64CpuState {
    std::array<std::uint64_t, 16> general_purpose_registers{};
    std::uint64_t rip{};
    std::uint64_t rflags{};

    [[nodiscard]] std::uint64_t& gpr(X64GeneralRegister reg) {
        return general_purpose_registers.at(static_cast<std::size_t>(reg));
    }

    [[nodiscard]] const std::uint64_t& gpr(X64GeneralRegister reg) const {
        return general_purpose_registers.at(static_cast<std::size_t>(reg));
    }
};

} // namespace xbridge::cpu
