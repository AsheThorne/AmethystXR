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
#include "axr/common/enums.h"

// ---- Image ----

/// Create image views for the given images
/// @param device Device to use
/// @param images Images to create image views from
/// @param format Format to use
/// @param aspectMask Aspect mask to use
/// @param mipLevelCount Mip level count to use
/// @param imageViews Output image views
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateImageViews(
    const vk::Device& device,
    const std::vector<vk::Image>& images,
    vk::Format format,
    vk::ImageAspectFlags aspectMask,
    uint32_t mipLevelCount,
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
/// @param mipLevelCount Mip level count to use
/// @param imageView Output image view
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateImageView(
    const vk::Device& device,
    const vk::Image& image,
    vk::Format format,
    vk::ImageAspectFlags aspectMask,
    uint32_t mipLevelCount,
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

/// Create a render pass
/// @param device Device to use
/// @param colorFormat Color format to use
/// @param depthStencilFormat Depth stencil format to use
/// @param finalImageLayout Final image layout
/// @param renderPass Output created render pass
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateRenderPass(
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

// ---- Framebuffer ----

/// Create a framebuffer for each swapchain color image view
/// @param device Device to use
/// @param renderPass Render pass to use
/// @param swapchainExtent Swapchain extent
/// @param swapchainColorImageViews Swapchain color image views
/// @param framebuffers Output created framebuffers
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateFramebuffers(
    const vk::Device& device,
    const vk::RenderPass& renderPass,
    const vk::Extent2D& swapchainExtent,
    const std::vector<vk::ImageView>& swapchainColorImageViews,
    std::vector<vk::Framebuffer>& framebuffers,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Destroy the given framebuffers
/// @param device Device to use
/// @param framebuffers Framebuffers to destroy
/// @param dispatch Dispatch to use
void axrDestroyFramebuffers(
    const vk::Device& device,
    std::vector<vk::Framebuffer>& framebuffers,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Create a framebuffer
/// @param device Device to use
/// @param renderPass Render pass to use
/// @param swapchainExtent Swapchain extent
/// @param swapchainColorImageView Swapchain color image view
/// @param framebuffer Output created framebuffer
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateFramebuffer(
    const vk::Device& device,
    const vk::RenderPass& renderPass,
    const vk::Extent2D& swapchainExtent,
    const vk::ImageView& swapchainColorImageView,
    vk::Framebuffer& framebuffer,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Destroy the given framebuffer
/// @param device Device to use
/// @param framebuffer Framebuffer to destroy
/// @param dispatch Dispatch to use
void axrDestroyFramebuffer(
    const vk::Device& device,
    vk::Framebuffer& framebuffer,
    const vk::DispatchLoaderDynamic& dispatch
);

// ---- Command Buffers ----

/// Create the given number of command buffers
/// @param device Device to use
/// @param commandPool Command pool to use
/// @param commandBufferCount Number of command buffers to create
/// @param commandBuffers Output created command buffers
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateCommandBuffers(
    const vk::Device& device,
    const vk::CommandPool& commandPool,
    uint32_t commandBufferCount,
    std::vector<vk::CommandBuffer>& commandBuffers,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Destroy the given number of command buffers
/// @param device Device to use
/// @param commandPool Command pool that was used to create the command buffers
/// @param commandBuffers Command buffers to destroy
/// @param dispatch Dispatch to use
void axrDestroyCommandBuffers(
    const vk::Device& device,
    const vk::CommandPool& commandPool,
    std::vector<vk::CommandBuffer>& commandBuffers,
    const vk::DispatchLoaderDynamic& dispatch
);

// ---- Sync Objects ----

/// Create the given number of semaphores
/// @param device Device to use
/// @param semaphoreCount Number of semaphores to create
/// @param semaphores Output created semaphores
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateSemaphores(
    const vk::Device& device,
    uint32_t semaphoreCount,
    std::vector<vk::Semaphore>& semaphores,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Destroy the given semaphores
/// @param device Device to use
/// @param semaphores Semaphores to destroy
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
void axrDestroySemaphores(
    const vk::Device& device,
    std::vector<vk::Semaphore>& semaphores,
    const vk::DispatchLoaderDynamic& dispatch
);

/// Create the given number of fences
/// @param device Device to use
/// @param fenceCount Number of fences to create
/// @param fences Output created fences
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrCreateFences(
    const vk::Device& device,
    uint32_t fenceCount,
    std::vector<vk::Fence>& fences,
    const vk::DispatchLoaderDynamic& dispatch
);
/// Destroy the given number of fences
/// @param device Device to use
/// @param fences Fences to destroy
/// @param dispatch Dispatch to use
/// @returns AXR_SUCCESS if the function succeeded
void axrDestroyFences(
    const vk::Device& device,
    std::vector<vk::Fence>& fences,
    const vk::DispatchLoaderDynamic& dispatch
);

#endif
