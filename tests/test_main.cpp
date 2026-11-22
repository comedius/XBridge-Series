#include "xbridge/core/config.hpp"
#include "xbridge/core/runtime.hpp"
#include "xbridge/cpu/test_cpu.hpp"
#include "xbridge/cpu/x64_cpu_state.hpp"
#include "xbridge/cpu/x64_instruction_decoder.hpp"
#include "xbridge/cpu/x64_instruction_interpreter.hpp"
#include "xbridge/memory/memory.hpp"
#include "xbridge/host/execution_policy.hpp"
#include <cstddef>
#include <iostream>
#include <limits>
#include <sstream>
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
    test_x64_decoder_no_state_mutation();
    test_x64_interpreter_nop();
    test_x64_interpreter_mov_imm32();
    test_x64_interpreter_mov_imm64();
    test_x64_interpreter_mov_reg_to_reg();
    test_x64_interpreter_mov_reg_to_reg_32bit_zero_extend();
    test_x64_interpreter_mov_memory_roundtrip();
    test_x64_interpreter_memory_bounds();
    test_x64_interpreter_error_no_state_mutation();
    test_cpu_execution_hardening();
    test_cpu_program_lifecycle();

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
