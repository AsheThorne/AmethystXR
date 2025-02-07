#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_set>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/logger.h"
#include "vulkanGraphicsSystem.hpp"
#include "vulkanutils.hpp"
#include "../../utils.hpp"
#include "../../windowSystem/windowSystem.hpp"

// ---- Special Functions ----

AxrVulkanGraphicsSystem::AxrVulkanGraphicsSystem(const Config& config):
    m_ApplicationName(config.ApplicationName),
    m_ApplicationVersion(config.ApplicationVersion),
    m_WindowPlatform(config.WindowPlatform),
    m_WindowSystem(config.WindowSystem),
    m_Instance(VK_NULL_HANDLE),
    m_DebugUtilsMessenger(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE) {
    if (config.VulkanConfig == nullptr) {
        axrLogErrorLocation("Vulkan Config is null.");
        return;
    }

    m_DynamicDispatchLoader.init();

    m_ApiLayers = axrCloneApiLayers(config.VulkanConfig->ApiLayersCount, config.VulkanConfig->ApiLayers);
    m_Extensions = axrCloneExtensions(config.VulkanConfig->ExtensionsCount, config.VulkanConfig->Extensions);
}

AxrVulkanGraphicsSystem::~AxrVulkanGraphicsSystem() {
    if (m_WindowSystem != nullptr) {
        m_WindowSystem->removeOnWindowOpenedCallback(onWindowOpenedCallback);
    }
    
    destroyLogicalDevice();
    destroyDebugUtils();
    destroyInstance();
    destroyExtensions();
    destroyApiLayers();
}

// ---- Public Functions ----

AxrResult AxrVulkanGraphicsSystem::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    // TODO: When something fails, we should cleanup all the objects that were already created.

    axrResult = createInstance();
    if (AXR_FAILED(axrResult)) return axrResult;

    axrResult = createDebugUtils();
    if (AXR_FAILED(axrResult)) return axrResult;

    axrResult = setupPhysicalDevice();
    if (AXR_FAILED(axrResult)) return axrResult;

    axrResult = createLogicalDevice();
    if (AXR_FAILED(axrResult)) return axrResult;

    if (m_WindowSystem != nullptr) {
        m_WindowSystem->addOnWindowOpenedCallback(this, onWindowOpenedCallback);

        // If the window is already open, then invoke the callback function.
        // Otherwise, it won't get called
        if (m_WindowSystem->isWindowOpen()) {
            onWindowOpenedCallback(this, m_WindowSystem);
        }
    }

    return axrResult;
}

// ---- Private Functions ----

AxrResult AxrVulkanGraphicsSystem::createInstance() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != VK_NULL_HANDLE) {
        axrLogWarningLocation("Instance already exists.");
        return AXR_SUCCESS;
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

    addRequiredInstanceExtensions();
    removeUnsupportedApiLayers();
    removeUnsupportedInstanceExtensions();

    const std::vector<const char*> instanceLayers = getAllApiLayerNames();
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

void AxrVulkanGraphicsSystem::removeUnsupportedApiLayers() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != VK_NULL_HANDLE) {
        axrLogWarningLocation("Instance already exists. It's too late to remove api layers.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const std::vector<std::string> supportedApiLayers = axrGetSupportedInstanceApiLayers(m_DynamicDispatchLoader);

    for (AxrVulkanApiLayer_T& apiLayer : m_ApiLayers) {
        if (apiLayer == nullptr) continue;

        if (!axrContainsString(axrGetApiLayerName(apiLayer->Type), supportedApiLayers)) {
            delete apiLayer;
            apiLayer = nullptr;

            axrLogWarning("Unsupported api layer: {0}", axrGetApiLayerName(apiLayer->Type));
        }
    }

    std::erase_if(
        m_ApiLayers,
        [](const AxrVulkanApiLayer_T apiLayer) {
            return apiLayer == nullptr;
        }
    );

    m_ApiLayers.shrink_to_fit();
}

void AxrVulkanGraphicsSystem::removeUnsupportedInstanceExtensions() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != VK_NULL_HANDLE) {
        axrLogWarningLocation("Instance already exists. It's too late to remove instance extensions.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const std::vector<std::string> supportedExtensions = axrGetSupportedInstanceExtensions(m_DynamicDispatchLoader);

    for (AxrVulkanExtension_T& extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) continue;

        if (!axrContainsString(axrGetExtensionName(extension->Type), supportedExtensions)) {
            delete extension;
            extension = nullptr;

            axrLogWarning("Unsupported instance extension: {0}", axrGetExtensionName(extension->Type));
        }
    }

    std::erase_if(
        m_Extensions,
        [](const AxrVulkanExtension_T extension) {
            return extension == nullptr;
        }
    );

    m_Extensions.shrink_to_fit();
}

void AxrVulkanGraphicsSystem::removeUnsupportedDeviceExtensions() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device != VK_NULL_HANDLE) {
        axrLogWarningLocation("Device already exists. It's too late to remove device extensions.");
        return;
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const std::vector<std::string> supportedExtensions = axrGetSupportedDeviceExtensions(
        m_PhysicalDevice,
        m_DynamicDispatchLoader
    );

    for (AxrVulkanExtension_T& extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE) continue;

        if (!axrContainsString(axrGetExtensionName(extension->Type), supportedExtensions)) {
            delete extension;
            extension = nullptr;

            axrLogWarning("Unsupported device extension: {0}", axrGetExtensionName(extension->Type));
        }
    }

    std::erase_if(
        m_Extensions,
        [](const AxrVulkanExtension_T extension) {
            return extension == nullptr;
        }
    );

    m_Extensions.shrink_to_fit();
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

std::vector<const char*> AxrVulkanGraphicsSystem::getAllApiLayerNames() const {
    std::vector<const char*> apiLayerNames;

    for (const AxrVulkanApiLayer_T apiLayer : m_ApiLayers) {
        if (apiLayer == nullptr) continue;

        apiLayerNames.push_back(axrGetApiLayerName(apiLayer->Type));
    }

    return apiLayerNames;
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllInstanceExtensionNames() const {
    std::vector<const char*> extensionNames;

    for (AxrVulkanExtension_T extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) continue;

        extensionNames.push_back(axrGetExtensionName(extension->Type));
    }

    return extensionNames;
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllDeviceExtensionNames() const {
    std::vector<const char*> extensionNames;

    for (AxrVulkanExtension_T extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE) continue;

        extensionNames.push_back(axrGetExtensionName(extension->Type));
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

void AxrVulkanGraphicsSystem::addRequiredDeviceExtensions() {
    auto swapchainExtension = AxrVulkanExtensionSwapchain{};
    addExtension(reinterpret_cast<AxrVulkanExtension_T>(&swapchainExtension));
}

void AxrVulkanGraphicsSystem::addExtension(const AxrVulkanExtension_T extension) {
    if (extension == nullptr) return;

    // Check if instance has been created. If it has, it's too late to add an instance extension
    if (extension->Level == AXR_VULKAN_EXTENSION_LEVEL_INSTANCE && m_Instance != VK_NULL_HANDLE) {
        axrLogErrorLocation("VkInstance has already been created.");
        return;
    }

    if (extension->Level == AXR_VULKAN_EXTENSION_LEVEL_DEVICE && m_Device != VK_NULL_HANDLE) {
        axrLogErrorLocation("VkDevice has already been created.");
        return;
    }

    if (extensionExists(extension->Type)) {
        return;
    }

    m_Extensions.push_back(axrCloneExtension(extension));
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

AxrResult AxrVulkanGraphicsSystem::setupPhysicalDevice() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_PhysicalDevice != VK_NULL_HANDLE) {
        axrLogWarningLocation("Physical device already setup.");
        return AXR_SUCCESS;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    addRequiredDeviceExtensions();

    m_PhysicalDevice = pickPhysicalDevice();
    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Failed to pick Physical device.");
        return AXR_ERROR;
    }

    const bool areApiLayersSupported = axrAreApiLayersSupportedForPhysicalDevice(
        m_PhysicalDevice,
        m_ApiLayers,
        m_DynamicDispatchLoader
    );
    if (!areApiLayersSupported) {
        axrLogWarning("Not all api layers are supported for the chosen physical device.");
    }

    const AxrResult axrResult = m_QueueFamilies.setQueueFamilyIndices(
        m_PhysicalDevice,
        m_WindowPlatform,
        m_DynamicDispatchLoader
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to set queue family indices.");
        return axrResult;
    }

    removeUnsupportedDeviceExtensions();

    return AXR_SUCCESS;
}

vk::PhysicalDevice AxrVulkanGraphicsSystem::pickPhysicalDevice() const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == VK_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return VK_NULL_HANDLE;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    // TODO: OpenXR chooses the physical device if that's set up.

    const auto physicalDevices = m_Instance.enumeratePhysicalDevices(m_DynamicDispatchLoader);
    axrLogVkResult(physicalDevices.result, "m_Instance.enumeratePhysicalDevices");
    if (axrVkFailed(physicalDevices.result)) return VK_NULL_HANDLE;

    vk::PhysicalDevice chosenPhysicalDevice = VK_NULL_HANDLE;
    uint32_t chosenPhysicalDeviceScore = 0;

    for (const vk::PhysicalDevice& physicalDevice : physicalDevices.value) {
        const uint32_t currentScore = axrScorePhysicalDeviceSuitability(
            physicalDevice,
            m_WindowPlatform,
            m_ApiLayers,
            m_Extensions,
            m_DynamicDispatchLoader
        );

        if (currentScore > chosenPhysicalDeviceScore) {
            chosenPhysicalDeviceScore = currentScore;
            chosenPhysicalDevice = physicalDevice;
        }
    }

    if (chosenPhysicalDevice == VK_NULL_HANDLE) {
        axrLogError("Failed to find a suitable physical device.");
        return VK_NULL_HANDLE;
    }

    return chosenPhysicalDevice;
}

AxrResult AxrVulkanGraphicsSystem::createLogicalDevice() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device != VK_NULL_HANDLE) {
        axrLogWarningLocation("Device already exists.");
        return AXR_SUCCESS;
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return AXR_ERROR;
    }

    if (!m_QueueFamilies.areIndicesValid()) {
        axrLogErrorLocation("Queue family indices are not valid.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const std::unordered_set<uint32_t> uniqueQueueFamilyIndices = m_QueueFamilies.getUniqueQueueFamilyIndices();
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos(uniqueQueueFamilyIndices.size());

    uint32_t index = 0;
    constexpr float queuePriority = 1.0f;
    for (uint32_t queueFamilyIndex : uniqueQueueFamilyIndices) {
        vk::DeviceQueueCreateInfo queueCreateInfo(
            {},
            queueFamilyIndex,
            1,
            &queuePriority
        );
        queueCreateInfos[index] = queueCreateInfo;

        ++index;
    }

    std::vector<const char*> deviceLayers;
    const std::vector<const char*> deviceExtensions = getAllDeviceExtensionNames();

    // If we're missing even one api layer, then pass none to the device create info.
    // We need to either pass all api layers that were passed during instance creation, or none.
    // Reference: "https://docs.vulkan.org/spec/latest/chapters/extensions.html#extendingvulkan-layers-devicelayerdeprecation"
    //
    // "The ppEnabledLayerNames and enabledLayerCount members of VkDeviceCreateInfo are deprecated and their values
    // must be ignored by implementations.
    // However, for compatibility, only an empty list of layers or a list that exactly matches the sequence enabled
    // at instance creation time are valid"
    if (axrAreApiLayersSupportedForPhysicalDevice(m_PhysicalDevice, m_ApiLayers, m_DynamicDispatchLoader)) {
        deviceLayers = getAllApiLayerNames();
    }

    vk::PhysicalDeviceFeatures supportedDeviceFeatures = m_PhysicalDevice.getFeatures(m_DynamicDispatchLoader);
    vk::PhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = supportedDeviceFeatures.samplerAnisotropy;
    deviceFeatures.sampleRateShading = supportedDeviceFeatures.sampleRateShading;

    vk::DeviceCreateInfo deviceCreateInfo(
        {},
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data(),
        static_cast<uint32_t>(deviceLayers.size()),
        deviceLayers.data(),
        static_cast<uint32_t>(deviceExtensions.size()),
        deviceExtensions.data(),
        &deviceFeatures
    );

    const vk::Result vkResult = m_PhysicalDevice.createDevice(
        &createDeviceChain(deviceCreateInfo).get<vk::DeviceCreateInfo>(),
        nullptr,
        &m_Device,
        m_DynamicDispatchLoader
    );
    axrLogVkResult(vkResult, "m_PhysicalDevice.createDevice");
    if (axrVkFailed(vkResult)) return AXR_ERROR;

    const AxrResult axrResult = m_QueueFamilies.setQueueFamilyQueues(m_Device, m_DynamicDispatchLoader);
    if (AXR_FAILED(axrResult)) {
        destroyLogicalDevice();
        axrLogErrorLocation("Failed to set queue family queues.");
        return AXR_ERROR;
    }

    m_DynamicDispatchLoader.init(m_Device);

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyLogicalDevice() {
    if (m_Device != VK_NULL_HANDLE) {
        m_Device.destroy(nullptr, m_DynamicDispatchLoader);
        m_Device = VK_NULL_HANDLE;
    }
}

AxrVulkanGraphicsSystem::DeviceChain_T AxrVulkanGraphicsSystem::createDeviceChain(
    const vk::DeviceCreateInfo& deviceCreateInfo
) const {
    DeviceChain_T chain{
        deviceCreateInfo
    };

    // No structures exist for the device next chain at the moment

    return chain;
}

void AxrVulkanGraphicsSystem::setupWindowGraphics() {
    axrLogInfo("Setting up window graphics.");
}

// ---- Private Static Functions ----

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

void AxrVulkanGraphicsSystem::onWindowOpenedCallback(void* userData, AxrWindowSystem_T windowSystem) {
    if (userData == nullptr) return;

    const auto self = static_cast<AxrVulkanGraphicsSystem*>(userData);
    self->setupWindowGraphics();
}

#endif
