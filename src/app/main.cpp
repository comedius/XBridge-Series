#include "xbridge/core/config.hpp"
#include "xbridge/core/runtime.hpp"
#include <iostream>
#include <string_view>

int main(int argc, char** argv) {
    xbridge::ProfileKind profile = xbridge::ProfileKind::SeriesX;

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg(argv[i]);
        if (arg == "--profile" && i + 1 < argc) {
            if (!xbridge::parse_profile(argv[++i], profile)) {
                std::cerr << "Unknown profile. The only target profile is series_x.\n";
                return 2;
            }
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: xbridge [--profile series_x]\n";
            return 0;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 2;
        }
    }

    xbridge::Runtime runtime(profile);
    return runtime.run_demo(std::cout);
}
