#include "xbridge/execution/cpu_feature_detector.hpp"

#if defined(_MSC_VER) && defined(_M_X64)
#include <intrin.h>
#endif

namespace xbridge::execution {
namespace {

[[nodiscard]] CpuFeatureSet baseline_only() noexcept {
    CpuFeatureSet features{};
    features.baseline_x64 = true;
    return features;
}

} // namespace

CpuFeatureSet HostCpuFeatureDetector::detect() const noexcept {
#if defined(_MSC_VER) && defined(_M_X64)
    CpuFeatureSet features = baseline_only();

    int registers[4] = {0, 0, 0, 0};
    __cpuid(registers, 0);
    const int max_leaf = registers[0];
    if (max_leaf < 1) {
        return features;
    }

    // Leaf 1: FMA is ECX bit 12, OSXSAVE is ECX bit 27.
    __cpuid(registers, 1);
    const int leaf1_ecx = registers[2];
    if ((leaf1_ecx & (1 << 12)) != 0) {
        features.fma = true;
    }
    if ((leaf1_ecx & (1 << 27)) != 0) {
        features.osxsave = true;
    }

    // XCR0 bit 1 (XMM) and bit 2 (YMM) are required for AVX-state use.
    if (features.osxsave) {
        const unsigned long long xcr0 = _xgetbv(0);
        if ((xcr0 & 0x6ULL) == 0x6ULL) {
            features.xgetbv = true;
        }
    }

    // Leaf 7 subleaf 0: BMI1 is EBX bit 3, AVX2 is bit 5, BMI2 is bit 8.
    // AVX2 is reported only when the OS has enabled XMM/YMM state.
    if (max_leaf >= 7) {
        __cpuidex(registers, 7, 0);
        const int leaf7_ebx = registers[1];
        if ((leaf7_ebx & (1 << 3)) != 0) {
            features.bmi1 = true;
        }
        if ((leaf7_ebx & (1 << 8)) != 0) {
            features.bmi2 = true;
        }
        if ((leaf7_ebx & (1 << 5)) != 0 && features.xgetbv) {
            features.avx2 = true;
        }
    }
    return features;
#else
    // No host intrinsic is available here. Baseline x64 is assumed as the
    // minimal synthetic proof contract; optional features stay false/unknown.
    return baseline_only();
#endif
}

CpuFeatureDetectionError HostCpuFeatureDetector::last_error() const noexcept {
#if defined(_MSC_VER) && defined(_M_X64)
    return CpuFeatureDetectionError::None;
#else
    return CpuFeatureDetectionError::UnsupportedPlatform;
#endif
}

const char* to_string(CpuFeature feature) noexcept {
    switch (feature) {
    case CpuFeature::BaselineX64: return "BaselineX64";
    case CpuFeature::Avx2: return "Avx2";
    case CpuFeature::Bmi1: return "Bmi1";
    case CpuFeature::Bmi2: return "Bmi2";
    case CpuFeature::Fma: return "Fma";
    case CpuFeature::Osxsave: return "Osxsave";
    case CpuFeature::Xgetbv: return "Xgetbv";
    }
    return "Unknown";
}

const char* to_string(CpuFeatureDetectionError error) noexcept {
    switch (error) {
    case CpuFeatureDetectionError::None: return "None";
    case CpuFeatureDetectionError::UnsupportedPlatform: return "UnsupportedPlatform";
    }
    return "Unknown";
}

} // namespace xbridge::execution
