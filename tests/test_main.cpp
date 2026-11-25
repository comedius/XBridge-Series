#include "xbridge/core/config.hpp"
#include "xbridge/core/runtime.hpp"
#include "xbridge/cpu/test_cpu.hpp"
#include "xbridge/cpu/x64_cpu_state.hpp"
#include "xbridge/cpu/x64_instruction_decoder.hpp"
#include "xbridge/cpu/x64_instruction_interpreter.hpp"
#include "xbridge/memory/memory.hpp"
#include "xbridge/host/execution_policy.hpp"
#include <span>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace {
using namespace xbridge;

int failures = 0;

void check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << "\n";
        ++failures;
    } else {
        std::cout << "[PASS] " << message << "\n";
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
