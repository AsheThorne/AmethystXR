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
// Enum Extensions
// ----------------------------------------- //

inline auto operator<=>(const vk::Result& result, const int number) noexcept {
    return static_cast<VkResult>(result) <=> number;
}

// ----------------------------------------- //
// Function Definitions
// ----------------------------------------- //

// ---- Logging ----

/// Log a vulkan result if it failed
/// @param result Vulkan result
/// @param functionName The name of the function that gave the result
/// @param location Source file location. You don't need to ever change this from the default
void axrLogVkResult(
    vk::Result result,
    const char* functionName,
    const std::source_location& location = std::source_location::current()
);

// ---- Enum Convertors ----

/// Convert an AxrVulkanPresentationModeEnum to a vk::PresentModeKHR
/// @param presentationMode Presentation mode to convert
/// @returns The converted presentation mode
vk::PresentModeKHR axrToVkPresentMode(AxrVulkanPresentationModeEnum presentationMode);

// ---- Api Layers / Extensions ----

/// Get the api layer name from the given vulkan api layer type
/// @param apiLayerType Vulkan api layer type
/// @returns The api layer name
const char* axrGetApiLayerName(AxrVulkanApiLayerTypeEnum apiLayerType);
/// Get the extension name from the given vulkan extension type
/// @param extensionType Vulkan extension type
/// @returns The extension name
const char* axrGetExtensionName(AxrVulkanExtensionTypeEnum extensionType);

// ---- Format ----

/// Check if the given format supports the given features
/// @param format Format to check against
/// @param tiling Image tiling option
/// @param features Features to check
/// @param physicalDevice Physical device to use
/// @param dispatch Dispatch to use
/// @returns True if the given format supports the given features
[[nodiscard]] bool axrAreFormatFeaturesSupported(
    vk::Format format,
    vk::ImageTiling tiling,
    vk::FormatFeatureFlags features,
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
);

#endif
