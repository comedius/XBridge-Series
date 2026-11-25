#include "xbridge/core/config.hpp"
#include "xbridge/core/runtime.hpp"
#include "xbridge/cpu/test_cpu.hpp"
#include "xbridge/cpu/x64_cpu_state.hpp"
#include "xbridge/cpu/x64_instruction_decoder.hpp"
#include "xbridge/cpu/x64_instruction_interpreter.hpp"
#include "xbridge/memory/memory.hpp"
#include "xbridge/memory/guest_address.hpp"
#include "xbridge/memory/guest_address_space.hpp"
#include "xbridge/memory/guest_virtual_memory.hpp"
#include "xbridge/memory/guest_memory_result.hpp"
#include "xbridge/memory/guest_physical_memory.hpp"
#include "xbridge/memory/guest_region.hpp"
#include "xbridge/host/execution_policy.hpp"
#include "xbridge/analysis/game_intelligence.hpp"
#include "xbridge/execution/native_execution_backend.hpp"
#include "xbridge/loader/executable_loader.hpp"
#include "xbridge/runtime/system_services.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <vector>

namespace {
using namespace xbridge;

int failures = 0;

void check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << std::endl;
        ++failures;
    } else {
        std::cout << "[PASS] " << message << std::endl;
    }
}

template <typename ExpectedException, typename F>
void check_throws(F&& fn, const char* message) {
    try { fn(); }
    catch (const ExpectedException&) { check(true, message); return; }
    catch (const std::exception&) { check(false, message); return; }
    check(false, message);
}

template <typename F>
void check_does_not_throw(F&& fn, const char* message) {
    try { fn(); check(true, message); }
    catch (const std::exception&) { check(false, message); }
}

bool registers_are_zero(const xbridge::cpu::CpuState& state) {
    return state.registers[0] == 0 && state.registers[1] == 0 &&
           state.registers[2] == 0 && state.registers[3] == 0;
}

bool state_is_reset(const xbridge::cpu::CpuState& state) {
    return registers_are_zero(state) && state.instruction_pointer == 0 && !state.halted;
}

void test_x64_cpu_state() {
    using namespace xbridge::cpu;

    X64CpuState state;
    const X64CpuState& const_state = state;
    static_assert(std::is_same_v<
                  decltype(state.gpr(X64GeneralRegister::Rax)), std::uint64_t&>);
    static_assert(std::is_same_v<
                  decltype(const_state.gpr(X64GeneralRegister::Rax)),
                  const std::uint64_t&>);

    bool all_registers_are_zero = true;
    for (const auto value : state.general_purpose_registers) {
        all_registers_are_zero = all_registers_are_zero && value == 0;
    }
    check(state.general_purpose_registers.size() == 16 &&
          all_registers_are_zero && state.rip == 0 && state.rflags == 0,
          "X64CpuState default initializes all storage to zero");

    state.gpr(X64GeneralRegister::Rax) = 0x1122334455667788;
    state.gpr(X64GeneralRegister::Rcx) = 0x8877665544332211;
    state.gpr(X64GeneralRegister::R8) = 0xFEDCBA9876543210;
    check(state.gpr(X64GeneralRegister::Rax) == 0x1122334455667788 &&
          state.gpr(X64GeneralRegister::Rcx) == 0x8877665544332211 &&
          state.gpr(X64GeneralRegister::R8) == 0xFEDCBA9876543210,
          "X64CpuState stores independently indexed general registers");

    state.rip = 0x123456789ABCDEF0;
    state.rflags = 0x0000000000000A95;
    check(state.rip == 0x123456789ABCDEF0 &&
          state.rflags == 0x0000000000000A95,
          "X64CpuState stores RIP and RFLAGS independently");

    state.gpr(X64GeneralRegister::R15) = 0xA5A5A5A55A5A5A5A;
    check(state.gpr(X64GeneralRegister::R15) == 0xA5A5A5A55A5A5A5A,
          "X64CpuState accesses the final R15 register slot");
    check(const_state.gpr(X64GeneralRegister::R15) == 0xA5A5A5A55A5A5A5A,
          "const X64CpuState access reads the selected register");

    const auto invalid_register = static_cast<X64GeneralRegister>(16);
    check_throws<std::out_of_range>(
        [&] { (void)state.gpr(invalid_register); },
        "mutable X64CpuState register access rejects an invalid index");
    check_throws<std::out_of_range>(
        [&] { (void)const_state.gpr(invalid_register); },
        "const X64CpuState register access rejects an invalid index");
}

void test_x64_register_views() {
    using namespace xbridge::cpu;

    X64CpuState state;
    state.gpr(X64GeneralRegister::Rax) = 0xA1B2C3D4E5F60718;
    const auto original_registers = state.general_purpose_registers;
    check(state.read_gpr(X64GeneralRegister::Rax, X64GprWidth::Bits64) ==
              0xA1B2C3D4E5F60718 &&
          state.read_gpr(X64GeneralRegister::Rax, X64GprWidth::Bits32) == 0xE5F60718 &&
          state.read_gpr(X64GeneralRegister::Rax, X64GprWidth::Bits16) == 0x0718 &&
          state.read_gpr(X64GeneralRegister::Rax, X64GprWidth::Bits8) == 0x18 &&
          state.read_gpr(X64GeneralRegister::Rax, X64GprWidth::High8) == 0x07,
          "x64 GPR reads return the selected register portions");
    check(state.general_purpose_registers == original_registers,
          "x64 GPR reads leave stored registers unchanged");

    state.write_gpr(X64GeneralRegister::Rax, X64GprWidth::Bits64, 0x1122334455667788);
    check(state.gpr(X64GeneralRegister::Rax) == 0x1122334455667788,
          "64-bit GPR write replaces the complete register");

    state.write_gpr(X64GeneralRegister::Rax, X64GprWidth::Bits32, 0xA1B2C3D4);
    check(state.gpr(X64GeneralRegister::Rax) == 0x00000000A1B2C3D4,
          "32-bit GPR write zero-extends to 64 bits");

    state.gpr(X64GeneralRegister::Rcx) = 0x1122334455667788;
    state.write_gpr(X64GeneralRegister::Rcx, X64GprWidth::Bits16, 0xABCD);
    check(state.gpr(X64GeneralRegister::Rcx) == 0x112233445566ABCD,
          "16-bit GPR write preserves the upper bits");

    state.gpr(X64GeneralRegister::Rdx) = 0x1122334455667788;
    state.write_gpr(X64GeneralRegister::Rdx, X64GprWidth::Bits8, 0xAB);
    check(state.gpr(X64GeneralRegister::Rdx) == 0x11223344556677AB,
          "8-bit GPR write preserves all other bits");

    state.gpr(X64GeneralRegister::Rax) = 0x1122334455667788;
    state.gpr(X64GeneralRegister::Rbx) = 0xFFEEDDCCBBAA9988;
    state.gpr(X64GeneralRegister::Rcx) = 0x0123456789ABCDEF;
    state.gpr(X64GeneralRegister::Rdx) = 0x8877665544332211;
    state.write_gpr(X64GeneralRegister::Rax, X64GprWidth::High8, 0xA1);
    state.write_gpr(X64GeneralRegister::Rbx, X64GprWidth::High8, 0xB2);
    state.write_gpr(X64GeneralRegister::Rcx, X64GprWidth::High8, 0xC3);
    state.write_gpr(X64GeneralRegister::Rdx, X64GprWidth::High8, 0xD4);
    check(state.read_gpr(X64GeneralRegister::Rax, X64GprWidth::High8) == 0xA1 &&
          state.read_gpr(X64GeneralRegister::Rbx, X64GprWidth::High8) == 0xB2 &&
          state.read_gpr(X64GeneralRegister::Rcx, X64GprWidth::High8) == 0xC3 &&
          state.read_gpr(X64GeneralRegister::Rdx, X64GprWidth::High8) == 0xD4,
          "high-byte views map AH, BH, CH, and DH to their registers");
    check(state.gpr(X64GeneralRegister::Rax) == 0x112233445566A188 &&
          state.gpr(X64GeneralRegister::Rbx) == 0xFFEEDDCCBBAAB288 &&
          state.gpr(X64GeneralRegister::Rcx) == 0x0123456789ABC3EF &&
          state.gpr(X64GeneralRegister::Rdx) == 0x887766554433D411,
          "high-byte writes preserve all bits outside bits 8 through 15");

    state.gpr(X64GeneralRegister::R8) = 0x123456789ABCDEF0;
    check_throws<std::invalid_argument>(
        [&] { (void)state.read_gpr(X64GeneralRegister::R8, X64GprWidth::High8); },
        "high-byte read rejects registers without a legacy high-byte view");
    check_throws<std::invalid_argument>(
        [&] { state.write_gpr(X64GeneralRegister::R8, X64GprWidth::High8, 0xFF); },
        "high-byte write rejects registers without a legacy high-byte view");
    check(state.gpr(X64GeneralRegister::R8) == 0x123456789ABCDEF0,
          "rejected high-byte write leaves the register unchanged");

    const auto invalid_register = static_cast<X64GeneralRegister>(16);
    check_throws<std::out_of_range>(
        [&] { (void)state.read_gpr(invalid_register, X64GprWidth::Bits8); },
        "GPR read rejects an invalid register index");
    check_throws<std::out_of_range>(
        [&] { state.write_gpr(invalid_register, X64GprWidth::Bits8, 0xFF); },
        "GPR write rejects an invalid register index");

    const auto invalid_width = static_cast<X64GprWidth>(0xFF);
    check_throws<std::invalid_argument>(
        [&] { (void)state.read_gpr(X64GeneralRegister::Rax, invalid_width); },
        "GPR read rejects an invalid width");
    check_throws<std::invalid_argument>(
        [&] { state.write_gpr(X64GeneralRegister::Rax, invalid_width, 0xFF); },
        "GPR write rejects an invalid width");
}

void test_x64_register_views_all_registers() {
    using namespace xbridge::cpu;

    X64CpuState state;
    bool all_64_bit_accesses_match = true;
    bool all_32_bit_writes_zero_extend = true;
    bool all_16_bit_writes_preserve_upper_bits = true;
    bool all_8_bit_writes_preserve_other_bits = true;

    for (std::size_t index = 0; index < state.general_purpose_registers.size(); ++index) {
        const auto reg = static_cast<X64GeneralRegister>(index);
        const auto index_value = static_cast<std::uint64_t>(index);

        const auto value64 = 0x1020304050607000ULL | index_value;
        state.write_gpr(reg, X64GprWidth::Bits64, value64);
        all_64_bit_accesses_match &=
            state.read_gpr(reg, X64GprWidth::Bits64) == value64;

        const auto initial32 = 0xA1B2C3D400000000ULL | index_value;
        const auto value32 = 0x76543210ULL + index_value;
        state.gpr(reg) = initial32;
        state.write_gpr(reg, X64GprWidth::Bits32, value32);
        all_32_bit_writes_zero_extend &=
            state.read_gpr(reg, X64GprWidth::Bits64) == value32 &&
            state.read_gpr(reg, X64GprWidth::Bits32) == value32;

        const auto initial16 = 0xFEDCBA9876540000ULL |
                               (index_value << 8) |
                               0x5AULL;
        const auto value16 = 0xA500ULL | index_value;
        const auto expected16 = (initial16 & ~0xFFFFULL) | value16;
        state.gpr(reg) = initial16;
        state.write_gpr(reg, X64GprWidth::Bits16, value16);
        all_16_bit_writes_preserve_upper_bits &=
            state.read_gpr(reg, X64GprWidth::Bits64) == expected16 &&
            state.read_gpr(reg, X64GprWidth::Bits16) == value16;

        const auto initial8 = 0xFEDCBA9876543200ULL | index_value;
        const auto value8 = 0xC0ULL + index_value;
        const auto expected8 = (initial8 & ~0xFFULL) | value8;
        state.gpr(reg) = initial8;
        state.write_gpr(reg, X64GprWidth::Bits8, value8);
        all_8_bit_writes_preserve_other_bits &=
            state.read_gpr(reg, X64GprWidth::Bits64) == expected8 &&
            state.read_gpr(reg, X64GprWidth::Bits8) == value8;
    }

    check(all_64_bit_accesses_match,
          "64-bit GPR reads and writes work for all 16 registers");
    check(all_32_bit_writes_zero_extend,
          "32-bit writes zero-extend for all 16 registers");
    check(all_16_bit_writes_preserve_upper_bits,
          "16-bit writes preserve upper bits for all 16 registers");
    check(all_8_bit_writes_preserve_other_bits,
          "8-bit writes preserve other bits for all 16 registers");
}

void test_x64_decoder_nop() {
    using namespace xbridge::cpu;

    // NOP ohne REX
    {
        const std::uint8_t bytes[] = {0x90};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "NOP: status Ok");
        check(result.opcode == 0x90, "NOP: opcode 0x90");
        check(result.length == 1, "NOP: length 1");
        check(result.operand_size == X64OperandSize::Bits32, "NOP: operand size Bits32");
    }

    // NOP mit REX-Präfix (wird ignoriert)
    {
        const std::uint8_t bytes[] = {0x48, 0x90};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "NOP with REX: status Ok");
        check(result.length == 2, "NOP with REX: length 2");
        check(result.rex.present, "NOP with REX: rex present");
        check(result.rex.w, "NOP with REX: rex.w set");
    }

    // Zusätzliche Bytes nach NOP werden nicht als Teil der NOP interpretiert
    {
        const std::uint8_t bytes[] = {0x90, 0x00, 0x00};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "NOP with tail: status Ok");
        check(result.length == 1, "NOP with tail: length 1");
    }
}

void test_x64_decoder_mov_imm32() {
    using namespace xbridge::cpu;

    // MOV EAX, 0x12345678: 0xB8 0x78 0x56 0x34 0x12
    {
        const std::uint8_t bytes[] = {0xB8, 0x78, 0x56, 0x34, 0x12};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "MOV EAX, imm32: status Ok");
        check(result.opcode == 0xB8, "MOV EAX, imm32: opcode 0xB8");
        check(result.length == 5, "MOV EAX, imm32: length 5");
        check(result.operand_size == X64OperandSize::Bits32, "MOV EAX, imm32: operand size Bits32");
        check(result.dest_reg.base == X64GeneralRegister::Rax, "MOV EAX, imm32: dest RAX");
        check(result.immediate == 0x12345678ULL, "MOV EAX, imm32: immediate value");
        check(result.immediate_size == 4, "MOV EAX, imm32: immediate size 4");
    }

    // MOV ECX, 0x0000000A: 0xB9 0x0A 0x00 0x00 0x00
    {
        const std::uint8_t bytes[] = {0xB9, 0x0A, 0x00, 0x00, 0x00};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "MOV ECX, imm32: status Ok");
        check(result.dest_reg.base == X64GeneralRegister::Rcx, "MOV ECX, imm32: dest RCX");
        check(result.immediate == 10ULL, "MOV ECX, imm32: immediate value 10");
    }
}

void test_x64_decoder_mov_imm64() {
    using namespace xbridge::cpu;

    // MOV RAX, 0x1122334455667788: 0x48 0xB8 0x88 0x77 0x66 0x55 0x44 0x33 0x22 0x11
    const std::uint8_t bytes[] = {0x48, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
    const auto result = decode_x64_instruction(bytes);
    check(result.status == X64DecodeStatus::Ok, "MOV RAX, imm64: status Ok");
    check(result.length == 10, "MOV RAX, imm64: length 10");
    check(result.operand_size == X64OperandSize::Bits64, "MOV RAX, imm64: operand size Bits64");
    check(result.rex.present, "MOV RAX, imm64: rex present");
    check(result.rex.w, "MOV RAX, imm64: rex.w set");
    check(result.dest_reg.base == X64GeneralRegister::Rax, "MOV RAX, imm64: dest RAX");
    check(result.immediate == 0x1122334455667788ULL, "MOV RAX, imm64: immediate value");
    check(result.immediate_size == 8, "MOV RAX, imm64: immediate size 8");

    // Unvollständige imm64 (nur 7 Bytes statt 8)
    {
        const std::uint8_t bytes[] = {0x48, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::TruncatedInput,
              "MOV RAX, truncated imm64: status TruncatedInput");
    }
}

void test_x64_decoder_mov_reg_to_reg() {
    using namespace xbridge::cpu;

    // MOV EAX, ECX: 0x8B 0xC8 (mod=11, reg=000=RAX, rm=001=RCX)
    {
        const std::uint8_t bytes[] = {0x8B, 0xC1};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "MOV EAX, ECX: status Ok");
        check(result.length == 2, "MOV EAX, ECX: length 2");
        check(result.operand_size == X64OperandSize::Bits32, "MOV EAX, ECX: operand size Bits32");
        check(result.dest_reg.base == X64GeneralRegister::Rax, "MOV EAX, ECX: dest RAX");
        check(result.src_reg.base == X64GeneralRegister::Rcx, "MOV EAX, ECX: src RCX");
        check(result.memory_operand == false, "MOV EAX, ECX: no memory operand");
    }

    // MOV RCX, RAX: 0x89 0xC8 (mod=11, reg=000=RAX, rm=001=RCX)
    {
        const std::uint8_t bytes[] = {0x89, 0xC1};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "MOV RCX, RAX: status Ok");
        check(result.dest_reg.base == X64GeneralRegister::Rcx, "MOV RCX, RAX: dest RCX");
        check(result.src_reg.base == X64GeneralRegister::Rax, "MOV RCX, RAX: src RAX");
    }
}

void test_x64_decoder_mov_reg_to_reg_rex() {
    using namespace xbridge::cpu;

    // MOV RAX, RCX (64-bit): 0x48 0x8B 0xC8
    {
        const std::uint8_t bytes[] = {0x48, 0x8B, 0xC1};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "MOV RAX, RCX (64): status Ok");
        check(result.length == 3, "MOV RAX, RCX (64): length 3");
        check(result.operand_size == X64OperandSize::Bits64, "MOV RAX, RCX (64): operand size Bits64");
        check(result.rex.present, "MOV RAX, RCX (64): rex present");
        check(result.rex.w, "MOV RAX, RCX (64): rex.w set");
        check(result.dest_reg.base == X64GeneralRegister::Rax, "MOV RAX, RCX (64): dest RAX");
        check(result.src_reg.base == X64GeneralRegister::Rcx, "MOV RAX, RCX (64): src RCX");
    }

    // MOV R8, RAX: 0x49 0x8B 0xC0 (REX.W=1, REX.R=1, mod=11, reg=000->R8, rm=000->RAX)
    {
        const std::uint8_t bytes[] = {0x4C, 0x8B, 0xC0};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::Ok, "MOV R8, RAX (64): status Ok");
        check(result.dest_reg.base == X64GeneralRegister::R8, "MOV R8, RAX (64): dest R8");
        check(result.dest_reg.extended, "MOV R8, RAX (64): dest extended via REX.R");
        check(result.src_reg.base == X64GeneralRegister::Rax, "MOV R8, RAX (64): src RAX");
    }
}

void test_x64_decoder_mov_memory() {
    using namespace xbridge::cpu;

    // MOV EAX, [disp32] ist nicht unterstützt (kein Displacement in dieser Version).
    // Stattdessen testen wir einfache Register-zu-Speicher und Speicher-zu-Register
    // mit ModR/M mod=00 (kein Displacement, kein SIB).
    // MOV [RAX], ECX ist nicht direkt testbar ohne gültige Speicheradresse im ModR/M rm-Feld,
    // da rm=000 mit mod=00 bedeutet [RAX] (disp32 folgt bei rm=101).
    // Wir testen daher nur Register-zu-Register und prüfen, dass Speicher-Formen
    // entweder dekodiert oder als InvalidEncoding zurückgewiesen werden.

    // MOV [disp32], EAX mit rm=101 (mod=00) -> erfordert Displacement, nicht unterstützt
    {
        const std::uint8_t bytes[] = {0x89, 0x05, 0x00, 0x00, 0x00, 0x00};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::InvalidEncoding,
              "MOV [disp32], EAX with rm=101: InvalidEncoding");
    }
}

void test_x64_decoder_empty_input() {
    using namespace xbridge::cpu;

    std::span<const std::uint8_t> empty{};
    const auto result = decode_x64_instruction(empty);
    check(result.status == X64DecodeStatus::EmptyInput, "empty input: status EmptyInput");
    check(result.length == 0, "empty input: length 0");
}

void test_x64_decoder_unknown_opcode() {
    using namespace xbridge::cpu;

    const std::uint8_t bytes[] = {0x0F};
    const auto result = decode_x64_instruction(bytes);
    check(result.status == X64DecodeStatus::UnknownOpcode, "unknown opcode: status UnknownOpcode");
    check(result.opcode == 0x0F, "unknown opcode: opcode preserved");
    check(result.length == 1, "unknown opcode: length 1");
}

void test_x64_decoder_truncated_input() {
    using namespace xbridge::cpu;

    // MOV EAX, imm32 mit nur 3 Bytes statt 5
    const std::uint8_t bytes[] = {0xB8, 0x78, 0x56};
    const auto result = decode_x64_instruction(bytes);
    check(result.status == X64DecodeStatus::TruncatedInput,
          "truncated imm32: status TruncatedInput");
}

void test_x64_decoder_invalid_encoding() {
    using namespace xbridge::cpu;

    // 0x88 (MOV r/m8, r8) wird nicht unterstützt (keine 8-bit Operanden)
    const std::uint8_t bytes[] = {0x88, 0xC0};
    const auto result = decode_x64_instruction(bytes);
    check(result.status == X64DecodeStatus::UnknownOpcode,
          "MOV r8 variant: UnknownOpcode");
}

void test_x64_decoder_8bit_mov_rejected() {
    using namespace xbridge::cpu;

    // 0x88 (MOV r/m8, r8) ohne REX: UnknownOpcode, Länge 1 (nur Opcode)
    {
        const std::uint8_t bytes[] = {0x88, 0xC0};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::UnknownOpcode,
              "0x88 without REX: status UnknownOpcode");
        check(result.opcode == 0x88, "0x88 without REX: opcode 0x88");
        check(result.length == 1, "0x88 without REX: length 1");
    }

    // 0x8A (MOV r8, r/m8) ohne REX: UnknownOpcode, Länge 1 (nur Opcode)
    {
        const std::uint8_t bytes[] = {0x8A, 0xC0};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::UnknownOpcode,
              "0x8A without REX: status UnknownOpcode");
        check(result.opcode == 0x8A, "0x8A without REX: opcode 0x8A");
        check(result.length == 1, "0x8A without REX: length 1");
    }

    // 0x48 0x88 0xC0: UnknownOpcode, Opcode 0x88, Länge 2 (REX + Opcode, kein ModR/M)
    {
        const std::uint8_t bytes[] = {0x48, 0x88, 0xC0};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::UnknownOpcode,
              "0x48 0x88 0xC0: status UnknownOpcode");
        check(result.opcode == 0x88, "0x48 0x88 0xC0: opcode 0x88");
        check(result.length == 2, "0x48 0x88 0xC0: length 2");
        check(result.rex.present, "0x48 0x88 0xC0: rex present");
        check(result.rex.w, "0x48 0x88 0xC0: rex.w set");
    }

    // 0x41 0x8A 0xC0: UnknownOpcode, Opcode 0x8A, Länge 2 (REX + Opcode, kein ModR/M)
    {
        const std::uint8_t bytes[] = {0x41, 0x8A, 0xC0};
        const auto result = decode_x64_instruction(bytes);
        check(result.status == X64DecodeStatus::UnknownOpcode,
              "0x41 0x8A 0xC0: status UnknownOpcode");
        check(result.opcode == 0x8A, "0x41 0x8A 0xC0: opcode 0x8A");
        check(result.length == 2, "0x41 0x8A 0xC0: length 2");
        check(result.rex.present, "0x41 0x8A 0xC0: rex present");
        check(result.rex.b, "0x41 0x8A 0xC0: rex.b set");
    }
}

void test_x64_decoder_no_state_mutation() {
    using namespace xbridge::cpu;

    X64CpuState state;
    state.gpr(X64GeneralRegister::Rax) = 0xDEADBEEFCAFEF00DULL;
    state.rip = 0x1234ULL;

    const std::uint8_t bytes[] = {0x90};
    const auto result = decode_x64_instruction(bytes);

    check(state.gpr(X64GeneralRegister::Rax) == 0xDEADBEEFCAFEF00DULL,
          "decoder does not modify general registers");
    check(state.rip == 0x1234ULL, "decoder does not modify RIP");
    check(result.status == X64DecodeStatus::Ok,
          "decoder returns Ok independent of CPU state");
}

void test_x64_interpreter_nop() {
    using namespace xbridge;
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;

    const std::uint8_t bytes[] = {0x90};
    const auto instr = decode_x64_instruction(bytes);
    const auto exec_status = execute_instruction(instr, state, memory);

    check(exec_status == X64ExecuteStatus::Ok, "interpreter NOP: status Ok");
    check(state.rip == 0x1001, "interpreter NOP: RIP advanced by 1");
}

void test_x64_interpreter_mov_imm32() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;

    // MOV EAX, 0x12345678
    const std::uint8_t bytes[] = {0xB8, 0x78, 0x56, 0x34, 0x12};
    const auto instr = decode_x64_instruction(bytes);
    const auto exec_status = execute_instruction(instr, state, memory);

    check(exec_status == X64ExecuteStatus::Ok, "interpreter MOV EAX, imm32: status Ok");
    check(state.gpr(X64GeneralRegister::Rax) == 0x12345678ULL,
          "interpreter MOV EAX, imm32: RAX = 0x12345678");
    check(state.rip == 0x1005, "interpreter MOV EAX, imm32: RIP advanced by 5");
}

void test_x64_interpreter_mov_imm64() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;

    // MOV RAX, 0x1122334455667788
    const std::uint8_t bytes[] = {0x48, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
    const auto instr = decode_x64_instruction(bytes);
    const auto exec_status = execute_instruction(instr, state, memory);

    check(exec_status == X64ExecuteStatus::Ok, "interpreter MOV RAX, imm64: status Ok");
    check(state.gpr(X64GeneralRegister::Rax) == 0x1122334455667788ULL,
          "interpreter MOV RAX, imm64: RAX = 0x1122334455667788");
    check(state.rip == 0x100A, "interpreter MOV RAX, imm64: RIP advanced by 10");
}

void test_x64_interpreter_mov_reg_to_reg() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rcx) = 0xAABBCCDD11223344ULL;

    // MOV RAX, RCX (64-bit): 0x48 0x8B 0xC8
    const std::uint8_t bytes[] = {0x48, 0x8B, 0xC1};
    const auto instr = decode_x64_instruction(bytes);
    const auto exec_status = execute_instruction(instr, state, memory);

    check(exec_status == X64ExecuteStatus::Ok, "interpreter MOV RAX, RCX: status Ok");
    check(state.gpr(X64GeneralRegister::Rax) == 0xAABBCCDD11223344ULL,
          "interpreter MOV RAX, RCX: RAX copied from RCX");
    check(state.gpr(X64GeneralRegister::Rcx) == 0xAABBCCDD11223344ULL,
          "interpreter MOV RAX, RCX: RCX unchanged");
    check(state.rip == 0x1003, "interpreter MOV RAX, RCX: RIP advanced by 3");
}

void test_x64_interpreter_mov_reg_to_reg_32bit_zero_extend() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rcx) = 0xFFFFFFFFAABBCCDDULL;

    // MOV EAX, ECX (32-bit): 0x8B 0xC8 -> sollte obere 32 Bits von RAX nullen
    const std::uint8_t bytes[] = {0x8B, 0xC1};
    const auto instr = decode_x64_instruction(bytes);
    const auto exec_status = execute_instruction(instr, state, memory);

    check(exec_status == X64ExecuteStatus::Ok, "interpreter MOV EAX, ECX: status Ok");
    check(state.gpr(X64GeneralRegister::Rax) == 0x00000000AABBCCDDULL,
          "interpreter MOV EAX, ECX: 32-bit write zero-extends to 64 bits");
    check(state.rip == 0x1002, "interpreter MOV EAX, ECX: RIP advanced by 2");
}

void test_x64_interpreter_mov_memory_roundtrip() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x00000010; // Adresse 16
    state.gpr(X64GeneralRegister::Rcx) = 0xDEADBEEF;

    // Wir können keine direkten Speicheroperanden mit ModR/M mod=00 testen,
    // da rm=000 mit mod=00 [RAX] bedeutet (kein Displacement).
    // Stattdessen testen wir, dass Register-zu-Register korrekt funktioniert
    // und Speicherfehler korrekt behandelt werden.

    // MOV [RAX], ECX wäre: 0x89 0x00 (mod=00, reg=001=ECX, rm=000=RAX)
    // Aber mod=00, rm=000 bedeutet [RAX] ohne Displacement - wird unterstützt.
    const std::uint8_t bytes[] = {0x89, 0x08};
    const auto instr = decode_x64_instruction(bytes);
    check(instr.status == X64DecodeStatus::Ok, "MOV [RAX], ECX: decode Ok");
    check(instr.memory_operand, "MOV [RAX], ECX: memory operand");

    const auto exec_status = execute_instruction(instr, state, memory);
    check(exec_status == X64ExecuteStatus::Ok, "interpreter MOV [RAX], ECX: status Ok");
    check(memory.read32(16) == 0xDEADBEEF,
          "interpreter MOV [RAX], ECX: memory at address 16 = 0xDEADBEEF");
    check(state.rip == 0x1002, "interpreter MOV [RAX], ECX: RIP advanced by 2");

    // Zurück lesen: MOV ECX, [RAX]
    state.gpr(X64GeneralRegister::Rcx) = 0;
    state.rip = 0x1000;
    const std::uint8_t bytes2[] = {0x8B, 0x08};
    const auto instr2 = decode_x64_instruction(bytes2);
    check(instr2.status == X64DecodeStatus::Ok, "MOV ECX, [RAX]: decode Ok");
    check(instr2.memory_operand, "MOV ECX, [RAX]: memory operand");

    const auto exec_status2 = execute_instruction(instr2, state, memory);
    check(exec_status2 == X64ExecuteStatus::Ok, "interpreter MOV ECX, [RAX]: status Ok");
    check(state.gpr(X64GeneralRegister::Rcx) == 0xDEADBEEF,
          "interpreter MOV ECX, [RAX]: RCX = 0xDEADBEEF");
    check(state.rip == 0x1002, "interpreter MOV ECX, [RAX]: RIP advanced by 2");
}

void test_x64_interpreter_memory_bounds() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(16); // Nur 16 Bytes
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x0000000C; // Adresse 12
    state.gpr(X64GeneralRegister::Rcx) = 0xDEADBEEF;

    // MOV [RAX], ECX schreibt 4 Bytes ab Adresse 12 (Bytes 12-15, OK)
    {
        const std::uint8_t bytes[] = {0x89, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV [RAX], ECX at boundary: status Ok");
        check(memory.read32(12) == 0xDEADBEEF, "MOV [RAX], ECX at boundary: value written");
    }

    // MOV [RAX], ECX mit Adresse 13 würde Bytes 13-16 schreiben (16 ist out of bounds)
    state.gpr(X64GeneralRegister::Rax) = 0x0000000D;
    {
        const std::uint8_t bytes[] = {0x89, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::MemoryAccessError,
              "MOV [RAX], ECX out of bounds: status MemoryAccessError");
    }

    // MOV ECX, [RAX] mit Adresse 13 (out of bounds)
    state.gpr(X64GeneralRegister::Rcx) = 0;
    {
        const std::uint8_t bytes[] = {0x8B, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::MemoryAccessError,
              "MOV ECX, [RAX] out of bounds: status MemoryAccessError");
    }
}

void test_x64_interpreter_mov_memory_registers() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rcx) = 0x00000020; // Adresse 32
    state.gpr(X64GeneralRegister::Rsi) = 0x00000040; // Adresse 64
    state.gpr(X64GeneralRegister::Rdx) = 0xCAFEBABE;
    state.gpr(X64GeneralRegister::Rax) = 0xDEADBEEF;

    // MOV [RCX], EDX: 0x89 0x11 (mod=00, reg=010=EDX, rm=001=RCX)
    {
        const std::uint8_t bytes[] = {0x89, 0x11};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV [RCX], EDX: decode Ok");
        check(instr.memory_operand, "MOV [RCX], EDX: memory operand");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV [RCX], EDX: execute Ok");
        check(memory.read32(32) == 0xCAFEBABE,
              "MOV [RCX], EDX: memory at address 32 = 0xCAFEBABE");
    }

    // MOV [RSI], ECX: 0x89 0x0E (mod=00, reg=001=ECX, rm=110=RSI)
    // Schreibt RCX (0x00000020) nach [RSI] = Adresse 64
    {
        const std::uint8_t bytes[] = {0x89, 0x0E};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV [RSI], ECX: decode Ok");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV [RSI], ECX: execute Ok");
        check(memory.read32(64) == 0x00000020,
              "MOV [RSI], ECX: memory at address 64 = 0x00000020");
    }

    // MOV ECX, [RSI]: 0x8B 0x0E (mod=00, reg=001=ECX, rm=110=RSI)
    // Liest von [RSI] = Adresse 64 (dort steht nun 0x00000020)
    {
        state.gpr(X64GeneralRegister::Rcx) = 0;
        const std::uint8_t bytes[] = {0x8B, 0x0E};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV ECX, [RSI]: decode Ok");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV ECX, [RSI]: execute Ok");
        check(state.gpr(X64GeneralRegister::Rcx) == 0x00000020,
              "MOV ECX, [RSI]: RCX = 0x00000020");
    }
}

void test_x64_interpreter_mov_memory_rex_b() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::R8) = 0x00000080; // Adresse 128
    state.gpr(X64GeneralRegister::Rdx) = 0xA1B2C3D4;

    // MOV [R8], EDX mit REX.B: 0x41 0x89 0x10
    // REX.B=1, mod=00, reg=010=EDX, rm=000=R8
    {
        const std::uint8_t bytes[] = {0x41, 0x89, 0x10};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV [R8], EDX: decode Ok");
        check(instr.memory_operand, "MOV [R8], EDX: memory operand");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV [R8], EDX: execute Ok");
        check(memory.read32(128) == 0xA1B2C3D4,
              "MOV [R8], EDX: memory at address 128 = 0xA1B2C3D4");
    }

    // MOV EDX, [R8] mit REX.B: 0x41 0x8B 0x10
    // REX.B=1, mod=00, reg=010=EDX, rm=000=R8
    {
        state.gpr(X64GeneralRegister::Rdx) = 0;
        const std::uint8_t bytes[] = {0x41, 0x8B, 0x10};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV EDX, [R8]: decode Ok");
        check(instr.operand_size == X64OperandSize::Bits32,
              "MOV EDX, [R8]: operand size Bits32");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV EDX, [R8]: execute Ok");
        check(state.gpr(X64GeneralRegister::Rdx) == 0xA1B2C3D4,
              "MOV EDX, [R8]: RDX = 0xA1B2C3D4");
    }

    // MOV [RAX], RCX (64-bit) gefolgt von MOV R9, [RAX] (64-bit) mit REX.W+REX.R
    state.gpr(X64GeneralRegister::Rax) = 0x00000010; // Adresse 16
    state.gpr(X64GeneralRegister::Rcx) = 0x1122334455667788ULL;

    // MOV [RAX], RCX (64-bit): 0x48 0x89 0x08
    {
        const std::uint8_t bytes[] = {0x48, 0x89, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV [RAX], RCX (64): decode Ok");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV [RAX], RCX (64): execute Ok");
    }

    // MOV R9, [RAX] (64-bit): 0x4C 0x8B 0x08
    // REX.W=1, REX.R=1, REX.B=0, mod=00, reg=001 (mit REX.R → R9), rm=000 (RAX)
    // 0x4C = 01001100: W=1, R=1, X=0, B=0
    {
        state.gpr(X64GeneralRegister::R9) = 0;
        const std::uint8_t bytes[] = {0x4C, 0x8B, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV R9, [RAX]: decode Ok");
        check(instr.operand_size == X64OperandSize::Bits64,
              "MOV R9, [RAX]: operand size Bits64");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV R9, [RAX]: execute Ok");
        check(state.gpr(X64GeneralRegister::R9) == 0x1122334455667788ULL,
              "MOV R9, [RAX]: R9 = 0x1122334455667788");
    }
}

void test_x64_interpreter_mov_memory_sib_rejected() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x00000010;
    state.gpr(X64GeneralRegister::Rcx) = 0xDEADBEEF;

    // SIB-Encoding: mod=00, rm=100 (0x89 0x04) → InvalidEncoding
    {
        const std::uint8_t bytes[] = {0x89, 0x04};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::InvalidEncoding,
              "SIB encoding: status InvalidEncoding");
    }
}

void test_x64_interpreter_mov_memory_rip_relative_rejected() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x00000010;
    state.gpr(X64GeneralRegister::Rcx) = 0xDEADBEEF;

    // RIP-relative: mod=00, rm=101 (0x89 0x05) → InvalidEncoding
    {
        const std::uint8_t bytes[] = {0x89, 0x05};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::InvalidEncoding,
              "RIP-relative encoding: status InvalidEncoding");
    }
}

void test_x64_interpreter_mov_memory_64bit() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x00000010; // Adresse 16
    state.gpr(X64GeneralRegister::Rcx) = 0x1122334455667788ULL;

    // MOV [RAX], RCX (64-bit): 0x48 0x89 0x08 (REX.W=1, mod=00, reg=001=RCX, rm=000=RAX)
    {
        const std::uint8_t bytes[] = {0x48, 0x89, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV [RAX], RCX (64): decode Ok");
        check(instr.operand_size == X64OperandSize::Bits64, "MOV [RAX], RCX (64): operand size Bits64");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV [RAX], RCX (64): execute Ok");
        // Little-endian Leseprüfung aller 8 Bytes
        check(memory.read8(16) == 0x88 && memory.read8(17) == 0x77 &&
                  memory.read8(18) == 0x66 && memory.read8(19) == 0x55 &&
                  memory.read8(20) == 0x44 && memory.read8(21) == 0x33 &&
                  memory.read8(22) == 0x22 && memory.read8(23) == 0x11,
              "MOV [RAX], RCX (64): alle 8 Bytes korrekt im Speicher");
    }

    // MOV RDX, [RAX] (64-bit): 0x48 0x8B 0x10 (REX.W=1, mod=00, reg=010=RDX, rm=000=RAX)
    {
        state.gpr(X64GeneralRegister::Rdx) = 0;
        const std::uint8_t bytes[] = {0x48, 0x8B, 0x10};
        const auto instr = decode_x64_instruction(bytes);
        check(instr.status == X64DecodeStatus::Ok, "MOV RDX, [RAX] (64): decode Ok");
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV RDX, [RAX] (64): execute Ok");
        check(state.gpr(X64GeneralRegister::Rdx) == 0x1122334455667788ULL,
              "MOV RDX, [RAX] (64): RDX = 0x1122334455667788");
    }
}

void test_x64_interpreter_mov_memory_64bit_write_atomic() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(16); // Nur 16 Bytes
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x00000008; // Adresse 8
    state.gpr(X64GeneralRegister::Rcx) = 0x1122334455667788ULL;

    // MOV [RAX], RCX (64-bit) ab Adresse 8 schreibt Bytes 8-15 (gültig)
    {
        const std::uint8_t bytes[] = {0x48, 0x89, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::Ok, "MOV [RAX], RCX (64) at boundary: status Ok");
        check(memory.read8(15) == 0x11, "MOV [RAX], RCX (64): letztes Byte geschrieben");
    }

    // MOV [RAX], RCX (64-bit) ab Adresse 9 schreibt Bytes 9-16 (16 ist out of bounds)
    // Muss MemoryAccessError liefern und keinerlei Teiländerung hinterlassen
    state.gpr(X64GeneralRegister::Rax) = 0x00000009;
    {
        // Alle acht Zielbytes vor dem Versuch speichern
        const auto byte8_before = memory.read8(8);
        const auto byte9_before = memory.read8(9);
        const auto byte10_before = memory.read8(10);
        const auto byte11_before = memory.read8(11);
        const auto byte12_before = memory.read8(12);
        const auto byte13_before = memory.read8(13);
        const auto byte14_before = memory.read8(14);
        const auto byte15_before = memory.read8(15);

        const std::uint8_t bytes[] = {0x48, 0x89, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::MemoryAccessError,
              "MOV [RAX], RCX (64) out of bounds: status MemoryAccessError");

        // Keine einzige der acht Zielbytes darf verändert worden sein
        check(memory.read8(8) == byte8_before, "MOV [RAX], RCX (64) out of bounds: byte 8 unchanged");
        check(memory.read8(9) == byte9_before, "MOV [RAX], RCX (64) out of bounds: byte 9 unchanged");
        check(memory.read8(10) == byte10_before, "MOV [RAX], RCX (64) out of bounds: byte 10 unchanged");
        check(memory.read8(11) == byte11_before, "MOV [RAX], RCX (64) out of bounds: byte 11 unchanged");
        check(memory.read8(12) == byte12_before, "MOV [RAX], RCX (64) out of bounds: byte 12 unchanged");
        check(memory.read8(13) == byte13_before, "MOV [RAX], RCX (64) out of bounds: byte 13 unchanged");
        check(memory.read8(14) == byte14_before, "MOV [RAX], RCX (64) out of bounds: byte 14 unchanged");
        check(memory.read8(15) == byte15_before, "MOV [RAX], RCX (64) out of bounds: byte 15 unchanged");
    }
}

void test_x64_interpreter_mov_memory_address_above_32bit() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(256);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x100000000ULL; // Über 32-Bit-Adressbereich
    state.gpr(X64GeneralRegister::Rcx) = 0xDEADBEEF;

    // Adresse > UINT32_MAX darf nicht stillschweigend gekürzt werden
    {
        const std::uint8_t bytes[] = {0x89, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::MemoryAccessError,
              "MOV [RAX], ECX above 32-bit: status MemoryAccessError");
    }
    check(state.gpr(X64GeneralRegister::Rax) == 0x100000000ULL,
          "MOV [RAX], ECX above 32-bit: RAX unchanged");
    check(state.rip == 0x1000,
          "MOV [RAX], ECX above 32-bit: RIP unchanged");
}

void test_x64_interpreter_memory_error_no_state_mutation() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(16);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x0000000D; // Adresse 13 (out of bounds für 4-Byte-Zugriff)
    state.gpr(X64GeneralRegister::Rcx) = 0xDEADBEEF;

    // Speicher vor dem Versuch markieren
    const auto rax_before = state.gpr(X64GeneralRegister::Rax);
    const auto rcx_before = state.gpr(X64GeneralRegister::Rcx);
    const auto rip_before = state.rip;
    const auto byte12_before = memory.read8(12);
    const auto byte13_before = memory.read8(13);
    const auto byte14_before = memory.read8(14);
    const auto byte15_before = memory.read8(15);

    // MOV [RAX], ECX mit Adresse 13 (Bytes 13-16 out of bounds)
    {
        const std::uint8_t bytes[] = {0x89, 0x08};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(exec_status == X64ExecuteStatus::MemoryAccessError,
              "MOV [RAX], ECX out of bounds: status MemoryAccessError");
    }

    // Keine Zustandsänderungen
    check(state.gpr(X64GeneralRegister::Rax) == rax_before,
          "MemoryAccessError: RAX unchanged");
    check(state.gpr(X64GeneralRegister::Rcx) == rcx_before,
          "MemoryAccessError: RCX unchanged");
    check(state.rip == rip_before,
          "MemoryAccessError: RIP unchanged");
    // Keine Teiländerungen im Speicher
    check(memory.read8(12) == byte12_before, "MemoryAccessError: byte 12 unchanged");
    check(memory.read8(13) == byte13_before, "MemoryAccessError: byte 13 unchanged");
    check(memory.read8(14) == byte14_before, "MemoryAccessError: byte 14 unchanged");
    check(memory.read8(15) == byte15_before, "MemoryAccessError: byte 15 unchanged");
}

void test_x64_interpreter_error_no_state_mutation() {
    using namespace xbridge::cpu;

    memory::GuestMemory memory(16);
    X64CpuState state;
    state.rip = 0x1000;
    state.gpr(X64GeneralRegister::Rax) = 0x1234;
    state.gpr(X64GeneralRegister::Rcx) = 0x5678;

    // Unvollständige imm64 kann nicht ausgeführt werden (TruncatedInput)
    const std::uint8_t bytes[] = {0x48, 0xB8, 0x88, 0x77};
    const auto instr = decode_x64_instruction(bytes);
    check(instr.status == X64DecodeStatus::TruncatedInput,
          "interpreter truncated: decode TruncatedInput");

    const auto exec_status = execute_instruction(instr, state, memory);
    check(exec_status == X64ExecuteStatus::Ok,
          "interpreter truncated: execute Ok (no-op)");
    check(state.gpr(X64GeneralRegister::Rax) == 0x1234,
          "interpreter truncated: RAX unchanged");
    check(state.gpr(X64GeneralRegister::Rcx) == 0x5678,
          "interpreter truncated: RCX unchanged");
    check(state.rip == 0x1000,
          "interpreter truncated: RIP unchanged");
}

void test_cpu_execution_hardening() {
    using namespace xbridge;

    memory::GuestMemory last_byte_memory(1);
    cpu::TestCpu last_byte_cpu(last_byte_memory);
    last_byte_cpu.load_program({0xFF});
    last_byte_cpu.run(1);
    check(last_byte_cpu.state().instruction_pointer == 1 && last_byte_cpu.state().halted,
          "TestCpu executes Halt at the last valid guest address");
    check_does_not_throw([&] { last_byte_cpu.run(0); },
                         "run after Halt is a no-op even with a zero instruction limit");
    check(last_byte_cpu.state().instruction_pointer == 1 && last_byte_cpu.state().halted,
          "run after Halt preserves the halted CPU state");

    const auto check_invalid_register = [](const std::vector<std::uint8_t>& program,
                                           const char* error_message,
                                           const char* state_message) {
        memory::GuestMemory memory(program.size());
        cpu::TestCpu test_cpu(memory);
        test_cpu.load_program(program);
        check_throws<std::runtime_error>([&] { test_cpu.run(); }, error_message);
        check(test_cpu.state().instruction_pointer == program.size() &&
              registers_are_zero(test_cpu.state()) && !test_cpu.state().halted,
              state_message);
    };
    check_invalid_register({0x01, 0x04, 0x01, 0x00, 0x00, 0x00},
                           "LoadImm rejects an invalid register",
                           "LoadImm failure preserves registers and records consumed operands");
    check_invalid_register({0x02, 0x04, 0x01, 0x00, 0x00, 0x00},
                           "AddImm rejects an invalid register",
                           "AddImm failure preserves registers and records consumed operands");
    check_invalid_register({0x03, 0x04, 0x00, 0x00, 0x00, 0x00},
                           "Store rejects an invalid register",
                           "Store register failure preserves registers and records consumed operands");

    const auto check_truncated_program = [](const std::vector<std::uint8_t>& program,
                                            std::uint32_t expected_ip,
                                            const char* error_message,
                                            const char* state_message) {
        memory::GuestMemory memory(program.size());
        cpu::TestCpu test_cpu(memory);
        test_cpu.load_program(program);
        check_throws<std::out_of_range>([&] { test_cpu.run(); }, error_message);
        check(test_cpu.state().instruction_pointer == expected_ip &&
              registers_are_zero(test_cpu.state()) && !test_cpu.state().halted,
              state_message);
    };
    check_truncated_program({0x01}, 1,
                            "LoadImm rejects a missing register operand",
                            "missing register leaves the IP at the program boundary");
    check_truncated_program({0x01, 0x00, 0x34, 0x12}, 4,
                            "LoadImm rejects a truncated immediate",
                            "truncated LoadImm leaves the IP and registers consistent");
    check_truncated_program({0x02, 0x00, 0x34}, 3,
                            "AddImm rejects a truncated immediate",
                            "truncated AddImm leaves the IP and registers consistent");
    check_truncated_program({0x03, 0x00, 0x0C, 0x00, 0x00}, 5,
                            "Store rejects a truncated address operand",
                            "truncated Store leaves the IP and registers consistent");

    memory::GuestMemory unknown_memory(2);
    cpu::TestCpu unknown_cpu(unknown_memory);
    unknown_cpu.load_program({0x7E, 0xFF});
    check_throws<std::runtime_error>([&] { unknown_cpu.run(); },
                                    "unknown opcode throws a runtime error");
    check(unknown_cpu.state().instruction_pointer == 1 &&
          registers_are_zero(unknown_cpu.state()) && !unknown_cpu.state().halted,
          "unknown opcode leaves a resumable state after its fetched byte");
    check_does_not_throw([&] { unknown_cpu.run(); },
                         "run after unknown-opcode error resumes from the current IP");
    check(unknown_cpu.state().instruction_pointer == 2 && unknown_cpu.state().halted,
          "resumed execution reaches the following Halt");

    const std::vector<std::uint8_t> limit_program{0x01, 0x00, 0x07, 0x00, 0x00, 0x00, 0xFF};
    memory::GuestMemory zero_limit_memory(limit_program.size());
    cpu::TestCpu zero_limit_cpu(zero_limit_memory);
    zero_limit_cpu.load_program(limit_program);
    check_throws<std::runtime_error>([&] { zero_limit_cpu.run(0); },
                                    "zero instruction limit fails before execution");
    check(state_is_reset(zero_limit_cpu.state()),
          "zero instruction limit preserves the initial CPU state");

    memory::GuestMemory normal_limit_memory(limit_program.size());
    cpu::TestCpu normal_limit_cpu(normal_limit_memory);
    normal_limit_cpu.load_program(limit_program);
    check_does_not_throw([&] { normal_limit_cpu.run(10); },
                         "normal instruction limit allows the program to halt");
    check(normal_limit_cpu.state().registers[0] == 7 &&
          normal_limit_cpu.state().instruction_pointer == limit_program.size() &&
          normal_limit_cpu.state().halted,
          "normal instruction limit preserves completed instruction state");

    memory::GuestMemory exact_limit_memory(limit_program.size());
    cpu::TestCpu exact_limit_cpu(exact_limit_memory);
    exact_limit_cpu.load_program(limit_program);
    check_does_not_throw([&] { exact_limit_cpu.run(2); },
                         "exact instruction limit allows the final Halt");
    check(exact_limit_cpu.state().halted,
          "exact instruction limit finishes in a halted state");

    memory::GuestMemory exceeded_limit_memory(limit_program.size());
    cpu::TestCpu exceeded_limit_cpu(exceeded_limit_memory);
    exceeded_limit_cpu.load_program(limit_program);
    check_throws<std::runtime_error>([&] { exceeded_limit_cpu.run(1); },
                                     "instruction limit rejects an additional instruction");
    check(exceeded_limit_cpu.state().registers[0] == 7 &&
          exceeded_limit_cpu.state().instruction_pointer == 6 &&
          !exceeded_limit_cpu.state().halted,
          "instruction-limit error preserves completed work and the next IP");
    check_does_not_throw([&] { exceeded_limit_cpu.run(1); },
                         "run after instruction-limit error resumes at the next instruction");
    check(exceeded_limit_cpu.state().halted &&
          exceeded_limit_cpu.state().instruction_pointer == limit_program.size(),
          "resumed instruction-limit run reaches Halt");

    const std::vector<std::uint8_t> store_to_end{
        0x01, 0x00, 0xD4, 0xC3, 0xB2, 0xA1,
        0x03, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xFF
    };
    memory::GuestMemory end_store_memory(32);
    cpu::TestCpu end_store_cpu(end_store_memory);
    end_store_cpu.load_program(store_to_end);
    check_does_not_throw([&] { end_store_cpu.run(); },
                         "CPU Store ending at the last valid guest byte succeeds");
    check(end_store_memory.read32(28) == 0xA1B2C3D4,
          "CPU Store writes the final valid guest word");

    const std::vector<std::uint8_t> unaligned_store{
        0x01, 0x00, 0xD4, 0xC3, 0xB2, 0xA1,
        0x03, 0x00, 0x11, 0x00, 0x00, 0x00, 0xFF
    };
    memory::GuestMemory unaligned_memory(32);
    cpu::TestCpu unaligned_cpu(unaligned_memory);
    unaligned_cpu.load_program(unaligned_store);
    check_does_not_throw([&] { unaligned_cpu.run(); },
                         "CPU Store supports the GuestMemory unaligned access contract");
    check(unaligned_memory.read32(17) == 0xA1B2C3D4,
          "unaligned CPU Store preserves the written value");

    const std::vector<std::uint8_t> failing_store{
        0x01, 0x00, 0xD4, 0xC3, 0xB2, 0xA1,
        0x03, 0x00, 0x0E, 0x00, 0x00, 0x00
    };
    memory::GuestMemory failing_store_memory(16);
    cpu::TestCpu failing_store_cpu(failing_store_memory);
    failing_store_cpu.load_program(failing_store);
    check_throws<std::out_of_range>([&] { failing_store_cpu.run(); },
                                    "out-of-range CPU Store propagates a memory exception");
    check(failing_store_cpu.state().registers[0] == 0xA1B2C3D4 &&
          failing_store_cpu.state().instruction_pointer == failing_store.size() &&
          !failing_store_cpu.state().halted &&
          failing_store_memory.read8(14) == 0 && failing_store_memory.read8(15) == 0,
          "failed CPU Store preserves registers, IP, and guest memory");
    check_throws<std::out_of_range>([&] { failing_store_cpu.run(); },
                                    "run after failed Store cannot fetch beyond the program");
    check(failing_store_cpu.state().instruction_pointer == failing_store.size() &&
          failing_store_cpu.state().registers[0] == 0xA1B2C3D4 &&
          !failing_store_cpu.state().halted,
          "repeated fetch failure leaves CPU state unchanged");
}

void test_x64_instruction_decoder() {
    using namespace xbridge::cpu;

    // NOP wird korrekt erkannt, Instruktionslänge beträgt 1 Byte
    {
        const std::uint8_t nop[] = {0x90};
        const auto result = decode_x64_instruction(nop);
        check(result.status == X64DecodeStatus::Ok, "NOP decode status is Ok");
        check(result.opcode == 0x90, "NOP decode opcode is 0x90");
        check(result.length == 1, "NOP decode length is 1");
    }

    // Zusätzliche Bytes nach NOP werden nicht als Teil der NOP interpretiert
    {
        const std::uint8_t nop_with_tail[] = {0x90, 0x00, 0x00};
        const auto result = decode_x64_instruction(nop_with_tail);
        check(result.status == X64DecodeStatus::Ok, "NOP with trailing bytes status is Ok");
        check(result.length == 1, "NOP with trailing bytes length is 1");
    }

    // Leere Eingabe wird korrekt behandelt (std::span)
    {
        std::span<const std::uint8_t> empty{};
        const auto result = decode_x64_instruction(empty);
        check(result.status == X64DecodeStatus::EmptyInput, "empty input status is EmptyInput");
        check(result.length == 0, "empty input length is 0");
    }

    // Unbekannter Opcode wird korrekt behandelt (keine Exception)
    {
        const std::uint8_t unknown[] = {0x00};
        const auto result = decode_x64_instruction(unknown);
        check(result.status == X64DecodeStatus::UnknownOpcode, "unknown opcode status is UnknownOpcode");
        check(result.opcode == 0x00, "unknown opcode preserved");
        check(result.length == 1, "unknown opcode length is 1 (opcode byte consumed)");
    }

    // Decoder verändert keinen CPU-Zustand (state-los)
    {
        X64CpuState state;
        state.gpr(X64GeneralRegister::Rax) = 0xDEADBEEFCAFEF00DULL;
        state.rip = 0x1234ULL;

        const std::uint8_t nop[] = {0x90};
        const auto result = decode_x64_instruction(nop);

        check(state.gpr(X64GeneralRegister::Rax) == 0xDEADBEEFCAFEF00DULL,
              "decoder does not modify general registers");
        check(state.rip == 0x1234ULL, "decoder does not modify RIP");
        check(result.status == X64DecodeStatus::Ok,
              "decoder returns Ok independent of CPU state");
    }
}

void test_x64_decoder_rex_extended_registers() {
    using namespace xbridge::cpu;

    // REX.W=1 with REX.B=1 selects R8-R15 as the MOV immediate destination (ARCH 115-118).
    bool imm64_all_registers = true;
    for (std::uint8_t index = 0; index < 8; ++index) {
        const std::uint64_t immediate =
            0x1122334455667700ULL | static_cast<std::uint64_t>(index);
        std::vector<std::uint8_t> bytes{0x49, static_cast<std::uint8_t>(0xB8 + index)};
        for (std::size_t byte_index = 0; byte_index < 8; ++byte_index) {
            bytes.push_back(
                static_cast<std::uint8_t>((immediate >> (8 * byte_index)) & 0xFFULL));
        }
        const auto decoded = decode_x64_instruction(
            std::span<const std::uint8_t>(bytes.data(), bytes.size()));
        const auto expected = static_cast<X64GeneralRegister>(8U + index);
        imm64_all_registers = imm64_all_registers &&
            decoded.status == X64DecodeStatus::Ok &&
            decoded.rex.w && decoded.rex.b &&
            decoded.operand_size == X64OperandSize::Bits64 &&
            decoded.dest_reg.base == expected && decoded.dest_reg.extended &&
            decoded.immediate == immediate && decoded.immediate_size == 8;
    }
    check(imm64_all_registers,
          "REX.W+B MOV imm64 decodes R8-R15 destinations with 64-bit immediates");

    // REX.B without REX.W selects R8-R15 for 32-bit MOV immediate destinations.
    bool imm32_all_registers = true;
    for (std::uint8_t index = 0; index < 8; ++index) {
        const std::uint32_t immediate =
            0x12345670U | static_cast<std::uint32_t>(index);
        std::vector<std::uint8_t> bytes{0x41, static_cast<std::uint8_t>(0xB8 + index)};
        for (std::size_t byte_index = 0; byte_index < 4; ++byte_index) {
            bytes.push_back(
                static_cast<std::uint8_t>((immediate >> (8 * byte_index)) & 0xFFU));
        }
        const auto decoded = decode_x64_instruction(
            std::span<const std::uint8_t>(bytes.data(), bytes.size()));
        const auto expected = static_cast<X64GeneralRegister>(8U + index);
        imm32_all_registers = imm32_all_registers &&
            decoded.status == X64DecodeStatus::Ok &&
            !decoded.rex.w && decoded.rex.b &&
            decoded.operand_size == X64OperandSize::Bits32 &&
            decoded.dest_reg.base == expected && decoded.dest_reg.extended &&
            decoded.immediate == static_cast<std::uint64_t>(immediate) &&
            decoded.immediate_size == 4;
    }
    check(imm32_all_registers,
          "REX.B without REX.W decodes R8-R15 32-bit MOV immediate destinations");

    // MOV r, r/m with REX.R+REX.B: reg is the destination, r/m is the source.
    bool reg_reg_load = true;
    for (std::uint8_t index = 0; index < 8; ++index) {
        const std::uint8_t dest_low = index;
        const std::uint8_t src_low = static_cast<std::uint8_t>(7U - index);
        const std::uint8_t modrm = static_cast<std::uint8_t>(0xC0 | (dest_low << 3) | src_low);
        const std::uint8_t bytes[] = {0x4D, 0x8B, modrm};
        const auto decoded = decode_x64_instruction(bytes);
        reg_reg_load = reg_reg_load &&
            decoded.status == X64DecodeStatus::Ok &&
            decoded.rex.r && decoded.rex.b &&
            decoded.operand_size == X64OperandSize::Bits64 &&
            !decoded.memory_operand &&
            decoded.dest_reg.base == static_cast<X64GeneralRegister>(8U + dest_low) &&
            decoded.dest_reg.extended &&
            decoded.src_reg.base == static_cast<X64GeneralRegister>(8U + src_low) &&
            decoded.src_reg.extended;
    }
    check(reg_reg_load,
          "REX.W+R+B MOV r, r/m selects R8-R15 for both register operands");

    // MOV r/m, r with REX.R+REX.B: r/m is the destination, reg is the source.
    bool reg_reg_store = true;
    for (std::uint8_t index = 0; index < 8; ++index) {
        const std::uint8_t dest_low = index;
        const std::uint8_t src_low = static_cast<std::uint8_t>(7U - index);
        const std::uint8_t modrm = static_cast<std::uint8_t>(0xC0 | (src_low << 3) | dest_low);
        const std::uint8_t bytes[] = {0x4D, 0x89, modrm};
        const auto decoded = decode_x64_instruction(bytes);
        reg_reg_store = reg_reg_store &&
            decoded.status == X64DecodeStatus::Ok &&
            decoded.rex.r && decoded.rex.b &&
            decoded.operand_size == X64OperandSize::Bits64 &&
            !decoded.memory_operand &&
            decoded.dest_reg.base == static_cast<X64GeneralRegister>(8U + dest_low) &&
            decoded.dest_reg.extended &&
            decoded.src_reg.base == static_cast<X64GeneralRegister>(8U + src_low) &&
            decoded.src_reg.extended;
    }
    check(reg_reg_store,
          "REX.W+R+B MOV r/m, r reverses R8-R15 destination and source selection");
}

void test_x64_interpreter_rex_extended_registers() {
    using namespace xbridge;
    using namespace xbridge::cpu;

    // MOV R15, imm64 with REX.W+B: 0x49 0xBF followed by the little-endian immediate.
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x3000;
        state.gpr(X64GeneralRegister::R8) = 0xDEADBEEFULL;
        const std::uint8_t bytes[] = {
            0x49, 0xBF, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(instr.status == X64DecodeStatus::Ok &&
              instr.dest_reg.base == X64GeneralRegister::R15 &&
              instr.dest_reg.extended &&
              exec_status == X64ExecuteStatus::Ok &&
              state.gpr(X64GeneralRegister::R15) == 0x1122334455667788ULL &&
              state.gpr(X64GeneralRegister::R8) == 0xDEADBEEFULL &&
              state.rip == 0x3000 + instr.length,
              "interpreter REX.W+B MOV imm64 writes R15 without touching R8");
    }

    // MOV R9, R14 with REX.W+R+B: mod=11, reg=001 -> R9, rm=110 -> R14.
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x4000;
        state.gpr(X64GeneralRegister::R14) = 0xA1B2C3D4E5F60718ULL;
        const std::uint8_t bytes[] = {0x4D, 0x8B, 0xCE};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(instr.status == X64DecodeStatus::Ok &&
              instr.dest_reg.base == X64GeneralRegister::R9 &&
              instr.src_reg.base == X64GeneralRegister::R14 &&
              exec_status == X64ExecuteStatus::Ok &&
              state.gpr(X64GeneralRegister::R9) == 0xA1B2C3D4E5F60718ULL &&
              state.gpr(X64GeneralRegister::R14) == 0xA1B2C3D4E5F60718ULL &&
              state.rip == 0x4000 + instr.length,
              "interpreter REX.W+R+B MOV r, r/m reads R14 into R9");
    }

    // MOV R14, R9 with REX.W+R+B: mod=11, rm=110 -> R14 destination, reg=001 -> R9 source.
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x5000;
        state.gpr(X64GeneralRegister::R9) = 0x8877665544332211ULL;
        const std::uint8_t bytes[] = {0x4D, 0x89, 0xCE};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(instr.status == X64DecodeStatus::Ok &&
              instr.dest_reg.base == X64GeneralRegister::R14 &&
              instr.src_reg.base == X64GeneralRegister::R9 &&
              exec_status == X64ExecuteStatus::Ok &&
              state.gpr(X64GeneralRegister::R14) == 0x8877665544332211ULL &&
              state.gpr(X64GeneralRegister::R9) == 0x8877665544332211ULL &&
              state.rip == 0x5000 + instr.length,
              "interpreter REX.W+R+B MOV r/m, r writes R14 from R9");
    }

    // MOV R8, imm32 with REX.B only: a 32-bit write zero-extends into R8.
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x6000;
        state.gpr(X64GeneralRegister::R8) = 0xFFFFFFFFFFFFFFFFULL;
        const std::uint8_t bytes[] = {0x41, 0xB8, 0x78, 0x56, 0x34, 0x12};
        const auto instr = decode_x64_instruction(bytes);
        const auto exec_status = execute_instruction(instr, state, memory);
        check(instr.status == X64DecodeStatus::Ok &&
              instr.operand_size == X64OperandSize::Bits32 &&
              instr.dest_reg.base == X64GeneralRegister::R8 &&
              exec_status == X64ExecuteStatus::Ok &&
              state.gpr(X64GeneralRegister::R8) == 0x0000000012345678ULL &&
              state.rip == 0x6000 + instr.length,
              "interpreter REX.B MOV imm32 zero-extends into R8");
    }
}

void test_x64_decoder_rex_x_recognized_unused() {
    using namespace xbridge::cpu;

    // REX.X is recorded but must not change decoding of a supported form.
    {
        const std::uint8_t without_x[] = {
            0x48, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
        const std::uint8_t with_x[] = {
            0x4A, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
        const auto plain = decode_x64_instruction(without_x);
        const auto rex_x = decode_x64_instruction(with_x);
        check(plain.status == X64DecodeStatus::Ok &&
              rex_x.status == X64DecodeStatus::Ok &&
              !plain.rex.x && rex_x.rex.x,
              "REX.X bit is recognized in the decoded prefix");
        check(plain.opcode == rex_x.opcode &&
              plain.length == rex_x.length &&
              plain.operand_size == rex_x.operand_size &&
              plain.dest_reg.base == rex_x.dest_reg.base &&
              plain.dest_reg.extended == rex_x.dest_reg.extended &&
              plain.src_reg.base == rex_x.src_reg.base &&
              plain.src_reg.extended == rex_x.src_reg.extended &&
              plain.memory_operand == rex_x.memory_operand &&
              plain.displacement == rex_x.displacement &&
              plain.immediate == rex_x.immediate &&
              plain.immediate_size == rex_x.immediate_size,
              "REX.X does not change MOV imm64 decoding");
    }

    // The same applies to a register-to-register form with REX.R.
    {
        const std::uint8_t without_x[] = {0x4C, 0x8B, 0xC0};
        const std::uint8_t with_x[] = {0x4E, 0x8B, 0xC0};
        const auto plain = decode_x64_instruction(without_x);
        const auto rex_x = decode_x64_instruction(with_x);
        check(plain.status == X64DecodeStatus::Ok &&
              rex_x.status == X64DecodeStatus::Ok &&
              rex_x.rex.x && !plain.rex.x,
              "REX.X bit is recognized on MOV reg/reg");
        check(plain.dest_reg.base == rex_x.dest_reg.base &&
              plain.dest_reg.extended == rex_x.dest_reg.extended &&
              plain.src_reg.base == rex_x.src_reg.base &&
              plain.src_reg.extended == rex_x.src_reg.extended &&
              !plain.memory_operand && !rex_x.memory_operand &&
              plain.length == rex_x.length,
              "REX.X does not change MOV reg/reg decoding");
    }
}

void test_x64_decoder_truncated_rex_modrm() {
    using namespace xbridge::cpu;

    // A REX prefix followed only by a ModR/M opcode is an incomplete instruction.
    const std::uint8_t rex_w_opcode_only[] = {0x48, 0x8B};
    check(decode_x64_instruction(rex_w_opcode_only).status == X64DecodeStatus::TruncatedInput,
          "REX.W + MOV r/m opcode without ModR/M: TruncatedInput");

    const std::uint8_t rex_b_opcode_only[] = {0x41, 0x89};
    check(decode_x64_instruction(rex_b_opcode_only).status == X64DecodeStatus::TruncatedInput,
          "REX.B + MOV r/m opcode without ModR/M: TruncatedInput");

    // imm32 truncation boundary: four of five bytes present.
    const std::uint8_t truncated_imm32[] = {0xB8, 0x78, 0x56, 0x34};
    check(decode_x64_instruction(truncated_imm32).status == X64DecodeStatus::TruncatedInput,
          "MOV imm32 missing the final byte: TruncatedInput");
}

void test_x64_interpreter_non_ok_decode_noop() {
    using namespace xbridge;
    using namespace xbridge::cpu;

    const auto check_noop = [](const std::vector<std::uint8_t>& bytes,
                               X64DecodeStatus expected_status,
                               const char* label) {
        const auto instr = decode_x64_instruction(
            std::span<const std::uint8_t>(bytes.data(), bytes.size()));
        check(instr.status == expected_status, label);

        memory::GuestMemory memory(64);
        std::vector<std::uint8_t> memory_before(memory.size());
        for (std::size_t address = 0; address < memory.size(); ++address) {
            const auto value = static_cast<std::uint8_t>(0xA5U + address);
            memory.write8(static_cast<std::uint32_t>(address), value);
            memory_before[address] = value;
        }

        X64CpuState state;
        state.rip = 0x2000;
        for (std::size_t index = 0; index < state.general_purpose_registers.size(); ++index) {
            state.general_purpose_registers[index] =
                0x1122334455667700ULL + static_cast<std::uint64_t>(index);
        }
        const auto registers_before = state.general_purpose_registers;
        const auto rip_before = state.rip;

        auto execution_status = X64ExecuteStatus::MemoryAccessError;
        check_does_not_throw(
            [&] { execution_status = execute_instruction(instr, state, memory); },
            "non-Ok decode executes without an exception");
        check(execution_status == X64ExecuteStatus::Ok,
              "non-Ok decode returns Ok from execute_instruction");
        check(state.rip == rip_before,
              "non-Ok decode leaves RIP unchanged");
        check(state.general_purpose_registers == registers_before,
              "non-Ok decode leaves every GPR unchanged");

        bool memory_unchanged = true;
        for (std::size_t address = 0; address < memory.size(); ++address) {
            memory_unchanged = memory_unchanged &&
                memory.read8(static_cast<std::uint32_t>(address)) == memory_before[address];
        }
        check(memory_unchanged,
              "non-Ok decode leaves GuestMemory unchanged");
    };

    check_noop(std::vector<std::uint8_t>{}, X64DecodeStatus::EmptyInput,
               "EmptyInput: decode is an execution no-op");
    check_noop({0x0F}, X64DecodeStatus::UnknownOpcode,
               "UnknownOpcode: decode is an execution no-op");
    check_noop({0x89, 0x04}, X64DecodeStatus::InvalidEncoding,
               "InvalidEncoding: decode is an execution no-op");
    check_noop({0x48, 0x8B}, X64DecodeStatus::TruncatedInput,
               "TruncatedInput: decode is an execution no-op");
}

void test_x64_step_instruction_facade() {
    using namespace xbridge;
    using namespace xbridge::cpu;

    // NOP {0x90}: Ok, RIP += 1
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x1000;
        const std::uint8_t bytes[] = {0x90};
        check(step_x64_instruction(bytes, state, memory) == X64ExecuteStatus::Ok,
              "step NOP: status Ok");
        check(state.rip == 0x1001, "step NOP: RIP advanced by 1");
    }

    // MOV EAX, imm32: Ok, RIP += 5, RAX = 0x12345678
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x1000;
        const std::uint8_t bytes[] = {0xB8, 0x78, 0x56, 0x34, 0x12};
        check(step_x64_instruction(bytes, state, memory) == X64ExecuteStatus::Ok,
              "step MOV EAX, imm32: status Ok");
        check(state.gpr(X64GeneralRegister::Rax) == 0x12345678ULL,
              "step MOV EAX, imm32: RAX = 0x12345678");
        check(state.rip == 0x1005, "step MOV EAX, imm32: RIP advanced by 5");
    }

    // MOV RAX, imm64 mit REX.W: Ok, RIP += 10, vollständiger 64-Bit-Wert
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x1000;
        const std::uint8_t bytes[] = {0x48, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
        check(step_x64_instruction(bytes, state, memory) == X64ExecuteStatus::Ok,
              "step MOV RAX, imm64: status Ok");
        check(state.gpr(X64GeneralRegister::Rax) == 0x1122334455667788ULL,
              "step MOV RAX, imm64: full 64-bit RAX value");
        check(state.rip == 0x100A, "step MOV RAX, imm64: RIP advanced by 10");
    }

    // MOV RAX, RCX (Register-zu-Register): 0x48 0x8B 0xC1
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x1000;
        state.gpr(X64GeneralRegister::Rcx) = 0xAABBCCDD11223344ULL;
        const std::uint8_t bytes[] = {0x48, 0x8B, 0xC1};
        check(step_x64_instruction(bytes, state, memory) == X64ExecuteStatus::Ok,
              "step MOV RAX, RCX: status Ok");
        check(state.gpr(X64GeneralRegister::Rax) == 0xAABBCCDD11223344ULL,
              "step MOV RAX, RCX: RAX copied from RCX");
        check(state.gpr(X64GeneralRegister::Rcx) == 0xAABBCCDD11223344ULL,
              "step MOV RAX, RCX: RCX unchanged");
        check(state.rip == 0x1003, "step MOV RAX, RCX: RIP advanced by 3");
    }

    // MOV [RAX], ECX / MOV ECX, [RAX] Round Trip
    {
        memory::GuestMemory memory(64);
        X64CpuState state;
        state.rip = 0x1000;
        state.gpr(X64GeneralRegister::Rax) = 0x10;
        state.gpr(X64GeneralRegister::Rcx) = 0xDEADBEEF;

        const std::uint8_t write_bytes[] = {0x89, 0x08};
        check(step_x64_instruction(write_bytes, state, memory) == X64ExecuteStatus::Ok,
              "step MOV [RAX], ECX: status Ok");
        check(memory.read32(16) == 0xDEADBEEF,
              "step MOV [RAX], ECX: memory at address 16 = 0xDEADBEEF");
        check(state.rip == 0x1002, "step MOV [RAX], ECX: RIP advanced by 2");

        state.gpr(X64GeneralRegister::Rcx) = 0;
        state.rip = 0x1000;
        const std::uint8_t read_bytes[] = {0x8B, 0x08};
        check(step_x64_instruction(read_bytes, state, memory) == X64ExecuteStatus::Ok,
              "step MOV ECX, [RAX]: status Ok");
        check(state.gpr(X64GeneralRegister::Rcx) == 0xDEADBEEF,
              "step MOV ECX, [RAX]: RCX = 0xDEADBEEF");
        check(state.rip == 0x1002, "step MOV ECX, [RAX]: RIP advanced by 2");
    }

    // Speicherfehler (4 Byte, 8 Byte, Adresse über UINT32_MAX):
    // MemoryAccessError ohne Änderung an CPU-Zustand oder GuestMemory.
    const auto check_memory_error = [](const std::vector<std::uint8_t>& bytes,
                                       std::size_t memory_size,
                                       std::uint64_t address,
                                       const char* label) {
        memory::GuestMemory memory(memory_size);
        std::vector<std::uint8_t> memory_before(memory.size());
        for (std::size_t addr = 0; addr < memory.size(); ++addr) {
            const auto value = static_cast<std::uint8_t>(0xA5U + addr);
            memory.write8(static_cast<std::uint32_t>(addr), value);
            memory_before[addr] = value;
        }

        X64CpuState state;
        state.rip = 0x1000;
        state.gpr(X64GeneralRegister::Rax) = address;
        state.gpr(X64GeneralRegister::Rcx) = 0x1122334455667788ULL;
        const auto registers_before = state.general_purpose_registers;
        const auto rip_before = state.rip;

        check(step_x64_instruction(
                  std::span<const std::uint8_t>(bytes.data(), bytes.size()),
                  state, memory) == X64ExecuteStatus::MemoryAccessError,
              (std::string(label) + ": status MemoryAccessError").c_str());
        check(state.general_purpose_registers == registers_before,
              (std::string(label) + ": GPRs unchanged").c_str());
        check(state.rip == rip_before,
              (std::string(label) + ": RIP unchanged").c_str());

        bool memory_unchanged = true;
        for (std::size_t addr = 0; addr < memory.size(); ++addr) {
            memory_unchanged = memory_unchanged &&
                memory.read8(static_cast<std::uint32_t>(addr)) == memory_before[addr];
        }
        check(memory_unchanged,
              (std::string(label) + ": GuestMemory unchanged").c_str());
    };

    // 4 Bytes ab Adresse 13 in 16 Bytes Speicher überschreitet das Ende.
    check_memory_error({0x89, 0x08}, 16, 0x0000000DULL,
                       "step MOV [RAX], ECX at 4-byte edge");
    // 8 Bytes ab Adresse 9 in 16 Bytes Speicher überschreitet das Ende.
    check_memory_error({0x48, 0x89, 0x08}, 16, 0x00000009ULL,
                       "step MOV [RAX], RCX at 8-byte edge");
    // Adresse über UINT32_MAX darf nicht stillschweigend gekürzt werden.
    check_memory_error({0x89, 0x08}, 64, 0x100000000ULL,
                       "step MOV [RAX], ECX above 32-bit");

    // Nicht-Ok-Dekodierungen sind Ok-No-Ops ohne Zustandsänderung.
    const auto check_noop = [](const std::vector<std::uint8_t>& bytes,
                               const char* label) {
        check(decode_x64_instruction(
                  std::span<const std::uint8_t>(bytes.data(), bytes.size())).status !=
                  X64DecodeStatus::Ok,
              (std::string(label) + ": decode is non-Ok").c_str());

        memory::GuestMemory memory(64);
        std::vector<std::uint8_t> memory_before(memory.size());
        for (std::size_t addr = 0; addr < memory.size(); ++addr) {
            const auto value = static_cast<std::uint8_t>(0xA5U + addr);
            memory.write8(static_cast<std::uint32_t>(addr), value);
            memory_before[addr] = value;
        }

        X64CpuState state;
        state.rip = 0x2000;
        for (std::size_t index = 0; index < state.general_purpose_registers.size(); ++index) {
            state.general_purpose_registers[index] =
                0x1122334455667700ULL + static_cast<std::uint64_t>(index);
        }
        const auto registers_before = state.general_purpose_registers;
        const auto rip_before = state.rip;

        check(step_x64_instruction(
                  std::span<const std::uint8_t>(bytes.data(), bytes.size()),
                  state, memory) == X64ExecuteStatus::Ok,
              (std::string(label) + ": status Ok (no-op)").c_str());
        check(state.rip == rip_before,
              (std::string(label) + ": RIP unchanged").c_str());
        check(state.general_purpose_registers == registers_before,
              (std::string(label) + ": GPRs unchanged").c_str());

        bool memory_unchanged = true;
        for (std::size_t addr = 0; addr < memory.size(); ++addr) {
            memory_unchanged = memory_unchanged &&
                memory.read8(static_cast<std::uint32_t>(addr)) == memory_before[addr];
        }
        check(memory_unchanged,
              (std::string(label) + ": GuestMemory unchanged").c_str());
    };

    check_noop({}, "step EmptyInput");
    check_noop({0x0F}, "step UnknownOpcode");
    check_noop({0x89, 0x04}, "step InvalidEncoding");
    check_noop({0x48, 0x8B}, "step TruncatedInput");
}

void test_cpu_program_lifecycle() {
    using namespace xbridge;

    memory::GuestMemory memory(16);
    cpu::TestCpu test_cpu(memory);
    check(state_is_reset(test_cpu.state()), "TestCpu starts with zeroed registers, IP, and halt state");

    const std::vector<std::uint8_t> previous_program{
        0x01, 0x02, 0x2A, 0x00, 0x00, 0x00,
        0x01, 0x01, 0x63, 0x00, 0x00, 0x00,
        0xFF
    };
    test_cpu.load_program(previous_program);
    check(state_is_reset(test_cpu.state()), "load_program resets the initial CPU state");
    test_cpu.run();
    check(test_cpu.state().registers[1] == 99 &&
          test_cpu.state().registers[2] == 42 &&
          test_cpu.state().halted,
          "previous program establishes state for reload regression");

    const std::vector<std::uint8_t> shorter_program{0x01, 0x00, 0x05, 0x00, 0x00, 0x00};
    test_cpu.load_program(shorter_program);
    check(state_is_reset(test_cpu.state()),
          "reloading after Halt resets all registers, IP, and halt state");
    check(memory.read8(6) == 0x01,
          "load_program does not clear guest bytes beyond the new program");
    check_throws<std::out_of_range>([&] { test_cpu.run(); },
                                    "short program cannot execute stale trailing instructions");
    check(test_cpu.state().instruction_pointer == shorter_program.size() &&
          test_cpu.state().registers[0] == 5 &&
          test_cpu.state().registers[1] == 0 &&
          test_cpu.state().registers[2] == 0 &&
          !test_cpu.state().halted,
          "program-boundary failure preserves new program state without stale execution");

    const std::vector<std::uint8_t> empty_program;
    test_cpu.load_program(empty_program);
    check(state_is_reset(test_cpu.state()), "loading an empty program resets CPU state");
    check(memory.read8(0) == 0x01,
          "loading an empty program leaves prior GuestMemory bytes unchanged");
    check_throws<std::out_of_range>([&] { test_cpu.run(); },
                                    "running an empty program rejects fetch outside its extent");
    check(state_is_reset(test_cpu.state()),
          "empty-program fetch failure preserves reset state");

    memory::GuestMemory small_memory(1);
    cpu::TestCpu small_cpu(small_memory);
    small_cpu.load_program({0xFF});
    small_cpu.run();
    check(small_cpu.state().halted, "small memory instance runs a fitting program");
    check_throws<std::out_of_range>([&] { small_cpu.load_program({0xFF, 0xFF}); },
                                    "load_program rejects programs larger than GuestMemory");
    check(state_is_reset(small_cpu.state()),
          "failed program load leaves the CPU state reset");
    check(small_memory.read8(0) == 0xFF,
          "failed program load leaves GuestMemory unchanged");
    check_throws<std::out_of_range>([&] { small_cpu.run(); },
                                    "run after failed load does not execute the old program");
    small_cpu.load_program({0xFF});
    small_cpu.run();
    check(small_cpu.state().instruction_pointer == 1 && small_cpu.state().halted,
          "TestCpu loads and halts a valid program after a failed load");
}

void test_host_capability_provider() {
    using namespace xbridge;
    using namespace xbridge::host;

    // Synthetische Host-Fakten: kein Windows-, Vulkan- oder D3D12-Probing.
    HostCapabilities capable_host{};
    capable_host.logical_threads = 16;
    capable_host.available_ram_mb = 8192;
    capable_host.dedicated_vram_mb = 8192;
    capable_host.cpu_jit_available = true;
    capable_host.vulkan_available = true;
    capable_host.direct3d12_available = true;

    HostCapabilities limited_host{};
    limited_host.software_renderer_available = true;

    const FixedCapabilityProvider capable_provider(capable_host);
    const FixedCapabilityProvider limited_provider(limited_host);

    // Der Provider hält eine eigene, unveränderliche Kopie der Fakten.
    const auto& first = capable_provider.capabilities();
    const auto& second = capable_provider.capabilities();
    check(&first == &second,
          "provider returns the same stored HostCapabilities object");
    check(first.logical_threads == capable_host.logical_threads &&
          first.available_ram_mb == capable_host.available_ram_mb &&
          first.dedicated_vram_mb == capable_host.dedicated_vram_mb &&
          first.cpu_jit_available == capable_host.cpu_jit_available &&
          first.vulkan_available == capable_host.vulkan_available &&
          first.direct3d12_available == capable_host.direct3d12_available &&
          first.software_renderer_available == capable_host.software_renderer_available &&
          first.gpu_name == capable_host.gpu_name &&
          first.driver_version == capable_host.driver_version,
          "provider exposes exactly the supplied synthetic facts");

    auto mutable_facts = capable_host;
    const FixedCapabilityProvider copied_provider(mutable_facts);
    mutable_facts.vulkan_available = false;
    mutable_facts.gpu_name = "changed after construction";
    check(copied_provider.capabilities().vulkan_available == capable_host.vulkan_available &&
          copied_provider.capabilities().gpu_name == capable_host.gpu_name,
          "provider reads its stored copy, not external mutable state");

    const ICapabilityProvider& seam = capable_provider;
    check(&seam.capabilities() == &first,
          "ICapabilityProvider seam exposes the same stored facts");

    const auto plans_match = [](const ExecutionPlan& direct, const ExecutionPlan& provided) {
        return direct.graphics == provided.graphics &&
               direct.cpu == provided.cpu &&
               direct.profile == provided.profile &&
               direct.graphics_accelerated == provided.graphics_accelerated &&
               direct.fallback_used == provided.fallback_used &&
               direct.report == provided.report;
    };

    const auto check_provider = [&](const ICapabilityProvider& provider,
                                    const ExecutionSettings& settings,
                                    GraphicsBackend expected_graphics,
                                    CpuBackend expected_cpu,
                                    const char* label) {
        const auto settings_before = settings;
        const auto direct = select_execution_plan(provider.capabilities(), settings);
        const auto provided = select_execution_plan(provider, settings);
        const auto repeated = select_execution_plan(provider, settings);

        check(direct.graphics == expected_graphics && provided.graphics == expected_graphics,
              (std::string(label) + ": expected graphics backend").c_str());
        check(direct.cpu == expected_cpu && provided.cpu == expected_cpu,
              (std::string(label) + ": expected CPU backend").c_str());
        check(plans_match(direct, provided),
              (std::string(label) + ": plan and report match direct overload").c_str());
        check(plans_match(provided, repeated),
              (std::string(label) + ": repeated provider call is identical").c_str());
        check(settings.graphics == settings_before.graphics &&
              settings.cpu == settings_before.cpu &&
              settings.profile == settings_before.profile &&
              settings.allow_software_fallback == settings_before.allow_software_fallback,
              (std::string(label) + ": ExecutionSettings unchanged").c_str());
    };

    check_provider(capable_provider, {}, GraphicsBackend::Vulkan, CpuBackend::Interpreter,
                   "provider automatic Vulkan");
    check_provider(limited_provider, {}, GraphicsBackend::Software, CpuBackend::Interpreter,
                   "provider software fallback");

    ExecutionSettings no_fallback{};
    no_fallback.allow_software_fallback = false;
    check_provider(limited_provider, no_fallback, GraphicsBackend::Unavailable,
                   CpuBackend::Interpreter, "provider fallback disabled");

    ExecutionSettings manual{};
    manual.graphics = GraphicsPreference::Direct3D12;
    manual.cpu = CpuPreference::Interpreter;
    check_provider(capable_provider, manual, GraphicsBackend::Direct3D12,
                   CpuBackend::Interpreter, "provider manual Direct3D12");

    ExecutionSettings jit_requested{};
    jit_requested.cpu = CpuPreference::Jit;
    check_provider(capable_provider, jit_requested, GraphicsBackend::Vulkan,
                   CpuBackend::Jit, "provider JIT available");
    check_provider(limited_provider, jit_requested, GraphicsBackend::Software,
                   CpuBackend::Interpreter, "provider JIT unavailable");

    const auto jit_unavailable_plan = select_execution_plan(limited_provider, jit_requested);
    check(jit_unavailable_plan.report.find("JIT requested but unavailable") != std::string::npos,
          "provider report explains unavailable requested JIT");
}

void test_x64_step_at_rip_bridge() {
    using namespace xbridge;
    using namespace xbridge::cpu;

    const auto fill_memory = [](memory::GuestMemory& memory) {
        std::vector<std::uint8_t> before(memory.size());
        for (std::size_t addr = 0; addr < memory.size(); ++addr) {
            const auto value = static_cast<std::uint8_t>(0xA5U + addr);
            memory.write8(static_cast<std::uint32_t>(addr), value);
            before[addr] = value;
        }
        return before;
    };

    const auto make_state = []() {
        X64CpuState state;
        state.rip = 0x1000;
        state.rflags = 0x0000000000000A95ULL;
        for (std::size_t index = 0; index < state.general_purpose_registers.size(); ++index) {
            state.general_purpose_registers[index] =
                0x1122334455667700ULL + static_cast<std::uint64_t>(index);
        }
        return state;
    };

    const auto verify_unchanged = [](const X64CpuState& before, const X64CpuState& after,
                                     const std::vector<std::uint8_t>& memory_before,
                                     const memory::GuestMemory& memory,
                                     const char* label) {
        check(after.rip == before.rip,
              (std::string(label) + ": RIP unchanged").c_str());
        check(after.rflags == before.rflags,
              (std::string(label) + ": RFLAGS unchanged").c_str());
        check(after.general_purpose_registers == before.general_purpose_registers,
              (std::string(label) + ": GPRs unchanged").c_str());
        bool memory_unchanged = true;
        for (std::size_t addr = 0; addr < memory.size(); ++addr) {
            memory_unchanged = memory_unchanged &&
                memory.read8(static_cast<std::uint32_t>(addr)) == memory_before[addr];
        }
        check(memory_unchanged,
              (std::string(label) + ": GuestMemory unchanged").c_str());
    };

    // {NOP, MOV imm32, NOP} ab 0x100: drei aufeinanderfolgende Schritte.
    {
        memory::GuestMemory memory(512);
        memory.load(0x100, {0x90, 0xB8, 0x78, 0x56, 0x34, 0x12, 0x90});
        X64CpuState state;
        state.rip = 0x100;

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::Ok,
              "step_x64_at_rip NOP: status Ok");
        check(state.rip == 0x101 && state.gpr(X64GeneralRegister::Rax) == 0,
              "step_x64_at_rip first step: RIP 0x101, RAX unchanged");

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::Ok,
              "step_x64_at_rip MOV imm32: status Ok");
        check(state.rip == 0x106 && state.gpr(X64GeneralRegister::Rax) == 0x12345678ULL,
              "step_x64_at_rip second step: RIP 0x106, RAX = 0x12345678");

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::Ok,
              "step_x64_at_rip final NOP: status Ok");
        check(state.rip == 0x107 && state.gpr(X64GeneralRegister::Rax) == 0x12345678ULL,
              "step_x64_at_rip third step: RIP 0x107, RAX preserved");
    }

    // MOV RAX, imm64 mit REX.W über den Fetch (10 Bytes).
    {
        memory::GuestMemory memory(256);
        memory.load(0x80, {0x48, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11});
        X64CpuState state;
        state.rip = 0x80;

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::Ok,
              "step_x64_at_rip MOV imm64: status Ok");
        check(state.gpr(X64GeneralRegister::Rax) == 0x1122334455667788ULL,
              "step_x64_at_rip MOV imm64: full 64-bit RAX");
        check(state.rip == 0x8A, "step_x64_at_rip MOV imm64: RIP advanced by 10");
    }

    // MOV [RAX], ECX / MOV ECX, [RAX] über den Fetch (Adresse 0x30).
    {
        memory::GuestMemory memory(512);
        memory.load(0x100, {0x89, 0x08, 0x8B, 0x08});
        X64CpuState state;
        state.rip = 0x100;
        state.gpr(X64GeneralRegister::Rax) = 0x30;
        state.gpr(X64GeneralRegister::Rcx) = 0xDEADBEEF;

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::Ok,
              "step_x64_at_rip MOV [RAX], ECX: status Ok");
        check(memory.read32(0x30) == 0xDEADBEEF && state.rip == 0x102,
              "step_x64_at_rip MOV [RAX], ECX: memory written, RIP advanced by 2");

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::Ok,
              "step_x64_at_rip MOV ECX, [RAX]: status Ok");
        check(state.gpr(X64GeneralRegister::Rcx) == 0xDEADBEEF && state.rip == 0x104,
              "step_x64_at_rip MOV ECX, [RAX]: RCX loaded, RIP advanced by 2");
    }

    // NOP am letzten Byte: Ok, RIP = memory.size(); nächster Schritt ohne Mutation.
    {
        memory::GuestMemory memory(16);
        auto memory_before = fill_memory(memory);
        memory.write8(15, 0x90);
        memory_before[15] = 0x90;
        auto state = make_state();
        state.rip = 15;

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::Ok,
              "step_x64_at_rip NOP at last byte: status Ok");
        check(state.rip == memory.size(),
              "step_x64_at_rip NOP at last byte: RIP advanced to memory.size()");

        const auto state_before = state;
        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::MemoryAccessError,
              "step_x64_at_rip after last byte: status MemoryAccessError");
        verify_unchanged(state_before, state, memory_before, memory,
                         "step_x64_at_rip after last byte");
    }

    // Ungültige RIP-Werte: MemoryAccessError ohne jede Zustands-/Speicheränderung.
    const auto check_error_no_mutation = [&](std::uint64_t rip, std::size_t memory_size,
                                            const char* label) {
        memory::GuestMemory memory(memory_size);
        const auto memory_before = fill_memory(memory);
        auto state = make_state();
        state.rip = rip;
        const auto state_before = state;

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::MemoryAccessError,
              (std::string(label) + ": status MemoryAccessError").c_str());
        verify_unchanged(state_before, state, memory_before, memory, label);
    };

    check_error_no_mutation(0x100000000ULL, 64, "step_x64_at_rip above UINT32_MAX");
    check_error_no_mutation(64, 64, "step_x64_at_rip at memory.size()");

    // Non-Ok-Decode (0x0F): Ok-No-op mit unverändertem RIP und Zustand.
    {
        memory::GuestMemory memory(64);
        auto memory_before = fill_memory(memory);
        memory.write8(8, 0x0F);
        memory_before[8] = 0x0F;
        auto state = make_state();
        state.rip = 8;
        const auto state_before = state;

        check(step_x64_at_rip(state, memory) == X64ExecuteStatus::Ok,
              "step_x64_at_rip non-Ok decode: status Ok");
        verify_unchanged(state_before, state, memory_before, memory,
                         "step_x64_at_rip non-Ok decode");
    }
}

bool bytes_equal(std::span<const std::uint8_t> lhs, std::span<const std::uint8_t> rhs) noexcept {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (std::size_t index = 0; index < lhs.size(); ++index) {
        if (lhs[index] != rhs[index]) {
            return false;
        }
    }
    return true;
}

memory::MemoryResult make_memory_result(bool ok,
                                         memory::MemoryFault fault,
                                         memory::MemoryAccess access,
                                         memory::MemoryAccessContext context,
                                         std::uint64_t address,
                                         std::uint64_t requested_length,
                                         std::uint64_t completed_bytes) noexcept {
    memory::MemoryResult result;
    result.ok = ok;
    result.fault = fault;
    result.access = access;
    result.context = context;
    result.address = address;
    result.requested_length = requested_length;
    result.completed_bytes = completed_bytes;
    result.translated_address.reset();
    result.region_base.reset();
    return result;
}

bool result_invariant(const memory::MemoryResult& result) noexcept {
    return result.ok == (result.fault == memory::MemoryFault::None) &&
           result.completed_bytes <= result.requested_length &&
           static_cast<bool>(result) == result.ok;
}

bool result_matches(const memory::MemoryResult& result,
                    memory::MemoryFault fault,
                    std::uint64_t address,
                    std::uint64_t requested_length) noexcept {
    return result_invariant(result) && result.fault == fault &&
           result.address == address && result.requested_length == requested_length;
}

bool data_result_matches(const memory::MemoryResult& result,
                         memory::MemoryFault fault,
                         std::uint64_t address,
                         std::uint64_t requested_length,
                         std::uint64_t completed_bytes) noexcept {
    return result_matches(result, fault, address, requested_length) &&
           result.completed_bytes == completed_bytes;
}

bool optional_equals(const std::optional<std::uint64_t>& value,
                     std::uint64_t expected) noexcept {
    return value.has_value() && *value == expected;
}

template <typename Result>
void check_named_result_helpers() {
    const auto access = memory::MemoryAccess::Execute;
    const auto context = memory::MemoryAccessContext::Fetch;
    constexpr std::uint64_t address = 0x1'0000'0000ULL;
    constexpr std::uint64_t length = 4;
    constexpr std::uint64_t completed = 4;

    const auto verify_success = [&](const Result& result,
                                    std::uint64_t expected_completed) {
        check(result_invariant(result) &&
              result.fault == memory::MemoryFault::None &&
              result.access == access &&
              result.context == context &&
              result.address == address &&
              result.requested_length == length &&
              result.completed_bytes == expected_completed &&
              !result.translated_address.has_value() &&
              !result.region_base.has_value(),
              "declared MemoryResult success helper initializes all diagnostic fields");
    };

    if constexpr (requires { Result::success(access, context, address, length, completed); }) {
        verify_success(Result::success(access, context, address, length, completed), completed);
    } else if constexpr (requires { Result::success(access, context, address, length); }) {
        verify_success(Result::success(access, context, address, length), length);
    } else if constexpr (requires { Result::make_success(access, context, address, length, completed); }) {
        verify_success(Result::make_success(access, context, address, length, completed), completed);
    } else if constexpr (requires { Result::make_success(access, context, address, length); }) {
        verify_success(Result::make_success(access, context, address, length), length);
    } else {
        check(false, "MemoryResult exposes a static success helper with the declared diagnostic fields");
    }

    const auto verify_failure = [&](const Result& result) {
        check(result_invariant(result) &&
              result.fault == memory::MemoryFault::Unmapped &&
              result.access == access &&
              result.context == context &&
              result.address == address &&
              result.requested_length == length &&
              result.completed_bytes == 0 &&
              !result.translated_address.has_value() &&
              !result.region_base.has_value(),
              "declared MemoryResult failure helper initializes all diagnostic fields");
    };

    if constexpr (requires {
                      Result::failure(memory::MemoryFault::Unmapped, access, context,
                                      address, length);
                  }) {
        verify_failure(Result::failure(memory::MemoryFault::Unmapped, access, context,
                                       address, length));
    } else if constexpr (requires {
                             Result::failure(memory::MemoryFault::Unmapped, access, context,
                                             address);
                         }) {
        verify_failure(Result::failure(memory::MemoryFault::Unmapped, access, context,
                                       address));
    } else if constexpr (requires {
                             Result::make_failure(memory::MemoryFault::Unmapped, access,
                                                  context, address, length);
                         }) {
        verify_failure(Result::make_failure(memory::MemoryFault::Unmapped, access,
                                            context, address, length));
    } else if constexpr (requires {
                             Result::make_failure(memory::MemoryFault::Unmapped, access,
                                                  context, address);
                         }) {
        verify_failure(Result::make_failure(memory::MemoryFault::Unmapped, access,
                                            context, address));
    } else {
        check(false, "MemoryResult exposes a static failure helper with the declared diagnostic fields");
    }

    if constexpr (requires { Result::success(access, context, address, length, 8U); }) {
        verify_success(Result::success(access, context, address, length, 8U), length);
    } else {
        check(false, "MemoryResult exposes a success helper that clamps completed bytes to the requested length");
    }

    if constexpr (requires {
                      Result::failure(memory::MemoryFault::None, access, context,
                                      address, length);
                  }) {
        const auto normalized_failure =
            Result::failure(memory::MemoryFault::None, access, context, address, length);
        check(result_invariant(normalized_failure) &&
              !normalized_failure.ok &&
              normalized_failure.fault == memory::MemoryFault::Unsupported &&
              normalized_failure.access == access &&
              normalized_failure.context == context &&
              normalized_failure.address == address &&
              normalized_failure.requested_length == length &&
              normalized_failure.completed_bytes == 0 &&
              !normalized_failure.translated_address.has_value() &&
              !normalized_failure.region_base.has_value(),
              "declared MemoryResult failure helper maps MemoryFault::None to Unsupported");
    } else {
        check(false, "MemoryResult exposes a failure helper accepting MemoryFault::None");
    }
}

class RecordingDeviceDispatcher final : public memory::IDeviceDispatcher {
public:
    bool fail{false};
    std::uint8_t read_pattern{0xC3};
    std::size_t calls{0};
    memory::GuestPhysicalAddress last_region_base{};
    std::uint64_t last_offset{0};
    memory::MemoryAccess last_access{memory::MemoryAccess::Read};
    memory::MemoryAccessContext last_context{memory::MemoryAccessContext::Data};
    std::array<std::uint8_t, 64> last_data{};
    std::size_t last_data_size{0};

    memory::MemoryResult dispatch(const memory::DeviceAccessRequest& request) noexcept override {
        ++calls;
        last_region_base = request.region_base;
        last_offset = request.offset;
        last_access = request.access;
        last_context = request.context;
        last_data_size = request.data.size() < last_data.size()
                             ? request.data.size()
                             : last_data.size();
        for (std::size_t index = 0; index < last_data_size; ++index) {
            last_data[index] = request.data[index];
        }

        const std::uint64_t address = request.region_base.value + request.offset;
        if (fail) {
            return make_memory_result(false, memory::MemoryFault::Unsupported,
                                      request.access, request.context, address,
                                      request.data.size(), 0);
        }
        if (request.access == memory::MemoryAccess::Read) {
            for (std::uint8_t& byte : request.data) {
                byte = read_pattern;
            }
        }
        return make_memory_result(true, memory::MemoryFault::None,
                                  request.access, request.context, address,
                                  request.data.size(), request.data.size());
    }
};

void test_guest_address_helpers() {
    using namespace xbridge::memory;

    static_assert(!std::is_same_v<GuestVirtualAddress, GuestPhysicalAddress>);
    static_assert(!std::is_convertible_v<GuestVirtualAddress, GuestPhysicalAddress>);
    static_assert(!std::is_convertible_v<GuestPhysicalAddress, GuestVirtualAddress>);
    static_assert(!std::is_convertible_v<std::uint64_t, GuestVirtualAddress>);
    static_assert(!std::is_convertible_v<std::uint64_t, GuestPhysicalAddress>);
    static_assert(!std::is_convertible_v<GuestVirtualAddress, std::uint64_t>);
    static_assert(!std::is_convertible_v<GuestPhysicalAddress, std::uint64_t>);

    constexpr GuestVirtualAddress virtual_a{0x1122334455667788ULL};
    constexpr GuestVirtualAddress virtual_copy{0x1122334455667788ULL};
    constexpr GuestVirtualAddress virtual_b{0x8877665544332211ULL};
    constexpr GuestPhysicalAddress physical_a{0x1'0000'0000ULL};
    constexpr GuestPhysicalAddress physical_copy{0x1'0000'0000ULL};
    constexpr GuestPhysicalAddress physical_b{0x2'0000'0000ULL};

    check(virtual_a == virtual_copy && virtual_a != virtual_b &&
              !(virtual_a == virtual_b) && virtual_b == GuestVirtualAddress{0x8877665544332211ULL},
          "guest virtual addresses are strong values with default equality");
    check(physical_a == physical_copy && physical_a != physical_b &&
              !(physical_a == physical_b) && physical_b == GuestPhysicalAddress{0x2'0000'0000ULL},
          "guest physical addresses are strong values with default equality");

    constexpr std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max();
    std::uint64_t end = 0xDEADBEEFULL;
    check(checked_range_end(0x1000ULL, 0x20ULL, end) && end == 0x1020ULL,
          "checked_range_end accepts an in-range 64-bit half-open interval");
    end = 0xDEADBEEFULL;
    check(checked_range_end(maximum, 0, end) && end == maximum,
          "checked_range_end accepts a zero-length range at UINT64_MAX");
    end = 0xDEADBEEFULL;
    check(checked_range_end(0, maximum, end) && end == maximum,
          "checked_range_end accepts the full non-overflowing range");
    end = 0xDEADBEEFULL;
    check(!checked_range_end(maximum, 1, end) && end == 0xDEADBEEFULL,
          "checked_range_end rejects UINT64_MAX plus one without changing end");
    end = 0xDEADBEEFULL;
    check(!checked_range_end(maximum - 1ULL, 2, end) && end == 0xDEADBEEFULL,
          "checked_range_end rejects a range crossing UINT64_MAX");
    end = 0xDEADBEEFULL;
    check(!checked_range_end(1, maximum, end) && end == 0xDEADBEEFULL,
          "checked_range_end rejects an overflowing length from a low base");
}

void test_memory_result_contract() {
    using namespace xbridge::memory;

    const MemoryResult default_result{};
    check(result_invariant(default_result) &&
              !default_result.ok &&
              !static_cast<bool>(default_result) &&
              default_result.fault == MemoryFault::Unsupported &&
              default_result.access == MemoryAccess::Read &&
              default_result.context == MemoryAccessContext::Data &&
              default_result.address == 0 &&
              default_result.requested_length == 0 &&
              default_result.completed_bytes == 0 &&
              !default_result.translated_address.has_value() &&
              !default_result.region_base.has_value(),
          "MemoryResult defaults satisfy the ok/fault invariant and leave diagnostics safe");

    check_named_result_helpers<MemoryResult>();
}

void test_guest_physical_memory_regions() {
    using namespace xbridge::memory;

    const MemoryPermissions all{true, true, true};
    constexpr std::uint64_t base = 0x1'0000'0000ULL;
    GuestPhysicalMemory physical;

    const auto first = physical.add_region(GuestPhysicalAddress{base}, 64,
                                           RegionKind::Normal, all);
    check(result_matches(first, MemoryFault::None, base, 64) &&
              result_invariant(first),
          "GuestPhysicalMemory adds a normal region above 4 GiB");

    const auto zero = physical.add_region(GuestPhysicalAddress{base + 128}, 0,
                                          RegionKind::Normal, all);
    check(!zero.ok && zero.fault == MemoryFault::InvalidRange &&
              zero.completed_bytes == 0 && result_invariant(zero) &&
              physical.region_at(GuestPhysicalAddress{base + 128}).length == 0,
          "GuestPhysicalMemory rejects a zero-length region atomically");

    constexpr std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max();
    const auto overflow = physical.add_region(GuestPhysicalAddress{maximum - 3ULL}, 8,
                                              RegionKind::Normal, all);
    check(!overflow.ok && overflow.fault == MemoryFault::AddressOverflow &&
              overflow.completed_bytes == 0 && result_invariant(overflow) &&
              !physical.region_covers(GuestPhysicalAddress{maximum - 3ULL}, 1),
          "GuestPhysicalMemory rejects a region whose end would overflow");

    const auto overlap = physical.add_region(GuestPhysicalAddress{base + 16}, 16,
                                             RegionKind::Normal, all);
    check(!overlap.ok && overlap.fault == MemoryFault::InvalidRange &&
              overlap.completed_bytes == 0 && result_invariant(overlap),
          "GuestPhysicalMemory rejects an overlapping region");

    const auto first_descriptor = physical.region_at(GuestPhysicalAddress{base});
    const auto first_tail_descriptor = physical.region_at(GuestPhysicalAddress{base + 63});
    check(first_descriptor.base.value == base && first_descriptor.length == 64 &&
              first_descriptor.kind == RegionKind::Normal &&
              first_descriptor.permissions.read && first_descriptor.permissions.write &&
              first_descriptor.permissions.execute &&
              first_tail_descriptor.base.value == base &&
              first_tail_descriptor.length == 64,
          "region_at reports the covering region descriptor inside its full range");
    check(physical.region_covers(GuestPhysicalAddress{base}, 64) &&
              physical.region_covers(GuestPhysicalAddress{base + 63}, 1) &&
              !physical.region_covers(GuestPhysicalAddress{base}, 65) &&
              !physical.region_covers(GuestPhysicalAddress{base + 63}, 2),
          "region_covers enforces exact 64-bit half-open bounds");

    const auto touching = physical.add_region(GuestPhysicalAddress{base + 64}, 16,
                                              RegionKind::Normal, all);
    check(touching.ok && touching.fault == MemoryFault::None &&
              physical.region_at(GuestPhysicalAddress{base + 64}).base.value == base + 64 &&
              physical.region_at(GuestPhysicalAddress{base + 64}).length == 16,
          "GuestPhysicalMemory allows touching regions without overlap");

    const auto unmapped = physical.region_at(GuestPhysicalAddress{base + 80});
    check(unmapped.length == 0 && unmapped.kind == RegionKind::Reserved &&
              !physical.region_covers(GuestPhysicalAddress{base + 80}, 1),
          "region_at returns a Reserved zero-length descriptor for unmapped addresses");

    constexpr std::uint64_t device_base = 0x2'0000'0000ULL;
    constexpr std::uint64_t reserved_base = 0x3'0000'0000ULL;
    const MemoryPermissions device_permissions{true, true, false};
    check(physical.add_region(GuestPhysicalAddress{device_base}, 16,
                              RegionKind::Device, device_permissions).ok &&
              physical.add_region(GuestPhysicalAddress{reserved_base}, 16,
                                  RegionKind::Reserved, all).ok,
          "GuestPhysicalMemory records Device and Reserved regions");
    const auto device_descriptor = physical.region_at(GuestPhysicalAddress{device_base});
    const auto reserved_descriptor = physical.region_at(GuestPhysicalAddress{reserved_base});
    check(device_descriptor.kind == RegionKind::Device &&
              device_descriptor.base.value == device_base &&
              device_descriptor.length == 16 &&
              device_descriptor.permissions.read &&
              device_descriptor.permissions.write &&
              !device_descriptor.permissions.execute &&
              reserved_descriptor.kind == RegionKind::Reserved &&
              reserved_descriptor.base.value == reserved_base &&
              reserved_descriptor.length == 16,
          "region_at preserves Device and Reserved kinds and permissions");
}

void test_guest_physical_memory_accesses() {
    using namespace xbridge::memory;

    constexpr std::uint64_t base = 0x1'0000'0000ULL;
    constexpr std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max();
    GuestPhysicalMemory physical;
    check(physical.add_region(GuestPhysicalAddress{base}, 16, RegionKind::Normal,
                              MemoryPermissions{true, true, false}).ok,
          "physical access fixture adds a writable region above 4 GiB");

    std::array<std::uint8_t, 4> source{0x11, 0x22, 0x33, 0x44};
    const auto write = physical.write(GuestPhysicalAddress{base + 4},
                                      std::span<const std::uint8_t>(source),
                                      MemoryAccessContext::Data);
    check(data_result_matches(write, MemoryFault::None, base + 4, 4, 4) &&
              write.access == MemoryAccess::Write &&
              write.context == MemoryAccessContext::Data,
          "physical write succeeds with exact completed-byte count");

    std::array<std::uint8_t, 4> destination{};
    const auto read = physical.read(GuestPhysicalAddress{base + 4},
                                    std::span<std::uint8_t>(destination),
                                    MemoryAccessContext::Data);
    check(data_result_matches(read, MemoryFault::None, base + 4, 4, 4) &&
              read.access == MemoryAccess::Read &&
              read.context == MemoryAccessContext::Data &&
              bytes_equal(destination, source),
          "physical read returns the bytes written above 4 GiB");

    std::array<std::uint8_t, 4> tail_source{0xAA, 0xBB, 0xCC, 0xDD};
    const auto tail_write = physical.write(GuestPhysicalAddress{base + 12},
                                           std::span<const std::uint8_t>(tail_source),
                                           MemoryAccessContext::Bulk);
    std::array<std::uint8_t, 4> tail_destination{};
    const auto tail_read = physical.read(GuestPhysicalAddress{base + 12},
                                         std::span<std::uint8_t>(tail_destination),
                                         MemoryAccessContext::Bulk);
    check(data_result_matches(tail_write, MemoryFault::None, base + 12, 4, 4) &&
              data_result_matches(tail_read, MemoryFault::None, base + 12, 4, 4) &&
              tail_write.context == MemoryAccessContext::Bulk &&
              tail_read.context == MemoryAccessContext::Bulk &&
              bytes_equal(tail_destination, tail_source),
          "physical accesses ending exactly at the region boundary succeed");

    const auto source_before_failure = source;
    const auto failed_write = physical.write(GuestPhysicalAddress{base + 13},
                                             std::span<const std::uint8_t>(source),
                                             MemoryAccessContext::Data);
    check(!failed_write.ok && failed_write.fault == MemoryFault::InvalidRange &&
              failed_write.completed_bytes == 0 && result_invariant(failed_write) &&
              source == source_before_failure,
          "physical write crossing the region writes zero bytes and leaves source unchanged");

    const auto destination_before_failure = destination;
    const auto failed_read = physical.read(GuestPhysicalAddress{base + 13},
                                           std::span<std::uint8_t>(destination),
                                           MemoryAccessContext::Data);
    check(!failed_read.ok && failed_read.fault == MemoryFault::InvalidRange &&
              failed_read.completed_bytes == 0 && result_invariant(failed_read) &&
              destination == destination_before_failure,
          "physical read crossing the region leaves the destination unchanged");

    constexpr std::uint64_t readonly_base = 0x2'0000'0000ULL;
    constexpr std::uint64_t no_read_base = 0x3'0000'0000ULL;
    constexpr std::uint64_t execute_only_base = 0x4'0000'0000ULL;
    check(physical.add_region(GuestPhysicalAddress{readonly_base}, 8, RegionKind::Normal,
                              MemoryPermissions{true, false, false}).ok &&
              physical.add_region(GuestPhysicalAddress{no_read_base}, 8, RegionKind::Normal,
                                  MemoryPermissions{false, true, false}).ok &&
              physical.add_region(GuestPhysicalAddress{execute_only_base}, 8,
                                  RegionKind::Normal,
                                  MemoryPermissions{false, false, true}).ok,
          "physical permission fixture adds read-only, write-only and execute-only regions");

    std::array<std::uint8_t, 2> readonly_source{0x01, 0x02};
    const auto readonly_source_before = readonly_source;
    const auto denied_write = physical.write(GuestPhysicalAddress{readonly_base},
                                             std::span<const std::uint8_t>(readonly_source),
                                             MemoryAccessContext::Data);
    check(!denied_write.ok && denied_write.fault == MemoryFault::AccessDenied &&
              denied_write.completed_bytes == 0 && result_invariant(denied_write) &&
              readonly_source == readonly_source_before,
          "physical write respects region write permission");

    std::array<std::uint8_t, 2> no_read_destination{0x5A, 0x5A};
    const auto no_read_before = no_read_destination;
    const auto denied_read = physical.read(GuestPhysicalAddress{no_read_base},
                                           std::span<std::uint8_t>(no_read_destination),
                                           MemoryAccessContext::Data);
    check(!denied_read.ok && denied_read.fault == MemoryFault::AccessDenied &&
              denied_read.completed_bytes == 0 && result_invariant(denied_read) &&
              no_read_destination == no_read_before,
          "physical read respects region read permission");

    std::array<std::uint8_t, 2> execute_only_destination{0x11, 0x22};
    const auto execute_only_before = execute_only_destination;
    const auto execute_only_read = physical.read(GuestPhysicalAddress{execute_only_base},
                                                 std::span<std::uint8_t>(execute_only_destination),
                                                 MemoryAccessContext::Data);
    check(!execute_only_read.ok &&
              execute_only_read.fault == MemoryFault::AccessDenied &&
              execute_only_read.completed_bytes == 0 &&
              execute_only_destination == execute_only_before,
          "physical data read does not inherit execute permission");

    constexpr std::uint64_t unmapped_base = 0x9'0000'0000ULL;
    std::array<std::uint8_t, 2> unmapped_destination{0x77, 0x88};
    const auto unmapped_before = unmapped_destination;
    const auto unmapped_read = physical.read(GuestPhysicalAddress{unmapped_base},
                                             std::span<std::uint8_t>(unmapped_destination),
                                             MemoryAccessContext::Data);
    check(!unmapped_read.ok && unmapped_read.fault == MemoryFault::Unmapped &&
              unmapped_read.completed_bytes == 0 && result_invariant(unmapped_read) &&
              unmapped_destination == unmapped_before,
          "physical access to an unmapped region reports Unmapped without mutation");

    const auto zero_read = physical.read(GuestPhysicalAddress{unmapped_base},
                                         std::span<std::uint8_t>{},
                                         MemoryAccessContext::Data);
    const auto zero_write = physical.write(GuestPhysicalAddress{maximum},
                                           std::span<const std::uint8_t>{},
                                           MemoryAccessContext::Bulk);
    check(data_result_matches(zero_read, MemoryFault::None, unmapped_base, 0, 0) &&
              zero_read.access == MemoryAccess::Read &&
              zero_read.context == MemoryAccessContext::Data &&
              data_result_matches(zero_write, MemoryFault::None, maximum, 0, 0) &&
              zero_write.access == MemoryAccess::Write &&
              zero_write.context == MemoryAccessContext::Bulk,
          "zero-length physical data accesses succeed without translation or mutation");

    constexpr std::uint64_t device_base = 0xA'0000'0000ULL;
    constexpr std::uint64_t reserved_base = 0xB'0000'0000ULL;
    check(physical.add_region(GuestPhysicalAddress{device_base}, 16, RegionKind::Device,
                              MemoryPermissions{true, true, true}).ok &&
              physical.add_region(GuestPhysicalAddress{reserved_base}, 16,
                                  RegionKind::Reserved,
                                  MemoryPermissions{true, true, true}).ok,
          "physical access fixture adds Device and Reserved regions");

    std::array<std::uint8_t, 2> device_destination{0x01, 0x02};
    const auto device_destination_before = device_destination;
    const auto device_read = physical.read(GuestPhysicalAddress{device_base + 2},
                                           std::span<std::uint8_t>(device_destination),
                                           MemoryAccessContext::Data);
    std::array<std::uint8_t, 2> device_source{0x03, 0x04};
    const auto device_source_before = device_source;
    const auto device_write = physical.write(GuestPhysicalAddress{device_base + 2},
                                             std::span<const std::uint8_t>(device_source),
                                             MemoryAccessContext::Data);
    check(!device_read.ok && device_read.fault == MemoryFault::DeviceAccessRequired &&
              device_read.completed_bytes == 0 &&
              device_destination == device_destination_before &&
              !device_write.ok && device_write.fault == MemoryFault::DeviceAccessRequired &&
              device_write.completed_bytes == 0 &&
              device_source == device_source_before,
          "ordinary physical Device accesses require dispatch and leave buffers unchanged");

    std::array<std::uint8_t, 2> reserved_destination{0x99, 0x88};
    const auto reserved_before = reserved_destination;
    const auto reserved_read = physical.read(GuestPhysicalAddress{reserved_base},
                                             std::span<std::uint8_t>(reserved_destination),
                                             MemoryAccessContext::Data);
    check(!reserved_read.ok && reserved_read.fault == MemoryFault::Unmapped &&
              reserved_read.completed_bytes == 0 &&
              reserved_destination == reserved_before,
          "ordinary physical Reserved accesses fault as Unmapped");
}

void test_guest_physical_memory_commitment_boundary() {
    using namespace xbridge::memory;

    // Internal commitment granularity only; this is not a guest-visible page size.
    constexpr std::uint64_t chunk_size = 4096U;
    constexpr std::uint64_t base = 0x8'0000'0000ULL;
    constexpr std::uint64_t crossing_address = base + chunk_size - 4U;
    constexpr std::uint64_t crossing_length = 8U;

    GuestPhysicalMemory physical;
    check(physical.add_region(GuestPhysicalAddress{base},
                              2U * chunk_size + crossing_length,
                              RegionKind::Normal, MemoryPermissions{true, true, false}).ok,
          "commitment-boundary fixture adds a Normal region spanning the internal chunk boundary");

    const std::array<std::uint8_t, 8> pattern{0x11U, 0x22U, 0x33U, 0x44U,
                                              0x55U, 0x66U, 0x77U, 0x88U};
    const auto write = physical.write(GuestPhysicalAddress{crossing_address},
                                      std::span<const std::uint8_t>(pattern),
                                      MemoryAccessContext::Data);
    check(data_result_matches(write, MemoryFault::None, crossing_address,
                              crossing_length, crossing_length) &&
              write.access == MemoryAccess::Write &&
              write.context == MemoryAccessContext::Data,
          "physical write spanning the internal commitment boundary reports all bytes completed");

    std::array<std::uint8_t, 8> destination{};
    const auto read = physical.read(GuestPhysicalAddress{crossing_address},
                                    std::span<std::uint8_t>(destination),
                                    MemoryAccessContext::Data);
    check(data_result_matches(read, MemoryFault::None, crossing_address,
                              crossing_length, crossing_length) &&
              read.access == MemoryAccess::Read &&
              read.context == MemoryAccessContext::Data &&
              bytes_equal(destination, pattern) &&
              destination == pattern,
          "physical read spanning the internal commitment boundary returns the exact written pattern");
}

void test_guest_address_space_mapping_contract() {
    using namespace xbridge::memory;

    const MemoryPermissions all{true, true, true};
    constexpr std::uint64_t physical_main = 0x1'0000'0000ULL;
    constexpr std::uint64_t physical_touch = physical_main + 64;
    constexpr std::uint64_t virtual_main = 0x2'0000'0000ULL;
    constexpr std::uint64_t virtual_touch = virtual_main + 64;
    constexpr std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max();

    GuestPhysicalMemory physical;
    check(physical.add_region(GuestPhysicalAddress{physical_main}, 64,
                              RegionKind::Normal, all).ok &&
              physical.add_region(GuestPhysicalAddress{physical_touch}, 16,
                                  RegionKind::Normal, all).ok,
          "address-space mapping fixture adds adjacent physical regions");

    GuestAddressSpace space(physical);
    const auto mapped = space.map(GuestVirtualAddress{virtual_main},
                                  GuestPhysicalAddress{physical_main}, 64, all);
    check(result_matches(mapped, MemoryFault::None, virtual_main, 64) &&
              mapped.context == MemoryAccessContext::Map &&
              result_invariant(mapped),
          "map succeeds for a range covered by one physical region");

    GuestPhysicalAddress translated{0};
    const auto translation = space.translate(GuestVirtualAddress{virtual_main + 8}, 8,
                                             MemoryAccess::Read,
                                             MemoryAccessContext::Data, translated);
    check(data_result_matches(translation, MemoryFault::None, virtual_main + 8, 8, 8) &&
              translation.access == MemoryAccess::Read &&
              translation.context == MemoryAccessContext::Data &&
              translated.value == physical_main + 8 &&
              optional_equals(translation.translated_address, physical_main + 8) &&
              optional_equals(translation.region_base, physical_main),
          "translate reports the physical address and region base for a valid range");

    const auto touched = space.map(GuestVirtualAddress{virtual_touch},
                                   GuestPhysicalAddress{physical_touch}, 16, all);
    GuestPhysicalAddress touch_translated{0};
    const auto touch_translation = space.translate(GuestVirtualAddress{virtual_touch}, 1,
                                                   MemoryAccess::Write,
                                                   MemoryAccessContext::Data,
                                                   touch_translated);
    check(touched.ok && touched.context == MemoryAccessContext::Map &&
              touch_translation.ok && touch_translated.value == physical_touch,
          "touching virtual mappings and regions are allowed");

    const auto zero_map = space.map(GuestVirtualAddress{virtual_main + 128},
                                    GuestPhysicalAddress{physical_main}, 0, all);
    check(!zero_map.ok && zero_map.fault == MemoryFault::InvalidRange &&
              zero_map.completed_bytes == 0 && result_invariant(zero_map),
          "map rejects a zero-length range");

    const auto overflow_map = space.map(GuestVirtualAddress{maximum - 3ULL},
                                        GuestPhysicalAddress{physical_main}, 8, all);
    check(!overflow_map.ok && overflow_map.fault == MemoryFault::AddressOverflow &&
              overflow_map.completed_bytes == 0 && result_invariant(overflow_map),
          "map rejects a virtual range that overflows 64 bits");

    GuestPhysicalAddress before_overlap{0};
    check(space.translate(GuestVirtualAddress{virtual_main + 8}, 1, MemoryAccess::Read,
                          MemoryAccessContext::Data, before_overlap).ok,
          "mapping fixture translates the range used for overlap regression");
    const auto overlap = space.map(GuestVirtualAddress{virtual_main + 8},
                                   GuestPhysicalAddress{physical_touch}, 8, all);
    GuestPhysicalAddress after_overlap{0};
    const auto overlap_state = space.translate(GuestVirtualAddress{virtual_main + 8}, 1,
                                               MemoryAccess::Read,
                                               MemoryAccessContext::Data, after_overlap);
    check(!overlap.ok && overlap.fault == MemoryFault::InvalidRange &&
              overlap.completed_bytes == 0 && result_invariant(overlap) &&
              overlap_state.ok && after_overlap.value == before_overlap.value,
          "overlapping map fails atomically and preserves the previous mapping");

    const auto alias = space.map(GuestVirtualAddress{virtual_main + 128},
                                 GuestPhysicalAddress{physical_main + 4}, 8, all);
    GuestPhysicalAddress alias_translated{0};
    const auto alias_state = space.translate(GuestVirtualAddress{virtual_main + 128}, 1,
                                             MemoryAccess::Read,
                                             MemoryAccessContext::Data, alias_translated);
    check(!alias.ok && alias.fault == MemoryFault::InvalidRange &&
              alias.completed_bytes == 0 && result_invariant(alias) &&
              !alias_state.ok && alias_state.fault == MemoryFault::Unmapped &&
              alias_translated.value == 0,
          "physical alias mapping is rejected without inserting a virtual mapping");

    const auto uncovered = space.map(GuestVirtualAddress{virtual_main + 192},
                                     GuestPhysicalAddress{physical_main + 60}, 8, all);
    GuestPhysicalAddress uncovered_translated{0};
    const auto uncovered_state = space.translate(GuestVirtualAddress{virtual_main + 192}, 1,
                                                 MemoryAccess::Read,
                                                 MemoryAccessContext::Data,
                                                 uncovered_translated);
    check(!uncovered.ok && uncovered.fault == MemoryFault::Unmapped &&
              uncovered.completed_bytes == 0 && result_invariant(uncovered) &&
              !uncovered_state.ok && uncovered_translated.value == 0,
          "map rejects a physical range not covered by exactly one region");

    constexpr std::uint64_t adjacent_first = 0x3'0000'0000ULL;
    constexpr std::uint64_t adjacent_second = adjacent_first + 16;
    check(physical.add_region(GuestPhysicalAddress{adjacent_first}, 16,
                              RegionKind::Normal, all).ok &&
              physical.add_region(GuestPhysicalAddress{adjacent_second}, 16,
                                  RegionKind::Normal, all).ok,
          "mapping fixture adds two adjacent physical regions");
    const auto across_regions = space.map(GuestVirtualAddress{virtual_main + 256},
                                          GuestPhysicalAddress{adjacent_first}, 32, all);
    check(!across_regions.ok && across_regions.completed_bytes == 0 &&
              result_invariant(across_regions),
          "map rejects a physical range spanning two adjacent regions");

    const auto unknown_unmap = space.unmap(GuestVirtualAddress{virtual_main + 320}, 64);
    check(!unknown_unmap.ok && unknown_unmap.fault == MemoryFault::Unmapped &&
              unknown_unmap.context == MemoryAccessContext::Unmap &&
              result_invariant(unknown_unmap),
          "unmap of an unknown virtual base fails without mutation");

    const auto mismatched_unmap = space.unmap(GuestVirtualAddress{virtual_main}, 32);
    GuestPhysicalAddress after_failed_unmap{0};
    const auto preserved = space.translate(GuestVirtualAddress{virtual_main}, 1,
                                           MemoryAccess::Read,
                                           MemoryAccessContext::Data, after_failed_unmap);
    check(!mismatched_unmap.ok &&
              mismatched_unmap.fault == MemoryFault::Unmapped &&
              mismatched_unmap.context == MemoryAccessContext::Unmap &&
              result_invariant(mismatched_unmap) &&
              preserved.ok && after_failed_unmap.value == physical_main,
          "unmap with a mismatched length fails atomically");

    const auto removed = space.unmap(GuestVirtualAddress{virtual_main}, 64);
    GuestPhysicalAddress after_removed{0};
    const auto removed_translation = space.translate(GuestVirtualAddress{virtual_main}, 1,
                                                     MemoryAccess::Read,
                                                     MemoryAccessContext::Data,
                                                     after_removed);
    GuestPhysicalAddress touch_after_removal{0};
    check(removed.ok && removed.fault == MemoryFault::None &&
              removed.context == MemoryAccessContext::Unmap &&
              result_invariant(removed) &&
              !removed_translation.ok &&
              removed_translation.fault == MemoryFault::Unmapped &&
              after_removed.value == 0 &&
              space.translate(GuestVirtualAddress{virtual_touch}, 1, MemoryAccess::Read,
                              MemoryAccessContext::Data,
                              touch_after_removal).ok &&
              touch_after_removal.value == physical_touch,
          "exact-base and exact-length unmap removes only the selected mapping");
}

void test_guest_address_space_permissions_and_fetch() {
    using namespace xbridge::memory;

    constexpr std::uint64_t physical_execute = 0x1'0000'0000ULL;
    constexpr std::uint64_t physical_rwx = 0x2'0000'0000ULL;
    constexpr std::uint64_t physical_no_execute = 0x3'0000'0000ULL;
    constexpr std::uint64_t physical_no_read = 0x4'0000'0000ULL;
    constexpr std::uint64_t physical_readonly = 0x5'0000'0000ULL;
    constexpr std::uint64_t physical_no_write = 0x6'0000'0000ULL;
    constexpr std::uint64_t physical_cross_first = 0x7'0000'0000ULL;
    constexpr std::uint64_t physical_cross_second = physical_cross_first + 8;
    constexpr std::uint64_t virtual_execute = 0xA'0000'0000ULL;
    constexpr std::uint64_t virtual_rwx = 0xB'0000'0000ULL;
    constexpr std::uint64_t virtual_no_execute = 0xC'0000'0000ULL;
    constexpr std::uint64_t virtual_no_read = 0xD'0000'0000ULL;
    constexpr std::uint64_t virtual_readonly = 0xE'0000'0000ULL;
    constexpr std::uint64_t virtual_no_write = 0xF'0000'0000ULL;
    constexpr std::uint64_t virtual_cross_first = 0x10'0000'0000ULL;
    constexpr std::uint64_t virtual_cross_second = virtual_cross_first + 8;
    constexpr std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max();

    GuestPhysicalMemory physical;
    check(physical.add_region(GuestPhysicalAddress{physical_execute}, 16, RegionKind::Normal,
                              MemoryPermissions{false, true, true}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_rwx}, 16, RegionKind::Normal,
                                  MemoryPermissions{true, true, true}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_no_execute}, 16,
                                  RegionKind::Normal,
                                  MemoryPermissions{true, true, false}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_no_read}, 16,
                                  RegionKind::Normal,
                                  MemoryPermissions{false, false, true}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_readonly}, 16,
                                  RegionKind::Normal,
                                  MemoryPermissions{true, true, false}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_no_write}, 16,
                                  RegionKind::Normal,
                                  MemoryPermissions{true, false, false}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_cross_first}, 8,
                                  RegionKind::Normal,
                                  MemoryPermissions{false, true, true}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_cross_second}, 8,
                                  RegionKind::Normal,
                                  MemoryPermissions{false, true, true}).ok,
          "permission fixture adds physical regions with independent read, write and execute bits");

    GuestAddressSpace space(physical);
    check(space.map(GuestVirtualAddress{virtual_execute},
                    GuestPhysicalAddress{physical_execute}, 16,
                    MemoryPermissions{false, false, true}).ok &&
              space.map(GuestVirtualAddress{virtual_rwx},
                        GuestPhysicalAddress{physical_rwx}, 16,
                        MemoryPermissions{true, true, false}).ok &&
              space.map(GuestVirtualAddress{virtual_no_execute},
                        GuestPhysicalAddress{physical_no_execute}, 16,
                        MemoryPermissions{false, false, true}).ok &&
              space.map(GuestVirtualAddress{virtual_no_read},
                        GuestPhysicalAddress{physical_no_read}, 16,
                        MemoryPermissions{true, true, true}).ok &&
              space.map(GuestVirtualAddress{virtual_readonly},
                        GuestPhysicalAddress{physical_readonly}, 16,
                        MemoryPermissions{true, false, false}).ok &&
              space.map(GuestVirtualAddress{virtual_no_write},
                        GuestPhysicalAddress{physical_no_write}, 16,
                        MemoryPermissions{true, true, false}).ok &&
              space.map(GuestVirtualAddress{virtual_cross_first},
                        GuestPhysicalAddress{physical_cross_first}, 8,
                        MemoryPermissions{false, false, true}).ok &&
              space.map(GuestVirtualAddress{virtual_cross_second},
                        GuestPhysicalAddress{physical_cross_second}, 8,
                        MemoryPermissions{false, false, true}).ok,
          "permission fixture adds mappings with independent read, write and execute bits");

    std::array<std::uint8_t, 15> fetch_source{};
    for (std::size_t index = 0; index < fetch_source.size(); ++index) {
        fetch_source[index] = static_cast<std::uint8_t>(0x10U + index);
    }
    check(physical.write(GuestPhysicalAddress{physical_execute},
                         std::span<const std::uint8_t>(fetch_source),
                         MemoryAccessContext::Bulk).ok,
          "execute fixture seeds fetch bytes through the physical write permission");

    std::array<std::uint8_t, 4> fetch_destination{};
    const auto fetch_ok = space.fetch(GuestVirtualAddress{virtual_execute},
                                      std::span<std::uint8_t>(fetch_destination));
    check(data_result_matches(fetch_ok, MemoryFault::None, virtual_execute, 4, 4) &&
              fetch_ok.access == MemoryAccess::Execute &&
              fetch_ok.context == MemoryAccessContext::Fetch &&
              bytes_equal(fetch_destination,
                          std::span<const std::uint8_t>(fetch_source.data(), 4)),
          "execute-only mapping fetches successfully without read permission");

    std::array<std::uint8_t, 15> full_fetch_destination{};
    const auto full_fetch = space.fetch(GuestVirtualAddress{virtual_execute},
                                        std::span<std::uint8_t>(full_fetch_destination));
    check(full_fetch.ok && full_fetch.fault == MemoryFault::None &&
              full_fetch.completed_bytes == 15 &&
              full_fetch.context == MemoryAccessContext::Fetch &&
              bytes_equal(full_fetch_destination, fetch_source),
          "15-byte fetch succeeds at the implementation window boundary");

    std::array<std::uint8_t, 16> oversized_fetch{};
    const auto oversized_before = oversized_fetch;
    const auto too_long = space.fetch(GuestVirtualAddress{virtual_execute},
                                      std::span<std::uint8_t>(oversized_fetch));
    check(!too_long.ok && too_long.fault == MemoryFault::InvalidRange &&
              too_long.completed_bytes == 0 && result_invariant(too_long) &&
              oversized_fetch == oversized_before,
          "16-byte fetch is rejected without touching the destination");

    const auto empty_fetch = space.fetch(GuestVirtualAddress{virtual_execute},
                                         std::span<std::uint8_t>{});
    check(!empty_fetch.ok && empty_fetch.fault == MemoryFault::InvalidRange &&
              empty_fetch.requested_length == 0 && empty_fetch.completed_bytes == 0 &&
              result_invariant(empty_fetch),
          "zero-length fetch is rejected as an invalid instruction window");

    std::array<std::uint8_t, 4> denied_fetch_destination{0x11, 0x22, 0x33, 0x44};
    const auto denied_fetch_before = denied_fetch_destination;
    const auto mapping_no_execute = space.fetch(GuestVirtualAddress{virtual_rwx},
                                                std::span<std::uint8_t>(denied_fetch_destination));
    check(!mapping_no_execute.ok &&
              mapping_no_execute.fault == MemoryFault::AccessDenied &&
              mapping_no_execute.completed_bytes == 0 &&
              result_invariant(mapping_no_execute) &&
              denied_fetch_destination == denied_fetch_before,
          "read/write mapping permission does not grant fetch even when the region permits execute");

    const auto region_no_execute = space.fetch(GuestVirtualAddress{virtual_no_execute},
                                               std::span<std::uint8_t>(denied_fetch_destination));
    check(!region_no_execute.ok &&
              region_no_execute.fault == MemoryFault::AccessDenied &&
              region_no_execute.completed_bytes == 0 &&
              denied_fetch_destination == denied_fetch_before,
          "execute mapping permission does not override a region without execute permission");

    std::array<std::uint8_t, 4> data_destination{0xAA, 0xBB, 0xCC, 0xDD};
    const auto data_before = data_destination;
    const auto execute_is_not_read = space.read(GuestVirtualAddress{virtual_execute},
                                                std::span<std::uint8_t>(data_destination));
    check(!execute_is_not_read.ok &&
              execute_is_not_read.fault == MemoryFault::AccessDenied &&
              execute_is_not_read.completed_bytes == 0 &&
              data_destination == data_before,
          "execute permission does not imply data-read permission on the mapping");

    const auto region_is_not_read = space.read(GuestVirtualAddress{virtual_no_read},
                                               std::span<std::uint8_t>(data_destination));
    check(!region_is_not_read.ok &&
              region_is_not_read.fault == MemoryFault::AccessDenied &&
              region_is_not_read.completed_bytes == 0 &&
              data_destination == data_before,
          "a mapping with read permission cannot bypass a region without read permission");

    const std::array<std::uint8_t, 4> zero_bytes{};
    std::array<std::uint8_t, 4> mapping_write_source{0xDE, 0xAD, 0xBE, 0xEF};
    const auto mapping_write_before = mapping_write_source;
    const auto mapping_write_denied = space.write(GuestVirtualAddress{virtual_readonly},
                                                  std::span<const std::uint8_t>(mapping_write_source));
    std::array<std::uint8_t, 4> readonly_verify{};
    const auto readonly_state = physical.read(GuestPhysicalAddress{physical_readonly},
                                              std::span<std::uint8_t>(readonly_verify),
                                              MemoryAccessContext::Data);
    check(!mapping_write_denied.ok &&
              mapping_write_denied.fault == MemoryFault::AccessDenied &&
              mapping_write_denied.completed_bytes == 0 &&
              mapping_write_source == mapping_write_before &&
              readonly_state.ok && bytes_equal(readonly_verify, zero_bytes),
          "mapping write denial writes zero bytes and leaves physical backing unchanged");

    std::array<std::uint8_t, 4> region_write_source{0x01, 0x02, 0x03, 0x04};
    const auto region_write_before = region_write_source;
    const auto region_write_denied = space.write(GuestVirtualAddress{virtual_no_write},
                                                 std::span<const std::uint8_t>(region_write_source));
    std::array<std::uint8_t, 4> no_write_verify{0x55, 0x55, 0x55, 0x55};
    const auto no_write_state = space.read(GuestVirtualAddress{virtual_no_write},
                                           std::span<std::uint8_t>(no_write_verify));
    check(!region_write_denied.ok &&
              region_write_denied.fault == MemoryFault::AccessDenied &&
              region_write_denied.completed_bytes == 0 &&
              region_write_source == region_write_before &&
              no_write_state.ok && bytes_equal(no_write_verify, zero_bytes),
          "region write denial writes zero bytes and cannot be bypassed by mapping permissions");

    std::array<std::uint8_t, 9> cross_fetch_destination{0x77, 0x77, 0x77, 0x77,
                                                        0x77, 0x77, 0x77, 0x77, 0x77};
    const auto cross_fetch_before = cross_fetch_destination;
    const auto cross_fetch = space.fetch(GuestVirtualAddress{virtual_cross_first},
                                         std::span<std::uint8_t>(cross_fetch_destination));
    check(!cross_fetch.ok && cross_fetch.completed_bytes == 0 &&
              result_invariant(cross_fetch) &&
              cross_fetch_destination == cross_fetch_before,
          "fetch spanning two executable mappings or regions is rejected atomically");

    std::array<std::uint8_t, 1> overflow_destination{0x5A};
    const auto overflow_before = overflow_destination;
    const auto overflow_fetch = space.fetch(GuestVirtualAddress{maximum},
                                            std::span<std::uint8_t>(overflow_destination));
    check(!overflow_fetch.ok &&
              overflow_fetch.fault == MemoryFault::AddressOverflow &&
              overflow_fetch.completed_bytes == 0 &&
              overflow_destination == overflow_before,
          "fetch at UINT64_MAX reports AddressOverflow without mutation");
}

void test_guest_address_space_device_dispatch() {
    using namespace xbridge::memory;

    constexpr std::uint64_t physical_device = 0x1'0000'0000ULL;
    constexpr std::uint64_t virtual_device = 0xB'0000'0000ULL;
    constexpr std::uint64_t physical_normal = 0x2'0000'0000ULL;

    GuestPhysicalMemory physical;
    check(physical.add_region(GuestPhysicalAddress{physical_device}, 16, RegionKind::Device,
                              MemoryPermissions{true, true, true}).ok,
          "device dispatch fixture adds a device region");
    GuestAddressSpace space(physical);
    check(space.map(GuestVirtualAddress{virtual_device},
                    GuestPhysicalAddress{physical_device}, 16,
                    MemoryPermissions{true, true, true}).ok,
          "device dispatch fixture maps the device region");

    RecordingDeviceDispatcher dispatcher;
    space.set_device_dispatcher(&dispatcher);

    std::array<std::uint8_t, 4> read_destination{};
    const auto dispatched_read = space.read(GuestVirtualAddress{virtual_device + 8},
                                            std::span<std::uint8_t>(read_destination));
    const std::array<std::uint8_t, 4> pattern{0xC3, 0xC3, 0xC3, 0xC3};
    check(dispatched_read.ok && dispatched_read.fault == MemoryFault::None &&
              dispatched_read.access == MemoryAccess::Read &&
              dispatched_read.context == MemoryAccessContext::DeviceDispatch &&
              dispatched_read.completed_bytes == 4 &&
              dispatcher.calls == 1 &&
              dispatcher.last_region_base.value == physical_device &&
              dispatcher.last_offset == 8 &&
              dispatcher.last_access == MemoryAccess::Read &&
              dispatcher.last_context == MemoryAccessContext::DeviceDispatch &&
              dispatcher.last_data_size == 4 &&
              bytes_equal(read_destination, pattern),
          "device read dispatches the region base, offset, access, context and data");

    std::array<std::uint8_t, 4> write_source{0xDE, 0xAD, 0xBE, 0xEF};
    const auto dispatched_write = space.write(GuestVirtualAddress{virtual_device + 4},
                                              std::span<const std::uint8_t>(write_source));
    check(dispatched_write.ok && dispatched_write.fault == MemoryFault::None &&
              dispatched_write.access == MemoryAccess::Write &&
              dispatched_write.context == MemoryAccessContext::DeviceDispatch &&
              dispatched_write.completed_bytes == 4 &&
              dispatcher.calls == 2 &&
              dispatcher.last_region_base.value == physical_device &&
              dispatcher.last_offset == 4 &&
              dispatcher.last_access == MemoryAccess::Write &&
              dispatcher.last_context == MemoryAccessContext::DeviceDispatch &&
              dispatcher.last_data_size == 4 &&
              bytes_equal(std::span<const std::uint8_t>(dispatcher.last_data.data(),
                                                        dispatcher.last_data_size),
                          write_source),
          "device write dispatches the source bytes with correct diagnostics");

    space.set_device_dispatcher(nullptr);
    std::array<std::uint8_t, 4> unbound_destination{0x11, 0x22, 0x33, 0x44};
    const auto unbound_before = unbound_destination;
    const auto unbound = space.read(GuestVirtualAddress{virtual_device},
                                    std::span<std::uint8_t>(unbound_destination));
    check(!unbound.ok && unbound.fault == MemoryFault::DeviceAccessRequired &&
              unbound.completed_bytes == 0 && result_invariant(unbound) &&
              unbound_destination == unbound_before,
          "device access without a dispatcher reports DeviceAccessRequired without mutation");

    dispatcher.fail = true;
    dispatcher.calls = 0;
    space.set_device_dispatcher(&dispatcher);
    std::array<std::uint8_t, 4> failed_destination{0x5A, 0x5A, 0x5A, 0x5A};
    const auto failed_before = failed_destination;
    const auto failed_read = space.read(GuestVirtualAddress{virtual_device},
                                        std::span<std::uint8_t>(failed_destination));
    check(!failed_read.ok && failed_read.fault == MemoryFault::Unsupported &&
              failed_read.completed_bytes == 0 && result_invariant(failed_read) &&
              dispatcher.calls == 1 && failed_destination == failed_before,
          "failed dispatch leaves the destination buffer unchanged");

    std::array<std::uint8_t, 4> failed_write_source{0x01, 0x02, 0x03, 0x04};
    const auto failed_write_before = failed_write_source;
    const auto calls_before_failed_write = dispatcher.calls;
    const auto failed_write = space.write(GuestVirtualAddress{virtual_device},
                                          std::span<const std::uint8_t>(failed_write_source));
    check(!failed_write.ok && failed_write.completed_bytes == 0 &&
              result_invariant(failed_write) &&
              dispatcher.calls == calls_before_failed_write + 1 &&
              failed_write_source == failed_write_before,
          "failed device write reports zero completed bytes and leaves source unchanged");

    dispatcher.fail = false;
    dispatcher.calls = 0;
    std::array<std::uint8_t, 4> fetch_destination{0x77, 0x77, 0x77, 0x77};
    const auto fetch_before = fetch_destination;
    const auto device_fetch = space.fetch(GuestVirtualAddress{virtual_device},
                                          std::span<std::uint8_t>(fetch_destination));
    check(!device_fetch.ok && device_fetch.completed_bytes == 0 &&
              result_invariant(device_fetch) &&
              dispatcher.calls == 0 && fetch_destination == fetch_before,
          "fetch from a Device region is rejected before any device dispatch");

    check(physical.add_region(GuestPhysicalAddress{physical_normal}, 8, RegionKind::Normal,
                              MemoryPermissions{true, true, false}).ok &&
              space.map(GuestVirtualAddress{virtual_device - 8},
                        GuestPhysicalAddress{physical_normal}, 8,
                        MemoryPermissions{true, true, false}).ok,
          "mixed bulk fixture maps a Normal region adjacent to the Device mapping");

    std::array<std::uint8_t, 16> mixed_destination{};
    const auto mixed_before = mixed_destination;
    const auto calls_before_mixed = dispatcher.calls;
    const auto mixed = space.read(GuestVirtualAddress{virtual_device - 8},
                                  std::span<std::uint8_t>(mixed_destination));
    check(!mixed.ok && mixed.completed_bytes == 0 && result_invariant(mixed) &&
              dispatcher.calls == calls_before_mixed &&
              mixed_destination == mixed_before,
          "bulk access spanning Normal and Device mappings is rejected before dispatch");
}

void test_guest_address_space_atomicity_and_boundaries() {
    using namespace xbridge::memory;

    constexpr std::uint64_t physical_first = 0x1'0000'0000ULL;
    constexpr std::uint64_t physical_second = physical_first + 8;
    constexpr std::uint64_t virtual_first = 0xD'0000'0000ULL;
    constexpr std::uint64_t virtual_second = virtual_first + 8;
    constexpr std::uint64_t physical_readonly = 0x2'0000'0000ULL;
    constexpr std::uint64_t virtual_readonly = 0xE'0000'0000ULL;
    constexpr std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max();

    GuestPhysicalMemory physical;
    check(physical.add_region(GuestPhysicalAddress{physical_first}, 8, RegionKind::Normal,
                              MemoryPermissions{true, true, false}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_second}, 8, RegionKind::Normal,
                                  MemoryPermissions{true, true, false}).ok &&
              physical.add_region(GuestPhysicalAddress{physical_readonly}, 8, RegionKind::Normal,
                                  MemoryPermissions{true, true, false}).ok,
          "atomicity fixture adds touching physical regions above 4 GiB");

    GuestAddressSpace space(physical);
    check(space.map(GuestVirtualAddress{virtual_first},
                    GuestPhysicalAddress{physical_first}, 8,
                    MemoryPermissions{true, true, false}).ok &&
              space.map(GuestVirtualAddress{virtual_second},
                        GuestPhysicalAddress{physical_second}, 8,
                        MemoryPermissions{true, true, false}).ok &&
              space.map(GuestVirtualAddress{virtual_readonly},
                        GuestPhysicalAddress{physical_readonly}, 8,
                        MemoryPermissions{true, false, false}).ok,
          "atomicity fixture maps touching virtual ranges and a read-only range");

    std::array<std::uint8_t, 4> first_source{0x01, 0x02, 0x03, 0x04};
    std::array<std::uint8_t, 4> second_source{0x05, 0x06, 0x07, 0x08};
    check(space.write(GuestVirtualAddress{virtual_first},
                      std::span<const std::uint8_t>(first_source)).ok &&
              space.write(GuestVirtualAddress{virtual_second},
                          std::span<const std::uint8_t>(second_source)).ok,
          "writes to touching mappings above 4 GiB succeed independently");

    std::array<std::uint8_t, 8> first_backing{};
    std::array<std::uint8_t, 8> second_backing{};
    check(physical.read(GuestPhysicalAddress{physical_first},
                        std::span<std::uint8_t>(first_backing),
                        MemoryAccessContext::Data).ok &&
              physical.read(GuestPhysicalAddress{physical_second},
                            std::span<std::uint8_t>(second_backing),
                            MemoryAccessContext::Data).ok &&
              bytes_equal(std::span<const std::uint8_t>(first_backing.data(), 4), first_source) &&
              bytes_equal(std::span<const std::uint8_t>(second_backing.data(), 4), second_source),
          "no 64-to-32 truncation occurs for successful accesses above 4 GiB");

    std::array<std::uint8_t, 16> spanning_source{};
    for (std::uint8_t& byte : spanning_source) {
        byte = 0xA5;
    }
    const auto spanning_write = space.write(GuestVirtualAddress{virtual_first},
                                            std::span<const std::uint8_t>(spanning_source));
    std::array<std::uint8_t, 8> first_after_span{};
    std::array<std::uint8_t, 8> second_after_span{};
    check(physical.read(GuestPhysicalAddress{physical_first},
                        std::span<std::uint8_t>(first_after_span),
                        MemoryAccessContext::Data).ok &&
              physical.read(GuestPhysicalAddress{physical_second},
                            std::span<std::uint8_t>(second_after_span),
                            MemoryAccessContext::Data).ok,
          "spanning-write regression reads both backing regions");
    check(!spanning_write.ok && spanning_write.completed_bytes == 0 &&
              result_invariant(spanning_write) &&
              first_after_span == first_backing && second_after_span == second_backing,
          "bulk write spanning two mappings or regions writes zero bytes");

    std::array<std::uint8_t, 16> spanning_destination{};
    const auto spanning_before = spanning_destination;
    const auto spanning_read = space.read(GuestVirtualAddress{virtual_first},
                                          std::span<std::uint8_t>(spanning_destination));
    check(!spanning_read.ok && spanning_read.completed_bytes == 0 &&
              result_invariant(spanning_read) &&
              spanning_destination == spanning_before,
          "bulk read spanning two mappings or regions leaves the destination unchanged");

    const std::array<std::uint8_t, 4> zero_bytes{};
    std::array<std::uint8_t, 4> scalar_source{0xDE, 0xAD, 0xBE, 0xEF};
    const auto scalar_before = scalar_source;
    const auto scalar_denied = space.write(GuestVirtualAddress{virtual_readonly},
                                           std::span<const std::uint8_t>(scalar_source));
    std::array<std::uint8_t, 4> readonly_backing{};
    const auto readonly_state = physical.read(GuestPhysicalAddress{physical_readonly},
                                              std::span<std::uint8_t>(readonly_backing),
                                              MemoryAccessContext::Data);
    check(!scalar_denied.ok && scalar_denied.fault == MemoryFault::AccessDenied &&
              scalar_denied.completed_bytes == 0 && result_invariant(scalar_denied) &&
              scalar_source == scalar_before && readonly_state.ok &&
              bytes_equal(readonly_backing, zero_bytes),
          "failed scalar virtual write writes zero bytes into physical backing");

    std::array<std::uint8_t, 2> noop_destination{0x11, 0x22};
    const auto noop_before = noop_destination;
    const auto zero_read = space.read(GuestVirtualAddress{0xF'0000'0000ULL},
                                      std::span<std::uint8_t>(noop_destination).first(0));
    const auto zero_write = space.write(GuestVirtualAddress{maximum},
                                        std::span<const std::uint8_t>{});
    check(data_result_matches(zero_read, MemoryFault::None, 0xF'0000'0000ULL, 0, 0) &&
              zero_read.access == MemoryAccess::Read &&
              zero_read.context == MemoryAccessContext::Data &&
              data_result_matches(zero_write, MemoryFault::None, maximum, 0, 0) &&
              zero_write.access == MemoryAccess::Write &&
              zero_write.context == MemoryAccessContext::Data &&
              noop_destination == noop_before,
          "zero-length virtual data accesses succeed at unmapped and maximum addresses");

    std::array<std::uint8_t, 1> overflow_destination{0x5A};
    const auto overflow_destination_before = overflow_destination;
    const auto overflow_read = space.read(GuestVirtualAddress{maximum},
                                          std::span<std::uint8_t>(overflow_destination));
    std::array<std::uint8_t, 1> overflow_source{0x5A};
    const auto overflow_source_before = overflow_source;
    const auto overflow_write = space.write(GuestVirtualAddress{maximum},
                                            std::span<const std::uint8_t>(overflow_source));
    GuestPhysicalAddress overflow_translated{0x1234};
    const auto overflow_translation = space.translate(GuestVirtualAddress{maximum}, 1,
                                                      MemoryAccess::Read,
                                                      MemoryAccessContext::Data,
                                                      overflow_translated);
    check(!overflow_read.ok && overflow_read.fault == MemoryFault::AddressOverflow &&
              overflow_read.completed_bytes == 0 &&
              overflow_destination == overflow_destination_before &&
              !overflow_write.ok && overflow_write.fault == MemoryFault::AddressOverflow &&
              overflow_write.completed_bytes == 0 &&
              overflow_source == overflow_source_before &&
              !overflow_translation.ok &&
              overflow_translation.fault == MemoryFault::AddressOverflow &&
              overflow_translated.value == 0x1234 &&
              !overflow_translation.translated_address.has_value() &&
              !overflow_translation.region_base.has_value(),
          "positive accesses at UINT64_MAX fail with AddressOverflow and no mutation");
}

cpu::X64CpuState make_integration_state(std::uint64_t rip) {
    cpu::X64CpuState state;
    state.rip = rip;
    state.rflags = 0x0000000000000A95ULL;
    for (std::size_t index = 0; index < state.general_purpose_registers.size(); ++index) {
        state.general_purpose_registers[index] =
            0x1122334455667700ULL + static_cast<std::uint64_t>(index);
    }
    return state;
}

bool x64_state_equals(const cpu::X64CpuState& lhs, const cpu::X64CpuState& rhs) noexcept {
    return lhs.rip == rhs.rip && lhs.rflags == rhs.rflags &&
           lhs.general_purpose_registers == rhs.general_purpose_registers;
}

bool memory_results_equal(const memory::MemoryResult& lhs,
                          const memory::MemoryResult& rhs) noexcept {
    return lhs.ok == rhs.ok && lhs.fault == rhs.fault &&
           lhs.access == rhs.access && lhs.context == rhs.context &&
           lhs.address == rhs.address && lhs.requested_length == rhs.requested_length &&
           lhs.completed_bytes == rhs.completed_bytes &&
           lhs.translated_address.has_value() == rhs.translated_address.has_value() &&
           (!lhs.translated_address.has_value() ||
            *lhs.translated_address == *rhs.translated_address) &&
           lhs.region_base.has_value() == rhs.region_base.has_value() &&
           (!lhs.region_base.has_value() || *lhs.region_base == *rhs.region_base);
}

const char* memory_fault_name(memory::MemoryFault fault) noexcept {
    switch (fault) {
    case memory::MemoryFault::None: return "None";
    case memory::MemoryFault::Unmapped: return "Unmapped";
    case memory::MemoryFault::AccessDenied: return "AccessDenied";
    case memory::MemoryFault::AddressOverflow: return "AddressOverflow";
    case memory::MemoryFault::InvalidRange: return "InvalidRange";
    case memory::MemoryFault::Alignment: return "Alignment";
    case memory::MemoryFault::ResourceExhausted: return "ResourceExhausted";
    case memory::MemoryFault::DeviceAccessRequired: return "DeviceAccessRequired";
    case memory::MemoryFault::Unsupported: return "Unsupported";
    }
    return "unknown";
}

std::vector<std::uint8_t> read_physical_snapshot(memory::GuestPhysicalMemory& physical,
                                                 memory::GuestPhysicalAddress base,
                                                 std::size_t length,
                                                 bool& ok) {
    std::vector<std::uint8_t> bytes(length, std::uint8_t{0U});
    const auto result = physical.read(
        base, std::span<std::uint8_t>(bytes.data(), bytes.size()),
        memory::MemoryAccessContext::Bulk);
    ok = result.ok && result.completed_bytes == static_cast<std::uint64_t>(length);
    return bytes;
}

class RecordingAddressSpaceView final : public memory::IGuestVirtualMemory {
public:
    explicit RecordingAddressSpaceView(memory::GuestAddressSpace& space) noexcept
        : view_(space) {}

    void reset() noexcept {
        fetch_calls = 0U;
        max_fetch_length = 0U;
        oversized_calls = 0U;
    }

    memory::MemoryResult fetch(memory::GuestVirtualAddress address,
                               std::span<std::uint8_t> destination) noexcept override {
        ++fetch_calls;
        if (destination.size() > max_fetch_length)
            max_fetch_length = destination.size();
        if (destination.size() > 15U) {
            ++oversized_calls;
            return memory::MemoryResult::failure(
                memory::MemoryFault::InvalidRange, memory::MemoryAccess::Execute,
                memory::MemoryAccessContext::Fetch, address.value,
                static_cast<std::uint64_t>(destination.size()));
        }
        return view_.fetch(address, destination);
    }

    memory::MemoryResult read(memory::GuestVirtualAddress address,
                              std::span<std::uint8_t> destination) noexcept override {
        return view_.read(address, destination);
    }

    memory::MemoryResult write(memory::GuestVirtualAddress address,
                               std::span<const std::uint8_t> source) noexcept override {
        return view_.write(address, source);
    }

    std::size_t fetch_calls{0U};
    std::size_t max_fetch_length{0U};
    std::size_t oversized_calls{0U};

private:
    memory::GuestAddressSpaceView view_;
};

class ConfigurableVirtualMemory final : public memory::IGuestVirtualMemory {
public:
    memory::MemoryResult fetch(memory::GuestVirtualAddress address,
                               std::span<std::uint8_t> destination) noexcept override {
        (void)address;
        ++fetch_calls;
        if (destination.size() > max_fetch_length)
            max_fetch_length = destination.size();
        if (fill_fetch_buffer) {
            for (std::uint8_t& byte : destination)
                byte = fill_byte;
        }
        return fetch_result;
    }

    memory::MemoryResult read(memory::GuestVirtualAddress address,
                              std::span<std::uint8_t> destination) noexcept override {
        (void)address;
        ++read_calls;
        if (fill_read_buffer) {
            for (std::uint8_t& byte : destination)
                byte = fill_byte;
        }
        return read_result;
    }

    memory::MemoryResult write(memory::GuestVirtualAddress address,
                               std::span<const std::uint8_t> source) noexcept override {
        (void)address;
        ++write_calls;
        last_write_size = source.size();
        for (std::size_t index = 0U;
             index < source.size() && index < last_write_source.size(); ++index) {
            last_write_source[index] = source[index];
        }
        return write_result;
    }

    memory::MemoryResult fetch_result{};
    memory::MemoryResult read_result{};
    memory::MemoryResult write_result{};
    std::size_t fetch_calls{0U};
    std::size_t read_calls{0U};
    std::size_t write_calls{0U};
    std::size_t max_fetch_length{0U};
    bool fill_fetch_buffer{false};
    bool fill_read_buffer{false};
    std::uint8_t fill_byte{0x5AU};
    std::array<std::uint8_t, 8U> last_write_source{};
    std::size_t last_write_size{0U};
};

memory::MemoryResult make_diagnostic_failure(memory::MemoryFault fault,
                                              memory::MemoryAccess access,
                                              memory::MemoryAccessContext context,
                                              std::uint64_t requested_length) {
    auto result = memory::MemoryResult::failure(
        fault, access, context, 0x123456789ABCDEF0ULL, requested_length);
    result.translated_address = 0x00FF00FF00FF00FFULL;
    result.region_base = 0xA5A5A5A5A5A5A5A5ULL;
    return result;
}

memory::MemoryResult make_diagnostic_short_success(memory::MemoryAccess access,
                                                   memory::MemoryAccessContext context,
                                                   std::uint64_t requested_length,
                                                   std::uint64_t completed_bytes) {
    auto result = memory::MemoryResult::success(
        access, context, 0x123456789ABCDEF0ULL, requested_length, completed_bytes);
    result.translated_address = 0x1357246835792468ULL;
    result.region_base = 0x2468135724681357ULL;
    return result;
}

void test_x64_step_at_rip_via_execute_only_above_4gib() {
    using namespace xbridge;

    constexpr std::uint64_t physical_base = 0x2'0000'0000ULL;
    constexpr std::uint64_t virtual_base = 0x1'0000'0000ULL;
    const std::array<std::uint8_t, 7U> program{
        0x90U, 0xB8U, 0x78U, 0x56U, 0x34U, 0x12U, 0x90U};

    memory::GuestPhysicalMemory physical;
    check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 32U,
                              memory::RegionKind::Normal,
                              memory::MemoryPermissions{false, true, true}).ok,
          "T1 execute-only fixture adds a writable Normal region above 4 GiB");
    check(physical.write(memory::GuestPhysicalAddress{physical_base},
                         std::span<const std::uint8_t>(program.data(), program.size()),
                         memory::MemoryAccessContext::Bulk).ok,
          "T1 execute-only fixture seeds NOP, MOV imm32, NOP above 4 GiB");

    memory::GuestAddressSpace space(physical);
    check(space.map(memory::GuestVirtualAddress{virtual_base},
                    memory::GuestPhysicalAddress{physical_base}, 32U,
                    memory::MemoryPermissions{false, false, true}).ok,
          "T1 virtual mapping grants execute without read or write permission");
    memory::GuestAddressSpaceView view(space);

    std::array<std::uint8_t, 1U> data_probe{0xA5U};
    const auto data_denied = view.read(memory::GuestVirtualAddress{virtual_base},
                                       std::span<std::uint8_t>(data_probe));
    check(!data_denied.ok && data_denied.fault == memory::MemoryFault::AccessDenied &&
              data_probe[0] == 0xA5U,
          "T1 execute-only code has no data-read permission at either layer");

    auto state = make_integration_state(virtual_base);
    const auto rax_initial = state.gpr(cpu::X64GeneralRegister::Rax);
    const auto rflags_initial = state.rflags;

    const auto first = cpu::step_x64_at_rip_via(state, view);
    check(first == cpu::X64ExecuteStatus::Ok && !first.memory_result.has_value() &&
              state.rip == virtual_base + 1U &&
              state.gpr(cpu::X64GeneralRegister::Rax) == rax_initial &&
              state.rflags == rflags_initial,
          "T1 first fetch step executes NOP at +1 without a data-read permission");

    const auto second = cpu::step_x64_at_rip_via(state, view);
    check(second == cpu::X64ExecuteStatus::Ok && !second.memory_result.has_value() &&
              state.rip == virtual_base + 6U &&
              state.gpr(cpu::X64GeneralRegister::Rax) == 0x12345678ULL &&
              state.rflags == rflags_initial,
          "T1 second fetch step executes MOV imm32 at +5 and sets RAX");

    const auto third = cpu::step_x64_at_rip_via(state, view);
    check(third == cpu::X64ExecuteStatus::Ok && !third.memory_result.has_value() &&
              state.rip == virtual_base + 7U &&
              state.gpr(cpu::X64GeneralRegister::Rax) == 0x12345678ULL &&
              state.rflags == rflags_initial,
          "T1 third fetch step executes NOP at +1 and preserves RAX");
}

void test_x64_execute_instruction_via_data_above_4gib_little_endian() {
    using namespace xbridge;

    constexpr std::uint64_t physical_base = 0x2'0000'0000ULL;
    constexpr std::uint64_t virtual_base = 0x1'0000'0000ULL;
    constexpr std::uint64_t data_offset = 0x40ULL;
    constexpr std::uint64_t store_address = virtual_base + data_offset;
    constexpr std::uint64_t value = 0x1122334455667788ULL;
    const std::array<std::uint8_t, 8U> expected_bytes{
        0x88U, 0x77U, 0x66U, 0x55U, 0x44U, 0x33U, 0x22U, 0x11U};
    const std::uint8_t store_bytes[] = {0x48U, 0x89U, 0x08U};
    const std::uint8_t load_bytes[] = {0x48U, 0x8BU, 0x10U};

    memory::GuestPhysicalMemory physical;
    check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 0x80U,
                              memory::RegionKind::Normal,
                              memory::MemoryPermissions{true, true, true}).ok,
          "T2 RW fixture adds a Normal region above 4 GiB");
    memory::GuestAddressSpace space(physical);
    check(space.map(memory::GuestVirtualAddress{virtual_base},
                    memory::GuestPhysicalAddress{physical_base}, 0x80U,
                    memory::MemoryPermissions{true, true, false}).ok,
          "T2 virtual mapping grants read and write above 4 GiB");
    memory::GuestAddressSpaceView view(space);

    const auto store = cpu::decode_x64_instruction(store_bytes);
    const auto load = cpu::decode_x64_instruction(load_bytes);
    check(store.status == cpu::X64DecodeStatus::Ok && load.status == cpu::X64DecodeStatus::Ok,
          "T2 64-bit MOV store and load forms decode successfully");

    auto state = make_integration_state(0x2000U);
    state.gpr(cpu::X64GeneralRegister::Rax) = store_address;
    state.gpr(cpu::X64GeneralRegister::Rcx) = value;
    const auto store_result = cpu::execute_instruction_via(store, state, view);
    check(store_result == cpu::X64ExecuteStatus::Ok &&
              !store_result.memory_result.has_value() &&
              state.rip == 0x2000U + store.length &&
              state.gpr(cpu::X64GeneralRegister::Rcx) == value,
          "T2 store via the virtual seam advances RIP exactly once and preserves the source GPR");

    std::array<std::uint8_t, 8U> backing{};
    const auto backing_read = physical.read(
        memory::GuestPhysicalAddress{physical_base + data_offset},
        std::span<std::uint8_t>(backing), memory::MemoryAccessContext::Data);
    check(backing_read.ok && bytes_equal(backing, expected_bytes),
          "T2 stored bytes are exact little-endian at the physical address above 4 GiB");

    const auto load_result = cpu::execute_instruction_via(load, state, view);
    check(load_result == cpu::X64ExecuteStatus::Ok &&
              !load_result.memory_result.has_value() &&
              state.rip == 0x2000U + store.length + load.length &&
              state.gpr(cpu::X64GeneralRegister::Rdx) == value,
          "T2 load via the virtual seam advances RIP once more and returns the stored value");
}

void test_x64_via_execute_permission_denied_at_mapping_and_region() {
    using namespace xbridge;

    constexpr std::uint64_t physical_base = 0x2'0000'0000ULL;
    constexpr std::uint64_t virtual_base = 0x1'0000'0000ULL;
    const std::uint8_t nop[] = {0x90U};

    const auto check_denied_fetch = [](memory::GuestPhysicalMemory& physical,
                                       memory::GuestAddressSpace& space,
                                       std::uint64_t physical_address,
                                       std::uint64_t virtual_address,
                                       const char* label) {
        memory::GuestAddressSpaceView view(space);
        auto state = make_integration_state(virtual_address);
        const auto before = state;
        bool snapshot_ok = false;
        const auto before_bytes = read_physical_snapshot(
            physical, memory::GuestPhysicalAddress{physical_address}, 1U, snapshot_ok);
        check(snapshot_ok, (std::string(label) + ": physical snapshot is readable").c_str());

        const auto result = cpu::step_x64_at_rip_via(state, view);
        check(result.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                  result.memory_result.has_value() &&
                  result.memory_result->fault == memory::MemoryFault::AccessDenied &&
                  result.memory_result->access == memory::MemoryAccess::Execute &&
                  result.memory_result->context == memory::MemoryAccessContext::Fetch &&
                  result.memory_result->address == virtual_address &&
                  result.memory_result->requested_length == 15U,
              (std::string(label) + ": fetch reports AccessDenied on the first 15-byte attempt").c_str());
        check(x64_state_equals(before, state),
              (std::string(label) + ": denied fetch leaves RIP, RFLAGS and GPRs unchanged").c_str());

        bool after_ok = false;
        const auto after_bytes = read_physical_snapshot(
            physical, memory::GuestPhysicalAddress{physical_address}, 1U, after_ok);
        check(after_ok && after_bytes == before_bytes,
              (std::string(label) + ": denied fetch leaves guest bytes unchanged").c_str());
    };

    {
        memory::GuestPhysicalMemory physical;
        check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 16U,
                                  memory::RegionKind::Normal,
                                  memory::MemoryPermissions{true, true, true}).ok &&
                  physical.write(memory::GuestPhysicalAddress{physical_base},
                                 std::span<const std::uint8_t>(nop),
                                 memory::MemoryAccessContext::Bulk).ok,
              "T3 mapping-denial fixture seeds a fully permitted Normal region");
        memory::GuestAddressSpace space(physical);
        check(space.map(memory::GuestVirtualAddress{virtual_base},
                        memory::GuestPhysicalAddress{physical_base}, 16U,
                        memory::MemoryPermissions{true, true, false}).ok,
              "T3 mapping permission fixture omits execute while the region permits it");
        check_denied_fetch(physical, space, physical_base, virtual_base,
                           "T3 execute missing at mapping layer");
    }

    {
        memory::GuestPhysicalMemory physical;
        check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 16U,
                                  memory::RegionKind::Normal,
                                  memory::MemoryPermissions{true, true, false}).ok &&
                  physical.write(memory::GuestPhysicalAddress{physical_base},
                                 std::span<const std::uint8_t>(nop),
                                 memory::MemoryAccessContext::Bulk).ok,
              "T3 region-denial fixture seeds a region without execute permission");
        memory::GuestAddressSpace space(physical);
        check(space.map(memory::GuestVirtualAddress{virtual_base},
                        memory::GuestPhysicalAddress{physical_base}, 16U,
                        memory::MemoryPermissions{false, false, true}).ok,
              "T3 region permission fixture grants execute at the mapping only");
        check_denied_fetch(physical, space, physical_base, virtual_base,
                           "T3 execute missing at region layer");
    }
}

void test_x64_via_data_permission_denied_no_partial_write() {
    using namespace xbridge;

    constexpr std::uint64_t data_offset = 0x40ULL;
    constexpr std::uint64_t value = 0xDEADBEEFULL;
    const std::uint8_t store_bytes[] = {0x89U, 0x08U};
    const std::uint8_t load_bytes[] = {0x8BU, 0x08U};
    const auto store = cpu::decode_x64_instruction(store_bytes);
    const auto load = cpu::decode_x64_instruction(load_bytes);

    const auto check_store_denied = [](memory::IGuestVirtualMemory& view,
                                       memory::GuestPhysicalMemory& physical,
                                       std::uint64_t physical_base,
                                       std::uint64_t address,
                                       const cpu::DecodedInstruction& instruction,
                                       const char* label) {
        auto state = make_integration_state(0x2000U);
        state.gpr(cpu::X64GeneralRegister::Rax) = address;
        state.gpr(cpu::X64GeneralRegister::Rcx) = value;
        const auto before = state;
        bool snapshot_ok = false;
        const auto before_bytes = read_physical_snapshot(
            physical, memory::GuestPhysicalAddress{physical_base + data_offset}, 4U,
            snapshot_ok);
        check(snapshot_ok, (std::string(label) + ": backing bytes are readable").c_str());

        const auto result = cpu::execute_instruction_via(instruction, state, view);
        check(result.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                  result.memory_result.has_value() &&
                  result.memory_result->fault == memory::MemoryFault::AccessDenied &&
                  result.memory_result->access == memory::MemoryAccess::Write &&
                  result.memory_result->context == memory::MemoryAccessContext::Data &&
                  result.memory_result->address == address &&
                  result.memory_result->requested_length == 4U,
              (std::string(label) + ": store reports AccessDenied with full diagnostics").c_str());
        check(x64_state_equals(before, state),
              (std::string(label) + ": denied store leaves CPU state unchanged").c_str());

        bool after_ok = false;
        const auto after_bytes = read_physical_snapshot(
            physical, memory::GuestPhysicalAddress{physical_base + data_offset}, 4U,
            after_ok);
        check(after_ok && after_bytes == before_bytes,
              (std::string(label) + ": denied store writes no partial bytes").c_str());
    };

    const auto check_load_denied = [](memory::IGuestVirtualMemory& view,
                                      std::uint64_t address,
                                      const cpu::DecodedInstruction& instruction,
                                      const char* label) {
        auto state = make_integration_state(0x2000U);
        state.gpr(cpu::X64GeneralRegister::Rax) = address;
        state.gpr(cpu::X64GeneralRegister::Rcx) = 0x5A5A5A5AULL;
        const auto before = state;

        const auto result = cpu::execute_instruction_via(instruction, state, view);
        check(result.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                  result.memory_result.has_value() &&
                  result.memory_result->fault == memory::MemoryFault::AccessDenied &&
                  result.memory_result->access == memory::MemoryAccess::Read &&
                  result.memory_result->context == memory::MemoryAccessContext::Data &&
                  result.memory_result->address == address &&
                  result.memory_result->requested_length == 4U,
              (std::string(label) + ": load reports AccessDenied with full diagnostics").c_str());
        check(x64_state_equals(before, state),
              (std::string(label) + ": denied load leaves CPU state unchanged").c_str());
    };

    {
        constexpr std::uint64_t physical_base = 0x2'0000'0000ULL;
        constexpr std::uint64_t virtual_base = 0x1'0000'0000ULL;
        memory::GuestPhysicalMemory physical;
        check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 0x80U,
                                  memory::RegionKind::Normal,
                                  memory::MemoryPermissions{true, true, true}).ok,
              "T4 mapping-denial fixture adds a fully permitted region");
        memory::GuestAddressSpace space(physical);
        check(space.map(memory::GuestVirtualAddress{virtual_base},
                        memory::GuestPhysicalAddress{physical_base}, 0x80U,
                        memory::MemoryPermissions{false, false, true}).ok,
              "T4 mapping allows execute but denies both data accesses");
        memory::GuestAddressSpaceView view(space);
        check_store_denied(view, physical, physical_base, virtual_base + data_offset, store,
                           "T4 execute-only mapping store");
        check_load_denied(view, virtual_base + data_offset, load,
                          "T4 execute-only mapping load");
    }

    {
        constexpr std::uint64_t physical_base = 0x3'0000'0000ULL;
        constexpr std::uint64_t virtual_base = 0x1'0000'1000ULL;
        memory::GuestPhysicalMemory physical;
        check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 0x80U,
                                  memory::RegionKind::Normal,
                                  memory::MemoryPermissions{true, false, true}).ok,
              "T4 region-write-denial fixture adds a readable execute region");
        memory::GuestAddressSpace space(physical);
        check(space.map(memory::GuestVirtualAddress{virtual_base},
                        memory::GuestPhysicalAddress{physical_base}, 0x80U,
                        memory::MemoryPermissions{true, true, true}).ok,
              "T4 mapping grants data accesses while the region denies write");
        memory::GuestAddressSpaceView view(space);
        check_store_denied(view, physical, physical_base, virtual_base + data_offset, store,
                           "T4 region without write permission");
    }

    {
        constexpr std::uint64_t physical_base = 0x4'0000'0000ULL;
        constexpr std::uint64_t virtual_base = 0x1'0000'2000ULL;
        const std::array<std::uint8_t, 4U> pattern{0x11U, 0x22U, 0x33U, 0x44U};
        memory::GuestPhysicalMemory physical;
        check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 0x80U,
                                  memory::RegionKind::Normal,
                                  memory::MemoryPermissions{false, true, true}).ok &&
                  physical.write(memory::GuestPhysicalAddress{physical_base + data_offset},
                                 std::span<const std::uint8_t>(pattern),
                                 memory::MemoryAccessContext::Bulk).ok,
              "T4 region-read-denial fixture seeds a writable execute region");
        memory::GuestAddressSpace space(physical);
        check(space.map(memory::GuestVirtualAddress{virtual_base},
                        memory::GuestPhysicalAddress{physical_base}, 0x80U,
                        memory::MemoryPermissions{true, true, true}).ok,
              "T4 mapping grants data reads while the region denies read");
        memory::GuestAddressSpaceView view(space);
        check_load_denied(view, virtual_base + data_offset, load,
                          "T4 region without read permission");
    }
}

void test_x64_via_invalid_addresses_no_truncation() {
    using namespace xbridge;

    constexpr std::uint64_t physical_base = 0x2'0000'0000ULL;
    constexpr std::uint64_t low_virtual = 0x40ULL;
    constexpr std::uint64_t first_span_virtual = 0x100ULL;
    constexpr std::uint64_t second_span_virtual = first_span_virtual + 4U;
    constexpr std::uint64_t gap_virtual = 0x180ULL;
    constexpr std::uint64_t high_virtual = 0x1'0000'0040ULL;
    constexpr std::uint64_t maximum = (std::numeric_limits<std::uint64_t>::max)();
    constexpr std::uint64_t value = 0xCAFEBABEULL;
    const std::uint8_t store4_bytes[] = {0x89U, 0x08U};
    const std::uint8_t store8_bytes[] = {0x48U, 0x89U, 0x08U};
    const auto store4 = cpu::decode_x64_instruction(store4_bytes);
    const auto store8 = cpu::decode_x64_instruction(store8_bytes);

    memory::GuestPhysicalMemory physical;
    check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 0x100U,
                              memory::RegionKind::Normal,
                              memory::MemoryPermissions{true, true, true}).ok,
          "T5 invalid-address fixture adds a 256-byte Normal region above 4 GiB");
    memory::GuestAddressSpace space(physical);
    check(space.map(memory::GuestVirtualAddress{low_virtual},
                    memory::GuestPhysicalAddress{physical_base}, 0x10U,
                    memory::MemoryPermissions{true, true, false}).ok &&
              space.map(memory::GuestVirtualAddress{first_span_virtual},
                        memory::GuestPhysicalAddress{physical_base + 0x20U}, 4U,
                        memory::MemoryPermissions{true, true, false}).ok &&
              space.map(memory::GuestVirtualAddress{second_span_virtual},
                        memory::GuestPhysicalAddress{physical_base + 0x24U}, 4U,
                        memory::MemoryPermissions{true, true, false}).ok,
          "T5 maps a low range and two touching ranges for truncation and spanning tests");
    memory::GuestAddressSpaceView view(space);

    {
        auto control = make_integration_state(0x2000U);
        control.gpr(cpu::X64GeneralRegister::Rax) = low_virtual;
        control.gpr(cpu::X64GeneralRegister::Rcx) = value;
        const auto control_result = cpu::execute_instruction_via(store4, control, view);
        std::array<std::uint8_t, 4U> control_bytes{};
        const auto control_read = physical.read(
            memory::GuestPhysicalAddress{physical_base},
            std::span<std::uint8_t>(control_bytes), memory::MemoryAccessContext::Data);
        check(control_result == cpu::X64ExecuteStatus::Ok && control_read.ok &&
                  control_bytes[0] == 0xBEU && control_bytes[1] == 0xBAU &&
                  control_bytes[2] == 0xFEU && control_bytes[3] == 0xCAU,
              "T5 positive control proves the low 32-bit mapping would accept a truncated address");
    }

    bool snapshot_ok = false;
    const auto backing_before = read_physical_snapshot(
        physical, memory::GuestPhysicalAddress{physical_base}, 0x100U, snapshot_ok);
    check(snapshot_ok, "T5 full physical backing snapshot is readable");

    const auto check_store_failure = [&](std::uint64_t address,
                                         const cpu::DecodedInstruction& instruction,
                                         memory::MemoryFault expected_fault,
                                         std::uint64_t expected_length,
                                         const char* label) {
        auto state = make_integration_state(0x2000U);
        state.gpr(cpu::X64GeneralRegister::Rax) = address;
        state.gpr(cpu::X64GeneralRegister::Rcx) = value;
        const auto before = state;

        const auto result = cpu::execute_instruction_via(instruction, state, view);
        check(result.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                  result.memory_result.has_value() &&
                  result.memory_result->fault == expected_fault &&
                  result.memory_result->access == memory::MemoryAccess::Write &&
                  result.memory_result->context == memory::MemoryAccessContext::Data &&
                  result.memory_result->address == address &&
                  result.memory_result->requested_length == expected_length,
              (std::string(label) + ": reports the expected fault and full 64-bit address").c_str());
        check(x64_state_equals(before, state),
              (std::string(label) + ": leaves RIP, RFLAGS and GPRs unchanged").c_str());
    };

    check_store_failure(maximum, store4, memory::MemoryFault::AddressOverflow, 4U,
                        "T5 store at UINT64_MAX");
    check_store_failure(gap_virtual, store4, memory::MemoryFault::Unmapped, 4U,
                        "T5 store into an unmapped gap");
    check_store_failure(first_span_virtual, store8, memory::MemoryFault::Unmapped, 8U,
                        "T5 8-byte store spanning two mappings");
    check_store_failure(high_virtual, store4, memory::MemoryFault::Unmapped, 4U,
                        "T5 high address with a valid truncated low address");

    bool after_ok = false;
    const auto backing_after = read_physical_snapshot(
        physical, memory::GuestPhysicalAddress{physical_base}, 0x100U, after_ok);
    check(after_ok && backing_after == backing_before,
          "T5 all invalid-address stores leave physical backing byte-for-byte unchanged");
}

void test_x64_step_at_rip_via_bounded_fetch_retry() {
    using namespace xbridge;

    constexpr std::uint64_t physical_base = 0x2'0000'0000ULL;
    constexpr std::uint64_t virtual_base = 0x1'0000'0000ULL;
    const std::array<std::uint64_t, 4U> lengths{1U, 4U, 14U, 15U};
    const std::uint8_t nop[] = {0x90U};

    memory::GuestPhysicalMemory physical;
    check(physical.add_region(memory::GuestPhysicalAddress{physical_base}, 0x100U,
                              memory::RegionKind::Normal,
                              memory::MemoryPermissions{false, true, true}).ok,
          "T6 bounded-retry fixture adds a writable execute region");
    memory::GuestAddressSpace space(physical);

    for (std::size_t index = 0U; index < lengths.size(); ++index) {
        const auto length = lengths[index];
        const auto physical_address = physical_base + static_cast<std::uint64_t>(index * 0x10U);
        const auto virtual_address = virtual_base + static_cast<std::uint64_t>(index * 0x10U);
        check(space.map(memory::GuestVirtualAddress{virtual_address},
                        memory::GuestPhysicalAddress{physical_address}, length,
                        memory::MemoryPermissions{false, false, true}).ok &&
                  physical.write(memory::GuestPhysicalAddress{physical_address + length - 1U},
                                 std::span<const std::uint8_t>(nop),
                                 memory::MemoryAccessContext::Bulk).ok,
              (std::string("T6 fixture maps length ") + std::to_string(length) +
               " with a NOP at the last byte").c_str());
    }
    constexpr std::uint64_t failure_virtual = virtual_base + 0x50U;
    check(space.map(memory::GuestVirtualAddress{failure_virtual},
                    memory::GuestPhysicalAddress{physical_base + 0x50U}, 4U,
                    memory::MemoryPermissions{false, false, true}).ok,
          "T6 one-past-end fixture maps exactly four bytes");
    RecordingAddressSpaceView view(space);

    for (std::size_t index = 0U; index < lengths.size(); ++index) {
        const auto length = lengths[index];
        const auto virtual_address = virtual_base + static_cast<std::uint64_t>(index * 0x10U);
        auto state = make_integration_state(virtual_address + length - 1U);
        const auto rflags_before = state.rflags;
        view.reset();

        const auto result = cpu::step_x64_at_rip_via(state, view);
        check(result == cpu::X64ExecuteStatus::Ok && !result.memory_result.has_value() &&
                  state.rip == virtual_address + length && state.rflags == rflags_before,
              (std::string("T6 mapping length ") + std::to_string(length) +
               ": bounded retry reaches the NOP and advances RIP once").c_str());
        // The NOP is placed at the final byte of each mapping, so the bounded
        // retry succeeds only when it descends to a one-byte fetch window.
        check(view.fetch_calls == 15U && view.max_fetch_length == 15U &&
                  view.oversized_calls == 0U,
              (std::string("T6 mapping length ") + std::to_string(length) +
               ": attempts descend from 15 to one byte without an oversized fetch").c_str());
    }

    auto failure_state = make_integration_state(failure_virtual + 4U);
    const auto failure_before = failure_state;
    view.reset();
    const auto failure = cpu::step_x64_at_rip_via(failure_state, view);
    check(failure.status == cpu::X64ExecuteStatus::MemoryAccessError &&
              failure.memory_result.has_value() &&
              failure.memory_result->fault == memory::MemoryFault::Unmapped &&
              failure.memory_result->access == memory::MemoryAccess::Execute &&
              failure.memory_result->context == memory::MemoryAccessContext::Fetch &&
              failure.memory_result->address == failure_virtual + 4U &&
              failure.memory_result->requested_length == 1U,
          "T6 no complete instruction reports the exact final Unmapped fault");
    check(x64_state_equals(failure_before, failure_state) &&
              view.fetch_calls == 15U && view.max_fetch_length == 15U &&
              view.oversized_calls == 0U,
          "T6 failed bounded retry changes no CPU state and never fetches more than 15 bytes");
}

void test_x64_via_memory_fault_propagation() {
    using namespace xbridge;

    constexpr std::array<memory::MemoryFault, 9U> all_faults{
        memory::MemoryFault::None,
        memory::MemoryFault::Unmapped,
        memory::MemoryFault::AccessDenied,
        memory::MemoryFault::AddressOverflow,
        memory::MemoryFault::InvalidRange,
        memory::MemoryFault::Alignment,
        memory::MemoryFault::ResourceExhausted,
        memory::MemoryFault::DeviceAccessRequired,
        memory::MemoryFault::Unsupported};
    const std::uint8_t load_bytes[] = {0x8BU, 0x08U};
    const std::uint8_t store_bytes[] = {0x89U, 0x08U};
    const auto load = cpu::decode_x64_instruction(load_bytes);
    const auto store = cpu::decode_x64_instruction(store_bytes);

    for (const auto fault : all_faults) {
        const std::string label = std::string("T7 fault ") + memory_fault_name(fault);

        {
            const auto result = make_diagnostic_failure(
                fault, memory::MemoryAccess::Execute, memory::MemoryAccessContext::Fetch, 15U);
            ConfigurableVirtualMemory fake;
            fake.fetch_result = result;
            auto state = make_integration_state(0x1000U);
            const auto before = state;

            const auto execution = cpu::step_x64_at_rip_via(state, fake);
            const auto expected_calls =
                (result.fault == memory::MemoryFault::Unmapped ||
                 result.fault == memory::MemoryFault::InvalidRange) ? 15U : 1U;
            check(execution.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                      execution.memory_result.has_value() &&
                      memory_results_equal(*execution.memory_result, result),
                  (label + ": fetch failure preserves the original MemoryResult").c_str());
            check(x64_state_equals(before, state) && fake.fetch_calls == expected_calls,
                  (label + ": fetch failure changes no CPU state and retries only permitted faults").c_str());
        }

        {
            const auto result = make_diagnostic_failure(
                fault, memory::MemoryAccess::Read, memory::MemoryAccessContext::Data, 4U);
            ConfigurableVirtualMemory fake;
            fake.read_result = result;
            auto state = make_integration_state(0x2000U);
            state.gpr(cpu::X64GeneralRegister::Rax) = 0x3000U;
            state.gpr(cpu::X64GeneralRegister::Rcx) = 0x5A5A5A5AULL;
            const auto before = state;

            const auto execution = cpu::execute_instruction_via(load, state, fake);
            check(execution.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                      execution.memory_result.has_value() &&
                      memory_results_equal(*execution.memory_result, result),
                  (label + ": data-read failure preserves the original MemoryResult").c_str());
            check(x64_state_equals(before, state) && fake.read_calls == 1U &&
                      fake.write_calls == 0U,
                  (label + ": data-read failure changes no CPU state and reads once").c_str());
        }

        {
            const auto result = make_diagnostic_failure(
                fault, memory::MemoryAccess::Write, memory::MemoryAccessContext::Data, 4U);
            ConfigurableVirtualMemory fake;
            fake.write_result = result;
            auto state = make_integration_state(0x2000U);
            state.gpr(cpu::X64GeneralRegister::Rax) = 0x3000U;
            state.gpr(cpu::X64GeneralRegister::Rcx) = 0x5A5A5A5AULL;
            const auto before = state;

            const auto execution = cpu::execute_instruction_via(store, state, fake);
            check(execution.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                      execution.memory_result.has_value() &&
                      memory_results_equal(*execution.memory_result, result),
                  (label + ": data-write failure preserves the original MemoryResult").c_str());
            check(x64_state_equals(before, state) && fake.write_calls == 1U &&
                      fake.read_calls == 0U,
                  (label + ": data-write failure changes no CPU state and writes once").c_str());
        }
    }
}

void test_x64_virtual_memory_short_completion_failure() {
    using namespace xbridge;

    const std::uint8_t load_bytes[] = {0x8BU, 0x08U};
    const std::uint8_t store_bytes[] = {0x89U, 0x08U};
    const auto load = cpu::decode_x64_instruction(load_bytes);
    const auto store = cpu::decode_x64_instruction(store_bytes);

    {
        const auto result = make_diagnostic_short_success(
            memory::MemoryAccess::Execute, memory::MemoryAccessContext::Fetch, 15U, 3U);
        ConfigurableVirtualMemory fake;
        fake.fetch_result = result;
        fake.fill_fetch_buffer = true;
        auto state = make_integration_state(0x1000U);
        const auto before = state;

        const auto execution = cpu::step_x64_at_rip_via(state, fake);
        check(execution.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                  execution.memory_result.has_value() &&
                  memory_results_equal(*execution.memory_result, result) &&
                  fake.fetch_calls == 1U,
              "T7 short fetch completion is preserved and rejected without retry");
        check(x64_state_equals(before, state),
              "T7 short fetch completion changes no CPU state");
    }

    {
        const auto result = make_diagnostic_short_success(
            memory::MemoryAccess::Read, memory::MemoryAccessContext::Data, 4U, 2U);
        ConfigurableVirtualMemory fake;
        fake.read_result = result;
        fake.fill_read_buffer = true;
        auto state = make_integration_state(0x2000U);
        state.gpr(cpu::X64GeneralRegister::Rax) = 0x3000U;
        state.gpr(cpu::X64GeneralRegister::Rcx) = 0x5A5A5A5AULL;
        const auto before = state;

        const auto execution = cpu::execute_instruction_via(load, state, fake);
        check(execution.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                  execution.memory_result.has_value() &&
                  memory_results_equal(*execution.memory_result, result) &&
                  fake.read_calls == 1U,
              "T7 short data-read completion is preserved unchanged");
        check(x64_state_equals(before, state),
              "T7 short data-read completion never commits a destination GPR");
    }

    {
        const auto result = make_diagnostic_short_success(
            memory::MemoryAccess::Write, memory::MemoryAccessContext::Data, 4U, 2U);
        ConfigurableVirtualMemory fake;
        fake.write_result = result;
        auto state = make_integration_state(0x2000U);
        state.gpr(cpu::X64GeneralRegister::Rax) = 0x3000U;
        state.gpr(cpu::X64GeneralRegister::Rcx) = 0xDEADBEEFULL;
        const auto before = state;

        const auto execution = cpu::execute_instruction_via(store, state, fake);
        check(execution.status == cpu::X64ExecuteStatus::MemoryAccessError &&
                  execution.memory_result.has_value() &&
                  memory_results_equal(*execution.memory_result, result) &&
                  fake.write_calls == 1U && fake.last_write_size == 4U &&
                  fake.last_write_source[0] == 0xEFU && fake.last_write_source[1] == 0xBEU &&
                  fake.last_write_source[2] == 0xADU && fake.last_write_source[3] == 0xDEU,
              "T7 short data-write completion receives the full little-endian source once");
        check(x64_state_equals(before, state),
              "T7 short data-write completion changes no CPU state");
    }
}

struct LegacyParityCase {
    std::size_t memory_size;
    std::uint32_t load_address;
    std::vector<std::uint8_t> load_bytes;
    std::uint64_t rip;
    std::uint64_t rax;
    std::uint64_t rcx;
    std::uint64_t rdx;
    std::vector<std::uint8_t> instruction_bytes;
    const char* label;
};

std::vector<LegacyParityCase> legacy_parity_cases() {
    std::vector<LegacyParityCase> cases;
    cases.push_back({512U, 0x100U,
                     {0x90U, 0xB8U, 0x78U, 0x56U, 0x34U, 0x12U, 0x90U},
                     0x100U, 0xAAAAAAAAAAAAAAAAULL, 0xBBBBBBBBBBBBBBBBULL, 0U,
                     {0x90U}, "success NOP"});
    cases.push_back({512U, 0x100U,
                     {0x90U, 0xB8U, 0x78U, 0x56U, 0x34U, 0x12U, 0x90U},
                     0x100U, 0U, 0xBBBBBBBBBBBBBBBBULL, 0U,
                     {0xB8U, 0x78U, 0x56U, 0x34U, 0x12U}, "success MOV imm32"});
    cases.push_back({256U, 0U, {}, 0x200U, 0x10U, 0xDEADBEEFULL, 0U,
                     {0x89U, 0x08U}, "success 4-byte store"});
    cases.push_back({16U, 0U, {0x89U, 0x08U}, 0U, 0x0DU, 0xDEADBEEFULL, 0U,
                     {0x89U, 0x08U}, "out-of-bounds 4-byte store"});
    cases.push_back({64U, 0U, {0x89U, 0x08U}, 0U, 0x1'0000'0000ULL, 0xDEADBEEFULL, 0U,
                     {0x89U, 0x08U}, "store above UINT32_MAX"});
    cases.push_back({16U, 15U, {0x90U}, 15U, 0x1111111111111111ULL,
                     0x2222222222222222ULL, 0x3333333333333333ULL,
                     {0x90U}, "NOP at the last valid byte"});
    cases.push_back({16U, 0U, {}, 16U, 0x1111111111111111ULL,
                     0x2222222222222222ULL, 0x3333333333333333ULL,
                     {0x90U}, "RIP one past the last valid byte"});
    return cases;
}

memory::GuestMemory make_parity_memory(const LegacyParityCase& test_case) {
    memory::GuestMemory memory(test_case.memory_size);
    if (!test_case.load_bytes.empty()) {
        memory.load(test_case.load_address, test_case.load_bytes);
    }
    return memory;
}

cpu::X64CpuState make_parity_state(const LegacyParityCase& test_case) {
    auto state = make_integration_state(test_case.rip);
    state.gpr(cpu::X64GeneralRegister::Rax) = test_case.rax;
    state.gpr(cpu::X64GeneralRegister::Rcx) = test_case.rcx;
    state.gpr(cpu::X64GeneralRegister::Rdx) = test_case.rdx;
    return state;
}

std::vector<std::uint8_t> snapshot_guest_memory(const memory::GuestMemory& memory) {
    std::vector<std::uint8_t> bytes(memory.size());
    for (std::size_t address = 0U; address < memory.size(); ++address) {
        bytes[address] = memory.read8(static_cast<std::uint32_t>(address));
    }
    return bytes;
}

void check_parity_outcome(const cpu::X64ExecutionResult& via_result,
                          cpu::X64ExecuteStatus legacy_status,
                          const cpu::X64CpuState& legacy_state,
                          const cpu::X64CpuState& via_state,
                          const memory::GuestMemory& legacy_memory,
                          const memory::GuestMemory& via_memory,
                          const char* label) {
    const std::string prefix = std::string("T8 ") + label;
    check(via_result.status == legacy_status,
          (prefix + ": identical execution status").c_str());
    check(x64_state_equals(legacy_state, via_state),
          (prefix + ": identical RIP, RFLAGS and GPRs").c_str());
    check(snapshot_guest_memory(legacy_memory) == snapshot_guest_memory(via_memory),
          (prefix + ": identical guest-memory bytes").c_str());
    if (legacy_status == cpu::X64ExecuteStatus::Ok) {
        check(!via_result.memory_result.has_value(),
              (prefix + ": success leaves memory_result empty").c_str());
    } else {
        check(via_result.memory_result.has_value() &&
                  result_invariant(*via_result.memory_result) &&
                  via_result.memory_result->fault != memory::MemoryFault::None,
              (prefix + ": failure carries a valid MemoryResult").c_str());
    }
}

void test_x64_execute_instruction_via_legacy_parity() {
    using namespace xbridge;

    for (const auto& test_case : legacy_parity_cases()) {
        auto legacy_memory = make_parity_memory(test_case);
        auto via_memory = make_parity_memory(test_case);
        auto legacy_state = make_parity_state(test_case);
        auto via_state = make_parity_state(test_case);
        const auto instruction = cpu::decode_x64_instruction(std::span<const std::uint8_t>(
            test_case.instruction_bytes.data(), test_case.instruction_bytes.size()));
        check(instruction.status == cpu::X64DecodeStatus::Ok,
              (std::string("T8 execute_instruction_via ") + test_case.label +
               ": instruction decodes Ok").c_str());

        const auto legacy_status =
            cpu::execute_instruction(instruction, legacy_state, legacy_memory);
        memory::GuestMemoryView view(via_memory);
        const auto via_result = cpu::execute_instruction_via(instruction, via_state, view);
        check_parity_outcome(via_result, legacy_status, legacy_state, via_state,
                             legacy_memory, via_memory,
                             (std::string("execute_instruction_via ") + test_case.label).c_str());
    }
}

void test_x64_step_instruction_via_legacy_parity() {
    using namespace xbridge;

    for (const auto& test_case : legacy_parity_cases()) {
        auto legacy_memory = make_parity_memory(test_case);
        auto via_memory = make_parity_memory(test_case);
        auto legacy_state = make_parity_state(test_case);
        auto via_state = make_parity_state(test_case);
        const std::span<const std::uint8_t> bytes(
            test_case.instruction_bytes.data(), test_case.instruction_bytes.size());

        const auto legacy_status = cpu::step_x64_instruction(bytes, legacy_state, legacy_memory);
        memory::GuestMemoryView view(via_memory);
        const auto via_result = cpu::step_x64_instruction_via(bytes, via_state, view);
        check_parity_outcome(via_result, legacy_status, legacy_state, via_state,
                             legacy_memory, via_memory,
                             (std::string("step_x64_instruction_via ") + test_case.label).c_str());
    }
}

void test_x64_step_at_rip_via_legacy_parity() {
    using namespace xbridge;

    for (const auto& test_case : legacy_parity_cases()) {
        auto legacy_memory = make_parity_memory(test_case);
        auto via_memory = make_parity_memory(test_case);
        auto legacy_state = make_parity_state(test_case);
        auto via_state = make_parity_state(test_case);

        const auto legacy_status = cpu::step_x64_at_rip(legacy_state, legacy_memory);
        memory::GuestMemoryView view(via_memory);
        const auto via_result = cpu::step_x64_at_rip_via(via_state, view);
        check_parity_outcome(via_result, legacy_status, legacy_state, via_state,
                             legacy_memory, via_memory,
                             (std::string("step_x64_at_rip_via ") + test_case.label).c_str());
    }
}

// XB-NEXT-DEVELOPMENT-02-F — analyzer tests with synthetic data.

constexpr std::uint16_t analysis_pe_offset = 0x80U;
constexpr std::uint16_t analysis_optional_size = 0xF0U;
constexpr std::uint32_t analysis_section_rva = 0x1000U;
constexpr std::size_t analysis_section_raw_offset = 0x400U;
constexpr std::size_t analysis_section_raw_size = 0x1000U;

void analysis_put_u16(std::vector<std::uint8_t>& bytes, std::size_t at, std::uint16_t value) {
    if (at + 2U > bytes.size())
        return;
    bytes[at] = static_cast<std::uint8_t>(value & 0xFFU);
    bytes[at + 1U] = static_cast<std::uint8_t>((value >> 8U) & 0xFFU);
}

void analysis_put_u32(std::vector<std::uint8_t>& bytes, std::size_t at, std::uint32_t value) {
    if (at + 4U > bytes.size())
        return;
    for (std::size_t index = 0U; index < 4U; ++index) {
        bytes[at + index] = static_cast<std::uint8_t>((value >> (8U * index)) & 0xFFU);
    }
}

void analysis_put_u64(std::vector<std::uint8_t>& bytes, std::size_t at, std::uint64_t value) {
    if (at + 8U > bytes.size())
        return;
    for (std::size_t index = 0U; index < 8U; ++index) {
        bytes[at + index] = static_cast<std::uint8_t>((value >> (8U * index)) & 0xFFU);
    }
}

void analysis_put_ascii(std::vector<std::uint8_t>& bytes, std::size_t at,
                        std::string_view text) {
    for (std::size_t index = 0U; index < text.size(); ++index) {
        if (at + index >= bytes.size())
            return;
        bytes[at + index] = static_cast<std::uint8_t>(static_cast<unsigned char>(text[index]));
    }
}

std::size_t analysis_rva_offset(std::uint32_t rva) noexcept {
    return analysis_section_raw_offset + static_cast<std::size_t>(rva - analysis_section_rva);
}

std::vector<std::uint8_t> analysis_bytes_from_text(std::string_view text) {
    std::vector<std::uint8_t> bytes;
    bytes.reserve(text.size());
    for (char c : text) {
        bytes.push_back(static_cast<std::uint8_t>(static_cast<unsigned char>(c)));
    }
    return bytes;
}

// Builds a small, self-contained PE32+ image whose directories all point into
// one Normal-looking .rdata section. No bytes are derived from a real binary.
std::vector<std::uint8_t> analysis_make_pe32plus_x64(std::uint32_t timestamp) {
    std::vector<std::uint8_t> bytes(0x1400U, std::uint8_t{0U});
    bytes[0U] = static_cast<std::uint8_t>('M');
    bytes[1U] = static_cast<std::uint8_t>('Z');
    analysis_put_u32(bytes, 0x3CU, analysis_pe_offset);

    const std::size_t coff = static_cast<std::size_t>(analysis_pe_offset) + 4U;
    bytes[analysis_pe_offset] = static_cast<std::uint8_t>('P');
    bytes[analysis_pe_offset + 1U] = static_cast<std::uint8_t>('E');
    analysis_put_u16(bytes, coff, 0x8664U);
    analysis_put_u16(bytes, coff + 2U, 1U);
    analysis_put_u32(bytes, coff + 4U, timestamp);
    analysis_put_u16(bytes, coff + 16U, analysis_optional_size);
    analysis_put_u16(bytes, coff + 18U, 0x0022U);

    const std::size_t optional = coff + 20U;
    analysis_put_u16(bytes, optional, 0x020BU);
    analysis_put_u32(bytes, optional + 16U, 0x1000U);
    analysis_put_u64(bytes, optional + 24U, 0x0000000140000000ULL);
    analysis_put_u32(bytes, optional + 60U, 0x400U);
    analysis_put_u16(bytes, optional + 68U, 14U);
    analysis_put_u16(bytes, optional + 70U, 0x0060U);
    analysis_put_u32(bytes, optional + 108U, 16U);

    const auto set_directory = [&bytes, optional](std::size_t index, std::uint32_t rva,
                                                  std::uint32_t size) {
        const std::size_t at = optional + 112U + index * 8U;
        analysis_put_u32(bytes, at, rva);
        analysis_put_u32(bytes, at + 4U, size);
    };
    set_directory(0U, 0x1000U, 0x400U); // export
    set_directory(1U, 0x1400U, 0x40U);  // import
    set_directory(2U, 0x1600U, 0x80U);  // resource
    set_directory(5U, 0x1700U, 0x0CU);  // base relocation
    set_directory(6U, 0x1780U, 0x1CU);  // debug

    const std::size_t section = optional + analysis_optional_size;
    analysis_put_ascii(bytes, section, ".rdata");
    analysis_put_u32(bytes, section + 8U, 0x1000U);
    analysis_put_u32(bytes, section + 12U, analysis_section_rva);
    analysis_put_u32(bytes, section + 16U, static_cast<std::uint32_t>(analysis_section_raw_size));
    analysis_put_u32(bytes, section + 20U, static_cast<std::uint32_t>(analysis_section_raw_offset));

    // Export directory: name, function name, forwarder, EAT, name and ordinal tables.
    analysis_put_u32(bytes, analysis_rva_offset(0x1000U) + 12U, 0x1200U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1000U) + 16U, 1U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1000U) + 20U, 1U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1000U) + 24U, 1U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1000U) + 28U, 0x1300U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1000U) + 32U, 0x1310U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1000U) + 36U, 0x1320U);
    analysis_put_ascii(bytes, analysis_rva_offset(0x1200U), "SynthExport.dll");
    analysis_put_ascii(bytes, analysis_rva_offset(0x1220U), "SyntheticFunction");
    analysis_put_ascii(bytes, analysis_rva_offset(0x1240U), "OTHER.dll.Forwarded");
    analysis_put_u32(bytes, analysis_rva_offset(0x1300U), 0x1240U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1310U), 0x1220U);
    analysis_put_u16(bytes, analysis_rva_offset(0x1320U), 0U);

    // Import directory: one descriptor, one DLL and one named import.
    analysis_put_u32(bytes, analysis_rva_offset(0x1400U), 0x1540U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1400U) + 12U, 0x1500U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1400U) + 16U, 0x1540U);
    analysis_put_ascii(bytes, analysis_rva_offset(0x1500U), "SYNTHAPI.dll");
    analysis_put_u64(bytes, analysis_rva_offset(0x1540U), 0x1560U);
    analysis_put_u16(bytes, analysis_rva_offset(0x1560U), 0x10U);
    analysis_put_ascii(bytes, analysis_rva_offset(0x1560U) + 2U, "SyntheticImport");

    // Resource tree: root type, language subdirectory, one data entry.
    analysis_put_u16(bytes, analysis_rva_offset(0x1600U) + 14U, 1U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1600U) + 16U, 16U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1600U) + 20U, 0x80000020U);
    analysis_put_u16(bytes, analysis_rva_offset(0x1620U) + 14U, 1U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1620U) + 16U, 1U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1620U) + 20U, 0x50U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1650U), 0x1660U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1650U) + 4U, 7U);

    // Base relocations: DIR64 and HIGHLOW entries in one complete block.
    analysis_put_u32(bytes, analysis_rva_offset(0x1700U), 0x1000U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1700U) + 4U, 12U);
    analysis_put_u16(bytes, analysis_rva_offset(0x1700U) + 8U,
                     static_cast<std::uint16_t>((10U << 12) | 0x123U));
    analysis_put_u16(bytes, analysis_rva_offset(0x1700U) + 10U,
                     static_cast<std::uint16_t>((3U << 12) | 0x456U));

    // Debug directory: one CODEVIEW descriptor with in-prefix raw pointers.
    analysis_put_u32(bytes, analysis_rva_offset(0x1780U) + 12U, 2U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1780U) + 20U, 0x17A0U);
    analysis_put_u32(bytes, analysis_rva_offset(0x1780U) + 24U,
                     static_cast<std::uint32_t>(analysis_rva_offset(0x17A0U)));
    return bytes;
}

// Creates the standard synthetic PE32+ image with the root resource type
// replaced by a UTF-16LE name. Every byte is test-generated.
std::vector<std::uint8_t> analysis_make_pe32plus_named_resource(
    std::uint32_t name_rva, std::uint16_t unit_count,
    std::string_view in_cap_units, char after_cap_unit) {
    auto bytes = analysis_make_pe32plus_x64(0x5F5E0FF0U);
    constexpr std::uint32_t resource_root_rva = 0x1600U;

    const std::size_t root = analysis_rva_offset(resource_root_rva);
    analysis_put_u16(bytes, root + 12U, 1U); // one named resource type
    analysis_put_u16(bytes, root + 14U, 0U); // no numeric resource type
    analysis_put_u32(bytes, root + 16U,
                     0x80000000U | (name_rva - resource_root_rva));

    const std::size_t name = analysis_rva_offset(name_rva);
    analysis_put_u16(bytes, name, unit_count);
    for (std::size_t index = 0U; index < unit_count; ++index) {
        const char unit = index < in_cap_units.size() ? in_cap_units[index] : after_cap_unit;
        analysis_put_u16(bytes, name + 2U + index * 2U,
                         static_cast<std::uint16_t>(static_cast<unsigned char>(unit)));
    }
    return bytes;
}

struct AnalysisFixture {
    std::filesystem::path root;
    bool ok{false};
};

AnalysisFixture analysis_make_fixture(std::string_view label) {
    AnalysisFixture fixture;
    std::error_code ec;
    const std::filesystem::path base = std::filesystem::temp_directory_path(ec);
    if (ec)
        return fixture;

    static std::uint64_t sequence = 0U;
    for (unsigned attempt = 0U; attempt < 64U; ++attempt) {
        const auto stamp = static_cast<std::uint64_t>(
            std::chrono::steady_clock::now().time_since_epoch().count());
        const std::string name = "xbridge_analysis_" + std::string(label) + "_" +
                                 std::to_string(stamp) + "_" + std::to_string(sequence++) +
                                 "_" + std::to_string(attempt);
        const std::filesystem::path candidate = base / name;
        std::error_code create_ec;
        if (std::filesystem::create_directories(candidate, create_ec) && !create_ec) {
            fixture.root = candidate;
            fixture.ok = true;
            return fixture;
        }
    }
    return fixture;
}

void analysis_cleanup_fixture(const AnalysisFixture& fixture) {
    if (fixture.root.empty())
        return;
    std::error_code ec;
    (void)std::filesystem::remove_all(fixture.root, ec);
}

bool analysis_write_file(const std::filesystem::path& path,
                         const std::vector<std::uint8_t>& bytes) {
    std::error_code ec;
    if (path.has_parent_path()) {
        (void)std::filesystem::create_directories(path.parent_path(), ec);
        if (ec)
            return false;
    }
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    if (!stream.is_open())
        return false;
    if (!bytes.empty()) {
        stream.write(reinterpret_cast<const char*>(bytes.data()),
                     static_cast<std::streamsize>(bytes.size()));
    }
    stream.close();
    return stream.good();
}

struct AnalysisFileSnapshot {
    bool ok{false};
    std::uintmax_t size{0U};
    std::filesystem::file_time_type modified{};
    std::vector<std::uint8_t> bytes;
};

AnalysisFileSnapshot analysis_snapshot_file(const std::filesystem::path& path) {
    AnalysisFileSnapshot snapshot;
    std::error_code ec;
    snapshot.size = std::filesystem::file_size(path, ec);
    if (ec)
        return snapshot;
    snapshot.modified = std::filesystem::last_write_time(path, ec);
    if (ec)
        return snapshot;

    std::ifstream stream(path, std::ios::binary);
    if (!stream.is_open())
        return snapshot;
    stream.seekg(0U, std::ios::end);
    const std::streamsize length = stream.tellg();
    if (length < 0)
        return snapshot;
    stream.seekg(0U, std::ios::beg);
    snapshot.bytes.resize(static_cast<std::size_t>(length));
    if (length > 0) {
        stream.read(reinterpret_cast<char*>(snapshot.bytes.data()), length);
    }
    snapshot.ok = stream.good() || stream.eof();
    return snapshot;
}

bool analysis_text_contains(std::string_view text, std::string_view needle) noexcept {
    return text.find(needle) != std::string_view::npos;
}

bool analysis_vector_contains(const std::vector<std::string>& values,
                              std::string_view needle) noexcept {
    for (const std::string& value : values) {
        if (analysis_text_contains(value, needle))
            return true;
    }
    return false;
}

bool analysis_vector_has_exact(const std::vector<std::string>& values,
                               std::string_view expected) noexcept {
    for (const std::string& value : values) {
        if (std::string_view(value) == expected)
            return true;
    }
    return false;
}

const analysis::AnalysisEntry* analysis_find_entry(const analysis::AnalysisReport& report,
                                                   std::string_view relative_path) noexcept {
    for (const analysis::AnalysisEntry& entry : report.entries) {
        if (std::string_view(entry.relative_path) == relative_path)
            return &entry;
    }
    return nullptr;
}

bool analysis_entries_sorted(const analysis::AnalysisReport& report) noexcept {
    for (std::size_t index = 1U; index < report.entries.size(); ++index) {
        if (report.entries[index - 1U].relative_path >= report.entries[index].relative_path)
            return false;
    }
    return true;
}

bool analysis_has_false_platform_claim(const analysis::AnalysisEntry& entry) noexcept {
    return analysis_text_contains(entry.architecture, "x64") ||
           analysis_text_contains(entry.architecture, "amd64") ||
           analysis_text_contains(entry.architecture, "pe32plus") ||
           analysis_vector_contains(entry.platform_indicators, "pe_machine=") ||
           analysis_vector_contains(entry.platform_indicators, "pe_subsystem=") ||
           analysis_vector_contains(entry.platform_indicators, "gdk_");
}

bool analysis_has_affirmative_windows_claim(const std::vector<std::string>& values) noexcept {
    for (const std::string& value : values) {
        if (analysis_text_contains(value, "establishes Windows compatibility") ||
            analysis_text_contains(value, "Windows compatibility confirmed") ||
            analysis_text_contains(value, "is Windows compatible"))
            return true;
    }
    return false;
}

void test_analysis_valid_pe32plus_x64_synthetic() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("valid_pe");
    check(fixture.ok, "analyzer valid-PE fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    check(analysis_write_file(fixture.root / "synthetic_pe64.bin",
                              analysis_make_pe32plus_x64(0x5F5E0FF0U)),
          "analyzer valid-PE fixture writes a synthetic PE32+ x64 image");

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer valid-PE scan does not throw");
    check(!error && report.entries.size() == 1U,
          "analyzer valid-PE scan succeeds with exactly one entry");
    const analysis::AnalysisEntry* entry = analysis_find_entry(report, "synthetic_pe64.bin");
    check(entry != nullptr, "analyzer valid-PE entry is found by relative path");
    if (entry == nullptr) {
        analysis_cleanup_fixture(fixture);
        return;
    }

    check(entry->status == analysis::DetectionStatus::Indicator &&
              analysis::status_name(entry->status) == "indicator",
          "synthetic PE32+ x64 image is classified only as an indicator");
    check(entry->architecture == "x64_amd64_pe32plus",
          "synthetic image reports x64 AMD64 PE32+ architecture");
    check(entry->file_type == "pe", "synthetic image reports file_type pe");

    check(analysis_vector_has_exact(
              entry->pe_fields,
              "machine=0x8664 (IMAGE_FILE_MACHINE_AMD64, x64 indicator only)") &&
              analysis_vector_has_exact(entry->pe_fields, "optional_magic=0x020b (PE32+)") &&
              analysis_vector_has_exact(
                  entry->pe_fields,
                  "subsystem=14 (IMAGE_SUBSYSTEM_XBOX indicator only)") &&
              analysis_vector_has_exact(entry->pe_fields, "number_of_sections=1") &&
              analysis_vector_has_exact(entry->pe_fields, "size_of_optional_header=240"),
          "synthetic PE fields contain the confirmed machine, magic, subsystem and header values");
    check(analysis_vector_has_exact(
              entry->pe_fields,
              "machine_magic_agreement=amd64_pe32plus (indicator only; not compatibility or executability)"),
          "machine/magic agreement is labeled as an indicator only");

    check(analysis_vector_has_exact(
              entry->platform_indicators,
              "pe_machine=0x8664+magic=0x20b (AMD64/PE32+ indicator only; not compatibility or executability)") &&
              analysis_vector_has_exact(
                  entry->platform_indicators,
                  "pe_subsystem=14 (IMAGE_SUBSYSTEM_XBOX indicator only; retail use unknown)"),
          "synthetic PE platform indicators are explicitly labeled as indicators");

    check(analysis_vector_has_exact(entry->imports, "SYNTHAPI.dll") &&
              analysis_vector_has_exact(entry->imports, "SYNTHAPI.dll!SyntheticImport") &&
              analysis_vector_has_exact(entry->dependencies, "SYNTHAPI.dll"),
          "synthetic import descriptor, named import and dependency are enumerated");
    check(analysis_vector_has_exact(entry->exports, "export_dll:SynthExport.dll") &&
              analysis_vector_has_exact(entry->exports, "SyntheticFunction (ordinal 1)") &&
              analysis_vector_has_exact(entry->exports, "forwarder:OTHER.dll.Forwarded"),
          "synthetic export name, ordinal and forwarder are enumerated");
    check(analysis_vector_has_exact(entry->relocation_types, "10 (DIR64)") &&
              analysis_vector_has_exact(entry->relocation_types, "3 (HIGHLOW)") &&
              analysis_vector_has_exact(entry->debug_types, "2 (CODEVIEW)") &&
              analysis_vector_has_exact(entry->resource_kinds, "resource_type=id:16") &&
              analysis_vector_has_exact(entry->resource_kinds, "resource_data_size=7"),
          "synthetic relocation, debug and resource directories are enumerated");

    check(analysis_vector_has_exact(
              entry->uncertainties,
              "PE32+ metadata does not establish Windows compatibility or direct executability") &&
              !analysis_has_affirmative_windows_claim(entry->uncertainties),
          "synthetic PE uncertainty explicitly denies a Windows-compatibility claim");

    analysis_cleanup_fixture(fixture);
}

void test_analysis_gdk_manifest_indicators() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("gdk_manifest");
    check(fixture.ok, "analyzer GDK fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    const std::string manifest =
        "<?xml version=\"1.0\"?>\n"
        "<Game TargetDeviceFamily=\"Scarlett\" Architecture=\"x64\" />\n";
    check(analysis_write_file(fixture.root / "MicrosoftGame.config",
                              analysis_bytes_from_text(manifest)),
          "analyzer GDK fixture writes a synthetic MicrosoftGame.config");

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer GDK scan does not throw");
    check(!error && report.entries.size() == 1U,
          "analyzer GDK scan succeeds with exactly one entry");
    const analysis::AnalysisEntry* entry = analysis_find_entry(report, "MicrosoftGame.config");
    check(entry != nullptr, "analyzer GDK entry is found by relative path");
    if (entry == nullptr) {
        analysis_cleanup_fixture(fixture);
        return;
    }

    check(entry->status == analysis::DetectionStatus::Indicator &&
              entry->file_type == "gdk_manifest",
          "GDK manifest is classified as an indicator with file_type gdk_manifest");
    check(analysis_vector_has_exact(
              entry->platform_indicators,
              "gdk_architecture=x64 (manifest indicator only)") &&
              analysis_vector_has_exact(
                  entry->platform_indicators,
                  "gdk_target_device_family=Scarlett (manifest indicator only)"),
          "GDK manifest exposes Scarlett and x64 as declarative indicators only");
    check(entry->imports.empty() && entry->exports.empty() && entry->dependencies.empty() &&
              entry->relocation_types.empty() && entry->debug_types.empty() &&
              !analysis_text_contains(entry->architecture, "x64_amd64_pe32plus"),
          "GDK manifest adds no PE-only fields or x64 PE architecture claim");
    check(analysis_vector_has_exact(
              entry->pe_fields,
              "pe_coff=not_applicable (file is not a PE/COFF image)") &&
              analysis_vector_contains(
                  entry->uncertainties,
                  "manifest values are declarative configuration; not verified platform behavior"),
          "GDK manifest keeps PE fields not applicable and records declarative uncertainty");

    analysis_cleanup_fixture(fixture);
}

void test_analysis_weak_extension_indicators() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("weak_ext");
    check(fixture.ok, "analyzer weak-extension fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    struct WeakCase {
        const char* name;
        const char* file_type;
        const char* indicator;
        const char* uncertainty;
    };
    const std::array<WeakCase, 4U> cases{{
        {"package.xvc", "xvc_container", "container_extension=.xvc",
         "container payload internals unknown"},
        {"package.msixvc", "msixvc_container", "container_extension=.msixvc",
         "container payload internals unknown"},
        {"texture.dds", "dds", "weak_resource_extension=.dds", "false positives"},
        {"pixel.dxil", "shader", "weak_shader_extension=.dxil", "false positives"},
    }};
    const std::vector<std::uint8_t> payload = analysis_bytes_from_text("synthetic weak payload");

    for (const WeakCase& test_case : cases) {
        check(analysis_write_file(fixture.root / test_case.name, payload),
              (std::string("analyzer weak fixture writes ") + test_case.name).c_str());
    }

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer weak-extension scan does not throw");
    check(!error && report.entries.size() == cases.size(),
          "analyzer weak-extension scan succeeds with all four entries");
    check(analysis_entries_sorted(report),
          "analyzer weak-extension entries are sorted by relative path");

    for (const WeakCase& test_case : cases) {
        const analysis::AnalysisEntry* entry = analysis_find_entry(report, test_case.name);
        const std::string prefix = std::string("analyzer weak ") + test_case.name;
        check(entry != nullptr, (prefix + ": entry is present").c_str());
        if (entry == nullptr)
            continue;

        check(entry->status == analysis::DetectionStatus::Indicator &&
                  entry->file_type == test_case.file_type,
              (prefix + ": status is indicator with the expected weak file_type").c_str());
        check(analysis_vector_contains(entry->platform_indicators, test_case.indicator) &&
                  analysis_vector_contains(entry->uncertainties, test_case.uncertainty),
              (prefix + ": weak indicator has explicit unknown/false-positive uncertainty").c_str());
        check(!analysis_has_false_platform_claim(*entry) &&
                  !analysis_vector_contains(entry->uncertainties, "confirmed Xbox format"),
              (prefix + ": makes no confirmed Xbox format claim").c_str());
    }

    analysis_cleanup_fixture(fixture);
}

void test_analysis_malformed_inputs_no_false_classification() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("malformed");
    check(fixture.ok, "analyzer malformed fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    std::vector<std::uint8_t> bare_mz{'M', 'Z'};

    // A short prefix with a non-PE signature: the signature cannot be trusted
    // and the e_lfanew window is incomplete.
    std::vector<std::uint8_t> bad_signature(0x90U, std::uint8_t{0U});
    bad_signature[0U] = static_cast<std::uint8_t>('M');
    bad_signature[1U] = static_cast<std::uint8_t>('Z');
    analysis_put_u32(bad_signature, 0x3CU, analysis_pe_offset);
    analysis_put_ascii(bad_signature, analysis_pe_offset, "PX");

    std::vector<std::uint8_t> oversized_lfanew(0x100U, std::uint8_t{0U});
    oversized_lfanew[0U] = static_cast<std::uint8_t>('M');
    oversized_lfanew[1U] = static_cast<std::uint8_t>('Z');
    analysis_put_u32(oversized_lfanew, 0x3CU, 0xFFFFFFF0U);

    const auto make_truncated_optional = [](std::uint16_t section_count) {
        std::vector<std::uint8_t> bytes(0xD8U, std::uint8_t{0U});
        bytes[0U] = static_cast<std::uint8_t>('M');
        bytes[1U] = static_cast<std::uint8_t>('Z');
        analysis_put_u32(bytes, 0x3CU, analysis_pe_offset);
        const std::size_t coff = static_cast<std::size_t>(analysis_pe_offset) + 4U;
        bytes[analysis_pe_offset] = static_cast<std::uint8_t>('P');
        bytes[analysis_pe_offset + 1U] = static_cast<std::uint8_t>('E');
        analysis_put_u16(bytes, coff, 0x014CU);
        analysis_put_u16(bytes, coff + 2U, section_count);
        analysis_put_u16(bytes, coff + 16U, analysis_optional_size);
        analysis_put_u16(bytes, coff + 20U, 0x010BU);
        return bytes;
    };

    struct MalformedCase {
        const char* name;
        std::vector<std::uint8_t> bytes;
        const char* uncertainty;
    };
    std::vector<MalformedCase> cases;
    cases.push_back({"bare_mz.bin", bare_mz, "DOS header truncated"});
    cases.push_back({"bad_signature.bin", bad_signature, "PE header offset e_lfanew"});
    cases.push_back({"oversized_lfanew.bin", oversized_lfanew, "PE header offset e_lfanew"});
    cases.push_back({"truncated_optional.bin", make_truncated_optional(1U),
                     "optional header is truncated"});
    cases.push_back({"excessive_sections.bin", make_truncated_optional(200U),
                     "NumberOfSections exceeds 96"});

    for (const MalformedCase& test_case : cases) {
        check(analysis_write_file(fixture.root / test_case.name, test_case.bytes),
              (std::string("analyzer malformed fixture writes ") + test_case.name).c_str());
    }

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer malformed scan does not throw");
    check(!error && report.entries.size() == cases.size(),
          "analyzer malformed scan succeeds with all five entries");

    for (const MalformedCase& test_case : cases) {
        const analysis::AnalysisEntry* entry = analysis_find_entry(report, test_case.name);
        const std::string prefix = std::string("analyzer malformed ") + test_case.name;
        check(entry != nullptr, (prefix + ": entry is present").c_str());
        if (entry == nullptr)
            continue;

        check(entry->status == analysis::DetectionStatus::Unknown &&
                  analysis::status_name(entry->status) == "unknown",
              (prefix + ": status is unknown").c_str());
        check(!entry->uncertainties.empty() &&
                  analysis_vector_contains(entry->uncertainties, test_case.uncertainty),
              (prefix + ": expected uncertainty is recorded").c_str());
        check(entry->platform_indicators.empty() && entry->imports.empty() &&
                  entry->exports.empty() && entry->relocation_types.empty() &&
                  entry->debug_types.empty() && entry->resource_kinds.empty() &&
                  !analysis_has_false_platform_claim(*entry),
              (prefix + ": no partial false classification or fabricated metadata").c_str());
    }

    analysis_cleanup_fixture(fixture);
}

void test_analysis_non_date_timestamps() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("timestamps");
    check(fixture.ok, "analyzer timestamp fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    check(analysis_write_file(fixture.root / "ts_zero.bin", analysis_make_pe32plus_x64(0U)) &&
              analysis_write_file(fixture.root / "ts_max.bin",
                                  analysis_make_pe32plus_x64(0xFFFFFFFFU)),
          "analyzer timestamp fixture writes synthetic zero and max timestamps");

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer timestamp scan does not throw");
    check(!error && report.entries.size() == 2U,
          "analyzer timestamp scan succeeds with both entries");

    const analysis::AnalysisEntry* zero = analysis_find_entry(report, "ts_zero.bin");
    const analysis::AnalysisEntry* maximum = analysis_find_entry(report, "ts_max.bin");
    check(zero != nullptr && maximum != nullptr,
          "analyzer timestamp entries are found by relative path");
    if (zero == nullptr || maximum == nullptr) {
        analysis_cleanup_fixture(fixture);
        return;
    }

    check(analysis_vector_has_exact(
              zero->pe_fields,
              "time_date_stamp=0x00000000 (not a date; placeholder or reproducibility value)") &&
              analysis_vector_contains(
                  zero->uncertainties,
                  "TimeDateStamp is 0 or 0xFFFFFFFF; it is not a date and not a build date"),
          "timestamp zero is reported as a placeholder, not a date");
    check(analysis_vector_has_exact(
              maximum->pe_fields,
              "time_date_stamp=0xffffffff (not a date; placeholder or reproducibility value)") &&
              analysis_vector_contains(
                  maximum->uncertainties,
                  "TimeDateStamp is 0 or 0xFFFFFFFF; it is not a date and not a build date"),
          "timestamp 0xFFFFFFFF is reported as a placeholder, not a date");
    check(!analysis_text_contains(zero->timestamp, "1970") &&
              !analysis_text_contains(maximum->timestamp, "2106") &&
              analysis_text_contains(zero->timestamp, "not a date") &&
              analysis_text_contains(maximum->timestamp, "not a date"),
          "analyzer fabricates no Unix-style build date from special timestamps");

    analysis_cleanup_fixture(fixture);
}

void test_analysis_report_json_fields_escaping_and_determinism() {
    using namespace xbridge;

    check(analysis::status_name(analysis::DetectionStatus::Confirmed) == "confirmed" &&
              analysis::status_name(analysis::DetectionStatus::Indicator) == "indicator" &&
              analysis::status_name(analysis::DetectionStatus::Unknown) == "unknown" &&
              analysis::status_name(analysis::DetectionStatus::NotApplicable) == "not_applicable",
          "status_name maps every DetectionStatus to its documented label");

    const std::string tricky = "q\" b\\ t\t n\n c\x01";
    analysis::AnalysisReport report;
    report.source_directory = tricky;
    report.notes.push_back(tricky);
    report.notes.emplace_back("plain note");

    analysis::AnalysisEntry entry;
    entry.relative_path = tricky;
    entry.size_bytes = 12345U;
    entry.file_type = "json_fixture";
    entry.architecture = "no_platform_claim";
    entry.platform_indicators.push_back(tricky);
    entry.pe_fields.push_back(tricky);
    entry.imports.push_back(tricky);
    entry.exports.push_back(tricky);
    entry.relocation_types.push_back(tricky);
    entry.debug_types.push_back(tricky);
    entry.dependencies.push_back(tricky);
    entry.resource_kinds.push_back(tricky);
    entry.timestamp = tricky;
    entry.uncertainties.push_back(tricky);
    entry.component_mapping.push_back(tricky);
    entry.status = analysis::DetectionStatus::Indicator;
    report.entries.push_back(entry);

    const std::string json = report.to_json();
    const std::array<std::string_view, 21U> keys{{
        "source_directory", "notes", "relative_path", "size_bytes", "file_type",
        "architecture", "platform_indicators", "pe_fields", "imports", "exports",
        "relocation_types", "debug_types", "dependencies", "resource_kinds", "timestamp",
        "uncertainties", "component_mapping", "runtime_service_candidates",
        "loader_requirements", "unsupported_or_unknown_structures", "status"}};
    for (const std::string_view key : keys) {
        const std::string needle = "\"" + std::string(key) + "\"";
        check(analysis_text_contains(json, needle),
              ("analyzer JSON contains field " + std::string(key)).c_str());
    }
    check(analysis_text_contains(json, "\"size_bytes\": 12345"),
          "analyzer JSON writes size_bytes as a numeric field");
    check(analysis_text_contains(json, "\"status\": \"indicator\""),
          "analyzer JSON writes the status label");

    const std::string escaped = "q\\\" b\\\\ t\\t n\\n c\\u0001";
    check(analysis_text_contains(json, escaped),
          "analyzer JSON escapes quotes, backslashes, tabs, newlines and control characters");
    check(json == report.to_json(),
          "analyzer JSON is byte-identical on repeated calls");
}

void test_analysis_invalid_directory_error_code() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("invalid_dir");
    check(fixture.ok, "analyzer invalid-directory fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    const std::filesystem::path missing = fixture.root / "does_not_exist";
    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(missing, error); },
                         "analyzer nonexistent directory does not throw");
    check(error && report.entries.empty() && report.notes.empty(),
          "analyzer nonexistent directory sets error_code and yields an empty report");

    const std::u8string u8_path = missing.u8string();
    const std::string expected_source(reinterpret_cast<const char*>(u8_path.data()),
                                      u8_path.size());
    check(report.source_directory == expected_source,
          "analyzer empty report preserves the requested source directory");

    const std::filesystem::path regular_file = fixture.root / "regular.bin";
    check(analysis_write_file(regular_file, analysis_bytes_from_text("not a directory")),
          "analyzer invalid-directory fixture writes a regular file");
    error.clear();
    report = analysis::AnalysisReport{};
    check_does_not_throw([&] { report = analysis::analyze_directory(regular_file, error); },
                         "analyzer regular-file path does not throw");
    check(error && report.entries.empty(),
          "analyzer regular-file path sets error_code and yields an empty report");

    analysis_cleanup_fixture(fixture);
}

void test_analysis_max_scan_bytes_truncation() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("scan_cap");
    check(fixture.ok, "analyzer scan-cap fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    check(analysis_write_file(fixture.root / "capped.bin",
                              analysis_make_pe32plus_x64(0x5F5E0FF0U)),
          "analyzer scan-cap fixture writes a synthetic PE image");

    analysis::AnalysisOptions options;
    options.max_scan_bytes = 0x100U;
    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw(
        [&] { report = analysis::analyze_directory(fixture.root, options, error); },
        "analyzer capped scan does not throw");
    const analysis::AnalysisEntry* entry = analysis_find_entry(report, "capped.bin");
    check(!error && entry != nullptr, "analyzer capped scan succeeds with one entry");
    if (entry != nullptr) {
        check(analysis_vector_contains(
                  entry->uncertainties,
                  "file exceeds max_scan_bytes; only a prefix was inspected") &&
                  entry->status == analysis::DetectionStatus::Unknown,
              "max_scan_bytes truncation is reported in uncertainties with unknown status");
    }

    options.max_scan_bytes = 0U;
    error.clear();
    report = analysis::AnalysisReport{};
    check_does_not_throw(
        [&] { report = analysis::analyze_directory(fixture.root, options, error); },
        "analyzer zero-byte scan cap does not throw");
    const analysis::AnalysisEntry* zero_entry = analysis_find_entry(report, "capped.bin");
    check(!error && zero_entry != nullptr && zero_entry->file_type == "unknown" &&
              analysis_vector_contains(
                  zero_entry->uncertainties,
                  "max_scan_bytes is zero; no file content was inspected") &&
              zero_entry->status == analysis::DetectionStatus::Unknown,
          "zero max_scan_bytes reports that no content was inspected");

    analysis_cleanup_fixture(fixture);
}

void test_analysis_read_only_behavior() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("read_only");
    check(fixture.ok, "analyzer read-only fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    const std::array<std::filesystem::path, 3U> paths{{
        fixture.root / "synthetic.bin",
        fixture.root / "MicrosoftGame.config",
        fixture.root / "texture.dds"}};
    const std::array<std::vector<std::uint8_t>, 3U> contents{{
        analysis_make_pe32plus_x64(0x5F5E0FF0U),
        analysis_bytes_from_text("<Game TargetDeviceFamily=\"Scarlett\" Architecture=\"x64\" />"),
        analysis_bytes_from_text("synthetic dds payload")}};

    std::vector<AnalysisFileSnapshot> before;
    for (std::size_t index = 0U; index < paths.size(); ++index) {
        check(analysis_write_file(paths[index], contents[index]),
              ("analyzer read-only fixture writes " + paths[index].filename().string()).c_str());
        before.push_back(analysis_snapshot_file(paths[index]));
    }
    check(std::all_of(before.begin(), before.end(),
                      [](const AnalysisFileSnapshot& snapshot) { return snapshot.ok; }),
          "analyzer read-only fixture snapshots size, mtime and bytes before the scan");

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer read-only scan does not throw");
    check(!error && report.entries.size() == paths.size(),
          "analyzer read-only scan succeeds with all entries");

    for (std::size_t index = 0U; index < paths.size(); ++index) {
        const AnalysisFileSnapshot after = analysis_snapshot_file(paths[index]);
        const std::string label = paths[index].filename().string();
        check(after.ok && after.size == before[index].size &&
                  after.modified == before[index].modified &&
                  after.bytes == before[index].bytes,
              ("analyzer left " + label + " size, mtime and bytes unchanged").c_str());
    }
    check(analysis_vector_contains(report.notes, "source files were not modified"),
          "analyzer report explicitly notes that source files were not modified");

    analysis_cleanup_fixture(fixture);
}

void test_analysis_deterministic_entry_order() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("ordering");
    check(fixture.ok, "analyzer ordering fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    const std::vector<std::uint8_t> payload = analysis_bytes_from_text("synthetic ordering payload");
    check(analysis_write_file(fixture.root / "z_last.dds", payload) &&
              analysis_write_file(fixture.root / "a_first.shader", payload) &&
              analysis_write_file(fixture.root / "m_middle.bin", payload),
          "analyzer ordering fixture writes files in reverse lexical order");

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer ordering scan does not throw");
    check(!error && report.entries.size() == 3U,
          "analyzer ordering scan succeeds with three entries");
    check(analysis_entries_sorted(report),
          "analyzer entries are sorted by relative path");
    check(report.entries[0U].relative_path == "a_first.shader" &&
              report.entries[1U].relative_path == "m_middle.bin" &&
              report.entries[2U].relative_path == "z_last.dds",
          "analyzer entry order is exactly lexical by relative path");

    analysis_cleanup_fixture(fixture);
}

void test_analysis_resource_name_odd_max_string_bytes_regression() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("odd_resource_cap");
    check(fixture.ok, "analyzer odd resource-cap fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    constexpr std::uint32_t extra_unit_name_rva = 0x1800U;
    // Section raw data ends at RVA 0x2000; this leaves exactly 255 bytes from
    // the name RVA to the mapped extent boundary with max_string_bytes = 255.
    constexpr std::uint32_t boundary_name_rva = 0x1F01U;
    constexpr std::uint16_t declared_units = 128U;
    const std::string in_cap(127U, 'A');

    check(analysis_write_file(fixture.root / "odd_cap_extra.bin",
                              analysis_make_pe32plus_named_resource(
                                  extra_unit_name_rva, declared_units, in_cap, 'Z')) &&
              analysis_write_file(fixture.root / "odd_cap_boundary.bin",
                                  analysis_make_pe32plus_named_resource(
                                      boundary_name_rva, declared_units, in_cap, 'Z')),
          "analyzer odd resource-cap fixture writes two synthetic PE32+ images");

    analysis::AnalysisOptions options;
    options.max_string_bytes = 255U;
    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw(
        [&] { report = analysis::analyze_directory(fixture.root, options, error); },
        "analyzer odd resource-cap scan does not throw");
    check(!error && report.entries.size() == 2U,
          "analyzer odd resource-cap scan succeeds with exactly two entries");

    const analysis::AnalysisEntry* extra =
        analysis_find_entry(report, "odd_cap_extra.bin");
    const analysis::AnalysisEntry* boundary =
        analysis_find_entry(report, "odd_cap_boundary.bin");
    check(extra != nullptr && boundary != nullptr,
          "analyzer odd resource-cap entries are found by relative path");
    if (extra == nullptr || boundary == nullptr) {
        analysis_cleanup_fixture(fixture);
        return;
    }

    const std::string expected_kind = "resource_type=name:" + in_cap;
    check(!extra->resource_kinds.empty() &&
              analysis_vector_has_exact(extra->resource_kinds, expected_kind),
          "odd string cap decodes exactly 127 printable units, stopping before the extra unit");
    for (const std::string& kind : extra->resource_kinds) {
        const std::string message =
            "odd string cap resource kind excludes the post-cap printable unit: " + kind;
        check(!analysis_text_contains(kind, "Z"), message.c_str());
    }

    check(analysis_vector_contains(boundary->uncertainties,
                                   "resource type name is unreadable") &&
              !boundary->uncertainties.empty(),
          "scanned prefix ending exactly at the odd cap records a truncation/limit uncertainty");
    check(!analysis_vector_contains(boundary->resource_kinds, "resource_type=name:"),
          "scanned prefix ending exactly at the odd cap fabricates no resource type name");

    check(extra->status == analysis::DetectionStatus::Indicator &&
              boundary->status == analysis::DetectionStatus::Indicator &&
              analysis::status_name(extra->status) == "indicator" &&
              analysis::status_name(boundary->status) == "indicator" &&
              !analysis_has_affirmative_windows_claim(extra->uncertainties) &&
              !analysis_has_affirmative_windows_claim(boundary->uncertainties),
          "odd resource-cap entries remain indicators and make no confirmed claim");

    analysis_cleanup_fixture(fixture);
}

void test_analysis_dll_characteristics_exact_output_regression() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("dll_characteristics");
    check(fixture.ok, "analyzer DLL-characteristics fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    check(analysis_write_file(fixture.root / "dll_chars.bin",
                              analysis_make_pe32plus_x64(0x5F5E0FF0U)),
          "analyzer DLL-characteristics fixture writes a synthetic PE32+ image");

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer DLL-characteristics scan does not throw");
    check(!error && report.entries.size() == 1U,
          "analyzer DLL-characteristics scan succeeds with exactly one entry");
    const analysis::AnalysisEntry* entry = analysis_find_entry(report, "dll_chars.bin");
    check(entry != nullptr, "analyzer DLL-characteristics entry is found by relative path");
    if (entry == nullptr) {
        analysis_cleanup_fixture(fixture);
        return;
    }

    const std::string expected =
        "dll_characteristics=0x0060[HIGH_ENTROPY_VA,DYNAMIC_BASE]";
    check(analysis_vector_has_exact(entry->pe_fields, expected),
          "DLL-characteristics field is byte-for-byte the authorized output format");
    check(!analysis_vector_has_exact(
              entry->pe_fields, "dll_characteristics=0x0060 [HIGH_ENTROPY_VA,DYNAMIC_BASE]"),
          "spaced separator variant is rejected by the exact field assertion");

    check(entry->status == analysis::DetectionStatus::Indicator &&
              analysis::status_name(entry->status) == "indicator",
          "DLL-characteristics PE remains an indicator, not a confirmed claim");
    check(!analysis_has_affirmative_windows_claim(entry->pe_fields) &&
              !analysis_has_affirmative_windows_claim(entry->platform_indicators) &&
              !analysis_has_affirmative_windows_claim(entry->uncertainties),
          "DLL-characteristics output introduces no Windows-compatibility claim");

    analysis_cleanup_fixture(fixture);
}

// XB-NEXT-DEVELOPMENT-03-N1 — synthetic native execution proof. No host CPUID
// result is consulted: every feature snapshot below is injected by the test.

static_assert(execution::NativeRoutineId::LoadViaMemory ==
              execution::NativeRoutineId::LoadU64ViaMemory);
static_assert(execution::NativeRoutineId::StoreViaMemory ==
              execution::NativeRoutineId::StoreU64ViaMemory);
static_assert(execution::NativeRoutineId::ServiceTransition ==
              execution::NativeRoutineId::RequestService);

class MutableCpuFeatureDetector final : public execution::ICpuFeatureDetector {
public:
    execution::CpuFeatureSet features{};

    [[nodiscard]] execution::CpuFeatureSet detect() const noexcept override { return features; }
    [[nodiscard]] execution::CpuFeatureDetectionError last_error() const noexcept override {
        return execution::CpuFeatureDetectionError::None;
    }
};

class NativeFailingMemory final : public memory::IGuestVirtualMemory {
public:
    bool fail_read{false};
    bool fail_write{false};
    std::uint64_t read_calls{0U};
    std::uint64_t write_calls{0U};

    [[nodiscard]] memory::MemoryResult fetch(
        memory::GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept override {
        return read(address, destination);
    }

    [[nodiscard]] memory::MemoryResult read(
        memory::GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept override {
        ++read_calls;
        if (fail_read) {
            return memory::MemoryResult::failure(
                memory::MemoryFault::AccessDenied, memory::MemoryAccess::Read,
                memory::MemoryAccessContext::Data, address.value, destination.size());
        }
        for (std::uint8_t& byte : destination) {
            byte = 0U;
        }
        return memory::MemoryResult::success(
            memory::MemoryAccess::Read, memory::MemoryAccessContext::Data, address.value,
            destination.size(), destination.size());
    }

    [[nodiscard]] memory::MemoryResult write(
        memory::GuestVirtualAddress address, std::span<const std::uint8_t> source) noexcept override {
        ++write_calls;
        if (fail_write) {
            return memory::MemoryResult::failure(
                memory::MemoryFault::AccessDenied, memory::MemoryAccess::Write,
                memory::MemoryAccessContext::Data, address.value, source.size());
        }
        return memory::MemoryResult::success(
            memory::MemoryAccess::Write, memory::MemoryAccessContext::Data, address.value,
            source.size(), source.size());
    }
};

execution::CpuFeatureSet native_baseline_features() noexcept {
    execution::CpuFeatureSet features{};
    features.baseline_x64 = true;
    return features;
}

execution::CpuFeatureSet native_all_features() noexcept {
    execution::CpuFeatureSet features{};
    features.baseline_x64 = true;
    features.avx2 = true;
    features.bmi1 = true;
    features.bmi2 = true;
    features.fma = true;
    features.osxsave = true;
    features.xgetbv = true;
    return features;
}

void test_native_feature_detection_and_gating() {
    using namespace xbridge;

    const execution::CpuFeatureSet empty_features{};
    const execution::FixedCpuFeatureDetector empty_detector(empty_features);
    const auto empty = empty_detector.detect();
    check(!empty.baseline_x64 && !empty.avx2 && !empty.bmi1 && !empty.bmi2 &&
              !empty.fma && !empty.osxsave && !empty.xgetbv &&
              empty_detector.last_error() == execution::CpuFeatureDetectionError::None,
          "fixed detector reports an injected empty snapshot with no detection error");

    execution::CpuFeatureSet mixed = native_all_features();
    mixed.avx2 = false;
    mixed.fma = false;
    const execution::FixedCpuFeatureDetector mixed_detector(mixed);
    const auto observed = mixed_detector.detect();
    check(observed.baseline_x64 && observed.bmi1 && observed.bmi2 &&
              observed.osxsave && observed.xgetbv &&
              !observed.avx2 && !observed.fma &&
              observed.has(execution::CpuFeature::BaselineX64) &&
              !observed.has(execution::CpuFeature::Avx2) &&
              !observed.has(execution::CpuFeature::Fma),
          "fixed detector reports each injected feature independently");

    check(std::string_view(execution::to_string(execution::CpuFeature::BaselineX64)) ==
              "BaselineX64" &&
              std::string_view(execution::to_string(execution::CpuFeature::Avx2)) == "Avx2" &&
              std::string_view(execution::to_string(execution::CpuFeature::Bmi1)) == "Bmi1" &&
              std::string_view(execution::to_string(execution::CpuFeature::Bmi2)) == "Bmi2" &&
              std::string_view(execution::to_string(execution::CpuFeature::Fma)) == "Fma" &&
              std::string_view(execution::to_string(execution::CpuFeature::Osxsave)) ==
                  "Osxsave" &&
              std::string_view(execution::to_string(execution::CpuFeature::Xgetbv)) == "Xgetbv" &&
              std::string_view(execution::to_string(
                  execution::CpuFeatureDetectionError::UnsupportedPlatform)) ==
                  "UnsupportedPlatform",
          "feature and detector-error names are stable and evidence-labeled");

    memory::GuestMemory memory(64U);
    memory::GuestMemoryView view(memory);
    execution::GuestExecutionContext context(view);
    context.registers().gprs[0U] = 0xDEADBEEFCAFEF00DULL;
    const auto original = context.registers();

    execution::NativeExecutionBackend empty_backend(empty_detector);
    check(!empty_backend.can_execute(execution::NativeRoutineId::Nop) &&
              !empty_backend.can_execute(execution::NativeRoutineId::Avx2Gate),
          "baseline routines require an injected baseline x64 feature");
    const auto gated = empty_backend.execute(execution::NativeRoutineId::MovImm32, context);
    check(gated.error == execution::NativeExecutionError::UnsupportedFeature &&
              gated.native_calls == 0U && context.registers().gprs[0U] == original.gprs[0U] &&
              gated.registers.gprs[0U] == original.gprs[0U],
          "unsupported features fail before routine mutation and native_calls stays zero");

    const auto unknown_id = static_cast<execution::NativeRoutineId>(99);
    const auto unknown = empty_backend.execute(unknown_id, context);
    check(!empty_backend.can_execute(unknown_id) &&
              unknown.error == execution::NativeExecutionError::UnknownRoutine &&
              unknown.native_calls == 0U && context.registers().gprs[0U] == original.gprs[0U],
          "unknown routine IDs fail without executing or mutating context");

    execution::NativeExecutionBackend mixed_backend(mixed_detector);
    check(mixed_backend.can_execute(execution::NativeRoutineId::Nop) &&
              mixed_backend.can_execute(execution::NativeRoutineId::Bmi1Gate) &&
              mixed_backend.can_execute(execution::NativeRoutineId::Bmi2Gate) &&
              !mixed_backend.can_execute(execution::NativeRoutineId::Avx2Gate) &&
              !mixed_backend.can_execute(execution::NativeRoutineId::FmaGate),
          "optional routines are gated strictly by their own injected feature bits");

    execution::CpuFeatureSet missing_osxsave = native_all_features();
    missing_osxsave.osxsave = false;
    const execution::FixedCpuFeatureDetector missing_osxsave_detector(missing_osxsave);
    const auto missing_osxsave_features = missing_osxsave_detector.detect();
    const execution::NativeExecutionBackend missing_osxsave_backend(missing_osxsave_detector);
    check(!missing_osxsave_features.avx2 &&
              !missing_osxsave_features.has(execution::CpuFeature::Avx2) &&
              !missing_osxsave_backend.can_execute(execution::NativeRoutineId::Avx2Gate),
          "injected AVX2 requires OSXSAVE before detection or native gating");

    execution::CpuFeatureSet missing_xgetbv = native_all_features();
    missing_xgetbv.xgetbv = false;
    const execution::FixedCpuFeatureDetector missing_xgetbv_detector(missing_xgetbv);
    const auto missing_xgetbv_features = missing_xgetbv_detector.detect();
    const execution::NativeExecutionBackend missing_xgetbv_backend(missing_xgetbv_detector);
    check(!missing_xgetbv_features.avx2 &&
              !missing_xgetbv_features.has(execution::CpuFeature::Avx2) &&
              !missing_xgetbv_backend.can_execute(execution::NativeRoutineId::Avx2Gate),
          "injected AVX2 requires XGETBV before detection or native gating");

    const execution::ExecutionBackendSelector selector(empty_detector);
    check(selector.select(execution::NativeRoutineId::Nop,
                          execution::ExecutionBackendPreference::Automatic) ==
              execution::ExecutionBackendKind::Interpreter &&
              selector.select(execution::NativeRoutineId::Nop,
                              execution::ExecutionBackendPreference::Interpreter) ==
                  execution::ExecutionBackendKind::Interpreter &&
              selector.select(execution::NativeRoutineId::Nop,
                              execution::ExecutionBackendPreference::Native) ==
                  execution::ExecutionBackendKind::Native,
          "selector honors explicit preferences and automatic fallback before execution");
    check(selector.select(execution::NativeRoutineId::Avx2Gate,
                          execution::ExecutionBackendPreference::Automatic) ==
              execution::ExecutionBackendKind::Interpreter,
          "automatic selection falls back to interpreter for an unsupported optional routine");

    check(std::string_view(execution::to_string(
              execution::ExecutionBackendPreference::Automatic)) == "Automatic" &&
              std::string_view(execution::to_string(
                  execution::ExecutionBackendKind::Interpreter)) == "Interpreter" &&
              std::string_view(execution::to_string(
                  execution::NativeExecutionError::UnsupportedFeature)) ==
                  "UnsupportedFeature" &&
              std::string_view(execution::to_string(
                  execution::NativeExecutionError::UnknownRoutine)) == "UnknownRoutine",
          "backend preference, backend kind and error names are stable");

    MutableCpuFeatureDetector mutable_detector;
    mutable_detector.features = native_baseline_features();
    execution::NativeExecutionBackend snapshot(mutable_detector);
    check(snapshot.can_execute(execution::NativeRoutineId::Nop) &&
              !snapshot.can_execute(execution::NativeRoutineId::Avx2Gate),
          "backend can execute injected baseline routines");
    mutable_detector.features.avx2 = true;
    check(mutable_detector.detect().avx2 &&
              !snapshot.can_execute(execution::NativeRoutineId::Avx2Gate),
          "backend keeps the detector snapshot taken at construction");
}

void test_native_backend_routines_and_interpreter_parity() {
    using namespace xbridge;

    const execution::FixedCpuFeatureDetector detector(native_all_features());
    execution::NativeExecutionBackend backend(detector);
    memory::GuestMemory memory(128U);
    memory::GuestMemoryView view(memory);
    execution::GuestExecutionContext context(view);

    context.registers().gprs[0U] = 0x1234U;
    const auto nop = backend.execute(execution::NativeRoutineId::Nop, context);
    check(nop.error == execution::NativeExecutionError::None && nop.native_calls == 1U &&
              context.registers().gprs[0U] == 0x1234U,
          "native Nop succeeds with one native call and no register mutation");

    memory::GuestMemory interpreter_memory(128U);
    cpu::X64CpuState interpreter_state;
    const std::uint8_t mov_imm_bytes[] = {0xB8U, 0x78U, 0x56U, 0x34U, 0x12U};
    check(cpu::step_x64_instruction(std::span<const std::uint8_t>(mov_imm_bytes),
                                    interpreter_state, interpreter_memory) ==
              cpu::X64ExecuteStatus::Ok,
          "interpreter MOV EAX,imm32 parity instruction executes");
    const auto mov_imm = backend.execute(execution::NativeRoutineId::MovImm32, context);
    check(mov_imm.error == execution::NativeExecutionError::None &&
              mov_imm.native_calls == 1U &&
              context.registers().gprs[0U] == execution::kMovImm32Immediate &&
              context.registers().gprs[0U] == interpreter_state.gpr(cpu::X64GeneralRegister::Rax),
          "native MovImm32 matches the interpreter and zero-extends its 32-bit immediate");

    cpu::X64CpuState interpreter_move_state;
    interpreter_move_state.gpr(cpu::X64GeneralRegister::Rcx) = 0x1122334455667788ULL;
    const std::uint8_t mov_reg_bytes[] = {0x48U, 0x89U, 0xC8U};
    check(cpu::step_x64_instruction(std::span<const std::uint8_t>(mov_reg_bytes),
                                    interpreter_move_state, interpreter_memory) ==
              cpu::X64ExecuteStatus::Ok,
          "interpreter MOV RAX,RCX parity instruction executes");
    execution::GuestRegisterState move_registers{};
    move_registers.gprs[1U] = 0x1122334455667788ULL;
    context.set_registers(move_registers);
    const auto mov_reg = backend.execute(execution::NativeRoutineId::MovRegToReg, context);
    check(mov_reg.error == execution::NativeExecutionError::None &&
              mov_reg.native_calls == 1U &&
              context.registers().gprs[0U] ==
                  interpreter_move_state.gpr(cpu::X64GeneralRegister::Rax),
          "native MovRegToReg matches the interpreter's register-to-register result");

    execution::GuestRegisterState add_registers{};
    add_registers.gprs[0U] = 0xFFFFFFFFULL;
    context.set_registers(add_registers);
    const auto add = backend.execute(execution::NativeRoutineId::AddImm32, context);
    check(add.error == execution::NativeExecutionError::None && add.native_calls == 1U &&
              context.registers().gprs[0U] == 9U,
          "native AddImm32 wraps in 32 bits and zero-extends the result");

    constexpr std::uint64_t load_value = 0x1122334455667788ULL;
    std::vector<std::uint8_t> seed(8U, 0U);
    analysis_put_u64(seed, 0U, load_value);
    const auto seeded = view.write(memory::GuestVirtualAddress{32U},
                                   std::span<const std::uint8_t>(seed.data(), seed.size()));
    check(seeded.ok, "native memory test seeds a synthetic little-endian u64");
    execution::GuestRegisterState load_registers{};
    load_registers.gprs[1U] = 32U;
    context.set_registers(load_registers);
    const auto load = backend.execute(execution::NativeRoutineId::LoadViaMemory, context);
    check(load.error == execution::NativeExecutionError::None && load.native_calls == 1U &&
              context.registers().gprs[0U] == load_value,
          "native LoadU64ViaMemory reads eight little-endian bytes through the memory seam");

    constexpr std::uint64_t store_value = 0x8877665544332211ULL;
    std::vector<std::uint8_t> expected_store(8U, 0U);
    analysis_put_u64(expected_store, 0U, store_value);
    execution::GuestRegisterState store_registers{};
    store_registers.gprs[0U] = store_value;
    store_registers.gprs[1U] = 40U;
    context.set_registers(store_registers);
    const auto store = backend.execute(execution::NativeRoutineId::StoreViaMemory, context);
    std::vector<std::uint8_t> stored(8U, 0U);
    const auto read_store = view.read(memory::GuestVirtualAddress{40U},
                                      std::span<std::uint8_t>(stored.data(), stored.size()));
    check(store.error == execution::NativeExecutionError::None && store.native_calls == 1U &&
              read_store.ok && stored == expected_store,
          "native StoreU64ViaMemory writes eight little-endian bytes through the memory seam");

    execution::GuestRegisterState transition_registers{};
    transition_registers.gprs[2U] = 7U;
    transition_registers.gprs[3U] = 0xABCDU;
    context.set_registers(transition_registers);
    const auto transition = backend.execute(execution::NativeRoutineId::ServiceTransition, context);
    check(transition.error == execution::NativeExecutionError::None &&
              transition.native_calls == 1U &&
              transition.transition.kind == execution::GuestHostTransitionKind::ServiceRequest &&
              transition.transition.service_id == 7U &&
              transition.transition.payload == 0xABCDU &&
              context.last_transition().kind == transition.transition.kind &&
              context.last_transition().service_id == transition.transition.service_id &&
              context.last_transition().payload == transition.transition.payload,
          "native RequestService records a synthetic guest/host service transition");

    struct GateCase {
        execution::NativeRoutineId routine;
        std::uint64_t left;
        std::uint64_t right;
        std::uint64_t expected;
    };
    const std::array<GateCase, 4U> gates{{
        {execution::NativeRoutineId::Avx2Gate, 0xF0F0U, 0x0FF0U, 0xFF00U},
        {execution::NativeRoutineId::Bmi1Gate, 0xFFFFU, 0x0F0FU, 0xF0F0U},
        {execution::NativeRoutineId::Bmi2Gate, 0xF0F0U, 0x0F0FU, 0xFFFFU},
        {execution::NativeRoutineId::FmaGate, 0x12345678ULL, 0x10ULL, 0x12345688ULL},
    }};
    for (const GateCase& gate_case : gates) {
        execution::GuestRegisterState gate_registers{};
        gate_registers.gprs[0U] = gate_case.left;
        gate_registers.gprs[1U] = gate_case.right;
        context.set_registers(gate_registers);
        const auto result = backend.execute(gate_case.routine, context);
        check(result.error == execution::NativeExecutionError::None &&
                  result.native_calls == 1U &&
                  context.registers().gprs[0U] == gate_case.expected,
              "feature-gated synthetic routine applies its documented plain-C++ operation");
    }

    execution::GuestRegisterState before_unknown{};
    before_unknown.gprs[0U] = 0x55U;
    context.set_registers(before_unknown);
    const auto unknown = backend.execute(static_cast<execution::NativeRoutineId>(99), context);
    check(unknown.error == execution::NativeExecutionError::UnknownRoutine &&
              unknown.native_calls == 0U && context.registers().gprs[0U] == 0x55U,
          "unknown native routine does not mutate registers or count a native call");

    NativeFailingMemory failing_read;
    failing_read.fail_read = true;
    execution::GuestExecutionContext read_failure_context(failing_read);
    execution::GuestRegisterState failing_read_registers{};
    failing_read_registers.gprs[1U] = 0U;
    read_failure_context.set_registers(failing_read_registers);
    const auto failed_load =
        backend.execute(execution::NativeRoutineId::LoadU64ViaMemory, read_failure_context);
    check(failed_load.error == execution::NativeExecutionError::MemoryAccessError &&
              failed_load.native_calls == 0U && failing_read.read_calls == 1U &&
              read_failure_context.registers().gprs[0U] == 0U,
          "native load memory failure is explicit and leaves the destination unchanged");

    NativeFailingMemory failing_write;
    failing_write.fail_write = true;
    execution::GuestExecutionContext write_failure_context(failing_write);
    execution::GuestRegisterState failing_write_registers{};
    failing_write_registers.gprs[0U] = 0x99U;
    failing_write_registers.gprs[1U] = 0U;
    write_failure_context.set_registers(failing_write_registers);
    const auto failed_store =
        backend.execute(execution::NativeRoutineId::StoreU64ViaMemory, write_failure_context);
    check(failed_store.error == execution::NativeExecutionError::MemoryAccessError &&
              failed_store.native_calls == 0U && failing_write.write_calls == 1U,
          "native store memory failure is explicit and counts no native call");

    check(std::string_view(execution::to_string(execution::GuestHostTransitionKind::ServiceRequest)) ==
              "ServiceRequest" &&
              std::string_view(execution::to_string(execution::GuestHostTransitionKind::NormalExit)) ==
                  "NormalExit" &&
              std::string_view(execution::to_string(execution::GuestHostTransitionKind::FaultExit)) ==
                  "FaultExit",
          "guest/host transition kind names are stable");
}

void test_guest_execution_context_stack_and_memory_failures() {
    using namespace xbridge;

    memory::GuestMemory memory(64U);
    memory::GuestMemoryView view(memory);
    execution::GuestExecutionContext context(view);
    context.set_stack(execution::GuestStack{memory::GuestVirtualAddress{16U}, 32U, 0U});

    constexpr std::uint64_t stack_value = 0x1122334455667788ULL;
    check(context.push_u64(stack_value) && context.stack().pointer_offset == 8U,
          "guest stack push advances the ascending pointer only after success");
    std::vector<std::uint8_t> stored(8U, 0U);
    std::vector<std::uint8_t> expected(8U, 0U);
    analysis_put_u64(expected, 0U, stack_value);
    const auto read_back = view.read(memory::GuestVirtualAddress{16U},
                                     std::span<std::uint8_t>(stored.data(), stored.size()));
    check(read_back.ok && stored == expected,
          "guest stack push writes eight little-endian bytes to base plus offset");

    std::uint64_t popped = 0U;
    check(context.pop_u64(popped) && popped == stack_value &&
              context.stack().pointer_offset == 0U,
          "guest stack pop reads the value and retreats the pointer by eight bytes");

    auto& registers = context.registers();
    registers.gprs[15U] = 0xA5A5A5A5A5A5A5A5ULL;
    registers.gpr(16U) = 0x1234U;
    check(registers.gpr(16U) == 0U && !registers.has_gpr(16U) &&
              registers.gprs[15U] == 0xA5A5A5A5A5A5A5A5ULL,
          "out-of-range guest register access uses an isolated scratch and cannot mutate GPRs");

    NativeFailingMemory failing;
    failing.fail_read = true;
    failing.fail_write = true;
    execution::GuestExecutionContext failure_context(failing);

    failure_context.set_stack(execution::GuestStack{memory::GuestVirtualAddress{8U}, 8U, 8U});
    check(!failure_context.push_u64(1U) && failing.write_calls == 0U &&
              failure_context.stack().pointer_offset == 8U,
          "push at full stack capacity fails before any memory access or pointer mutation");

    std::uint64_t untouched = 0xAAU;
    failure_context.set_stack(execution::GuestStack{memory::GuestVirtualAddress{8U}, 8U, 0U});
    check(!failure_context.pop_u64(untouched) && failing.read_calls == 0U &&
              untouched == 0xAAU && failure_context.stack().pointer_offset == 0U,
          "pop at empty stack fails before any memory access or output mutation");

    failure_context.set_stack(execution::GuestStack{memory::GuestVirtualAddress{8U}, 16U, 0U});
    failing.write_calls = 0U;
    check(!failure_context.push_u64(2U) && failing.write_calls == 1U &&
              failure_context.stack().pointer_offset == 0U,
          "failed memory write leaves the stack pointer unchanged");

    failure_context.set_stack(execution::GuestStack{memory::GuestVirtualAddress{8U}, 16U, 8U});
    failing.read_calls = 0U;
    untouched = 0xAAU;
    check(!failure_context.pop_u64(untouched) && failing.read_calls == 1U &&
              untouched == 0xAAU && failure_context.stack().pointer_offset == 8U,
          "failed memory read leaves both the output value and stack pointer unchanged");

    failure_context.set_stack(execution::GuestStack{
        memory::GuestVirtualAddress{(std::numeric_limits<std::uint64_t>::max)() - 2U}, 16U, 8U});
    failing.write_calls = 0U;
    check(!failure_context.push_u64(3U) && failing.write_calls == 0U,
          "stack address overflow is rejected before any memory access");
}

// XB-NEXT-DEVELOPMENT-03-B1 — bounded synthetic PE32+ loader fixtures. Every
// byte is generated here; no real executable or game file is read.

constexpr std::uint16_t loader_pe_offset = 0x80U;
constexpr std::uint16_t loader_optional_size = 0xF0U;
constexpr std::uint32_t loader_text_rva = 0x1000U;
constexpr std::uint32_t loader_data_rva = 0x2000U;
constexpr std::uint32_t loader_relocation_directory_rva = 0x2100U;
constexpr std::size_t loader_text_raw_offset = 0x400U;
constexpr std::size_t loader_data_raw_offset = 0x600U;
constexpr std::uint32_t loader_section_raw_size = 0x200U;
constexpr std::uint32_t loader_size_of_image = 0x4000U;
constexpr std::uint64_t loader_image_base = 0x0000000140000000ULL;
constexpr std::uint32_t loader_entry_rva = 0x1010U;

struct LoaderRelocationEntry {
    std::uint16_t type{10U};
    std::uint32_t page_rva{0x2000U};
    std::uint16_t offset{0x30U};
};

struct LoaderImageOptions {
    std::uint64_t image_base{loader_image_base};
    std::uint32_t entry_rva{loader_entry_rva};
    std::uint16_t machine{0x8664U};
    std::uint16_t section_count{2U};
    std::uint16_t magic{0x020BU};
    std::uint32_t section_alignment{0x1000U};
    std::uint32_t file_alignment{0x200U};
    std::uint64_t relocation_value{0x1122334400000000ULL};
    std::vector<LoaderRelocationEntry> relocations{
        LoaderRelocationEntry{10U, 0x2000U, 0x30U}};
};

std::vector<std::uint8_t> loader_make_pe32plus(const LoaderImageOptions& options =
                                                   LoaderImageOptions{}) {
    std::vector<std::uint8_t> bytes(0x1000U, std::uint8_t{0U});
    bytes[0U] = static_cast<std::uint8_t>('M');
    bytes[1U] = static_cast<std::uint8_t>('Z');
    analysis_put_u32(bytes, 0x3CU, loader_pe_offset);

    const std::size_t coff = static_cast<std::size_t>(loader_pe_offset) + 4U;
    bytes[loader_pe_offset] = static_cast<std::uint8_t>('P');
    bytes[loader_pe_offset + 1U] = static_cast<std::uint8_t>('E');
    analysis_put_u16(bytes, coff, options.machine);
    analysis_put_u16(bytes, coff + 2U, options.section_count);
    analysis_put_u16(bytes, coff + 16U, loader_optional_size);
    analysis_put_u16(bytes, coff + 18U, 0x0022U);

    const std::size_t optional = coff + 20U;
    analysis_put_u16(bytes, optional, options.magic);
    analysis_put_u32(bytes, optional + 16U, options.entry_rva);
    analysis_put_u64(bytes, optional + 24U, options.image_base);
    analysis_put_u32(bytes, optional + 32U, options.section_alignment);
    analysis_put_u32(bytes, optional + 36U, options.file_alignment);
    analysis_put_u32(bytes, optional + 56U, loader_size_of_image);
    analysis_put_u32(bytes, optional + 60U, 0x400U);
    analysis_put_u32(bytes, optional + 108U, 16U);

    const std::size_t directory_base = optional + 112U;
    if (!options.relocations.empty()) {
        std::vector<std::uint32_t> pages;
        for (const LoaderRelocationEntry& entry : options.relocations) {
            if (std::find(pages.begin(), pages.end(), entry.page_rva) == pages.end()) {
                pages.push_back(entry.page_rva);
            }
        }

        std::uint32_t directory_size = 0U;
        for (const std::uint32_t page : pages) {
            const auto count = static_cast<std::uint32_t>(std::count_if(
                options.relocations.begin(), options.relocations.end(),
                [page](const LoaderRelocationEntry& entry) { return entry.page_rva == page; }));
            directory_size += 8U + 2U * count;
        }
        analysis_put_u32(bytes, directory_base + 5U * 8U, loader_relocation_directory_rva);
        analysis_put_u32(bytes, directory_base + 5U * 8U + 4U, directory_size);

        const std::size_t blocks =
            loader_data_raw_offset +
            static_cast<std::size_t>(loader_relocation_directory_rva - loader_data_rva);
        std::uint32_t consumed = 0U;
        for (const std::uint32_t page : pages) {
            std::vector<std::size_t> members;
            for (std::size_t index = 0U; index < options.relocations.size(); ++index) {
                if (options.relocations[index].page_rva == page) {
                    members.push_back(index);
                }
            }
            const std::uint32_t block_size =
                8U + static_cast<std::uint32_t>(2U * members.size());
            const std::size_t block = blocks + consumed;
            analysis_put_u32(bytes, block, page);
            analysis_put_u32(bytes, block + 4U, block_size);
            for (std::size_t member = 0U; member < members.size(); ++member) {
                const LoaderRelocationEntry& entry = options.relocations[members[member]];
                analysis_put_u16(bytes, block + 8U + member * 2U,
                                 static_cast<std::uint16_t>(
                                     (entry.type << 12U) | (entry.offset & 0x0FFFU)));
                if (entry.page_rva >= loader_data_rva) {
                    const std::size_t target = loader_data_raw_offset +
                        static_cast<std::size_t>(entry.page_rva - loader_data_rva) + entry.offset;
                    analysis_put_u64(bytes, target, options.relocation_value);
                }
            }
            consumed += block_size;
        }
    }

    const std::size_t section_table = optional + loader_optional_size;
    analysis_put_ascii(bytes, section_table, ".text");
    analysis_put_u32(bytes, section_table + 8U, 0x1000U);
    analysis_put_u32(bytes, section_table + 12U, loader_text_rva);
    analysis_put_u32(bytes, section_table + 16U, loader_section_raw_size);
    analysis_put_u32(bytes, section_table + 20U,
                     static_cast<std::uint32_t>(loader_text_raw_offset));
    analysis_put_u32(bytes, section_table + 36U,
                     loader::kSectionMemRead | loader::kSectionMemExecute);

    const std::size_t data_section = section_table + 40U;
    analysis_put_ascii(bytes, data_section, ".data");
    analysis_put_u32(bytes, data_section + 8U, 0x1000U);
    analysis_put_u32(bytes, data_section + 12U, loader_data_rva);
    analysis_put_u32(bytes, data_section + 16U, loader_section_raw_size);
    analysis_put_u32(bytes, data_section + 20U,
                     static_cast<std::uint32_t>(loader_data_raw_offset));
    analysis_put_u32(bytes, data_section + 36U,
                     loader::kSectionMemRead | loader::kSectionMemWrite);

    if (options.section_count >= 3U) {
        constexpr std::uint32_t extra_rva = 0x3000U;
        constexpr std::size_t extra_raw_offset = 0x800U;
        const std::size_t extra_section = data_section + 40U;
        analysis_put_ascii(bytes, extra_section, ".extra");
        analysis_put_u32(bytes, extra_section + 8U, 0x1000U);
        analysis_put_u32(bytes, extra_section + 12U, extra_rva);
        analysis_put_u32(bytes, extra_section + 16U, loader_section_raw_size);
        analysis_put_u32(bytes, extra_section + 20U, extra_raw_offset);
        analysis_put_u32(bytes, extra_section + 36U,
                         loader::kSectionMemRead | loader::kSectionMemWrite);
    }

    bytes[loader_text_raw_offset + (options.entry_rva - loader_text_rva)] = 0x90U;
    return bytes;
}

std::uint64_t loader_load_u64(std::span<const std::uint8_t> bytes) noexcept {
    std::uint64_t value = 0U;
    const std::size_t count = (std::min)(bytes.size(), std::size_t{8U});
    for (std::size_t index = 0U; index < count; ++index) {
        value |= static_cast<std::uint64_t>(bytes[index]) << (index * 8U);
    }
    return value;
}

void test_loader_synthetic_valid_lifecycle_and_permissions() {
    using namespace xbridge;

    const auto bytes = loader_make_pe32plus();
    loader::LoadOptions options;
    options.image_base_override = 0x0000000140010000ULL;

    memory::GuestPhysicalMemory physical;
    memory::GuestAddressSpace address_space(physical);
    loader::ExecutableLoader image_loader(address_space, physical);

    check(image_loader.state() == loader::LoaderState::Unloaded &&
              loader::can_transition(loader::LoaderState::Unloaded,
                                     loader::LoaderState::Validated) &&
              !loader::can_transition(loader::LoaderState::Unloaded,
                                      loader::LoaderState::Ready),
          "loader starts unloaded and exposes the documented state transitions");

    check(image_loader.validate(std::span<const std::uint8_t>(bytes.data(), bytes.size()),
                                options) == loader::LoadError::None &&
              image_loader.state() == loader::LoaderState::Validated &&
              image_loader.image_base() == options.image_base_override &&
              image_loader.entry_point() == options.image_base_override + loader_entry_rva &&
              image_loader.image_info().sections.size() == 2U,
          "valid synthetic PE32+ image validates with two sections and a computed entry point");

    check(image_loader.map() == loader::LoadError::None &&
              image_loader.state() == loader::LoaderState::Mapped &&
              image_loader.virtual_mappings().size() == 2U &&
              image_loader.physical_regions().size() == 2U,
          "map creates two virtual mappings and two physical backing regions");

    const auto& text_map = image_loader.virtual_mappings()[0U];
    const auto& data_map = image_loader.virtual_mappings()[1U];
    check(text_map.virtual_base == options.image_base_override + loader_text_rva &&
              data_map.virtual_base == options.image_base_override + loader_data_rva &&
              text_map.length == 0x1000U && data_map.length == 0x1000U,
          "sections are mapped at the effective image base plus their section RVAs");

    memory::GuestPhysicalAddress translated{};
    const auto text_read = address_space.translate(
        memory::GuestVirtualAddress{options.image_base_override + loader_entry_rva}, 1U,
        memory::MemoryAccess::Read, memory::MemoryAccessContext::Data, translated);
    const auto text_write = address_space.translate(
        memory::GuestVirtualAddress{options.image_base_override + loader_entry_rva}, 1U,
        memory::MemoryAccess::Write, memory::MemoryAccessContext::Data, translated);
    const auto data_write = address_space.translate(
        memory::GuestVirtualAddress{options.image_base_override + 0x2030U}, 1U,
        memory::MemoryAccess::Write, memory::MemoryAccessContext::Data, translated);
    check(text_read.ok && !text_write.ok &&
              text_write.fault == memory::MemoryFault::AccessDenied && data_write.ok,
          "section characteristics produce read-only execute text and writable data mappings");

    std::array<std::uint8_t, 1U> sentinel{};
    const auto sentinel_read = address_space.read(
        memory::GuestVirtualAddress{options.image_base_override + loader_entry_rva},
        std::span<std::uint8_t>(sentinel.data(), sentinel.size()));
    check(sentinel_read.ok && sentinel[0U] == 0x90U,
          "loader maps entry-point bytes without executing them");

    const std::uint64_t target = options.image_base_override + 0x2030U;
    std::array<std::uint8_t, 8U> before{};
    const auto before_read = address_space.read(
        memory::GuestVirtualAddress{target},
        std::span<std::uint8_t>(before.data(), before.size()));
    check(before_read.ok && loader_load_u64(std::span<const std::uint8_t>(
                                before.data(), before.size())) == 0x1122334400000000ULL,
          "synthetic DIR64 target contains the injected little-endian value before relocation");

    check(image_loader.relocate() == loader::LoadError::None &&
              image_loader.state() == loader::LoaderState::Relocated,
          "DIR64 relocation succeeds on a synthetic image");

    std::array<std::uint8_t, 8U> after{};
    const auto after_read = address_space.read(
        memory::GuestVirtualAddress{target},
        std::span<std::uint8_t>(after.data(), after.size()));
    check(after_read.ok && loader_load_u64(std::span<const std::uint8_t>(
                               after.data(), after.size())) == 0x1122334400010000ULL,
          "DIR64 writes image-base-override minus PE ImageBase into the mapped target");

    std::array<std::uint8_t, 1U> sentinel_after{};
    const auto sentinel_after_read = address_space.read(
        memory::GuestVirtualAddress{options.image_base_override + loader_entry_rva},
        std::span<std::uint8_t>(sentinel_after.data(), sentinel_after.size()));
    check(sentinel_after_read.ok && sentinel_after[0U] == 0x90U,
          "relocation and prepare do not execute the recorded entry point");

    check(image_loader.prepare() == loader::LoadError::None &&
              image_loader.state() == loader::LoaderState::Ready,
          "prepare records readiness after mapping and relocation only");
    const auto diagnostic_before_unload = image_loader.first_memory_result();
    const auto unload_error = image_loader.unload();
    check(unload_error == loader::LoadError::None &&
              image_loader.state() == loader::LoaderState::Unloaded &&
              image_loader.virtual_mappings().empty() &&
              image_loader.inaccessible_physical_regions().size() == 2U,
          (std::string("successful unload returns None, clears virtual mappings and marks "
                       "both backing regions inaccessible (error=") +
           loader::to_string(unload_error) + ", state=" +
           loader::to_string(image_loader.state()) + ", inaccessible=" +
           std::to_string(image_loader.inaccessible_physical_regions().size()) + ")")
              .c_str());

    const auto diagnostic_after_unload = image_loader.first_memory_result();
    const auto same_memory_result = [](const memory::MemoryResult& left,
                                       const memory::MemoryResult& right) noexcept {
        return left.ok == right.ok && left.fault == right.fault &&
               left.access == right.access && left.context == right.context &&
               left.address == right.address &&
               left.requested_length == right.requested_length &&
               left.completed_bytes == right.completed_bytes &&
               left.translated_address == right.translated_address &&
               left.region_base == right.region_base;
    };
    check(same_memory_result(diagnostic_before_unload, diagnostic_after_unload),
          "successful unload does not create or report a new memory failure diagnostic");
}

void test_loader_validation_failure_boundaries() {
    using namespace xbridge;

    check(loader::can_transition(loader::LoaderState::Validated, loader::LoaderState::Mapped) &&
              loader::can_transition(loader::LoaderState::Mapped, loader::LoaderState::Relocated) &&
              loader::can_transition(loader::LoaderState::Relocated, loader::LoaderState::Ready) &&
              loader::can_transition(loader::LoaderState::Ready, loader::LoaderState::Unloaded) &&
              loader::can_transition(loader::LoaderState::Failed, loader::LoaderState::Unloaded) &&
              !loader::can_transition(loader::LoaderState::Ready, loader::LoaderState::Mapped) &&
              !loader::can_transition(loader::LoaderState::Failed, loader::LoaderState::Ready),
          "loader state machine permits forward progress and rejects skipped or backward steps");

    check(std::string_view(loader::to_string(loader::LoaderState::Ready)) == "Ready" &&
              std::string_view(loader::to_string(loader::LoadError::UnsupportedRelocation)) ==
                  "UnsupportedRelocation" &&
              std::string_view(loader::to_string(loader::LoadError::InvalidRelocation)) ==
                  "InvalidRelocation",
          "loader state and error names are stable");

    const auto valid = loader_make_pe32plus();
    auto expect_validate_error = [](std::span<const std::uint8_t> bytes,
                                    const loader::LoadOptions& options,
                                    loader::LoadError expected, const char* message) {
        memory::GuestPhysicalMemory physical;
        memory::GuestAddressSpace address_space(physical);
        loader::ExecutableLoader image_loader(address_space, physical);
        check(image_loader.validate(bytes, options) == expected &&
                  image_loader.state() == loader::LoaderState::Failed &&
                  image_loader.last_error() == expected,
              message);
    };

    const std::vector<std::uint8_t> truncated(valid.begin(), valid.begin() + 32);
    expect_validate_error(std::span<const std::uint8_t>(truncated.data(), truncated.size()), {},
                          loader::LoadError::TruncatedData,
                          "truncated DOS/PE prefix is rejected as TruncatedData");

    auto bad_mz = valid;
    bad_mz[0U] = static_cast<std::uint8_t>('N');
    expect_validate_error(std::span<const std::uint8_t>(bad_mz.data(), bad_mz.size()), {},
                          loader::LoadError::InvalidHeader,
                          "invalid DOS signature is rejected as InvalidHeader");

    auto bad_signature = valid;
    bad_signature[loader_pe_offset] = static_cast<std::uint8_t>('X');
    expect_validate_error(std::span<const std::uint8_t>(bad_signature.data(),
                                                        bad_signature.size()), {},
                          loader::LoadError::InvalidHeader,
                          "invalid PE signature is rejected as InvalidHeader");

    LoaderImageOptions unsupported_machine;
    unsupported_machine.machine = 0x014CU;
    const auto machine_bytes = loader_make_pe32plus(unsupported_machine);
    expect_validate_error(std::span<const std::uint8_t>(machine_bytes.data(),
                                                        machine_bytes.size()), {},
                          loader::LoadError::UnsupportedImage,
                          "non-AMD64 machine is rejected as UnsupportedImage");

    LoaderImageOptions pe32_options;
    pe32_options.magic = 0x010BU;
    const auto pe32_bytes = loader_make_pe32plus(pe32_options);
    expect_validate_error(std::span<const std::uint8_t>(pe32_bytes.data(), pe32_bytes.size()), {},
                          loader::LoadError::UnsupportedImage,
                          "PE32 optional magic is rejected as UnsupportedImage");

    auto unaligned = valid;
    const std::size_t optional = static_cast<std::size_t>(loader_pe_offset) + 24U;
    const std::size_t section_table = optional + loader_optional_size;
    analysis_put_u32(unaligned, section_table + 12U, 0x1800U);
    expect_validate_error(std::span<const std::uint8_t>(unaligned.data(), unaligned.size()), {},
                          loader::LoadError::AlignmentError,
                          "misaligned section RVA is rejected as AlignmentError");

    auto overlap = valid;
    analysis_put_u32(overlap, section_table + 40U + 12U, loader_text_rva);
    expect_validate_error(std::span<const std::uint8_t>(overlap.data(), overlap.size()), {},
                          loader::LoadError::Overlap,
                          "overlapping virtual sections are rejected as Overlap");

    auto excessive_sections = valid;
    analysis_put_u16(excessive_sections, static_cast<std::size_t>(loader_pe_offset) + 6U, 200U);
    expect_validate_error(std::span<const std::uint8_t>(excessive_sections.data(),
                                                        excessive_sections.size()), {},
                          loader::LoadError::LimitExceeded,
                          "excessive section count is rejected as LimitExceeded");

    auto huge_lfanew = valid;
    analysis_put_u32(huge_lfanew, 0x3CU, 0xFFFFFFF0U);
    expect_validate_error(std::span<const std::uint8_t>(huge_lfanew.data(), huge_lfanew.size()), {},
                          loader::LoadError::TruncatedData,
                          "out-of-range e_lfanew is rejected as TruncatedData");

    loader::LoadOptions small_limit;
    small_limit.limits.max_bytes = 16U;
    expect_validate_error(std::span<const std::uint8_t>(valid.data(), valid.size()), small_limit,
                          loader::LoadError::LimitExceeded,
                          "image over an explicit byte limit is rejected as LimitExceeded");

    loader::LoadOptions small_virtual;
    small_virtual.limits.max_virtual_address = 1U;
    expect_validate_error(std::span<const std::uint8_t>(valid.data(), valid.size()), small_virtual,
                          loader::LoadError::Overflow,
                          "image beyond the virtual-address limit is rejected as Overflow");

    memory::GuestPhysicalMemory physical;
    memory::GuestAddressSpace address_space(physical);
    loader::ExecutableLoader sequence(address_space, physical);
    check(sequence.map() == loader::LoadError::InvalidState &&
              sequence.state() == loader::LoaderState::Failed,
          "map before validate is rejected as InvalidState");
}

void test_loader_relocation_failure_paths_and_rollback() {
    using namespace xbridge;

    LoaderImageOptions unsupported_options;
    unsupported_options.relocations.push_back(
        LoaderRelocationEntry{3U, 0x2000U, 0x40U});
    const auto unsupported_bytes = loader_make_pe32plus(unsupported_options);
    loader::LoadOptions relocate_options;
    relocate_options.image_base_override = 0x0000000140010000ULL;

    memory::GuestPhysicalMemory unsupported_physical;
    memory::GuestAddressSpace unsupported_space(unsupported_physical);
    loader::ExecutableLoader unsupported_loader(unsupported_space, unsupported_physical);
    check(unsupported_loader.validate(
              std::span<const std::uint8_t>(unsupported_bytes.data(), unsupported_bytes.size()),
              relocate_options) == loader::LoadError::None &&
              unsupported_loader.map() == loader::LoadError::None,
          "mixed DIR64 and unsupported relocation image validates and maps");

    const std::uint64_t target = relocate_options.image_base_override + 0x2030U;
    std::array<std::uint8_t, 8U> before{};
    const auto before_read = unsupported_space.read(
        memory::GuestVirtualAddress{target},
        std::span<std::uint8_t>(before.data(), before.size()));
    check(before_read.ok, "mixed relocation target is readable before relocation");

    check(unsupported_loader.relocate() == loader::LoadError::UnsupportedRelocation &&
              unsupported_loader.state() == loader::LoaderState::Failed,
          "unsupported relocation after a valid DIR64 entry fails the whole relocation phase");
    std::array<std::uint8_t, 8U> after{};
    const auto after_read = unsupported_space.read(
        memory::GuestVirtualAddress{target},
        std::span<std::uint8_t>(after.data(), after.size()));
    check(after_read.ok && after == before,
          "two-phase relocation performs no writes when a later entry is unsupported");

    LoaderImageOptions gap_options;
    gap_options.relocations.push_back(LoaderRelocationEntry{10U, 0x3000U, 0x30U});
    const auto gap_bytes = loader_make_pe32plus(gap_options);
    memory::GuestPhysicalMemory gap_physical;
    memory::GuestAddressSpace gap_space(gap_physical);
    loader::ExecutableLoader gap_loader(gap_space, gap_physical);
    check(gap_loader.validate(std::span<const std::uint8_t>(gap_bytes.data(), gap_bytes.size()),
                              relocate_options) == loader::LoadError::None &&
              gap_loader.map() == loader::LoadError::None &&
              gap_loader.relocate() == loader::LoadError::InvalidRelocation,
          "DIR64 target outside any mapped section is rejected as InvalidRelocation");
    std::array<std::uint8_t, 8U> gap_after{};
    const auto gap_read = gap_space.read(
        memory::GuestVirtualAddress{target},
        std::span<std::uint8_t>(gap_after.data(), gap_after.size()));
    check(gap_read.ok && gap_after == before,
          "invalid DIR64 target leaves the previously valid target unmodified");

    const auto valid_bytes = loader_make_pe32plus();
    memory::GuestPhysicalMemory rollback_physical;
    memory::GuestAddressSpace rollback_space(rollback_physical);
    constexpr std::uint64_t blocker_physical_base = 0x0000000100000000ULL;
    const auto blocker_region =
        rollback_physical.add_region(memory::GuestPhysicalAddress{blocker_physical_base},
                                     0x1000U, memory::RegionKind::Normal,
                                     memory::MemoryPermissions{true, true, false});
    const auto blocker_map = rollback_space.map(
        memory::GuestVirtualAddress{loader_image_base + loader_data_rva},
        memory::GuestPhysicalAddress{blocker_physical_base}, 0x1000U,
        memory::MemoryPermissions{true, true, false});
    check(blocker_region.ok && blocker_map.ok,
          "rollback fixture pre-occupies the second section's virtual range");

    loader::ExecutableLoader rollback_loader(rollback_space, rollback_physical);
    check(rollback_loader.validate(
              std::span<const std::uint8_t>(valid_bytes.data(), valid_bytes.size()), {}) ==
              loader::LoadError::None,
          "rollback fixture validates the synthetic image");
    check(rollback_loader.map() == loader::LoadError::MemoryFailure &&
              rollback_loader.state() == loader::LoaderState::Failed &&
              !rollback_loader.first_memory_result().ok,
          "mid-map virtual mapping failure is reported as MemoryFailure");
    check(rollback_loader.virtual_mappings().empty() &&
              rollback_loader.inaccessible_physical_regions().size() == 2U,
          (std::string("mid-map failure rolls back virtual mappings and records both backing "
                       "regions as inaccessible (virtual=") +
           std::to_string(rollback_loader.virtual_mappings().size()) +
           ", inaccessible=" +
           std::to_string(rollback_loader.inaccessible_physical_regions().size()) + ")")
              .c_str());

    std::array<std::uint8_t, 1U> probe{};
    const auto probe_read = rollback_space.read(
        memory::GuestVirtualAddress{loader_image_base + 0x1000U},
        std::span<std::uint8_t>(probe.data(), probe.size()));
    check(!probe_read.ok,
          "rolled-back first section is no longer readable through GuestAddressSpace");

    LoaderImageOptions three_section_options;
    three_section_options.section_count = 3U;
    const auto three_section_bytes = loader_make_pe32plus(three_section_options);
    constexpr std::uint64_t overflow_base = 0x0000000140020000ULL;
    loader::LoadOptions overflow_options;
    overflow_options.image_base_override = overflow_base;
    overflow_options.limits.max_physical_address = overflow_base + 0x2000U + 0x100U;

    memory::GuestPhysicalMemory overflow_physical;
    memory::GuestAddressSpace overflow_space(overflow_physical);
    loader::ExecutableLoader overflow_loader(overflow_space, overflow_physical);
    check(overflow_loader.validate(
              std::span<const std::uint8_t>(three_section_bytes.data(),
                                            three_section_bytes.size()),
              overflow_options) == loader::LoadError::None &&
              overflow_loader.state() == loader::LoaderState::Validated &&
              overflow_loader.image_info().sections.size() == 3U,
          "three-section fixture validates with an injected third section");
    check(overflow_loader.map() == loader::LoadError::Overflow &&
              overflow_loader.state() == loader::LoaderState::Failed &&
              overflow_loader.virtual_mappings().empty() &&
              overflow_loader.inaccessible_physical_regions().size() == 2U,
          (std::string("post-map physical limit rollback reports Overflow and clears two "
                       "mappings (error=") +
           loader::to_string(overflow_loader.last_error()) + ", state=" +
           loader::to_string(overflow_loader.state()) + ", virtual=" +
           std::to_string(overflow_loader.virtual_mappings().size()) + ", inaccessible=" +
           std::to_string(overflow_loader.inaccessible_physical_regions().size()) + ")")
              .c_str());

    std::array<std::uint8_t, 1U> overflow_probe{};
    const auto overflow_probe_read = overflow_space.read(
        memory::GuestVirtualAddress{overflow_base + loader_text_rva},
        std::span<std::uint8_t>(overflow_probe.data(), overflow_probe.size()));
    check(!overflow_probe_read.ok,
          "rolled-back first section is no longer readable after Overflow rollback");
}

// XB-NEXT-DEVELOPMENT-03-C1 — neutral runtime service surface and registry.

struct SyntheticSystemService final : runtime::ISystemService {};
struct SyntheticGraphicsService final : runtime::IGraphicsService {};

void test_runtime_service_registry_and_transitions() {
    using namespace xbridge;

    static_assert(runtime::kServiceCount == 13U,
                  "the neutral service registry has exactly thirteen families");
    static_assert(runtime::ServiceStatus::Unavailable == runtime::ServiceStatus::Unsupported,
                  "Unavailable is the documented alias of Unsupported");

    const std::array<runtime::ServiceId, 13U> ids{{
        runtime::ServiceId::System,
        runtime::ServiceId::RuntimeInitialization,
        runtime::ServiceId::GuestProcess,
        runtime::ServiceId::Thread,
        runtime::ServiceId::Synchronization,
        runtime::ServiceId::VirtualMemory,
        runtime::ServiceId::File,
        runtime::ServiceId::Storage,
        runtime::ServiceId::Input,
        runtime::ServiceId::Audio,
        runtime::ServiceId::Graphics,
        runtime::ServiceId::Timing,
        runtime::ServiceId::ErrorLogging,
    }};
    const std::array<std::string_view, 13U> names{{
        "System", "RuntimeInitialization", "GuestProcess", "Thread", "Synchronization",
        "VirtualMemory", "File", "Storage", "Input", "Audio", "Graphics", "Timing",
        "ErrorLogging",
    }};

    bool all_ids_document_named = true;
    for (std::size_t index = 0U; index < ids.size(); ++index) {
        const auto descriptor = runtime::describe_service(ids[index]);
        all_ids_document_named =
            all_ids_document_named && runtime::is_valid_service(ids[index]) &&
            descriptor.id == ids[index] && descriptor.name == names[index] &&
            runtime::service_id_name(ids[index]) == names[index] &&
            std::string_view(runtime::to_string(ids[index])) == names[index];
    }
    const auto invalid_id = static_cast<runtime::ServiceId>(13U);
    check(all_ids_document_named && !runtime::is_valid_service(invalid_id) &&
              runtime::service_id_name(invalid_id) == "Unknown" &&
              std::string_view(runtime::to_string(invalid_id)) == "Unknown",
          "all thirteen neutral service IDs have stable names and invalid IDs stay unknown");

    SyntheticSystemService system_service;
    SyntheticGraphicsService graphics_service;
    runtime::SystemServiceRegistry registry;
    check(registry.count() == 0U && !registry.is_available(runtime::ServiceId::System) &&
              registry.status(runtime::ServiceId::System) == runtime::ServiceStatus::Unsupported &&
              registry.query(runtime::ServiceId::System).status ==
                  runtime::ServiceStatus::Unsupported,
          "empty registry reports Unsupported rather than a simulated service");

    check(!registry.register_service(runtime::ServiceId::System, nullptr) &&
              !registry.register_service(invalid_id, &system_service) && registry.count() == 0U,
          "registry rejects null pointers and unknown service IDs");

    check(registry.register_service(runtime::ServiceId::System, &system_service) &&
              registry.register_service(runtime::ServiceId::Graphics, &graphics_service) &&
              registry.count() == 2U,
          "registry accepts distinct non-owning service pointers");
    check(!registry.register_service(runtime::ServiceId::System, &system_service) &&
              registry.count() == 2U,
          "duplicate registration is rejected without changing the count");

    const runtime::SystemServiceRegistry& const_registry = registry;
    check(registry.find(runtime::ServiceId::System) == &system_service &&
              const_registry.find(runtime::ServiceId::Graphics) == &graphics_service &&
              registry.find_as<runtime::ISystemService>(runtime::ServiceId::System) ==
                  &system_service &&
              registry.find_as<runtime::IGraphicsService>(runtime::ServiceId::System) == nullptr &&
              registry.find(invalid_id) == nullptr,
          "find and find_as return the supplied pointer only for its registered family");

    check(registry.query(runtime::ServiceId::System).status ==
                  runtime::ServiceStatus::Available &&
              registry.query(runtime::ServiceId::System).value == 0U &&
              registry.is_available(runtime::ServiceId::System) &&
              registry.query(invalid_id).status == runtime::ServiceStatus::NotRegistered,
          "query distinguishes available, unsupported and not-registered services");

    check(registry.unregister_service(runtime::ServiceId::System) &&
              !registry.unregister_service(runtime::ServiceId::System) &&
              !registry.unregister_service(invalid_id) && registry.count() == 1U &&
              registry.find(runtime::ServiceId::System) == nullptr,
          "unregister is idempotent for absent and invalid IDs");

    registry.clear();
    check(registry.count() == 0U && registry.find(runtime::ServiceId::Graphics) == nullptr,
          "clear removes all non-owning registrations");

    const auto service_transition =
        runtime::make_service_transition(runtime::ServiceId::VirtualMemory, 0x1234U);
    check(service_transition.kind == runtime::TransitionKind::ServiceRequest &&
              service_transition.service == runtime::ServiceId::VirtualMemory &&
              service_transition.service_id ==
                  static_cast<std::uint64_t>(runtime::ServiceId::VirtualMemory) &&
              service_transition.payload == 0x1234U,
          "service transition descriptor carries a neutral ID and payload");
    const auto invalid_transition = runtime::make_service_transition(invalid_id, 7U);
    check(invalid_transition.service_id == 0U && invalid_transition.payload == 7U,
          "invalid service transition uses a zero neutral service ID");
    const auto fault_transition =
        runtime::make_exit_transition(runtime::TransitionKind::FaultExit, 9U);
    check(fault_transition.kind == runtime::TransitionKind::FaultExit &&
              fault_transition.service == runtime::ServiceId::System &&
              fault_transition.service_id == 0U && fault_transition.payload == 9U,
          "exit transition descriptor is independent of any guest execution backend");

    check(std::string_view(runtime::to_string(runtime::TransitionKind::ServiceRequest)) ==
              "ServiceRequest" &&
              std::string_view(runtime::to_string(runtime::TransitionKind::NormalExit)) ==
                  "NormalExit" &&
              std::string_view(runtime::to_string(runtime::ServiceStatus::Available)) ==
                  "Available" &&
              runtime::unsupported_result().status == runtime::ServiceStatus::Unsupported &&
              runtime::not_registered_result().status == runtime::ServiceStatus::NotRegistered,
          "neutral transition and status names are stable");
}

// XB-NEXT-DEVELOPMENT-03-F1 — analyzer linkage, labels and exact JSON contract.

bool analysis_vector_is_sorted_unique(const std::vector<std::string>& values) noexcept {
    for (std::size_t index = 1U; index < values.size(); ++index) {
        if (values[index - 1U] >= values[index]) {
            return false;
        }
    }
    return true;
}

bool analysis_new_field_labels_are_known(const std::vector<std::string>& values) noexcept {
    constexpr std::string_view marker = "label=";
    for (const std::string& value : values) {
        const std::size_t at = value.rfind(marker.data(), value.size(), marker.size());
        if (at == std::string::npos) {
            return false;
        }
        const std::string_view label(value.data() + at + marker.size(),
                                     value.size() - at - marker.size());
        if (label != "Confirmed" && label != "Indicator" && label != "Unknown") {
            return false;
        }
    }
    return true;
}

void test_analysis_runtime_linkage_and_exact_json() {
    using namespace xbridge;

    AnalysisFixture fixture = analysis_make_fixture("runtime_linkage");
    check(fixture.ok, "analyzer linkage fixture creates a unique temporary directory");
    if (!fixture.ok)
        return;

    auto bytes = analysis_make_pe32plus_x64(0x5F5E0FF0U);
    const std::size_t optional = static_cast<std::size_t>(analysis_pe_offset) + 24U;
    analysis_put_u32(bytes, optional + 112U + 8U, 0x1400U);
    analysis_put_u32(bytes, optional + 112U + 12U, 0x78U);

    const std::array<std::string_view, 5U> names{{
        "xmem", "xg_xs", "d3d12_xs", "xgameruntime", "UnknownDep.dll"}};
    const std::array<std::uint32_t, 5U> name_rvas{{
        0x1500U, 0x1520U, 0x1540U, 0x1560U, 0x1580U}};
    for (std::size_t index = 0U; index < names.size(); ++index) {
        const std::size_t descriptor = analysis_rva_offset(0x1400U + 20U * index);
        analysis_put_u32(bytes, descriptor + 12U, name_rvas[index]);
        const std::size_t name_offset = analysis_rva_offset(name_rvas[index]);
        analysis_put_ascii(bytes, name_offset, names[index]);
        bytes[name_offset + names[index].size()] = std::uint8_t{0U};
    }
    analysis_put_u32(bytes, analysis_rva_offset(0x1400U + 5U * 20U) + 12U, 0U);

    check(analysis_write_file(fixture.root / "linkage.bin", bytes),
          "analyzer linkage fixture writes a synthetic import-dependency PE image");

    std::error_code error;
    analysis::AnalysisReport report;
    check_does_not_throw([&] { report = analysis::analyze_directory(fixture.root, error); },
                         "analyzer linkage scan does not throw");
    const analysis::AnalysisEntry* entry = analysis_find_entry(report, "linkage.bin");
    check(!error && entry != nullptr, "analyzer linkage scan succeeds with one entry");
    if (entry == nullptr) {
        analysis_cleanup_fixture(fixture);
        return;
    }

    const std::array<std::string, 6U> expected_candidates{{
        "dependency=UnknownDep.dll; service=unknown; label=Unknown",
        "dependency=d3d12_xs; service=Graphics; label=Indicator",
        "dependency=xg_xs; service=RuntimeInitialization; label=Indicator",
        "dependency=xgameruntime; service=RuntimeInitialization; label=Indicator",
        "dependency=xmem; service=System; label=Indicator",
        "dependency=xmem; service=VirtualMemory; label=Indicator",
    }};
    check(entry->runtime_service_candidates == std::vector<std::string>(
              expected_candidates.begin(), expected_candidates.end()),
          "reviewed dependency names map only to neutral service indicators; others stay Unknown");
    check(analysis_vector_is_sorted_unique(entry->runtime_service_candidates) &&
              analysis_vector_is_sorted_unique(entry->loader_requirements) &&
              analysis_vector_is_sorted_unique(entry->unsupported_or_unknown_structures) &&
              analysis_new_field_labels_are_known(entry->runtime_service_candidates) &&
              analysis_new_field_labels_are_known(entry->loader_requirements) &&
              analysis_new_field_labels_are_known(entry->unsupported_or_unknown_structures),
          "new analyzer fields are sorted, deduplicated and use only evidence labels");

    const std::string size_requirement =
        "file_size=" + std::to_string(entry->size_bytes) + "; label=Confirmed";
    check(analysis_vector_has_exact(entry->loader_requirements, size_requirement) &&
              analysis_vector_has_exact(entry->loader_requirements,
                                        "section_count=1; label=Confirmed") &&
              analysis_vector_has_exact(entry->loader_requirements,
                                        "relocation_type=10 (DIR64); label=Confirmed") &&
              analysis_vector_has_exact(entry->loader_requirements,
                                        "relocation_type=3 (HIGHLOW); label=Confirmed") &&
              analysis_vector_has_exact(entry->loader_requirements,
                                        "import=xmem; label=Confirmed"),
          "loader requirements report confirmed size, sections, relocations and imports");

    const std::array<std::string_view, 3U> unknown_structures{{
        "image_base=unknown; label=Unknown",
        "relocation_offsets=unknown; label=Unknown",
        "section_characteristics=unknown; label=Unknown",
    }};
    bool all_unknown_structures = true;
    for (const std::string_view expected : unknown_structures) {
        all_unknown_structures = all_unknown_structures &&
            analysis_vector_has_exact(entry->unsupported_or_unknown_structures, expected);
    }
    check(all_unknown_structures,
          "unavailable section and relocation details are reported as Unknown, not invented");

    const std::string linkage_json = report.to_json();
    check(!analysis_text_contains(linkage_json, "%") &&
              !analysis_text_contains(linkage_json, "readiness") &&
              !analysis_text_contains(linkage_json, "ready"),
          "analyzer linkage output contains no readiness percentage or claim");
    check(linkage_json == report.to_json(),
          "analyzer linkage JSON is byte-identical on repeated calls");

    analysis_cleanup_fixture(fixture);

    analysis::AnalysisReport exact_report;
    exact_report.source_directory = "source";
    exact_report.notes.push_back("note");
    analysis::AnalysisEntry exact_entry;
    exact_entry.relative_path = "entry.bin";
    exact_entry.size_bytes = 7U;
    exact_entry.file_type = "data";
    exact_entry.architecture = "no_platform_claim";
    exact_entry.timestamp = "not_applicable";
    exact_entry.runtime_service_candidates.push_back(
        "dependency=xmem; service=System; label=Indicator");
    exact_entry.loader_requirements.push_back("file_size=7; label=Confirmed");
    exact_entry.unsupported_or_unknown_structures.push_back(
        "image_base=unknown; label=Unknown");
    exact_entry.status = analysis::DetectionStatus::Indicator;
    exact_report.entries.push_back(exact_entry);

    const std::string expected_json =
        "{\n"
        "  \"source_directory\": \"source\",\n"
        "  \"entries\": [\n"
        "    {\n"
        "      \"relative_path\": \"entry.bin\",\n"
        "      \"size_bytes\": 7,\n"
        "      \"file_type\": \"data\",\n"
        "      \"architecture\": \"no_platform_claim\",\n"
        "      \"platform_indicators\": []\n"
        ",\n"
        "      \"pe_fields\": []\n"
        ",\n"
        "      \"imports\": []\n"
        ",\n"
        "      \"exports\": []\n"
        ",\n"
        "      \"relocation_types\": []\n"
        ",\n"
        "      \"debug_types\": []\n"
        ",\n"
        "      \"dependencies\": []\n"
        ",\n"
        "      \"resource_kinds\": []\n"
        ",\n"
        "      \"timestamp\": \"not_applicable\",\n"
        "      \"uncertainties\": []\n"
        ",\n"
        "      \"component_mapping\": []\n"
        ",\n"
        "      \"runtime_service_candidates\": [\n"
        "        \"dependency=xmem; service=System; label=Indicator\"\n"
        "      ]\n"
        ",\n"
        "      \"loader_requirements\": [\n"
        "        \"file_size=7; label=Confirmed\"\n"
        "      ]\n"
        ",\n"
        "      \"unsupported_or_unknown_structures\": [\n"
        "        \"image_base=unknown; label=Unknown\"\n"
        "      ]\n"
        ",\n"
        "      \"status\": \"indicator\"\n"
        "    }\n"
        "  ],\n"
        "  \"notes\": [\n"
        "    \"note\"\n"
        "  ]\n"
        "}\n";
    const std::string actual_json = exact_report.to_json();
    if (actual_json != expected_json) {
        std::cerr << "[DIAG] exact analyzer JSON mismatch; actual output follows:\n"
                  << actual_json;
    }
    check(actual_json == expected_json,
          "exact analyzer JSON is unchanged except for the three deterministic fields before status");
}

}

int main() {
    using namespace xbridge;

    ProfileKind parsed{};
    check(parse_profile("series_x", parsed) && parsed == ProfileKind::SeriesX,
          "parse Series X profile");
    check(!parse_profile("series_s", parsed),
          "Series S is not a supported target");
    check(!parse_profile("unknown", parsed), "reject unknown profile");

    test_x64_cpu_state();
    test_x64_register_views();
    test_x64_register_views_all_registers();
    test_x64_decoder_nop();
    test_x64_decoder_mov_imm32();
    test_x64_decoder_mov_imm64();
    test_x64_decoder_mov_reg_to_reg();
    test_x64_decoder_mov_reg_to_reg_rex();
    test_x64_decoder_mov_memory();
    test_x64_decoder_empty_input();
    test_x64_decoder_unknown_opcode();
    test_x64_decoder_truncated_input();
    test_x64_decoder_invalid_encoding();
    test_x64_decoder_8bit_mov_rejected();
    test_x64_decoder_no_state_mutation();
    test_x64_interpreter_nop();
    test_x64_interpreter_mov_imm32();
    test_x64_interpreter_mov_imm64();
    test_x64_interpreter_mov_reg_to_reg();
    test_x64_interpreter_mov_reg_to_reg_32bit_zero_extend();
    test_x64_interpreter_mov_memory_roundtrip();
    test_x64_interpreter_memory_bounds();
    test_x64_interpreter_mov_memory_registers();
    test_x64_interpreter_mov_memory_rex_b();
    test_x64_interpreter_mov_memory_sib_rejected();
    test_x64_interpreter_mov_memory_rip_relative_rejected();
    test_x64_interpreter_mov_memory_64bit();
    test_x64_interpreter_mov_memory_64bit_write_atomic();
    test_x64_interpreter_mov_memory_address_above_32bit();
    test_x64_interpreter_memory_error_no_state_mutation();
    test_x64_interpreter_error_no_state_mutation();
    test_x64_instruction_decoder();
    test_x64_decoder_rex_extended_registers();
    test_x64_interpreter_rex_extended_registers();
    test_x64_decoder_rex_x_recognized_unused();
    test_x64_decoder_truncated_rex_modrm();
    test_x64_interpreter_non_ok_decode_noop();
    test_x64_step_instruction_facade();
    test_cpu_execution_hardening();
    test_cpu_program_lifecycle();
    test_host_capability_provider();
    test_x64_step_at_rip_bridge();
    test_guest_address_helpers();
    test_memory_result_contract();
    test_guest_physical_memory_regions();
    test_guest_physical_memory_accesses();
    test_guest_physical_memory_commitment_boundary();
    test_guest_address_space_mapping_contract();
    test_guest_address_space_permissions_and_fetch();
    test_guest_address_space_device_dispatch();
    test_guest_address_space_atomicity_and_boundaries();

    test_x64_step_at_rip_via_execute_only_above_4gib();
    test_x64_execute_instruction_via_data_above_4gib_little_endian();
    test_x64_via_execute_permission_denied_at_mapping_and_region();
    test_x64_via_data_permission_denied_no_partial_write();
    test_x64_via_invalid_addresses_no_truncation();
    test_x64_step_at_rip_via_bounded_fetch_retry();
    test_x64_via_memory_fault_propagation();
    test_x64_virtual_memory_short_completion_failure();
    test_x64_execute_instruction_via_legacy_parity();
    test_x64_step_instruction_via_legacy_parity();
    test_x64_step_at_rip_via_legacy_parity();

    test_analysis_valid_pe32plus_x64_synthetic();
    test_analysis_gdk_manifest_indicators();
    test_analysis_weak_extension_indicators();
    test_analysis_malformed_inputs_no_false_classification();
    test_analysis_non_date_timestamps();
    test_analysis_report_json_fields_escaping_and_determinism();
    test_analysis_invalid_directory_error_code();
    test_analysis_max_scan_bytes_truncation();
    test_analysis_read_only_behavior();
    test_analysis_deterministic_entry_order();
    test_analysis_resource_name_odd_max_string_bytes_regression();
    test_analysis_dll_characteristics_exact_output_regression();

    test_native_feature_detection_and_gating();
    test_native_backend_routines_and_interpreter_parity();
    test_guest_execution_context_stack_and_memory_failures();
    test_loader_synthetic_valid_lifecycle_and_permissions();
    test_loader_validation_failure_boundaries();
    test_loader_relocation_failure_paths_and_rollback();
    test_runtime_service_registry_and_transitions();
    test_analysis_runtime_linkage_and_exact_json();

    host::HostCapabilities capable_host{};
    capable_host.logical_threads = 16;
    capable_host.available_ram_mb = 8192;
    capable_host.dedicated_vram_mb = 8192;
    capable_host.cpu_jit_available = true;
    capable_host.vulkan_available = true;
    capable_host.direct3d12_available = true;
    const auto accelerated = host::select_execution_plan(capable_host, {});
    check(accelerated.graphics == host::GraphicsBackend::Vulkan &&
          accelerated.graphics_accelerated,
          "automatic policy selects supported graphics acceleration");
    check(accelerated.cpu == host::CpuBackend::Interpreter,
          "automatic CPU selection uses interpreter when JIT is available");

    host::HostCapabilities limited_host{};
    limited_host.software_renderer_available = true;
    const auto fallback = host::select_execution_plan(limited_host, {});
    check(fallback.graphics == host::GraphicsBackend::Software && fallback.fallback_used,
          "automatic policy uses software fallback when acceleration is unavailable");
    check(fallback.cpu == host::CpuBackend::Interpreter,
          "automatic CPU selection uses interpreter when JIT is unavailable");

    host::ExecutionSettings no_fallback{};
    no_fallback.allow_software_fallback = false;
    const auto unavailable = host::select_execution_plan(limited_host, no_fallback);
    check(unavailable.graphics == host::GraphicsBackend::Unavailable,
          "policy reports unavailable backend when fallback is disabled");

    host::ExecutionSettings manual{};
    manual.graphics = host::GraphicsPreference::Direct3D12;
    manual.cpu = host::CpuPreference::Interpreter;
    const auto overridden = host::select_execution_plan(capable_host, manual);
    check(overridden.graphics == host::GraphicsBackend::Direct3D12,
          "manual graphics preference overrides automatic selection");
    check(overridden.cpu == host::CpuBackend::Interpreter,
          "explicit interpreter preference selects interpreter");

    host::ExecutionSettings jit_requested{};
    jit_requested.cpu = host::CpuPreference::Jit;
    const auto jit_available = host::select_execution_plan(capable_host, jit_requested);
    check(jit_available.cpu == host::CpuBackend::Jit,
          "explicit JIT preference selects JIT when available");
    const auto jit_unavailable = host::select_execution_plan(limited_host, jit_requested);
    check(jit_unavailable.cpu == host::CpuBackend::Interpreter,
          "explicit JIT preference falls back to interpreter when unavailable");
    check(jit_unavailable.report.find("JIT requested but unavailable") != std::string::npos,
          "CPU report explains unavailable requested JIT");

    memory::GuestMemory memory(256);
    check(memory.read8(0) == 0, "guest memory is zero initialized");
    memory.write32(16, 0x12345678);
    check(memory.read32(16) == 0x12345678, "32-bit guest memory round trip");
    check(memory.read8(16) == 0x78 && memory.read8(19) == 0x12,
          "32-bit guest memory accesses are little-endian");
    memory.write32(17, 0xA1B2C3D4);
    check(memory.read32(17) == 0xA1B2C3D4, "32-bit accesses support unaligned addresses");
    memory.write8(255, 0x5A);
    check(memory.read8(255) == 0x5A, "read and write the last valid guest address");
    memory.write32(252, 0x89ABCDEF);
    check(memory.read32(252) == 0x89ABCDEF, "32-bit access ending at memory boundary succeeds");
    check_throws<std::out_of_range>([&] { memory.read8(256); },
                                    "reject read at one-past-end address");
    check_throws<std::out_of_range>([&] { memory.write8(256, 0); },
                                    "reject write at one-past-end address");
    check_throws<std::out_of_range>([&] { memory.read32(253); },
                                    "reject 32-bit read crossing memory boundary");
    check_throws<std::out_of_range>([&] { memory.write32(253, 0); },
                                    "reject 32-bit write crossing memory boundary");

    const std::vector<std::uint8_t> tail{0xAB, 0xCD};
    memory.load(254, tail);
    check(memory.read8(254) == 0xAB && memory.read8(255) == 0xCD,
          "load succeeds when data ends at memory boundary");
    const std::vector<std::uint8_t> empty_data;
    check_does_not_throw([&] { memory.load(256, empty_data); },
                         "empty load at one-past-end is a no-op");
    check_throws<std::out_of_range>([&] { memory.load(257, empty_data); },
                                    "reject empty load beyond one-past-end");
    check_throws<std::out_of_range>([&] { memory.load(255, tail); },
                                    "reject load crossing memory boundary");

    memory::GuestMemory empty_memory(0);
    check_throws<std::out_of_range>([&] { empty_memory.read8(0); },
                                    "reject read from zero-sized memory");
    check_throws<std::out_of_range>([&] { empty_memory.write8(0, 0); },
                                    "reject write to zero-sized memory");
    check_does_not_throw([&] { empty_memory.load(0, empty_data); },
                         "empty load into zero-sized memory is a no-op");
    check_throws<std::out_of_range>(
        [&] { memory.read32(std::numeric_limits<std::uint32_t>::max() - 1U); },
        "reject high guest address without wrapping");

    const auto address_space_size =
        static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max()) + 1U;
    if (std::numeric_limits<std::size_t>::max() > address_space_size) {
        check_throws<std::length_error>(
            [&] { memory::GuestMemory oversized(static_cast<std::size_t>(address_space_size + 1U)); },
            "reject memory size beyond 32-bit guest address space");
    }

    // r0 = 7; r0 += 5; store r0 at address 64; halt
    const std::vector<std::uint8_t> program{
        0x01, 0x00, 0x07, 0x00, 0x00, 0x00,
        0x02, 0x00, 0x05, 0x00, 0x00, 0x00,
        0x03, 0x00, 0x40, 0x00, 0x00, 0x00,
        0xFF
    };
    memory::GuestMemory cpu_memory(256);
    cpu::TestCpu cpu(cpu_memory);
    cpu.load_program(program);
    cpu.run();
    check(cpu_memory.read32(64) == 12, "TestCpu executes synthetic program");

    std::ostringstream output;
    Runtime runtime(ProfileKind::SeriesX);
    check(runtime.run_demo(output) == 0, "runtime demo succeeds");
    check(output.str().find("not Xbox emulation") != std::string::npos,
          "runtime clearly identifies scaffold status");

    if (failures != 0) {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}
