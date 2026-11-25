#include "xbridge/runtime/system_services.hpp"

namespace xbridge::runtime {

IService::~IService() = default;
ISystemService::~ISystemService() = default;
IRuntimeInitializationService::~IRuntimeInitializationService() = default;
IGuestProcessService::~IGuestProcessService() = default;
IThreadService::~IThreadService() = default;
ISynchronizationService::~ISynchronizationService() = default;
IVirtualMemoryService::~IVirtualMemoryService() = default;
IFileService::~IFileService() = default;
IStorageService::~IStorageService() = default;
IInputService::~IInputService() = default;
IAudioService::~IAudioService() = default;
IGraphicsService::~IGraphicsService() = default;
ITimingService::~ITimingService() = default;
IErrorLoggingService::~IErrorLoggingService() = default;

bool is_valid_service(ServiceId id) noexcept {
    return static_cast<std::uint8_t>(id) < kServiceCount;
}

std::string_view service_id_name(ServiceId id) noexcept {
    switch (id) {
    case ServiceId::System: return "System";
    case ServiceId::RuntimeInitialization: return "RuntimeInitialization";
    case ServiceId::GuestProcess: return "GuestProcess";
    case ServiceId::Thread: return "Thread";
    case ServiceId::Synchronization: return "Synchronization";
    case ServiceId::VirtualMemory: return "VirtualMemory";
    case ServiceId::File: return "File";
    case ServiceId::Storage: return "Storage";
    case ServiceId::Input: return "Input";
    case ServiceId::Audio: return "Audio";
    case ServiceId::Graphics: return "Graphics";
    case ServiceId::Timing: return "Timing";
    case ServiceId::ErrorLogging: return "ErrorLogging";
    }
    return "Unknown";
}

ServiceDescriptor describe_service(ServiceId id) noexcept {
    return ServiceDescriptor{id, service_id_name(id)};
}

ServiceResult unsupported_result() noexcept {
    return ServiceResult{ServiceStatus::Unsupported, 0U};
}

ServiceResult not_registered_result() noexcept {
    return ServiceResult{ServiceStatus::NotRegistered, 0U};
}

GuestHostTransitionDescriptor make_service_transition(ServiceId id,
                                                       std::uint64_t payload) noexcept {
    return GuestHostTransitionDescriptor{
        TransitionKind::ServiceRequest, id,
        is_valid_service(id) ? static_cast<std::uint64_t>(id) : 0U, payload};
}

GuestHostTransitionDescriptor make_exit_transition(TransitionKind kind,
                                                    std::uint64_t payload) noexcept {
    return GuestHostTransitionDescriptor{kind, ServiceId::System, 0U, payload};
}

const char* to_string(ServiceId id) noexcept {
    switch (id) {
    case ServiceId::System: return "System";
    case ServiceId::RuntimeInitialization: return "RuntimeInitialization";
    case ServiceId::GuestProcess: return "GuestProcess";
    case ServiceId::Thread: return "Thread";
    case ServiceId::Synchronization: return "Synchronization";
    case ServiceId::VirtualMemory: return "VirtualMemory";
    case ServiceId::File: return "File";
    case ServiceId::Storage: return "Storage";
    case ServiceId::Input: return "Input";
    case ServiceId::Audio: return "Audio";
    case ServiceId::Graphics: return "Graphics";
    case ServiceId::Timing: return "Timing";
    case ServiceId::ErrorLogging: return "ErrorLogging";
    }
    return "Unknown";
}

const char* to_string(ServiceStatus status) noexcept {
    switch (status) {
    case ServiceStatus::NotRegistered: return "NotRegistered";
    case ServiceStatus::Unsupported: return "Unsupported";
    case ServiceStatus::Available: return "Available";
    }
    return "Unknown";
}

const char* to_string(TransitionKind kind) noexcept {
    switch (kind) {
    case TransitionKind::ServiceRequest: return "ServiceRequest";
    case TransitionKind::NormalExit: return "NormalExit";
    case TransitionKind::FaultExit: return "FaultExit";
    }
    return "Unknown";
}

bool SystemServiceRegistry::register_service(ServiceId id, IService* service) noexcept {
    if (!is_valid_service(id) || service == nullptr) {
        return false;
    }
    const std::size_t index = static_cast<std::size_t>(id);
    if (services_[index] != nullptr) {
        return false;
    }
    services_[index] = service;
    ++count_;
    return true;
}

bool SystemServiceRegistry::unregister_service(ServiceId id) noexcept {
    if (!is_valid_service(id)) {
        return false;
    }
    const std::size_t index = static_cast<std::size_t>(id);
    if (services_[index] == nullptr) {
        return false;
    }
    services_[index] = nullptr;
    --count_;
    return true;
}

IService* SystemServiceRegistry::find(ServiceId id) noexcept {
    if (!is_valid_service(id)) {
        return nullptr;
    }
    return services_[static_cast<std::size_t>(id)];
}

const IService* SystemServiceRegistry::find(ServiceId id) const noexcept {
    if (!is_valid_service(id)) {
        return nullptr;
    }
    return services_[static_cast<std::size_t>(id)];
}

bool SystemServiceRegistry::is_available(ServiceId id) const noexcept {
    return status(id) == ServiceStatus::Available;
}

ServiceStatus SystemServiceRegistry::status(ServiceId id) const noexcept {
    if (!is_valid_service(id)) {
        return ServiceStatus::NotRegistered;
    }
    return services_[static_cast<std::size_t>(id)] != nullptr ? ServiceStatus::Available
                                                                : ServiceStatus::Unsupported;
}

ServiceResult SystemServiceRegistry::query(ServiceId id) const noexcept {
    if (!is_valid_service(id)) {
        return not_registered_result();
    }
    if (services_[static_cast<std::size_t>(id)] == nullptr) {
        return unsupported_result();
    }
    return ServiceResult{ServiceStatus::Available, 0U};
}

void SystemServiceRegistry::clear() noexcept {
    for (IService*& service : services_) {
        service = nullptr;
    }
    count_ = 0U;
}

} // namespace xbridge::runtime
