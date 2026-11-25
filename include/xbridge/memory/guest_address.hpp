#pragma once

#include <compare>
#include <cstdint>
#include <limits>

namespace xbridge::memory {

// Guest address domains are intentionally distinct types. Host pointers are never
// represented by these types.
struct GuestVirtualAddress {
    std::uint64_t value{};

    friend constexpr bool operator==(const GuestVirtualAddress&, const GuestVirtualAddress&) = default;
    friend constexpr auto operator<=>(const GuestVirtualAddress&, const GuestVirtualAddress&) = default;
};

struct GuestPhysicalAddress {
    std::uint64_t value{};

    friend constexpr bool operator==(const GuestPhysicalAddress&, const GuestPhysicalAddress&) = default;
    friend constexpr auto operator<=>(const GuestPhysicalAddress&, const GuestPhysicalAddress&) = default;
};

// Computes the exclusive end of [base, base + length) without wrapping.
// A zero-length range at UINT64_MAX is valid; any positive length that wraps is not.
[[nodiscard]] inline bool checked_range_end(
    std::uint64_t base, std::uint64_t length, std::uint64_t& end) noexcept {
    if (length > (std::numeric_limits<std::uint64_t>::max)() - base)
        return false;
    end = base + length;
    return true;
}

} // namespace xbridge::memory
