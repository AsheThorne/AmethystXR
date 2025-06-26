#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

#define VK_SUCCEEDED(result) ((result) >= 0)
#define VK_FAILED(result) ((result) < 0)

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/vulkanApi.h"
#include "axr/common/enums.h"
#include "axr/assets.h"
#include "axr/graphicsSystem.h"

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

/// Check if the given format includes a stencil component
/// @param format Format to check
/// @returns True if the format includes a stencil component
[[nodiscard]] bool axrFormatHasStencilComponent(vk::Format format);

// ---- Memory ----

/// Find a suitable memory type index from the given type filter and properties
/// @param physicalDevice Physical device to use
/// @param typeFilter Type filter
/// @param properties Memory properties
/// @param memoryTypeIndex Output memory type index
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrFindMemoryTypeIndex(
    const vk::PhysicalDevice& physicalDevice,
    uint32_t typeFilter,
    vk::MemoryPropertyFlags properties,
    uint32_t& memoryTypeIndex,
    const vk::DispatchLoaderDynamic& dispatch
);

// ---- Command Buffer ----

/// Begin a single time command
/// @param device Device to use
/// @param commandPool Command pool to use
/// @param commandBuffer Output command buffer to record commands on
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrBeginSingleTimeCommand(
    vk::Device device,
    vk::CommandPool commandPool,
    vk::CommandBuffer& commandBuffer,
    const vk::DispatchLoaderDynamic& dispatch
);
/// End a single time command
/// @param device Device to use
/// @param commandPool Command pool to use
/// @param queue Queue to use
/// @param commandBuffer Command buffer to use
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrEndSingleTimeCommand(
    vk::Device device,
    vk::CommandPool commandPool,
    vk::Queue queue,
    vk::CommandBuffer& commandBuffer,
    const vk::DispatchLoaderDynamic& dispatch
);

// ---- Image Sampler ----

/// Convert an AxrImageSamplerFilterEnum to a vk::Filter
/// @param samplerFilter Image sampler filter to convert
/// @returns The converted image sampler filter
[[nodiscard]] vk::Filter axrToVkFilter(AxrImageSamplerFilterEnum samplerFilter);

/// Convert an AxrImageSamplerFilterEnum to a vk::SamplerMipmapMode
/// @param samplerFilter Image sampler filter to convert
/// @returns The converted image sampler filter
[[nodiscard]] vk::SamplerMipmapMode axrToVkSamplerMipmapMode(AxrImageSamplerFilterEnum samplerFilter);

/// Convert an AxrImageSamplerWrapEnum to a vk::SamplerAddressMode
/// @param samplerWrap Image sampler wrap to convert
/// @returns The converted image sampler wrap
[[nodiscard]] vk::SamplerAddressMode axrToVkSamplerAddressMode(AxrImageSamplerWrapEnum samplerWrap);

// ---- Msaa ----

/// Convert an AxrMsaaSampleCountEnum to a vk::SampleCountFlagBits
/// @param sampleCount Sample count to convert
/// @returns The converted sample count
[[nodiscard]] vk::SampleCountFlagBits axrToVkSampleCount(AxrMsaaSampleCountEnum sampleCount);

/// Check if msaa is enabled
/// @param sampleCount Msaa sample count
/// @returns True if msaa is enabled
[[nodiscard]] bool axrIsVulkanMsaaEnabled(vk::SampleCountFlagBits sampleCount);

/// Get the highest sample count that we can support
/// @param physicalDevice Physical device to use
/// @param dispatch Dispatch to use
/// @returns The highest sample count that we can support
[[nodiscard]] vk::SampleCountFlagBits axrGetVulkanSampleCountLimit(
    vk::PhysicalDevice physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Get the vk::SampleCountFlagBit to use based on our desired sample count.
/// If `sampleCount` isn't supported, it chooses the highest available sample count.
/// @param physicalDevice Physical device to use
/// @param sampleCount Desired sample count
/// @param dispatch Dispatch to use
[[nodiscard]] vk::SampleCountFlagBits axrGetVulkanSampleCountToUse(
    vk::PhysicalDevice physicalDevice,
    AxrMsaaSampleCountEnum sampleCount,
    const vk::DispatchLoaderDynamic& dispatch
);

#endif
