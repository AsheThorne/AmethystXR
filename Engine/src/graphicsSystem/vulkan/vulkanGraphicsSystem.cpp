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
    m_Instance(nullptr) {
    if (config.VulkanConfig == nullptr) {
        axrLogErrorLocation("Vulkan Config is null.");
        return;
    }

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
    destroyInstance();
    destroyExtensions();
    destroyApiLayers();
}

// ---- Public Functions ----

AxrResult AxrVulkanGraphicsSystem::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    // TODO...
    // setupDebugUtilsCreateInfo();

    axrResult = createInstance();
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

    const auto [vkResult, instance] = vk::createInstance(instanceCreateInfo, nullptr);
    axrLogVkResult(vkResult, "vk::createInstance");

    if (axrVkFailed(vkResult)) {
        return AXR_ERROR;
    }

    m_Instance = instance;

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyInstance() {
    if (m_Instance != nullptr) {
        m_Instance.destroy();
        m_Instance = nullptr;
    }
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedInstanceApiLayers() const {
    auto instanceLayerProperties = vk::enumerateInstanceLayerProperties();
    axrLogVkResult(instanceLayerProperties.result, "vk::enumerateInstanceLayerProperties");

    if (axrVkFailed(instanceLayerProperties.result)) {
        return {};
    }

    std::vector<std::string> supportedApiLayers(instanceLayerProperties.value.size());

    for (size_t i = 0; i < instanceLayerProperties.value.size(); ++i) {
        supportedApiLayers[i] = instanceLayerProperties.value[i].layerName.data();
    }

    return supportedApiLayers;
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedInstanceExtensions() const {
    auto instanceExtensionProperties = vk::enumerateInstanceExtensionProperties();
    axrLogVkResult(instanceExtensionProperties.result, "vk::enumerateInstanceExtensionProperties");

    if (axrVkFailed(instanceExtensionProperties.result)) {
        return {};
    }

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

#endif
