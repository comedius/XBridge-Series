#pragma once

#include "xbridge/memory/guest_address.hpp"
#include "xbridge/memory/guest_virtual_memory.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace xbridge::execution {

// Storage slots mirror the 16 architectural x64 GPRs; no guest identity or
// Xbox-specific register meaning is implied.
struct GuestRegisterState {
    std::array<std::uint64_t, 16> gprs{};
    std::uint64_t rip{0};
    std::uint64_t rflags{0};
    std::uint64_t out_of_range_scratch_{0U};

    // Bounds-checked accessor, non-throwing. For index >= 16 a per-object
    // scratch slot is reset to zero and returned: reads yield 0 and writes are
    // discarded from the register array. Callers that must mutate registers
    // should use gprs[] with an explicit range check.
    [[nodiscard]] std::uint64_t& gpr(unsigned index) noexcept {
        if (index < gprs.size()) {
            return gprs[index];
        }
        out_of_range_scratch_ = 0U;
        return out_of_range_scratch_;
    }

    [[nodiscard]] const std::uint64_t& gpr(unsigned index) const noexcept {
        static const std::uint64_t out_of_range_zero{0};
        return index < gprs.size() ? gprs[index] : out_of_range_zero;
    }

    [[nodiscard]] bool has_gpr(unsigned index) const noexcept { return index < gprs.size(); }
};

// Ascending synthetic stack: pointer_offset is an offset from base, grows on
// push and shrinks on pop. It is not a claim about any guest stack convention.
struct GuestStack {
    memory::GuestVirtualAddress base{};
    std::uint64_t size{0};
    std::uint64_t pointer_offset{0};
};

enum class GuestHostTransitionKind { ServiceRequest, NormalExit, FaultExit };

struct GuestHostTransition {
    GuestHostTransitionKind kind{GuestHostTransitionKind::NormalExit};
    std::uint64_t service_id{0};
    std::uint64_t payload{0};
};

// Owns only synthetic register/stack/transition values. Guest bytes are always
// accessed through the non-owning IGuestVirtualMemory reference; no raw host
// pointer into guest backing storage is stored or returned.
class GuestExecutionContext {
public:
    explicit GuestExecutionContext(memory::IGuestVirtualMemory& memory) noexcept : memory_(memory) {}

    [[nodiscard]] GuestRegisterState& registers() noexcept { return registers_; }
    [[nodiscard]] const GuestRegisterState& registers() const noexcept { return registers_; }
    void set_registers(const GuestRegisterState& registers) noexcept { registers_ = registers; }

    [[nodiscard]] GuestStack& stack() noexcept { return stack_; }
    [[nodiscard]] const GuestStack& stack() const noexcept { return stack_; }
    void set_stack(const GuestStack& stack) noexcept { stack_ = stack; }

    [[nodiscard]] const GuestHostTransition& last_transition() const noexcept { return transition_; }
    void set_transition(const GuestHostTransition& transition) noexcept { transition_ = transition; }

    [[nodiscard]] memory::IGuestVirtualMemory& memory() const noexcept { return memory_; }

    // Writes eight little-endian bytes at base + pointer_offset and advances
    // pointer_offset by 8 only when the memory write succeeds.
    [[nodiscard]] bool push_u64(std::uint64_t value) noexcept;

    // Reads eight little-endian bytes at base + pointer_offset - 8 and retreats
    // pointer_offset by 8 only when the memory read succeeds.
    [[nodiscard]] bool pop_u64(std::uint64_t& value) noexcept;

private:
    memory::IGuestVirtualMemory& memory_;
    GuestRegisterState registers_{};
    GuestStack stack_{};
    GuestHostTransition transition_{};
};

[[nodiscard]] const char* to_string(GuestHostTransitionKind kind) noexcept;

} // namespace xbridge::execution
