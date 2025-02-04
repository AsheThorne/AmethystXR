#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanUtils.hpp"
#include "axr/common/defines.h"
#include "axr/logger.h"
#include "../../utils.hpp"
#include "vulkanQueueFamilies.hpp"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan_core.h>

bool axrVkSucceeded(vk::Result result) {
    return VK_SUCCEEDED(static_cast<VkResult>(result));
}

bool axrVkFailed(vk::Result result) {
    return VK_FAILED(static_cast<VkResult>(result));
}

void axrLogVkResult(
    const vk::Result result,
    const char* functionName,
    const std::source_location& location
) {
    if (axrVkFailed(result)) {
        axrLogErrorLocation(
            {"{0} failed with a result of {1}.", location},
            functionName,
            to_string(result).c_str()
        );
    }
}

const char* axrGetVulkanApiLayerName(const AxrVulkanApiLayerTypeEnum apiLayerType) {
    switch (apiLayerType) {
        case AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION: {
            return "VK_LAYER_KHRONOS_validation";
        }
        case AXR_VULKAN_API_LAYER_TYPE_END:
        case AXR_VULKAN_API_LAYER_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown api layer type {0}.", static_cast<uint32_t>(apiLayerType));
            return "";
        }
    }
}

const char* axrGetVulkanExtensionName(const AxrVulkanExtensionTypeEnum extensionType) {
    switch (extensionType) {
        case AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS: {
            return VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        }
        case AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN: {
            return VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        }
        case AXR_VULKAN_EXTENSION_TYPE_SURFACE: {
            return VK_KHR_SURFACE_EXTENSION_NAME;
        }
#ifdef AXR_PLATFORM_WIN32
        case AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE: {
            return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
        }
#endif
        case AXR_VULKAN_EXTENSION_TYPE_END:
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extensionType));
            return "";
        }
    }
}

std::vector<AxrVulkanApiLayer_T> axrCloneVulkanApiLayers(
    const uint32_t apiLayersCount,
    const AxrVulkanApiLayer_T* apiLayers
) {
    std::vector<AxrVulkanApiLayer_T> clonedApiLayers{};

    for (uint32_t i = 0; i < apiLayersCount; ++i) {
        if (apiLayers[i] == nullptr) continue;

        clonedApiLayers.push_back(axrCloneVulkanApiLayer(apiLayers[i]));
    }

    return clonedApiLayers;
}

std::vector<AxrVulkanExtension_T> axrCloneVulkanExtensions(
    const uint32_t extensionsCount,
    const AxrVulkanExtension_T* extensions
) {
    std::vector<AxrVulkanExtension_T> clonedExtensions{};

    for (uint32_t i = 0; i < extensionsCount; ++i) {
        if (extensions[i] == nullptr) continue;

        clonedExtensions.push_back(axrCloneVulkanExtension(extensions[i]));
    }

    return clonedExtensions;
}

AxrVulkanApiLayer_T axrCloneVulkanApiLayer(const AxrVulkanApiLayer_T apiLayer) {
    if (apiLayer == nullptr) return nullptr;

    switch (apiLayer->Type) {
        case AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION: {
            const auto castApiLayer = *reinterpret_cast<AxrVulkanApiLayerCoreValidation*>(apiLayer);
            return reinterpret_cast<AxrVulkanApiLayer_T>(new AxrVulkanApiLayerCoreValidation(castApiLayer));
        }
        case AXR_VULKAN_API_LAYER_TYPE_END:
        case AXR_VULKAN_API_LAYER_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown api layer type {0}.", static_cast<uint32_t>(apiLayer->Type));
            return nullptr;
        }
    }
}

AxrVulkanExtension_T axrCloneVulkanExtension(const AxrVulkanExtension_T extension) {
    if (extension == nullptr) return nullptr;

    switch (extension->Type) {
        case AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS: {
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionDebugUtils*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionDebugUtils(castExtension));
        }
        case AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN: {
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionSwapchain*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionSwapchain(castExtension));
        }
        case AXR_VULKAN_EXTENSION_TYPE_SURFACE: {
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionSurface*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionSurface(castExtension));
        }
        case AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE: {
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionWin32Surface*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionWin32Surface(castExtension));
        }
        case AXR_VULKAN_EXTENSION_TYPE_END:
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extension->Type));
            return nullptr;
        }
    }
}

std::vector<std::string> axrGetSupportedInstanceApiLayers(
    const vk::DispatchLoaderDynamic& dispatch
) {
    const auto instanceLayerProperties = vk::enumerateInstanceLayerProperties(
        dispatch
    );
    axrLogVkResult(instanceLayerProperties.result, "vk::enumerateInstanceLayerProperties");

    if (axrVkFailed(instanceLayerProperties.result)) return {};

    std::vector<std::string> supportedApiLayers(instanceLayerProperties.value.size());

    for (size_t i = 0; i < instanceLayerProperties.value.size(); ++i) {
        supportedApiLayers[i] = instanceLayerProperties.value[i].layerName.data();
    }

    return supportedApiLayers;
}

std::vector<std::string> axrGetSupportedDeviceApiLayers(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return {};
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto deviceLayerProperties = physicalDevice.enumerateDeviceLayerProperties(dispatch);
    axrLogVkResult(deviceLayerProperties.result, "physicalDevice.enumerateDeviceLayerProperties");

    if (axrVkFailed(deviceLayerProperties.result)) return {};

    std::vector<std::string> supportedApiLayers(deviceLayerProperties.value.size());

    for (size_t i = 0; i < deviceLayerProperties.value.size(); ++i) {
        supportedApiLayers[i] = deviceLayerProperties.value[i].layerName.data();
    }

    return supportedApiLayers;
}

std::vector<std::string> axrGetSupportedInstanceExtensions(
    const vk::DispatchLoaderDynamic& dispatch
) {
    const auto instanceExtensionProperties = vk::enumerateInstanceExtensionProperties(
        nullptr,
        dispatch
    );
    axrLogVkResult(instanceExtensionProperties.result, "vk::enumerateInstanceExtensionProperties");

    if (axrVkFailed(instanceExtensionProperties.result)) return {};

    std::vector<std::string> supportedExtensions(instanceExtensionProperties.value.size());

    for (size_t i = 0; i < instanceExtensionProperties.value.size(); ++i) {
        supportedExtensions[i] = instanceExtensionProperties.value[i].extensionName.data();
    }

    return supportedExtensions;
}

std::vector<std::string> axrGetSupportedDeviceExtensions(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return {};
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto deviceExtensionProperties =
        physicalDevice.enumerateDeviceExtensionProperties(nullptr, dispatch);
    axrLogVkResult(deviceExtensionProperties.result, "physicalDevice.enumerateDeviceExtensionProperties");

    if (axrVkFailed(deviceExtensionProperties.result)) return {};

    std::vector<std::string> supportedExtensions(deviceExtensionProperties.value.size());

    for (size_t i = 0; i < deviceExtensionProperties.value.size(); ++i) {
        supportedExtensions[i] = deviceExtensionProperties.value[i].extensionName.data();
    }

    return supportedExtensions;
}

uint32_t axrScorePhysicalDeviceSuitability(
    const vk::PhysicalDevice& physicalDevice,
    const AxrWindowPlatformEnum windowPlatform,
    const std::vector<AxrVulkanApiLayer_T>& apiLayers,
    const std::vector<AxrVulkanExtension_T>& extensions,
    const vk::DispatchLoaderDynamic& dispatch
) {
    const uint32_t queueFamiliesScore = axrScorePhysicalDeviceQueueFamilies(
        physicalDevice,
        windowPlatform,
        dispatch
    );
    if (queueFamiliesScore == 0) {
        return 0;
    }

    const uint32_t apiLayersScore = axrScorePhysicalDeviceApiLayers(physicalDevice, apiLayers, dispatch);
    if (apiLayersScore == 0) {
        return 0;
    }

    const uint32_t extensionsScore = axrScorePhysicalDeviceExtensions(physicalDevice, extensions, dispatch);
    if (extensionsScore == 0) {
        return 0;
    }

    const uint32_t featuresScore = axrScorePhysicalDeviceFeatures(physicalDevice, dispatch);
    if (featuresScore == 0) {
        return 0;
    }

    const uint32_t propertiesScore = axrScorePhysicalDeviceProperties(physicalDevice, dispatch);
    if (featuresScore == 0) {
        return 0;
    }

    return queueFamiliesScore + apiLayersScore + extensionsScore + featuresScore + propertiesScore;
}

uint32_t axrScorePhysicalDeviceQueueFamilies(
    const vk::PhysicalDevice& physicalDevice,
    const AxrWindowPlatformEnum windowPlatform,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return 0;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrVulkanQueueFamilies queueFamilies;
    const AxrResult axrResult = queueFamilies.setQueueFamilyIndices(physicalDevice, windowPlatform, dispatch);

    if (AXR_FAILED(axrResult)) {
        // Failed to find required queue families
        return 0;
    }

    if (!queueFamilies.hasDedicatedTransferQueue()) {
        // We met the minimum requirements.
        // All queue families have been found, but it's not ideal without a dedicated transfer queue family.
        return 1;
    }

    // The ideal case. We have all the queue families and a dedicated transfer queue.
    // '5' is an arbitrary value currently.
    return 5;
}

uint32_t axrScorePhysicalDeviceApiLayers(
    const vk::PhysicalDevice& physicalDevice,
    const std::vector<AxrVulkanApiLayer_T>& apiLayers,
    const vk::DispatchLoaderDynamic& dispatch
) {
    constexpr uint32_t maxScore = 50;

    if (apiLayers.empty()) {
        // All api layers are supported. Cos there are none. duhh.
        return maxScore;
    }

    if (!axrAreApiLayersSupportedForPhysicalDevice(physicalDevice, apiLayers, dispatch)) {
        // We met the minimum requirements.
        // Missing api layer(s)
        return 1;
    }

    // All api layers are supported
    return maxScore;
}

bool axrAreApiLayersSupportedForPhysicalDevice(
    const vk::PhysicalDevice& physicalDevice,
    const std::vector<AxrVulkanApiLayer_T>& apiLayers,
    const vk::DispatchLoaderDynamic& dispatch
) {
    const std::vector<std::string> supportedApiLayers = axrGetSupportedDeviceApiLayers(physicalDevice, dispatch);

    for (const AxrVulkanApiLayer_T apiLayer : apiLayers) {
        if (!axrContainsString(axrGetVulkanApiLayerName(apiLayer->Type), supportedApiLayers)) {
            // Api layer isn't supported
            return false;
        }
    }

    return true;
}

uint32_t axrScorePhysicalDeviceExtensions(
    const vk::PhysicalDevice& physicalDevice,
    const std::vector<AxrVulkanExtension_T>& extensions,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // Isolate device level extensions
    std::vector<AxrVulkanExtension_T> deviceExtensions = extensions;
    std::erase_if(
        deviceExtensions,
        [](const AxrVulkanExtension_T extension) {
            return extension->Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE;
        }
    );

    constexpr uint32_t maxScore = 50;

    if (deviceExtensions.empty()) {
        return maxScore;
    }

    const std::vector<std::string> supportedExtensions = axrGetSupportedDeviceExtensions(physicalDevice, dispatch);

    // 50 is the max score if all extensions are supported.
    // So the closer we get to 50 as the final score, the more extensions were found.
    const float extensionWeightedScore = static_cast<float>(maxScore) / static_cast<float>(deviceExtensions.size());
    float score = 0.0f;

    for (const AxrVulkanExtension_T extension : deviceExtensions) {
        // TODO: Some extensions may be required and this function should fail if it doesn't have it.
        //  That isn't important for now though.
        if (axrContainsString(axrGetVulkanExtensionName(extension->Type), supportedExtensions)) {
            score += extensionWeightedScore;
        }
    }

    // ---- We shouldn't return 0 beyond this point ----

    // We need to return at least 1 to signal that we at least meet the minimum requirements.
    return std::max(static_cast<uint32_t>(1), static_cast<uint32_t>(score));
}

uint32_t axrScorePhysicalDeviceFeatures(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return {};
    }
    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t score = 0;
    const vk::PhysicalDeviceFeatures features = physicalDevice.getFeatures(dispatch);

    if (features.samplerAnisotropy) {
        score += 5;
    }
    if (features.sampleRateShading) {
        score += 5;
    }

    return score;
}

uint32_t axrScorePhysicalDeviceProperties(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return {};
    }
    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t score = 0;
    const vk::PhysicalDeviceProperties properties = physicalDevice.getProperties(dispatch);

    if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
        score += 1000;
    } else {
        score += 1;
    }

    return score;
}

#endif
