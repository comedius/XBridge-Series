#pragma once

#include <cstdint>

// Host CPU feature observations are deliberately separate from guest CPU
// semantics. A reported host feature never identifies a guest, proves Xbox
// compatibility, or authorizes execution of arbitrary guest bytes.
namespace xbridge::execution {

enum class CpuFeature : unsigned {
    BaselineX64,
    Avx2,
    Bmi1,
    Bmi2,
    Fma,
    Osxsave,
    Xgetbv
};

struct CpuFeatureSet {
    bool baseline_x64{false};
    bool avx2{false};
    bool bmi1{false};
    bool bmi2{false};
    bool fma{false};
    bool osxsave{false};
    bool xgetbv{false};

    [[nodiscard]] bool has(CpuFeature feature) const noexcept {
        switch (feature) {
        case CpuFeature::BaselineX64: return baseline_x64;
        case CpuFeature::Avx2: return avx2 && osxsave && xgetbv;
        case CpuFeature::Bmi1: return bmi1;
        case CpuFeature::Bmi2: return bmi2;
        case CpuFeature::Fma: return fma;
        case CpuFeature::Osxsave: return osxsave;
        case CpuFeature::Xgetbv: return xgetbv;
        }
        return false;
    }
};

enum class CpuFeatureDetectionError { None, UnsupportedPlatform };

class ICpuFeatureDetector {
public:
    virtual ~ICpuFeatureDetector() = default;
    [[nodiscard]] virtual CpuFeatureSet detect() const noexcept = 0;
    [[nodiscard]] virtual CpuFeatureDetectionError last_error() const noexcept = 0;
};

// Deterministic detector for tests and for a caller-supplied, explicit feature
// snapshot. It calls no host instruction and performs no probing.
class FixedCpuFeatureDetector final : public ICpuFeatureDetector {
public:
    explicit FixedCpuFeatureDetector(CpuFeatureSet features) noexcept : features_(features) {
        // Injected snapshots obey the same AVX2 OS-state dependency as the
        // host detector. The stored snapshot is otherwise unchanged.
        if (!features_.osxsave || !features_.xgetbv) {
            features_.avx2 = false;
        }
    }

    [[nodiscard]] CpuFeatureSet detect() const noexcept override { return features_; }
    [[nodiscard]] CpuFeatureDetectionError last_error() const noexcept override {
        return CpuFeatureDetectionError::None;
    }

private:
    CpuFeatureSet features_{};
};

// Windows x64/MSVC: CPUID leaf 1 and leaf 7 subleaf 0 plus XGETBV.
// Other toolchains: baseline only, with UnsupportedPlatform; no intrinsic is
// called there. Detection itself is observation, never a guest-identity claim.
class HostCpuFeatureDetector final : public ICpuFeatureDetector {
public:
    [[nodiscard]] CpuFeatureSet detect() const noexcept override;
    [[nodiscard]] CpuFeatureDetectionError last_error() const noexcept override;
};

[[nodiscard]] const char* to_string(CpuFeature feature) noexcept;
[[nodiscard]] const char* to_string(CpuFeatureDetectionError error) noexcept;

} // namespace xbridge::execution
