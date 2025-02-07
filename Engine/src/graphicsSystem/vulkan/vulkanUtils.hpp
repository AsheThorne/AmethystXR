#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/vulkanApi.h"
#include "axr/windowSystem.h"

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

/// Find the supported instance api layers
/// @param dispatch Dispatch to use
/// @returns The supported instance api layers
std::vector<std::string> axrGetSupportedInstanceApiLayers(
    const vk::DispatchLoaderDynamic& dispatch
);
/// Find the supported device api layers
/// @param physicalDevice Physical device to use
/// @param dispatch Dispatch to use
/// @returns The supported device api layers
std::vector<std::string> axrGetSupportedDeviceApiLayers(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Find the supported instance extensions
/// @param dispatch Dispatch to use
/// @returns The supported instance extensions
std::vector<std::string> axrGetSupportedInstanceExtensions(
    const vk::DispatchLoaderDynamic& dispatch
);
/// Find the supported device extensions
/// @param physicalDevice Physical device to use
/// @param dispatch Dispatch to use
/// @returns The supported device extensions
std::vector<std::string> axrGetSupportedDeviceExtensions(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
);

// ---- Physical Device ----

/// Score the given physical device. The higher the number, the better.
/// 0 Means it cannot be used at all.
/// @param physicalDevice Physical device to score
/// @param windowPlatform Window platform to use
/// @param apiLayers Api layers we want to use
/// @param extensions Extensions we want to use
/// @param dispatch Dispatch loader to use
/// @returns The physical device score
uint32_t axrScorePhysicalDeviceSuitability(
    const vk::PhysicalDevice& physicalDevice,
    AxrWindowPlatformEnum windowPlatform,
    const std::vector<AxrVulkanApiLayer_T>& apiLayers,
    const std::vector<AxrVulkanExtension_T>& extensions,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Score the given physical device on its available queue families.
/// @param physicalDevice Physical device to score
/// @param windowPlatform Window platform to use
/// @param dispatch Dispatch loader to use
/// @returns 0 -> Not suitable at all.
/// @returns 1 -> Meets the minimum requirements.
/// @returns More than 1, has extra desired features.
uint32_t axrScorePhysicalDeviceQueueFamilies(
    const vk::PhysicalDevice& physicalDevice,
    AxrWindowPlatformEnum windowPlatform,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Score the given physical device on its available api layers.
/// @details
/// All api layers used by the instance should also be used for the device.
/// This is for compatibility with older version of vulkan.
/// "https://docs.vulkan.org/spec/latest/chapters/extensions.html#extendingvulkan-layers-devicelayerdeprecation"
/// If any of the api layers aren't supported, then it's still ok for more recent versions of vulkan.
/// So, a score of 1 indicates that not all api layers are supported, but it's not desirable.
/// A score of more than 1 means all api layers are supported.
/// @param physicalDevice Physical device to score
/// @param apiLayers Api layers we want to use
/// @param dispatch Dispatch loader to use
/// @returns 0 -> Not suitable at all.
/// @returns 1 -> Meets the minimum requirements.
/// @returns More than 1, supports all api layers
uint32_t axrScorePhysicalDeviceApiLayers(
    const vk::PhysicalDevice& physicalDevice,
    const std::vector<AxrVulkanApiLayer_T>& apiLayers,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Check if all the given api layers are supported for the given physical device
/// @param physicalDevice Physical device to check
/// @param apiLayers Api layers we want to check
/// @param dispatch Dispatch loader to use
/// @returns True if all the given api layers are supported for the given physical device
bool axrAreApiLayersSupportedForPhysicalDevice(
    const vk::PhysicalDevice& physicalDevice,
    const std::vector<AxrVulkanApiLayer_T>& apiLayers,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Score the given physical device on its available extensions.
/// @param physicalDevice Physical device to score
/// @param extensions Extensions we want to use
/// @param dispatch Dispatch loader to use
/// @returns 0 -> Not suitable at all.
/// @returns 1 -> Meets the minimum requirements.
/// @returns More than 1, supports more than just the required extensions.
uint32_t axrScorePhysicalDeviceExtensions(
    const vk::PhysicalDevice& physicalDevice,
    const std::vector<AxrVulkanExtension_T>& extensions,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Score the given physical device on its available features.
/// @param physicalDevice Physical device to score
/// @param dispatch Dispatch loader to use
/// @returns 0 -> Not suitable at all.
/// @returns 1 -> Meets the minimum requirements.
/// @returns More than 1, has extra desired features.
uint32_t axrScorePhysicalDeviceFeatures(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Score the given physical device on its available properties.
/// @param physicalDevice Physical device to score
/// @param dispatch Dispatch loader to use
/// @returns 0 -> Not suitable at all.
/// @returns 1 -> Meets the minimum requirements.
/// @returns More than 1, has extra desired features.
uint32_t axrScorePhysicalDeviceProperties(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
);

#endif
