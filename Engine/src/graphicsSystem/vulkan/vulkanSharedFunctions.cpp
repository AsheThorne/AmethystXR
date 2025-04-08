#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanSharedFunctions.hpp"
#include "axr/common/defines.h"
#include "axr/logger.h"
#include "vulkanUtils.hpp"

AxrResult axrCreateImageViews(
    const vk::Device& device,
    const std::vector<vk::Image>& images,
    const vk::Format format,
    const vk::ImageAspectFlags aspectMask,
    const uint32_t mipLevels,
    std::vector<vk::ImageView>& imageViews,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!imageViews.empty()) {
        axrLogErrorLocation("Image views already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (images.empty()) {
        axrLogErrorLocation("Images are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;
    imageViews.resize(images.size());

    for (size_t i = 0; i < imageViews.size(); ++i) {
        axrResult = axrCreateImageView(
            device,
            images[i],
            format,
            aspectMask,
            mipLevels,
            imageViews[i],
            dispatch
        );
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrDestroyImageViews(device, imageViews, dispatch);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void axrDestroyImageViews(
    const vk::Device& device,
    std::vector<vk::ImageView>& imageViews,
    const vk::DispatchLoaderDynamic& dispatch
) {
    for (vk::ImageView& imageView : imageViews) {
        axrDestroyImageView(device, imageView, dispatch);
    }
    imageViews.clear();
}

AxrResult axrCreateImageView(
    const vk::Device& device,
    const vk::Image& image,
    const vk::Format format,
    const vk::ImageAspectFlags aspectMask,
    const uint32_t mipLevels,
    vk::ImageView& imageView,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (imageView != VK_NULL_HANDLE) {
        axrLogErrorLocation("Image view already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (image == VK_NULL_HANDLE) {
        axrLogErrorLocation("Image is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::ImageViewCreateInfo imageViewCreateInfo(
        {},
        image,
        vk::ImageViewType::e2D,
        format,
        vk::ComponentMapping(),
        vk::ImageSubresourceRange{
            aspectMask,
            0,
            mipLevels,
            0,
            1
        }
    );

    const auto imageViewResult = device.createImageView(imageViewCreateInfo, nullptr, dispatch);
    axrLogVkResult(imageViewResult.result, "device.createImageView");
    if (VK_FAILED(imageViewResult.result)) {
        return AXR_ERROR;
    }

    imageView = imageViewResult.value;

    return AXR_SUCCESS;
}

void axrDestroyImageView(
    const vk::Device& device,
    vk::ImageView& imageView,
    const vk::DispatchLoaderDynamic& dispatch
) {
    if (imageView == VK_NULL_HANDLE) return;

    device.destroyImageView(imageView, nullptr, dispatch);
    imageView = VK_NULL_HANDLE;
}

AxrResult axrCreateRenderPass(
    const vk::Device& device,
    const vk::Format colorFormat,
    const vk::Format depthStencilFormat,
    const vk::ImageLayout finalImageLayout,
    vk::RenderPass& renderPass,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (renderPass != VK_NULL_HANDLE) {
        axrLogErrorLocation("Render pass already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::AttachmentDescription colorAttachment(
        {},
        colorFormat,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        finalImageLayout
    );

    // TODO: Implement depth buffer
    // const vk::AttachmentDescription depthStencilAttachment(
    //     {},
    //     depthStencilFormat,
    //     vk::SampleCountFlagBits::e1,
    //     vk::AttachmentLoadOp::eClear,
    //     vk::AttachmentStoreOp::eDontCare,
    //     vk::AttachmentLoadOp::eDontCare,
    //     vk::AttachmentStoreOp::eDontCare,
    //     vk::ImageLayout::eUndefined,
    //     vk::ImageLayout::eDepthStencilAttachmentOptimal
    // );

    const std::array attachments{
        colorAttachment,
        // TODO: Implement depth buffer
        // depthStencilAttachment,
    };

    constexpr vk::AttachmentReference colorAttachmentRef(
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );

    // TODO: Implement depth buffer
    // constexpr vk::AttachmentReference depthStencilAttachmentRef(
    //     1,
    //     vk::ImageLayout::eDepthStencilAttachmentOptimal
    // );

    const vk::SubpassDescription subpass(
        {},
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        &colorAttachmentRef,
        nullptr
        // TODO: Implement depth buffer
        // &depthStencilAttachmentRef
    );

    constexpr vk::SubpassDependency dependency(
        vk::SubpassExternal,
        0,
        vk::PipelineStageFlagBits::eColorAttachmentOutput |
        vk::PipelineStageFlagBits::eEarlyFragmentTests,
        vk::PipelineStageFlagBits::eColorAttachmentOutput |
        vk::PipelineStageFlagBits::eEarlyFragmentTests,
        {},
        vk::AccessFlagBits::eColorAttachmentWrite |
        vk::AccessFlagBits::eDepthStencilAttachmentWrite
    );

    const vk::RenderPassCreateInfo renderPassCreateInfo(
        {},
        static_cast<uint32_t>(attachments.size()),
        attachments.data(),
        1,
        &subpass,
        1,
        &dependency
    );

    const vk::Result vkResult = device.createRenderPass(
        &renderPassCreateInfo,
        nullptr,
        &renderPass,
        dispatch
    );
    axrLogVkResult(vkResult, "device.createRenderPass");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void axrDestroyRenderPass(
    const vk::Device& device,
    vk::RenderPass& renderPass,
    const vk::DispatchLoaderDynamic& dispatch
) {
    if (renderPass == VK_NULL_HANDLE) return;

    device.destroyRenderPass(renderPass, nullptr, dispatch);
    renderPass = VK_NULL_HANDLE;
}

[[nodiscard]] AxrResult axrCreateFramebuffers(
    const vk::Device& device,
    const vk::RenderPass& renderPass,
    const vk::Extent2D& swapchainExtent,
    const std::vector<vk::ImageView>& swapchainColorImageViews,
    std::vector<vk::Framebuffer>& framebuffers,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!framebuffers.empty()) {
        axrLogErrorLocation("Framebuffers already exist.");
        return AXR_ERROR;
    }

    if (swapchainColorImageViews.empty()) {
        axrLogErrorLocation("Swapchain color image views don't exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;
    framebuffers.resize(swapchainColorImageViews.size());

    for (size_t i = 0; i < swapchainColorImageViews.size(); ++i) {
        axrResult = axrCreateFramebuffer(
            device,
            renderPass,
            swapchainExtent,
            swapchainColorImageViews[i],
            framebuffers[i],
            dispatch
        );
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create swapchain framebuffers.");
        axrDestroyFramebuffers(device, framebuffers, dispatch);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void axrDestroyFramebuffers(
    const vk::Device& device,
    std::vector<vk::Framebuffer>& framebuffers,
    const vk::DispatchLoaderDynamic& dispatch
) {
    for (auto& framebuffer : framebuffers) {
        axrDestroyFramebuffer(device, framebuffer, dispatch);
    }
    framebuffers.clear();
}

AxrResult axrCreateFramebuffer(
    const vk::Device& device,
    const vk::RenderPass& renderPass,
    const vk::Extent2D& swapchainExtent,
    const vk::ImageView& swapchainColorImageView,
    vk::Framebuffer& framebuffer,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (framebuffer != VK_NULL_HANDLE) {
        axrLogErrorLocation("Framebuffer already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (renderPass == VK_NULL_HANDLE) {
        axrLogErrorLocation("Render pass is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    std::array attachments{
        swapchainColorImageView,
    };

    const vk::FramebufferCreateInfo framebufferCreateInfo(
        {},
        renderPass,
        static_cast<uint32_t>(attachments.size()),
        attachments.data(),
        swapchainExtent.width,
        swapchainExtent.height,
        1
    );

    const vk::Result vkResult = device.createFramebuffer(
        &framebufferCreateInfo,
        nullptr,
        &framebuffer,
        dispatch
    );
    axrLogVkResult(vkResult, "device.createFramebuffer");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void axrDestroyFramebuffer(
    const vk::Device& device,
    vk::Framebuffer& framebuffer,
    const vk::DispatchLoaderDynamic& dispatch
) {
    if (framebuffer == VK_NULL_HANDLE) return;

    device.destroyFramebuffer(framebuffer, nullptr, dispatch);
    framebuffer = VK_NULL_HANDLE;
}

#endif
