#include "xbridge/analysis/game_intelligence.hpp"
#include "xbridge/core/config.hpp"
#include "xbridge/core/runtime.hpp"

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

int main(int argc, char** argv) {
    xbridge::ProfileKind profile = xbridge::ProfileKind::SeriesX;
    bool analyze_requested = false;
    std::filesystem::path analyze_directory;

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg(argv[i]);
        if (arg == "--analyze") {
            if (i + 1 >= argc) {
                std::cerr << "--analyze requires a directory path.\n";
                return 2;
            }
            try {
                analyze_directory = std::filesystem::path(argv[++i]);
            } catch (const std::exception& error) {
                std::cerr << "Invalid --analyze directory path: " << error.what() << "\n";
                return 2;
            }
            analyze_requested = true;
        } else if (arg == "--profile" && i + 1 < argc) {
            if (!xbridge::parse_profile(argv[++i], profile)) {
                std::cerr << "Unknown profile. The only target profile is series_x.\n";
                return 2;
            }
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: xbridge [--profile series_x] [--analyze <directory>]\n"
                      << "  --profile series_x  Select the only target profile.\n"
                      << "  --analyze <directory>  Read-only local inventory of an explicitly\n"
                      << "                         supplied directory; prints one JSON report.\n"
                      << "  --help, -h          Show this help.\n"
                      << "Without --analyze, the profile demo runs. No analysis path is built in.\n";
            return 0;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 2;
        }
    }

    if (analyze_requested) {
        try {
            std::error_code error;
            const xbridge::analysis::AnalysisReport report =
                xbridge::analysis::analyze_directory(analyze_directory, error);
            if (error) {
                const std::u8string path_text = analyze_directory.u8string();
                const std::string display(reinterpret_cast<const char*>(path_text.data()),
                                          path_text.size());
                std::cerr << "Analysis failed for '" << display
                          << "': " << error.message() << "\n";
                return 2;
            }
            std::cout << report.to_json();
            return 0;
        } catch (const std::exception& failure) {
            std::cerr << "Analysis failed: " << failure.what() << "\n";
            return 2;
        } catch (...) {
            std::cerr << "Analysis failed: unknown error.\n";
            return 2;
        }
    }

    xbridge::Runtime runtime(profile);
    return runtime.run_demo(std::cout);
}
