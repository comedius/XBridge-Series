#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace xbridge::memory {

class GuestMemory {
public:
    explicit GuestMemory(std::size_t size);
    std::size_t size() const noexcept;
    void write8(std::uint32_t address, std::uint8_t value);
    std::uint8_t read8(std::uint32_t address) const;
    void write32(std::uint32_t address, std::uint32_t value);
    std::uint32_t read32(std::uint32_t address) const;
    void load(std::uint32_t address, const std::vector<std::uint8_t>& bytes);

private:
    void check_range(std::uint32_t address, std::size_t length) const;
    std::vector<std::uint8_t> bytes_;
};

} // namespace xbridge::memory
