#include "xbridge/core/runtime.hpp"
#include "xbridge/cpu/test_cpu.hpp"
#include "xbridge/memory/memory.hpp"
#include <ostream>
#include <vector>

namespace xbridge {

Runtime::Runtime(ProfileKind profile) : profile_(profile) {}

const HardwareProfile& Runtime::profile() const noexcept {
    return profile_for(profile_);
}

int Runtime::run_demo(std::ostream& output) const {
    memory::GuestMemory memory(4096);

    // Synthetic program:
    // r0 = 40; r0 += 2; memory[128] = r0; halt
    const std::vector<std::uint8_t> program{
        0x01, 0x00, 0x28, 0x00, 0x00, 0x00,
        0x02, 0x00, 0x02, 0x00, 0x00, 0x00,
        0x03, 0x00, 0x80, 0x00, 0x00, 0x00,
        0xFF
    };

    cpu::TestCpu cpu(memory);
    cpu.load_program(program);
    cpu.run();

    output << "XBridge Series " << profile().id << "\n";
    output << "Target profile: " << profile().display_name << "\n";
    output << "Profile metadata: " << profile().cpu_cores << " cores, "
           << profile().logical_threads << " threads, "
           << profile().memory_gib << " GiB target memory metadata\n";
    output << "Synthetic test CPU result: " << memory.read32(128) << "\n";
    output << "Status: scaffold demo completed (not Xbox emulation)\n";
    return memory.read32(128) == 42 ? 0 : 1;
}

} // namespace xbridge
