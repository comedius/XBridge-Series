#pragma once

#include "xbridge/memory/guest_address.hpp"
#include "xbridge/memory/guest_address_space.hpp"
#include "xbridge/memory/guest_memory_result.hpp"
#include "xbridge/memory/memory.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#include <stdexcept>

namespace xbridge::memory {

// Virtual-memory seam used by the x64 interpreter for fetch, data reads and
// data writes. Implementations never own the guest memory they expose.
class IGuestVirtualMemory {
public:
    virtual ~IGuestVirtualMemory() = default;

    [[nodiscard]] virtual MemoryResult fetch(
        GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept = 0;
    [[nodiscard]] virtual MemoryResult read(
        GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept = 0;
    [[nodiscard]] virtual MemoryResult write(
        GuestVirtualAddress address, std::span<const std::uint8_t> source) noexcept = 0;
};

// Non-owning adapter over the existing software address space.
class GuestAddressSpaceView final : public IGuestVirtualMemory {
public:
    explicit GuestAddressSpaceView(GuestAddressSpace& address_space) noexcept
        : address_space_(address_space) {}

    [[nodiscard]] MemoryResult fetch(
        GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept override {
        return address_space_.fetch(address, destination);
    }

    [[nodiscard]] MemoryResult read(
        GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept override {
        return address_space_.read(address, destination);
    }

    [[nodiscard]] MemoryResult write(
        GuestVirtualAddress address, std::span<const std::uint8_t> source) noexcept override {
        return address_space_.write(address, source);
    }

private:
    GuestAddressSpace& address_space_;
};

// Non-owning adapter preserving the legacy GuestMemory 32-bit bounded contract.
class GuestMemoryView final : public IGuestVirtualMemory {
public:
    explicit GuestMemoryView(GuestMemory& memory) noexcept : memory_(memory) {}

    [[nodiscard]] MemoryResult fetch(
        GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept override {
        constexpr auto access = MemoryAccess::Execute;
        constexpr auto context = MemoryAccessContext::Fetch;
        const auto length = destination.size();

        if (address.value > kLegacyAddressLimit) {
            return make_failure(MemoryFault::AddressOverflow, access, context, address, length);
        }
        if (length == 0U || length > kMaxFetchLength || !checked_legacy_range(address.value, length)) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        }

        try {
            for (std::size_t index = 0U; index < length; ++index) {
                destination[index] = memory_.read8(static_cast<std::uint32_t>(address.value + index));
            }
        } catch (const std::out_of_range&) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        } catch (const std::length_error&) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        }

        return MemoryResult::success(
            access, context, address.value, static_cast<std::uint64_t>(length),
            static_cast<std::uint64_t>(length));
    }

    [[nodiscard]] MemoryResult read(
        GuestVirtualAddress address, std::span<std::uint8_t> destination) noexcept override {
        constexpr auto access = MemoryAccess::Read;
        constexpr auto context = MemoryAccessContext::Data;
        const auto length = destination.size();

        if (address.value > kLegacyAddressLimit) {
            return make_failure(MemoryFault::AddressOverflow, access, context, address, length);
        }
        if (!checked_legacy_range(address.value, length)) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        }

        try {
            for (std::size_t index = 0U; index < length; ++index) {
                destination[index] = memory_.read8(static_cast<std::uint32_t>(address.value + index));
            }
        } catch (const std::out_of_range&) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        } catch (const std::length_error&) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        }

        return MemoryResult::success(
            access, context, address.value, static_cast<std::uint64_t>(length),
            static_cast<std::uint64_t>(length));
    }

    [[nodiscard]] MemoryResult write(
        GuestVirtualAddress address, std::span<const std::uint8_t> source) noexcept override {
        constexpr auto access = MemoryAccess::Write;
        constexpr auto context = MemoryAccessContext::Data;
        const auto length = source.size();

        if (address.value > kLegacyAddressLimit) {
            return make_failure(MemoryFault::AddressOverflow, access, context, address, length);
        }
        if (!checked_legacy_range(address.value, length)) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        }

        try {
            for (std::size_t index = 0U; index < length; ++index) {
                memory_.write8(static_cast<std::uint32_t>(address.value + index), source[index]);
            }
        } catch (const std::out_of_range&) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        } catch (const std::length_error&) {
            return make_failure(MemoryFault::InvalidRange, access, context, address, length);
        }

        return MemoryResult::success(
            access, context, address.value, static_cast<std::uint64_t>(length),
            static_cast<std::uint64_t>(length));
    }

private:
    static constexpr std::uint64_t kLegacyAddressLimit =
        static_cast<std::uint64_t>((std::numeric_limits<std::uint32_t>::max)());
    static constexpr std::size_t kMaxFetchLength = 15U;

    [[nodiscard]] bool checked_legacy_range(std::uint64_t address, std::size_t length) const noexcept {
        if (address > kLegacyAddressLimit) {
            return false;
        }
        const auto offset = static_cast<std::size_t>(address);
        return offset <= memory_.size() && length <= memory_.size() - offset;
    }

    [[nodiscard]] static MemoryResult make_failure(
        MemoryFault fault,
        MemoryAccess access,
        MemoryAccessContext context,
        GuestVirtualAddress address,
        std::size_t requested_length) noexcept {
        return MemoryResult::failure(
            fault, access, context, address.value, static_cast<std::uint64_t>(requested_length));
    }

    GuestMemory& memory_;
};

} // namespace xbridge::memory
