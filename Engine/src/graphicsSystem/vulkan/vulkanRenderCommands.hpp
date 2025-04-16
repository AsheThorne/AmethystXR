#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <array>

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>
#include "vulkanUtils.hpp"

/// Wrapper for recording vulkan render commands
/// @tparam RenderTarget Render target class. Like a window or xr device
template <typename RenderTarget>
class AxrVulkanRenderCommands {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param renderTarget Render target to use
    /// @param device Device to use
    /// @param dispatch Dispatch to use
    AxrVulkanRenderCommands(
        RenderTarget& renderTarget,
        const vk::Device device,
        const vk::DispatchLoaderDynamic& dispatch
    ):
        m_RenderTarget(renderTarget),
        m_Device(device),
        m_Dispatch(dispatch) {
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Wait for the current frame's fence
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult waitForFrameFence() const {
        const vk::Fence fence = m_RenderTarget.getRenderingFence();

        const vk::Result vkResult = m_Device.waitForFences(1, &fence, vk::True, UINT64_MAX, m_Dispatch);
        axrLogVkResult(vkResult, "m_Device.waitForFences");
        if (AXR_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        return AXR_SUCCESS;
    }

    /// Acquire the next image in the swapchain
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult acquireNextSwapchainImage() const {
        return m_RenderTarget.acquireNextSwapchainImage();
    }

    /// Reset the render target's command buffer
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult resetCommandBuffer() const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();

        const vk::Result vkResult = commandBuffer.reset({}, m_Dispatch);
        axrLogVkResult(vkResult, "commandBuffer.reset");
        if (VK_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        return AXR_SUCCESS;
    }

    /// Begin the render target's command buffer
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult beginCommandBuffer() const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();

        constexpr vk::CommandBufferBeginInfo commandBufferBeginInfo(
            {},
            {}
        );

        const vk::Result vkResult = commandBuffer.begin(&commandBufferBeginInfo, m_Dispatch);
        axrLogVkResult(vkResult, "commandBuffer.begin");
        if (VK_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        return AXR_SUCCESS;
    }

    /// End the render target's command buffer
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult endCommandBuffer() const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();

        const vk::Result vkResult = commandBuffer.end(m_Dispatch);
        axrLogVkResult(vkResult, "commandBuffer.end");
        if (VK_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        return AXR_SUCCESS;
    }

    /// Submit the render target's command buffer
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult submitCommandBuffer(const vk::Queue queue) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();
        const std::vector<vk::Semaphore>& waitSemaphores = m_RenderTarget.getRenderingWaitSemaphores();
        const std::vector<vk::PipelineStageFlags>& waitStages = m_RenderTarget.getRenderingWaitStages();
        const std::vector<vk::Semaphore>& signalSemaphores = m_RenderTarget.getRenderingSignalSemaphores();
        const vk::Fence fence = m_RenderTarget.getRenderingFence();

        vk::Result vkResult = m_Device.resetFences(1, &fence, m_Dispatch);
        axrLogVkResult(vkResult, "m_Device.resetFences");
        if (VK_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        const vk::SubmitInfo submitInfo(
            static_cast<uint32_t>(waitSemaphores.size()),
            waitSemaphores.data(),
            waitStages.data(),
            1,
            &commandBuffer,
            static_cast<uint32_t>(signalSemaphores.size()),
            signalSemaphores.data()
        );

        vkResult = queue.submit(1, &submitInfo, fence, m_Dispatch);
        axrLogVkResult(vkResult, "queue.submit");
        if (VK_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        return AXR_SUCCESS;
    }

    /// Add a vkCmdBeginRenderPass command to the render target's command buffer
    void beginRenderPass() const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();
        const vk::RenderPass renderPass = m_RenderTarget.getRenderPass();
        const vk::Framebuffer framebuffer = m_RenderTarget.getFramebuffer();
        const vk::Extent2D swapchainExtent = m_RenderTarget.getSwapchainExtent();
        const vk::ClearColorValue clearColorValue = m_RenderTarget.getClearColorValue();

        const vk::Rect2D renderArea(
            vk::Offset2D(0, 0),
            swapchainExtent
        );

        const std::array clearValues{
            vk::ClearValue(clearColorValue)
        };

        const vk::RenderPassBeginInfo renderPassBeginInfo(
            renderPass,
            framebuffer,
            renderArea,
            static_cast<uint32_t>(clearValues.size()),
            clearValues.data()
        );

        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline, m_Dispatch);
    }

    /// Add a vkCmdEndRenderPass command to the render target's command buffer
    void endRenderPass() const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();

        commandBuffer.endRenderPass(m_Dispatch);
    }

    /// Add a vkCmdSetViewport command to the render target's command buffer
    void setViewport() const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();
        const vk::Extent2D swapchainExtent = m_RenderTarget.getSwapchainExtent();

        const vk::Viewport viewport(
            0.0f,
            0.0f,
            static_cast<float>(swapchainExtent.width),
            static_cast<float>(swapchainExtent.height),
            0.0f,
            1.0f
        );

        commandBuffer.setViewport(0, 1, &viewport, m_Dispatch);
    }

    /// Add a vkCmdSetScissor command to the render target's command buffer
    void setScissor() const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();
        const vk::Extent2D swapchainExtent = m_RenderTarget.getSwapchainExtent();

        const vk::Rect2D scissor(
            vk::Offset2D(0.0f, 0.0f),
            swapchainExtent
        );

        commandBuffer.setScissor(0, 1, &scissor, m_Dispatch);
    }

    /// Add a vkCmdBindPipeline command to the render target's command buffer
    /// @param pipeline Pipeline to bind
    void bindPipeline(const vk::Pipeline pipeline) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline, m_Dispatch);
    }

    /// Add commands to draw the given mesh
    /// @param mesh Mesh to draw
    void draw(const AxrVulkanSceneData::MeshForRendering& mesh) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();

        commandBuffer.bindIndexBuffer(mesh.Buffer, mesh.BufferIndicesOffset, vk::IndexType::eUint32, m_Dispatch);
        commandBuffer.bindVertexBuffers(0, 1, &mesh.Buffer, &mesh.BufferVerticesOffset, m_Dispatch);

        commandBuffer.drawIndexed(mesh.IndexCount, 1, 0, 0, 0, m_Dispatch);
    }

    /// Present the current frame
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult presentFrame() const {
        return m_RenderTarget.presentFrame();
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    RenderTarget& m_RenderTarget;
    vk::Device m_Device;
    const vk::DispatchLoaderDynamic& m_Dispatch;
};

#endif
