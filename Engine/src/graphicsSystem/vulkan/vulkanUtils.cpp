#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanUtils.hpp"
#include "axr/common/defines.h"
#include "axr/logger.h"
#include "../../utils.hpp"

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
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extensionType));
            return "";
        }
    }
}

std::vector<AxrVulkanApiLayer_T> axrCloneSupportedVulkanInstanceApiLayers(
    const uint32_t apiLayersCount,
    const AxrVulkanApiLayer_T* apiLayers,
    const std::vector<std::string>& supportedApiLayers
) {
    std::vector<AxrVulkanApiLayer_T> clonedApiLayers{};

    for (uint32_t i = 0; i < apiLayersCount; ++i) {
        if (apiLayers[i] == nullptr) continue;
        if (apiLayers[i]->Level != AXR_VULKAN_API_LAYER_LEVEL_INSTANCE) continue;

        const char* apiLayerName = axrGetVulkanApiLayerName(apiLayers[i]->Type);
        if (!axrContainsString(apiLayerName, supportedApiLayers)) {
            axrLogWarning("{0} Api Layer is not supported.", apiLayerName);
            continue;
        }

        clonedApiLayers.push_back(axrCloneVulkanApiLayer(apiLayers[i]));
    }

    return clonedApiLayers;
}

std::vector<AxrVulkanExtension_T> axrCloneSupportedVulkanInstanceExtensions(
    const uint32_t extensionsCount,
    const AxrVulkanExtension_T* extensions,
    const std::vector<std::string>& supportedExtensions
) {
    std::vector<AxrVulkanExtension_T> clonedExtensions{};

    for (uint32_t i = 0; i < extensionsCount; ++i) {
        if (extensions[i] == nullptr) continue;
        if (extensions[i]->Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) continue;

        const char* extensionName = axrGetVulkanExtensionName(extensions[i]->Type);
        if (!axrContainsString(extensionName, supportedExtensions)) {
            axrLogWarning("{0} Extension is not supported.", extensionName);
            continue;
        }

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
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extension->Type));
            return nullptr;
        }
    }
}

#endif
