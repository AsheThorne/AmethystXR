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
#include "../../utils.hpp"
#include "../../assets/pushConstantBuffer.hpp"
#include "sceneData/vulkanSceneData.hpp"
#include "../../scene/sceneUtils.hpp"
#include "axr/utils.h"

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

    /// Update all necessary uniform buffers for the current frame
    /// @param sceneData The active scene
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult updateUniformBuffers(AxrVulkanSceneData* sceneData) const {
        AxrResult axrResult = AXR_SUCCESS;

        axrResult = m_RenderTarget.updateSceneDataUniformBuffer(sceneData);
        if (AXR_FAILED(axrResult)) {
            return axrResult;
        }

        return AXR_SUCCESS;
    }

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
            vk::ClearValue(clearColorValue),
            vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0)),
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

    /// Add a vkCmdPushConstants command to the render target's command buffer
    /// @param pipelineLayout Pipeline layout to use
    /// @param pushConstant Push constant to use
    /// @param sceneData Scene data to search for the push constant data in
    void pushConstants(
        const vk::PipelineLayout& pipelineLayout,
        const AxrVulkanSceneData::PushConstantForRendering& pushConstant,
        const AxrVulkanSceneData* sceneData
    ) const {
        if (axrStringIsEmpty(pushConstant.BufferName)) return;

        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();

        // ---- Set Model Matrix Push Constant Buffer ----

        if (strcmp(
            axrEngineAssetGetPushConstantBufferName(AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_MODEL_MATRIX),
            pushConstant.BufferName
        ) == 0) {
            const auto engineAssetData = AxrEngineAssetPushConstantBuffer_ModelMatrix{
                .ModelMatrix = axrTransformGetMatrix(*pushConstant.TransformComponent)
            };

            commandBuffer.pushConstants(
                pipelineLayout,
                *pushConstant.ShaderStages,
                0,
                sizeof(engineAssetData),
                &engineAssetData,
                m_Dispatch
            );
            return;
        }

        // ---- Set User Defined Push Constant Buffer ----

        const AxrPushConstantBuffer* foundBuffer = sceneData->findPushConstantBuffer_shared(
            pushConstant.BufferName
        );
        if (foundBuffer == nullptr) {
            axrLogErrorLocation("Failed to find push constant buffer named: {0}.", pushConstant.BufferName);
            return;
        }

        commandBuffer.pushConstants(
            pipelineLayout,
            *pushConstant.ShaderStages,
            0,
            foundBuffer->getSize(),
            foundBuffer->getData(),
            m_Dispatch
        );
    }

    /// Add a vkCmdBindDescriptorSets command to the render target's command buffer
    /// @param pipelineLayout Pipeline layout to use
    /// @param descriptorSets Descriptor sets to use
    void bindDescriptorSets(
        const vk::PipelineLayout& pipelineLayout,
        const std::vector<vk::DescriptorSet>& descriptorSets
    ) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer();
        const uint32_t currentFrame = m_RenderTarget.getCurrentRenderingFrame();

        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            pipelineLayout,
            0,
            1,
            &descriptorSets[currentFrame],
            0,
            nullptr,
            m_Dispatch
        );
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
