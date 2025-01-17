#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

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

/// Get the api layer name from the given vulkan api layer type
/// @param apiLayerType Vulkan api layer type
/// @returns The api layer name
const char* axrGetVulkanApiLayerName(AxrVulkanApiLayerTypeEnum apiLayerType);

/// Get the extension name from the given vulkan extension type
/// @param extensionType Vulkan extension type
/// @returns The extension name
const char* axrGetVulkanExtensionName(AxrVulkanExtensionTypeEnum extensionType);

/// Clone all the supported vulkan api layers
/// @param apiLayersCount Number of api layers in the given apiLayers pointer
/// @param apiLayers Api layers to clone
/// @param supportedApiLayers Supported api layers
/// @returns A collection of cloned api layers
std::vector<AxrVulkanApiLayer_T> axrCloneSupportedVulkanInstanceApiLayers(
    uint32_t apiLayersCount,
    const AxrVulkanApiLayer_T* apiLayers,
    const std::vector<std::string>& supportedApiLayers
);

/// Clone all the supported vulkan extensions
/// @param extensionsCount Number of extensions in the given extensions pointer
/// @param extensions Extensions to clone
/// @param supportedExtensions Supported extensions
/// @returns A collection of cloned extensions
std::vector<AxrVulkanExtension_T> axrCloneSupportedVulkanInstanceExtensions(
    uint32_t extensionsCount,
    const AxrVulkanExtension_T* extensions,
    const std::vector<std::string>& supportedExtensions
);

/// Clone the given api layer
/// @param apiLayer  Api layer to clone
/// @returns A clone of the api layer
AxrVulkanApiLayer_T axrCloneVulkanApiLayer(AxrVulkanApiLayer_T apiLayer);

/// Clone the given extension
/// @param extension Extension to clone
/// @returns A clone of the extension
AxrVulkanExtension_T axrCloneVulkanExtension(AxrVulkanExtension_T extension);

#endif
