#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanSharedFunctions.hpp"
#include "axr/common/defines.h"
#include "axr/logger.h"
#include "vulkanUtils.hpp"

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

    const vk::AttachmentDescription depthStencilAttachment(
        {},
        depthStencilFormat,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eDontCare,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eDepthStencilAttachmentOptimal
    );

    const std::array attachments{
        colorAttachment,
        depthStencilAttachment,
    };

    constexpr vk::AttachmentReference colorAttachmentRef(
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );

    constexpr vk::AttachmentReference depthStencilAttachmentRef(
        1,
        vk::ImageLayout::eDepthStencilAttachmentOptimal
    );

    const vk::SubpassDescription subpass(
        {},
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        &colorAttachmentRef,
        nullptr,
        &depthStencilAttachmentRef,
        0,
        nullptr
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
    const std::vector<vk::ImageView>& swapchainDepthImageViews,
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

    if (swapchainDepthImageViews.empty()) {
        axrLogErrorLocation("Swapchain depth image views don't exist.");
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
            swapchainDepthImageViews[i],
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
    const vk::ImageView& swapchainDepthImageView,
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

    const std::array attachments{
        swapchainColorImageView,
        swapchainDepthImageView
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

AxrResult axrCreateCommandBuffers(
    const vk::Device& device,
    const vk::CommandPool& commandPool,
    const uint32_t commandBufferCount,
    std::vector<vk::CommandBuffer>& commandBuffers,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!commandBuffers.empty()) {
        axrLogErrorLocation("Command buffers already exist.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (commandPool == VK_NULL_HANDLE) {
        axrLogErrorLocation("Command pool is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    commandBuffers.resize(commandBufferCount);

    const vk::CommandBufferAllocateInfo commandBufferAllocateInfo(
        commandPool,
        vk::CommandBufferLevel::ePrimary,
        commandBufferCount
    );

    const vk::Result vkResult = device.allocateCommandBuffers(
        &commandBufferAllocateInfo,
        commandBuffers.data(),
        dispatch
    );
    axrLogVkResult(vkResult, "device.allocateCommandBuffers");
    if (VK_FAILED(vkResult)) {
        axrDestroyCommandBuffers(device, commandPool, commandBuffers, dispatch);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void axrDestroyCommandBuffers(
    const vk::Device& device,
    const vk::CommandPool& commandPool,
    std::vector<vk::CommandBuffer>& commandBuffers,
    const vk::DispatchLoaderDynamic& dispatch
) {
    if (commandBuffers.empty()) return;

    device.freeCommandBuffers(
        commandPool,
        static_cast<uint32_t>(commandBuffers.size()),
        commandBuffers.data(),
        dispatch
    );
    commandBuffers.clear();
}

AxrResult axrCreateSemaphores(
    const vk::Device& device,
    const uint32_t semaphoreCount,
    std::vector<vk::Semaphore>& semaphores,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!semaphores.empty()) {
        axrLogErrorLocation("Semaphores already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    vk::Result vkResult = vk::Result::eSuccess;
    semaphores.resize(semaphoreCount, VK_NULL_HANDLE);

    constexpr vk::SemaphoreCreateInfo semaphoreCreateInfo(
        {},
        nullptr
    );

    for (uint32_t i = 0; i < semaphoreCount; ++i) {
        vkResult = device.createSemaphore(&semaphoreCreateInfo, nullptr, &semaphores[i], dispatch);
        axrLogVkResult(vkResult, "device.createSemaphore");
        if (VK_FAILED(vkResult)) {
            break;
        }
    }

    if (VK_FAILED(vkResult)) {
        axrLogErrorLocation("Failed to create semaphores.");
        axrDestroySemaphores(device, semaphores, dispatch);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void axrDestroySemaphores(
    const vk::Device& device,
    std::vector<vk::Semaphore>& semaphores,
    const vk::DispatchLoaderDynamic& dispatch
) {
    for (vk::Semaphore& semaphore : semaphores) {
        device.destroySemaphore(semaphore, nullptr, dispatch);
        semaphore = VK_NULL_HANDLE;
    }
    semaphores.clear();
}

AxrResult axrCreateFences(
    const vk::Device& device,
    const uint32_t fenceCount,
    std::vector<vk::Fence>& fences,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!fences.empty()) {
        axrLogErrorLocation("Fences already exists.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    vk::Result vkResult = vk::Result::eSuccess;
    fences.resize(fenceCount, VK_NULL_HANDLE);

    constexpr vk::FenceCreateInfo fenceCreateInfo(
        vk::FenceCreateFlagBits::eSignaled,
        nullptr
    );

    for (uint32_t i = 0; i < fenceCount; ++i) {
        vkResult = device.createFence(&fenceCreateInfo, nullptr, &fences[i], dispatch);
        axrLogVkResult(vkResult, "device.createFence");
        if (VK_FAILED(vkResult)) {
            break;
        }
    }

    if (VK_FAILED(vkResult)) {
        axrLogErrorLocation("Failed to create fences.");
        axrDestroyFences(device, fences, dispatch);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void axrDestroyFences(
    const vk::Device& device,
    std::vector<vk::Fence>& fences,
    const vk::DispatchLoaderDynamic& dispatch
) {
    for (vk::Fence& fence : fences) {
        device.destroyFence(fence, nullptr, dispatch);
        fence = VK_NULL_HANDLE;
    }
    fences.clear();
}

#endif
