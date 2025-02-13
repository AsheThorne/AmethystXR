#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

#define VK_SUCCEEDED(result) ((result) >= 0)
#define VK_FAILED(result) ((result) < 0)

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/result.h"
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

// ---- Image ----

/// Create image views for the given images
/// @param device Device to use
/// @param images Images to create image views from
/// @param format Format to use
/// @param aspectMask Aspect mask to use
/// @param mipLevels Mip levels to use
/// @param imageViews Output image views
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateImageViews(
    const vk::Device& device,
    const std::vector<vk::Image>& images,
    vk::Format format,
    vk::ImageAspectFlags aspectMask,
    uint32_t mipLevels,
    std::vector<vk::ImageView>& imageViews,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Destroy the given image views
/// @param device Device to use
/// @param imageViews Image views to destroy
/// @param dispatch Dispatch to use
void axrDestroyImageViews(
    const vk::Device& device,
    std::vector<vk::ImageView>& imageViews,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Create an image view for the given image
/// @param device Device to use
/// @param image Image to create image view from
/// @param format Format to use
/// @param aspectMask Aspect mask to use
/// @param mipLevels Mip levels to use
/// @param imageView Output image view
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateImageView(
    const vk::Device& device,
    const vk::Image& image,
    vk::Format format,
    vk::ImageAspectFlags aspectMask,
    uint32_t mipLevels,
    vk::ImageView& imageView,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Destroy the given image view
/// @param device Device to use
/// @param imageView Image view to destroy
/// @param dispatch Dispatch to use
void axrDestroyImageView(
    const vk::Device& device,
    vk::ImageView& imageView,
    const vk::DispatchLoaderDynamic& dispatch
);

#endif
