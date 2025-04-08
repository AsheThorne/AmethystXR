#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// --------------------------------------------------------------------------------------------------------- //
// This is for shared functionality between multiple vulkan classes.
//
// It differs from vulkanUtils.hpp because that is more for helper functions used on vulkan objects.
// But this is for the creation/destruction of vulkan objects that need to be used in multiple classes.
//
// It just helps reduce some unnecessary code duplication for common object creation/destruction.
// --------------------------------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/result.h"

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

// ---- Render Pass ----

/// Create a simple render pass
/// @param device Device to use
/// @param colorFormat Color format to use
/// @param depthStencilFormat Depth stencil format to use
/// @param finalImageLayout Final image layout
/// @param renderPass Output created render pass
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateSimpleRenderPass(
    const vk::Device& device,
    vk::Format colorFormat,
    vk::Format depthStencilFormat,
    vk::ImageLayout finalImageLayout,
    vk::RenderPass& renderPass,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Destroy the given render pass
/// @param device Device to use
/// @param renderPass Render pass to destroy
/// @param dispatch Dispatch to use
void axrDestroyRenderPass(
    const vk::Device& device,
    vk::RenderPass& renderPass,
    const vk::DispatchLoaderDynamic& dispatch
);

#endif
