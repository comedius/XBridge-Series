#include "xbridge/memory/memory.hpp"
#include <limits>
#include <stdexcept>

namespace xbridge::memory {

GuestMemory::GuestMemory(std::size_t size) : bytes_(size, 0) {}
std::size_t GuestMemory::size() const noexcept { return bytes_.size(); }

void GuestMemory::check_range(std::uint32_t address, std::size_t length) const {
    const auto start = static_cast<std::size_t>(address);
    if (start > bytes_.size() || length > bytes_.size() - start)
        throw std::out_of_range("Guest memory access out of range");
}

void GuestMemory::write8(std::uint32_t address, std::uint8_t value) {
    check_range(address, 1);
    bytes_[address] = value;
}

std::uint8_t GuestMemory::read8(std::uint32_t address) const {
    check_range(address, 1);
    return bytes_[address];
}

void GuestMemory::write32(std::uint32_t address, std::uint32_t value) {
    check_range(address, 4);
    for (unsigned i = 0; i < 4; ++i)
        bytes_[address + i] = static_cast<std::uint8_t>((value >> (i * 8)) & 0xFF);
}

std::uint32_t GuestMemory::read32(std::uint32_t address) const {
    check_range(address, 4);
    std::uint32_t value = 0;
    for (unsigned i = 0; i < 4; ++i)
        value |= static_cast<std::uint32_t>(bytes_[address + i]) << (i * 8);
    return value;
}

void GuestMemory::load(std::uint32_t address, const std::vector<std::uint8_t>& bytes) {
    check_range(address, bytes.size());
    for (std::size_t i = 0; i < bytes.size(); ++i)
        bytes_[static_cast<std::size_t>(address) + i] = bytes[i];
}

} // namespace xbridge::memory
