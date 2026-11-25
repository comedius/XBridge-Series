#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

// Interface-only seams for thirteen future runtime service families. No Xbox
// function is named or simulated, and no host API is called. These markers are
// deliberately host-independent.
namespace xbridge::runtime {

enum class ServiceId : std::uint8_t {
    System = 0U,
    RuntimeInitialization,
    GuestProcess,
    Thread,
    Synchronization,
    VirtualMemory,
    File,
    Storage,
    Input,
    Audio,
    Graphics,
    Timing,
    ErrorLogging
};

inline constexpr std::size_t kServiceCount = 13U;

[[nodiscard]] bool is_valid_service(ServiceId id) noexcept;
[[nodiscard]] std::string_view service_id_name(ServiceId id) noexcept;

enum class ServiceStatus { NotRegistered, Unsupported, Available, Unavailable = Unsupported };

struct ServiceDescriptor {
    ServiceId id{ServiceId::System};
    std::string_view name{"Unknown"};
};

struct ServiceResult {
    ServiceStatus status{ServiceStatus::Unsupported};
    std::uint64_t value{0U};
};

class IService {
public:
    virtual ~IService() = 0;
};

class ISystemService : public IService {
public:
    virtual ~ISystemService() = 0;
};

class IRuntimeInitializationService : public IService {
public:
    virtual ~IRuntimeInitializationService() = 0;
};

class IGuestProcessService : public IService {
public:
    virtual ~IGuestProcessService() = 0;
};

class IThreadService : public IService {
public:
    virtual ~IThreadService() = 0;
};

class ISynchronizationService : public IService {
public:
    virtual ~ISynchronizationService() = 0;
};

class IVirtualMemoryService : public IService {
public:
    virtual ~IVirtualMemoryService() = 0;
};

class IFileService : public IService {
public:
    virtual ~IFileService() = 0;
};

class IStorageService : public IService {
public:
    virtual ~IStorageService() = 0;
};

class IInputService : public IService {
public:
    virtual ~IInputService() = 0;
};

class IAudioService : public IService {
public:
    virtual ~IAudioService() = 0;
};

class IGraphicsService : public IService {
public:
    virtual ~IGraphicsService() = 0;
};

class ITimingService : public IService {
public:
    virtual ~ITimingService() = 0;
};

class IErrorLoggingService : public IService {
public:
    virtual ~IErrorLoggingService() = 0;
};

// Common spelling aliases. They do not create additional service families.
using ISystemFamilyService = ISystemService;
using IRuntimeService = IRuntimeInitializationService;
using IProcessService = IGuestProcessService;
using IMemoryService = IVirtualMemoryService;
using ITimingPerformanceService = ITimingService;
using ILoggingService = IErrorLoggingService;

// Neutral guest/host transition descriptor. It intentionally has no dependency
// on xbridge::execution::GuestHostTransition.
enum class TransitionKind { ServiceRequest, NormalExit, FaultExit };

struct GuestHostTransitionDescriptor {
    TransitionKind kind{TransitionKind::NormalExit};
    ServiceId service{ServiceId::System};
    std::uint64_t service_id{0U};
    std::uint64_t payload{0U};
};

using TransitionDescriptor = GuestHostTransitionDescriptor;

[[nodiscard]] ServiceDescriptor describe_service(ServiceId id) noexcept;
[[nodiscard]] ServiceResult unsupported_result() noexcept;
[[nodiscard]] ServiceResult not_registered_result() noexcept;
[[nodiscard]] GuestHostTransitionDescriptor make_service_transition(
    ServiceId id, std::uint64_t payload) noexcept;
[[nodiscard]] GuestHostTransitionDescriptor make_exit_transition(
    TransitionKind kind, std::uint64_t payload) noexcept;
[[nodiscard]] const char* to_string(ServiceId id) noexcept;
[[nodiscard]] const char* to_string(ServiceStatus status) noexcept;
[[nodiscard]] const char* to_string(TransitionKind kind) noexcept;

// Non-owning registry: pointers remain the caller's property and are never
// deleted, copied into owned storage or used to call any service method.
class SystemServiceRegistry {
public:
    [[nodiscard]] bool register_service(ServiceId id, IService* service) noexcept;
    [[nodiscard]] bool unregister_service(ServiceId id) noexcept;
    [[nodiscard]] IService* find(ServiceId id) noexcept;
    [[nodiscard]] const IService* find(ServiceId id) const noexcept;

    template <typename T>
    [[nodiscard]] T* find_as(ServiceId id) noexcept {
        return dynamic_cast<T*>(find(id));
    }

    template <typename T>
    [[nodiscard]] const T* find_as(ServiceId id) const noexcept {
        return dynamic_cast<const T*>(find(id));
    }

    [[nodiscard]] bool is_available(ServiceId id) const noexcept;
    [[nodiscard]] ServiceStatus status(ServiceId id) const noexcept;
    [[nodiscard]] ServiceResult query(ServiceId id) const noexcept;
    [[nodiscard]] std::size_t count() const noexcept { return count_; }
    void clear() noexcept;

private:
    IService* services_[kServiceCount]{};
    std::size_t count_{0U};
};

} // namespace xbridge::runtime
