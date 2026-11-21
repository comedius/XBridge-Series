#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

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

enum class X64GprWidth : std::uint8_t {
    Bits8,
    Bits16,
    Bits32,
    Bits64,
    High8
};

// This stores general x86-64/AMD64 state and provides explicit GPR slice access.
// It defines no instruction semantics, decoder rules, CPU reset semantics,
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

    [[nodiscard]] std::uint64_t read_gpr(X64GeneralRegister reg, X64GprWidth width) const {
        const auto value = gpr(reg);
        switch (width) {
        case X64GprWidth::Bits64:
            return value;
        case X64GprWidth::Bits32:
            return value & 0xFFFFFFFFULL;
        case X64GprWidth::Bits16:
            return value & 0xFFFFULL;
        case X64GprWidth::Bits8:
            return value & 0xFFULL;
        case X64GprWidth::High8:
            if (!has_high_byte_register(reg)) {
                throw std::invalid_argument("High-byte access is only valid for RAX, RBX, RCX, or RDX");
            }
            return (value >> 8) & 0xFFULL;
        }
        throw std::invalid_argument("Invalid x64 GPR width");
    }

    void write_gpr(X64GeneralRegister reg, X64GprWidth width, std::uint64_t value) {
        auto& register_value = gpr(reg);
        switch (width) {
        case X64GprWidth::Bits64:
            register_value = value;
            return;
        case X64GprWidth::Bits32:
            register_value = static_cast<std::uint32_t>(value);
            return;
        case X64GprWidth::Bits16:
            register_value = (register_value & ~0xFFFFULL) | (value & 0xFFFFULL);
            return;
        case X64GprWidth::Bits8:
            register_value = (register_value & ~0xFFULL) | (value & 0xFFULL);
            return;
        case X64GprWidth::High8:
            if (!has_high_byte_register(reg)) {
                throw std::invalid_argument("High-byte access is only valid for RAX, RBX, RCX, or RDX");
            }
            register_value = (register_value & ~0xFF00ULL) | ((value & 0xFFULL) << 8);
            return;
        }
        throw std::invalid_argument("Invalid x64 GPR width");
    }

private:
    static bool has_high_byte_register(X64GeneralRegister reg) noexcept {
        return reg == X64GeneralRegister::Rax ||
               reg == X64GeneralRegister::Rbx ||
               reg == X64GeneralRegister::Rcx ||
               reg == X64GeneralRegister::Rdx;
    }
};

} // namespace xbridge::cpu
