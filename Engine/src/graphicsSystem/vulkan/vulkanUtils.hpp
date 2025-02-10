#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

#define VK_SUCCEEDED(result) ((result) >= 0)
#define VK_FAILED(result) ((result) < 0)

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/vulkanApi.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <source_location>

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

// ----------------------------------------- //
// Function Definitions
// ----------------------------------------- //

// ---- Logging ----

/// Check if the vulkan result succeeded
/// @param result Vulkan result
/// @returns True if the result succeeded
bool axrVkSucceeded(vk::Result result);

/// Check if the vulkan result failed
/// @param result Vulkan result
/// @returns True if the result failed
bool axrVkFailed(vk::Result result);

/// Log a vulkan result if it failed
/// @param result Vulkan result
/// @param functionName The name of the function that gave the result
/// @param location Source file location. You don't need to ever change this from the default
void axrLogVkResult(
    vk::Result result,
    const char* functionName,
    const std::source_location& location = std::source_location::current()
);

// ---- Api Layers / Extensions ----

/// Get the api layer name from the given vulkan api layer type
/// @param apiLayerType Vulkan api layer type
/// @returns The api layer name
const char* axrGetApiLayerName(AxrVulkanApiLayerTypeEnum apiLayerType);
/// Get the extension name from the given vulkan extension type
/// @param extensionType Vulkan extension type
/// @returns The extension name
const char* axrGetExtensionName(AxrVulkanExtensionTypeEnum extensionType);

/// Clone all the given vulkan api layers
/// @param apiLayersCount Number of api layers in the given apiLayers pointer
/// @param apiLayers Api layers to clone
/// @returns A collection of cloned api layers
[[nodiscard]] std::vector<AxrVulkanApiLayer_T> axrCloneApiLayers(
    uint32_t apiLayersCount,
    const AxrVulkanApiLayer_T* apiLayers
);
/// Clone all the given vulkan extensions
/// @param extensionsCount Number of extensions in the given extensions pointer
/// @param extensions Extensions to clone
/// @returns A collection of cloned extensions
[[nodiscard]] std::vector<AxrVulkanExtension_T> axrCloneExtensions(
    uint32_t extensionsCount,
    const AxrVulkanExtension_T* extensions
);

/// Clone the given api layer
/// @param apiLayer  Api layer to clone
/// @returns A clone of the api layer
[[nodiscard]] AxrVulkanApiLayer_T axrCloneApiLayer(AxrVulkanApiLayer_T apiLayer);
/// Clone the given extension
/// @param extension Extension to clone
/// @returns A clone of the extension
[[nodiscard]] AxrVulkanExtension_T axrCloneExtension(AxrVulkanExtension_T extension);

#endif
