#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/logger.h"
#include "vulkanGraphicsSystem.hpp"
#include "vulkanutils.hpp"
#include "../../utils.hpp"

// ---- Special Functions ----

AxrVulkanGraphicsSystem::AxrVulkanGraphicsSystem(const Config& config):
    m_ApplicationName(config.ApplicationName),
    m_ApplicationVersion(config.ApplicationVersion),
    m_WindowPlatform(config.WindowPlatform),
    m_Instance(VK_NULL_HANDLE),
    m_DebugUtilsMessenger(VK_NULL_HANDLE) {
    if (config.VulkanConfig == nullptr) {
        axrLogErrorLocation("Vulkan Config is null.");
        return;
    }

    m_DynamicDispatchLoader.init();

    m_SupportedInstanceApiLayers = getSupportedInstanceApiLayers();
    m_SupportedInstanceExtensions = getSupportedInstanceExtensions();

    m_ApiLayers = axrCloneSupportedVulkanInstanceApiLayers(
        config.VulkanConfig->ApiLayersCount,
        config.VulkanConfig->ApiLayers,
        m_SupportedInstanceApiLayers
    );
    m_Extensions = axrCloneSupportedVulkanInstanceExtensions(
        config.VulkanConfig->ExtensionsCount,
        config.VulkanConfig->Extensions,
        m_SupportedInstanceExtensions
    );

    addRequiredInstanceExtensions();
}

AxrVulkanGraphicsSystem::~AxrVulkanGraphicsSystem() {
    destroyDebugUtils();
    destroyInstance();
    destroyExtensions();
    destroyApiLayers();
}

// ---- Public Functions ----

AxrResult AxrVulkanGraphicsSystem::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createInstance();
    if (AXR_FAILED(axrResult)) return axrResult;

    axrResult = createDebugUtils();
    if (AXR_FAILED(axrResult)) return axrResult;

    return axrResult;
}

// ---- Private Functions ----

AxrResult AxrVulkanGraphicsSystem::createInstance() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != nullptr) {
        axrLogErrorLocation("Instance already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::ApplicationInfo appInfo(
        m_ApplicationName,
        m_ApplicationVersion,
        AxrEngineName,
        AXR_ENGINE_VERSION,
        // TODO: OpenXR decides on the api version when that's in use
        vk::ApiVersion13
    );

    const std::vector<const char*> instanceLayers = getAllInstanceApiLayerNames();
    const std::vector<const char*> instanceExtensions = getAllInstanceExtensionNames();

    const vk::InstanceCreateInfo instanceCreateInfo(
        {},
        &appInfo,
        static_cast<uint32_t>(instanceLayers.size()),
        instanceLayers.data(),
        static_cast<uint32_t>(instanceExtensions.size()),
        instanceExtensions.data()
    );

    const vk::Result vkResult = vk::createInstance(
        &createInstanceChain(instanceCreateInfo).get<vk::InstanceCreateInfo>(),
        nullptr,
        &m_Instance,
        m_DynamicDispatchLoader
    );
    axrLogVkResult(vkResult, "vk::createInstance");

    if (axrVkFailed(vkResult)) return AXR_ERROR;

    m_DynamicDispatchLoader.init(m_Instance);

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyInstance() {
    if (m_Instance != VK_NULL_HANDLE) {
        m_Instance.destroy(nullptr, m_DynamicDispatchLoader);
        m_Instance = VK_NULL_HANDLE;
    }
}

AxrVulkanGraphicsSystem::InstanceChain_T AxrVulkanGraphicsSystem::createInstanceChain(
    const vk::InstanceCreateInfo& instanceCreateInfo
) const {
    InstanceChain_T chain{
        instanceCreateInfo,
        createDebugUtilsCreateInto()
    };

    if (!extensionExists(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)) {
        chain.unlink<vk::DebugUtilsMessengerCreateInfoEXT>();
    }

    return chain;
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedInstanceApiLayers() const {
    const auto instanceLayerProperties = vk::enumerateInstanceLayerProperties(
        m_DynamicDispatchLoader
    );
    axrLogVkResult(instanceLayerProperties.result, "vk::enumerateInstanceLayerProperties");

    if (axrVkFailed(instanceLayerProperties.result)) return {};

    std::vector<std::string> supportedApiLayers(instanceLayerProperties.value.size());

    for (size_t i = 0; i < instanceLayerProperties.value.size(); ++i) {
        supportedApiLayers[i] = instanceLayerProperties.value[i].layerName.data();
    }

    return supportedApiLayers;
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedInstanceExtensions() const {
    const auto instanceExtensionProperties = vk::enumerateInstanceExtensionProperties(
        nullptr,
        m_DynamicDispatchLoader
    );
    axrLogVkResult(instanceExtensionProperties.result, "vk::enumerateInstanceExtensionProperties");

    if (axrVkFailed(instanceExtensionProperties.result)) return {};

    std::vector<std::string> supportedExtensions(instanceExtensionProperties.value.size());

    for (size_t i = 0; i < instanceExtensionProperties.value.size(); ++i) {
        supportedExtensions[i] = instanceExtensionProperties.value[i].extensionName.data();
    }

    return supportedExtensions;
}

void AxrVulkanGraphicsSystem::destroyApiLayers() {
    for (AxrVulkanApiLayer_T& apiLayer : m_ApiLayers) {
        delete apiLayer;
        apiLayer = nullptr;
    }

    m_ApiLayers.clear();
}

void AxrVulkanGraphicsSystem::destroyExtensions() {
    for (AxrVulkanExtension_T& extension : m_Extensions) {
        delete extension;
        extension = nullptr;
    }

    m_Extensions.clear();
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllInstanceApiLayerNames() {
    std::vector<const char*> apiLayerNames;

    for (AxrVulkanApiLayer_T apiLayer : m_ApiLayers) {
        if (apiLayer == nullptr || apiLayer->Level != AXR_VULKAN_API_LAYER_LEVEL_INSTANCE) continue;

        apiLayerNames.push_back(axrGetVulkanApiLayerName(apiLayer->Type));
    }

    return apiLayerNames;
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllInstanceExtensionNames() {
    std::vector<const char*> extensionNames;

    for (AxrVulkanExtension_T extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) continue;

        extensionNames.push_back(axrGetVulkanExtensionName(extension->Type));
    }

    return extensionNames;
}

bool AxrVulkanGraphicsSystem::extensionExists(const AxrVulkanExtensionTypeEnum extensionType) const {
    for (const AxrVulkanExtension_T extension : m_Extensions) {
        if (extension != nullptr && extension->Type == extensionType) return true;
    }

    return false;
}

void AxrVulkanGraphicsSystem::addRequiredInstanceExtensions() {
    auto surfaceExtension = AxrVulkanExtensionSurface{};
    addExtension(reinterpret_cast<AxrVulkanExtension_T>(&surfaceExtension));

    switch (m_WindowPlatform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
            auto win32SurfaceExtension = AxrVulkanExtensionWin32Surface{};
            addExtension(reinterpret_cast<AxrVulkanExtension_T>(&win32SurfaceExtension));
            break;
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED: {
            axrLogErrorLocation("Unknown window platform.");
            break;
        }
    }
}

void AxrVulkanGraphicsSystem::addExtension(const AxrVulkanExtension_T extension) {
    if (extension == nullptr) return;

    // Check if instance has been created. If it has, it's too late to add an instance extension
    if (extension->Level == AXR_VULKAN_EXTENSION_LEVEL_INSTANCE && m_Instance != VK_NULL_HANDLE) {
        axrLogErrorLocation("VkInstance has already been created.");
        return;
    }

    // TODO: Check if device has been created. If it has, it's too late to add a device extension

    if (extensionExists(extension->Type)) {
        return;
    }

    const char* extensionName = axrGetVulkanExtensionName(extension->Type);

    switch (extension->Level) {
        case AXR_VULKAN_EXTENSION_LEVEL_INSTANCE: {
            if (!axrContainsString(extensionName, m_SupportedInstanceExtensions)) {
                axrLogErrorLocation("{0} Extension is not supported.", extensionName);
                return;
            }
            break;
        }
        case AXR_VULKAN_EXTENSION_LEVEL_DEVICE: {
            //TODO...
            // if (!axrContainsString(extensionName, m_SupportedDeviceExtensions)) {
            //     axrLogErrorLocation("{0} Extension is not supported.", extensionName);
            //     return;
            // }
            break;
        }
        case AXR_VULKAN_EXTENSION_LEVEL_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension level.");
            break;
        }
    }

    m_Extensions.push_back(axrCloneVulkanExtension(extension));
}

AxrVulkanExtension_T AxrVulkanGraphicsSystem::getExtension(const AxrVulkanExtensionTypeEnum type) const {
    for (const AxrVulkanExtension_T extension : m_Extensions) {
        if (extension != nullptr && extension->Type == type) {
            return extension;
        }
    }

    return nullptr;
}

vk::DebugUtilsMessengerCreateInfoEXT AxrVulkanGraphicsSystem::createDebugUtilsCreateInto() const {
    auto debugUtilsExtension = reinterpret_cast<AxrVulkanExtensionDebugUtils*>(
        getExtension(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)
    );

    if (debugUtilsExtension == nullptr) {
        axrLogErrorLocation("No debug utils extension found.");
        return {};
    }

    return {
        {},
        static_cast<vk::DebugUtilsMessageSeverityFlagsEXT>(debugUtilsExtension->SeverityFlags),
        static_cast<vk::DebugUtilsMessageTypeFlagBitsEXT>(debugUtilsExtension->TypeFlags),
        debugUtilsCallback
    };
}

AxrResult AxrVulkanGraphicsSystem::createDebugUtils() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!extensionExists(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)) {
        // Debug utils aren't needed to be created
        return AXR_SUCCESS;
    }

    if (m_DebugUtilsMessenger != VK_NULL_HANDLE) {
        axrLogWarningLocation("Debug utils have already been created.");
        return AXR_SUCCESS;
    }

    if (m_Instance == VK_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::DebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo = createDebugUtilsCreateInto();
    const vk::Result vkResult = m_Instance.createDebugUtilsMessengerEXT(
        &debugUtilsCreateInfo,
        nullptr,
        &m_DebugUtilsMessenger,
        m_DynamicDispatchLoader
    );
    axrLogVkResult(vkResult, "m_Instance.createDebugUtilsMessengerEXT");
    if (axrVkFailed(vkResult)) return AXR_ERROR;

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyDebugUtils() {
    if (m_DebugUtilsMessenger == VK_NULL_HANDLE) return;

    m_Instance.destroyDebugUtilsMessengerEXT(m_DebugUtilsMessenger, nullptr, m_DynamicDispatchLoader);
    m_DebugUtilsMessenger = VK_NULL_HANDLE;
}

VkBool32 AxrVulkanGraphicsSystem::debugUtilsCallback(
    const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    const VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    AxrLogLevelEnum logLevel = AXR_LOG_LEVEL_ERROR;
    const char* messageSeverityString;
    const char* messageTypeString;

    switch (messageType) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: {
            messageTypeString = "General";
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: {
            messageTypeString = "Validation";
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: {
            messageTypeString = "Performance";
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: {
            messageTypeString = "Device Address Binding";
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT:
        default: {
            messageTypeString = "Unknown Type";
            break;
        }
    }

    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            messageSeverityString = "Verbose";
            logLevel = AXR_LOG_LEVEL_INFO;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            messageSeverityString = "Info";
            logLevel = AXR_LOG_LEVEL_INFO;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            messageSeverityString = "Warning";
            logLevel = AXR_LOG_LEVEL_WARNING;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            messageSeverityString = "Error";
            logLevel = AXR_LOG_LEVEL_ERROR;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            messageSeverityString = "Unknown Severity";
            logLevel = AXR_LOG_LEVEL_ERROR;
            break;
        }
    }

    axrLog(
        logLevel,
        "[Vulkan | {0} | {1}] : {2}",
        messageTypeString,
        messageSeverityString,
        pCallbackData->pMessage
    );

    return VK_FALSE;
}

#endif
