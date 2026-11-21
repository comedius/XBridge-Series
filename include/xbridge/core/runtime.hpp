#pragma once
#include "xbridge/core/config.hpp"
#include <iosfwd>

namespace xbridge {

class Runtime {
public:
    explicit Runtime(ProfileKind profile);
    int run_demo(std::ostream& output) const;
    const HardwareProfile& profile() const noexcept;

private:
    ProfileKind profile_;
};

} // namespace xbridge
