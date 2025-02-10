#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanUtils.hpp"
#include "axr/logger.h"

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

const char* axrGetApiLayerName(const AxrVulkanApiLayerTypeEnum apiLayerType) {
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

const char* axrGetExtensionName(const AxrVulkanExtensionTypeEnum extensionType) {
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
        case AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE: {
#ifdef AXR_USE_PLATFORM_WIN32
            return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#else
            axrLogErrorLocation("Win32 surface extension not supported.");
            return "";
#endif
        }
        case AXR_VULKAN_EXTENSION_TYPE_END:
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extensionType));
            return "";
        }
    }
}

std::vector<AxrVulkanApiLayer_T> axrCloneApiLayers(
    const uint32_t apiLayersCount,
    const AxrVulkanApiLayer_T* apiLayers
) {
    std::vector<AxrVulkanApiLayer_T> clonedApiLayers{};

    for (uint32_t i = 0; i < apiLayersCount; ++i) {
        if (apiLayers[i] == nullptr) continue;

        clonedApiLayers.push_back(axrCloneApiLayer(apiLayers[i]));
    }

    return clonedApiLayers;
}

std::vector<AxrVulkanExtension_T> axrCloneExtensions(
    const uint32_t extensionsCount,
    const AxrVulkanExtension_T* extensions
) {
    std::vector<AxrVulkanExtension_T> clonedExtensions{};

    for (uint32_t i = 0; i < extensionsCount; ++i) {
        if (extensions[i] == nullptr) continue;

        clonedExtensions.push_back(axrCloneExtension(extensions[i]));
    }

    return clonedExtensions;
}

AxrVulkanApiLayer_T axrCloneApiLayer(const AxrVulkanApiLayer_T apiLayer) {
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

AxrVulkanExtension_T axrCloneExtension(const AxrVulkanExtension_T extension) {
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
#ifdef AXR_USE_PLATFORM_WIN32
            const auto castExtension = *reinterpret_cast<AxrVulkanExtensionWin32Surface*>(extension);
            return reinterpret_cast<AxrVulkanExtension_T>(new AxrVulkanExtensionWin32Surface(castExtension));
#else
            axrLogErrorLocation("Win32 surface extension is not supported.");
            return nullptr;
#endif
        }
        case AXR_VULKAN_EXTENSION_TYPE_END:
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown extension type {0}.", static_cast<uint32_t>(extension->Type));
            return nullptr;
        }
    }
}

#endif
