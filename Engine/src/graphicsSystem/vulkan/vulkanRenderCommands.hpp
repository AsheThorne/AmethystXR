﻿#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <array>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanUtils.hpp"
#include "../../utils.hpp"
#include "../../assets/pushConstantBuffer.hpp"
#include "sceneData/vulkanSceneData.hpp"
#include "../../scene/sceneUtils.hpp"
#include "axr/common/utils.h"
#include "../../assets/engineAssets.hpp"
#include "vulkanRenderStructs.hpp"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Render command pipelines
struct AxrVulkanRenderCommandPipelines {
    vk::Pipeline WindowPipeline = VK_NULL_HANDLE;
    vk::Pipeline XrSessionPipeline = VK_NULL_HANDLE;
};

constexpr bool operator==(
    const AxrVulkanRenderCommandPipelines& first,
    const AxrVulkanRenderCommandPipelines& second
) noexcept {
    return first.WindowPipeline == second.WindowPipeline &&
        first.XrSessionPipeline == second.XrSessionPipeline;
}

/// Render command descriptor sets
struct AxrVulkanRenderCommandDescriptorSets {
    const std::vector<vk::DescriptorSet>& WindowDescriptorSets;
    const std::vector<vk::DescriptorSet>& XrSessionDescriptorSets;
};

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
    /// @param physicalDevice Physical device to use
    /// @param device Device to use
    /// @param dispatch Dispatch to use
    AxrVulkanRenderCommands(
        RenderTarget& renderTarget,
        const vk::PhysicalDevice physicalDevice,
        const vk::Device device,
        const vk::DispatchLoaderDynamic& dispatch
    ):
        m_RenderTarget(renderTarget),
        m_PhysicalDevice(physicalDevice),
        m_Device(device),
        m_Dispatch(dispatch) {
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Signal the render target that we're starting the render
    /// @param sceneData The active scene
    /// @returns AXR_SUCCESS if the function succeeded
    /// @returns AXR_DONT_RENDER if we should skip rendering this frame.
    [[nodiscard]] AxrResult beginRendering(const AxrVulkanSceneData* sceneData) const {
        return m_RenderTarget.beginRendering(sceneData);
    }

    /// Get the number of views for the render target
    /// @returns The number of views for the render target
    [[nodiscard]] uint32_t getViewCount() const {
        return m_RenderTarget.getViewCount();
    }

    /// Get the platform type for the render target
    /// @returns the platform type for the render target
    [[nodiscard]] AxrPlatformType getPlatformType() const {
        return m_RenderTarget.getPlatformType();
    }

    /// Get the UI region
    /// @returns The UI region
    [[nodiscard]] vk::Extent2D getUIRegion() const {
        return m_RenderTarget.getUIRegion();
    }

    /// Update all necessary uniform buffers for the current frame
    /// @param viewIndex The view index
    /// @param sceneData The active scene
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult updateUniformBuffers(const uint32_t viewIndex, const AxrVulkanSceneData* sceneData) const {
        AxrResult axrResult = AXR_SUCCESS;
        const uint32_t currentFrame = m_RenderTarget.getCurrentRenderingFrame();
        const AxrPlatformType platformType = m_RenderTarget.getPlatformType();

        glm::mat4 viewMatrix;
        float nearPlane;
        float farPlane;
        axrResult = m_RenderTarget.getCameraData(
            viewIndex,
            viewMatrix,
            nearPlane,
            farPlane
        );
        if (AXR_FAILED(axrResult)) {
            axrLogErrorLocation("Failed to get camera data.");
            return axrResult;
        }

        AxrEngineAssetUniformBuffer_SceneData sceneDataUniformBuffer{};
        sceneDataUniformBuffer.CameraNearPlane = nearPlane;
        sceneDataUniformBuffer.CameraFarPlane = farPlane;

        m_RenderTarget.getRenderingMatrices(
            viewIndex,
            sceneDataUniformBuffer.ViewMatrix,
            sceneDataUniformBuffer.ProjectionMatrix
        );
        sceneDataUniformBuffer.ViewProjectionMatrix =
            sceneDataUniformBuffer.ProjectionMatrix * sceneDataUniformBuffer.ViewMatrix;

        axrResult = sceneData->setPlatformUniformBufferData(
            platformType,
            axrEngineAssetGetUniformBufferName(AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA),
            currentFrame,
            viewIndex,
            0,
            sizeof(sceneDataUniformBuffer),
            &sceneDataUniformBuffer
        );
        if (AXR_FAILED(axrResult)) {
            axrLogErrorLocation("Failed to set engine asset uniform buffer scene data.");
            return axrResult;
        }

        return AXR_SUCCESS;
    }

    /// Get the camera data for the given view
    /// @param viewIndex View index
    /// @param viewMatrix Output view matrix
    /// @param nearPlane Output near plane
    /// @param farPlane Output far plane
    [[nodiscard]] AxrResult getCameraData(
        const uint32_t viewIndex,
        glm::mat4& viewMatrix,
        float& nearPlane,
        float& farPlane
    ) const {
        return m_RenderTarget.getCameraData(
            viewIndex,
            viewMatrix,
            nearPlane,
            farPlane
        );
    }

    /// Wait for the current frame's fence
    /// @param viewIndex The view index
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult waitForFrameFence(const uint32_t viewIndex) const {
        const vk::Fence fence = m_RenderTarget.getRenderingFence(viewIndex);
        if (fence == VK_NULL_HANDLE) return AXR_SUCCESS;

        const vk::Result vkResult = m_Device.waitForFences(1, &fence, vk::True, UINT64_MAX, m_Dispatch);
        axrLogVkResult(vkResult, "m_Device.waitForFences");
        if (AXR_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        return AXR_SUCCESS;
    }

    /// Acquire the next image in the swapchain
    /// @param viewIndex The view index
    /// @returns AXR_SUCCESS if the function succeeded.
    /// @returns AXR_DONT_RENDER if we should skip rendering this frame.
    [[nodiscard]] AxrResult acquireNextSwapchainImage(const uint32_t viewIndex) const {
        return m_RenderTarget.acquireNextSwapchainImage(viewIndex);
    }

    /// Reset the render target's command buffer
    /// @param viewIndex The view index
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult resetCommandBuffer(const uint32_t viewIndex) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return AXR_ERROR;

        const vk::Result vkResult = commandBuffer.reset({}, m_Dispatch);
        axrLogVkResult(vkResult, "commandBuffer.reset");
        if (VK_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        return AXR_SUCCESS;
    }

    /// Begin the render target's command buffer
    /// @param viewIndex The view index
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult beginCommandBuffer(const uint32_t viewIndex) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return AXR_ERROR;

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
    /// @param viewIndex The view index
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult endCommandBuffer(const uint32_t viewIndex) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return AXR_ERROR;

        const vk::Result vkResult = commandBuffer.end(m_Dispatch);
        axrLogVkResult(vkResult, "commandBuffer.end");
        if (VK_FAILED(vkResult)) {
            return AXR_ERROR;
        }

        return AXR_SUCCESS;
    }

    /// Submit the render target's command buffer
    /// @param viewIndex The view index
    /// @param queue Queue to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult submitCommandBuffer(const uint32_t viewIndex, const vk::Queue queue) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return AXR_ERROR;

        const std::vector<vk::Semaphore>& waitSemaphores = m_RenderTarget.getRenderingWaitSemaphores(viewIndex);
        const std::vector<vk::PipelineStageFlags>& waitStages = m_RenderTarget.getRenderingWaitStages(viewIndex);
        const std::vector<vk::Semaphore>& signalSemaphores = m_RenderTarget.getRenderingSignalSemaphores(viewIndex);
        const vk::Fence fence = m_RenderTarget.getRenderingFence(viewIndex);
        vk::Result vkResult;

        if (fence != VK_NULL_HANDLE) {
            vkResult = m_Device.resetFences(1, &fence, m_Dispatch);
            axrLogVkResult(vkResult, "m_Device.resetFences");
            if (VK_FAILED(vkResult)) {
                return AXR_ERROR;
            }
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
    /// @param viewIndex The view index
    void beginRenderPass(const uint32_t viewIndex) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;

        const vk::RenderPass renderPass = m_RenderTarget.getRenderPass();
        const vk::Framebuffer framebuffer = m_RenderTarget.getFramebuffer(viewIndex);
        if (framebuffer == VK_NULL_HANDLE) return;

        const vk::Extent2D swapchainExtent = m_RenderTarget.getSwapchainExtent(viewIndex);
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
    /// @param viewIndex The view index
    void endRenderPass(const uint32_t viewIndex) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;

        commandBuffer.endRenderPass(m_Dispatch);
    }

    /// Add a vkCmdSetViewport command to the render target's command buffer
    /// @param viewIndex The view index
    void setViewport(const uint32_t viewIndex) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;

        const vk::Extent2D swapchainExtent = m_RenderTarget.getSwapchainExtent(viewIndex);

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
    /// @param viewIndex The view index
    void setScissor(const uint32_t viewIndex) const {
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;

        const vk::Extent2D swapchainExtent = m_RenderTarget.getSwapchainExtent(viewIndex);

        const vk::Rect2D scissor(
            vk::Offset2D(0.0f, 0.0f),
            swapchainExtent
        );

        commandBuffer.setScissor(0, 1, &scissor, m_Dispatch);
    }

    /// Add a vkCmdBindPipeline command to the render target's command buffer
    /// @param viewIndex The view index
    /// @param pipelines Pipeline options
    void bindPipeline(const uint32_t viewIndex, const AxrVulkanRenderCommandPipelines& pipelines) const {
        const AxrPlatformType platformType = m_RenderTarget.getPlatformType();
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;

        vk::Pipeline pipeline = VK_NULL_HANDLE;

        switch (platformType) {
            case AXR_PLATFORM_TYPE_WINDOW: {
                pipeline = pipelines.WindowPipeline;
                break;
            }
            case AXR_PLATFORM_TYPE_XR_DEVICE: {
                pipeline = pipelines.XrSessionPipeline;
                break;
            }
            case AXR_PLATFORM_TYPE_UNDEFINED:
            default: {
                axrLogErrorLocation("Unknown platform type.");
                return;
            }
        }

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline, m_Dispatch);
    }

    /// Add a vkCmdPushConstants command to the render target's command buffer
    /// @param viewIndex The view index
    /// @param pipelineLayout Pipeline layout to use
    /// @param stageFlags Push constant shader stage flags
    /// @param bufferName Push constant buffer name
    /// @param transformComponent Mesh transform component
    /// @param sceneData Scene data to search for the push constant data in
    void pushConstants(
        const uint32_t viewIndex,
        const vk::PipelineLayout& pipelineLayout,
        const vk::ShaderStageFlags stageFlags,
        const char* bufferName,
        const AxrTransformComponent* transformComponent,
        const AxrVulkanSceneData* sceneData
    ) const {
        if (axrStringIsEmpty(bufferName)) return;

        if (stageFlags == static_cast<vk::ShaderStageFlags>(VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM)) {
            axrLogErrorLocation("Shader stages are null.");
            return;
        }

        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;

        // ---- Set Model Matrix Push Constant Buffer ----

        if (strcmp(
            axrEngineAssetGetPushConstantBufferName(AXR_ENGINE_ASSET_PUSH_CONSTANT_BUFFER_MODEL_MATRIX),
            bufferName
        ) == 0) {
            if (transformComponent == nullptr) {
                axrLogErrorLocation("Unable to use model matrix push constant. Transform component is null.");
                return;
            }

            const auto engineAssetData = AxrEngineAssetPushConstantBuffer_ModelMatrix{
                .ModelMatrix = axrTransformGetMatrix(*transformComponent)
            };

            commandBuffer.pushConstants(
                pipelineLayout,
                stageFlags,
                0,
                sizeof(engineAssetData),
                &engineAssetData,
                m_Dispatch
            );
            return;
        }

        // ---- Set User Defined Push Constant Buffer ----

        const AxrPushConstantBuffer* foundBuffer = sceneData->findPushConstantBuffer_shared(bufferName);
        if (foundBuffer == nullptr) {
            axrLogErrorLocation("Failed to find push constant buffer named: {0}.", bufferName);
            return;
        }

        commandBuffer.pushConstants(
            pipelineLayout,
            stageFlags,
            0,
            foundBuffer->getSize(),
            foundBuffer->getData(),
            m_Dispatch
        );
    }

    /// Add a vkCmdBindDescriptorSets command to the render target's command buffer
    /// @param viewIndex The view index
    /// @param pipelineLayout Pipeline layout to use
    /// @param descriptorSets Descriptor sets to use
    /// @param dynamicOffsets Dynamic offsets to use
    void bindDescriptorSets(
        const uint32_t viewIndex,
        const vk::PipelineLayout& pipelineLayout,
        const AxrVulkanRenderCommandDescriptorSets& descriptorSets,
        const std::vector<uint32_t>& dynamicOffsets
    ) const {
        const AxrPlatformType platformType = m_RenderTarget.getPlatformType();
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;

        const uint32_t currentFrame = m_RenderTarget.getCurrentRenderingFrame();

        vk::DescriptorSet descriptorSet = VK_NULL_HANDLE;

        switch (platformType) {
            case AXR_PLATFORM_TYPE_WINDOW: {
                descriptorSet = descriptorSets.WindowDescriptorSets[currentFrame];
                break;
            }
            case AXR_PLATFORM_TYPE_XR_DEVICE: {
                const uint32_t framesInFlight = descriptorSets.XrSessionDescriptorSets.size() / m_RenderTarget.
                    getViewCount();
                const uint32_t viewIndexOffset = framesInFlight * viewIndex;
                descriptorSet = descriptorSets.XrSessionDescriptorSets[viewIndexOffset + currentFrame];
                break;
            }
            case AXR_PLATFORM_TYPE_UNDEFINED:
            default: {
                axrLogErrorLocation("Unknown platform type.");
                return;
            }
        }

        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            pipelineLayout,
            0,
            1,
            &descriptorSet,
            static_cast<uint32_t>(dynamicOffsets.size()),
            dynamicOffsets.data(),
            m_Dispatch
        );
    }

    /// Add commands to draw the given mesh
    /// @param viewIndex The view index
    /// @param mesh Mesh to draw
    void draw(const uint32_t viewIndex, const AxrVulkanMeshForRendering& mesh) const {
        if (mesh.Buffer == nullptr ||
            mesh.BufferIndicesOffset == nullptr ||
            mesh.BufferVerticesOffset == nullptr ||
            mesh.IndexCount == nullptr
        ) {
            axrLogErrorLocation("Mesh for rendering is incomplete.");
            return;
        }

        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(viewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;

        commandBuffer.bindIndexBuffer(*mesh.Buffer, *mesh.BufferIndicesOffset, vk::IndexType::eUint32, m_Dispatch);
        commandBuffer.bindVertexBuffers(0, 1, mesh.Buffer, mesh.BufferVerticesOffset, m_Dispatch);

        commandBuffer.drawIndexed(*mesh.IndexCount, 1, 0, 0, 0, m_Dispatch);
    }

    /// Add commands to blit from the given xr device render source to the render target
    /// @tparam RenderSource Source render target class. Like a window or xr device
    /// @param windowRenderSource The window render source
    /// @param renderSource Source render target to use
    template <typename RenderSource>
    void blitFromXrDevice(
        const AxrWindowRenderSourceEnum windowRenderSource,
        RenderSource& renderSource
    ) const {
        constexpr uint32_t windowViewIndex = 0;
        const vk::CommandBuffer commandBuffer = m_RenderTarget.getRenderingCommandBuffer(windowViewIndex);
        if (commandBuffer == VK_NULL_HANDLE) return;
        const vk::Image dstImage = m_RenderTarget.getSwapchainImage(windowViewIndex);
        if (dstImage == VK_NULL_HANDLE) return;

        std::vector<std::pair<vk::Image, vk::Extent2D>> srcImages;
        switch (windowRenderSource) {
            case AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_LEFT_EYE: {
                const vk::Extent2D extent = renderSource.getSwapchainExtent(0);
                const vk::Image image = renderSource.getSwapchainImage(0);
                if (image == VK_NULL_HANDLE) {
                    axrLogErrorLocation("Xr device left eye swapchain image is null.");
                    return;
                }

                srcImages.emplace_back(image, extent);
                break;
            }
            case AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_RIGHT_EYE: {
                const vk::Extent2D extent = renderSource.getSwapchainExtent(1);
                const vk::Image image = renderSource.getSwapchainImage(1);
                if (image == VK_NULL_HANDLE) {
                    axrLogErrorLocation("Xr device right eye swapchain image is null.");
                    return;
                }

                srcImages.emplace_back(image, extent);
                break;
            }
            case AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_BOTH_EYES: {
                const vk::Extent2D extent1 = renderSource.getSwapchainExtent(0);
                const vk::Image image1 = renderSource.getSwapchainImage(0);
                if (image1 == VK_NULL_HANDLE) {
                    axrLogErrorLocation("Xr device left eye swapchain image is null.");
                    return;
                }

                const vk::Extent2D extent2 = renderSource.getSwapchainExtent(1);
                const vk::Image image2 = renderSource.getSwapchainImage(1);
                if (image2 == VK_NULL_HANDLE) {
                    axrLogErrorLocation("Xr device right eye swapchain image is null.");
                    return;
                }

                srcImages.emplace_back(image1, extent1);
                srcImages.emplace_back(image2, extent2);
                break;
            }
            case AXR_WINDOW_RENDER_SOURCE_SCENE_MAIN_CAMERA: {
                axrLogErrorLocation("Window render source cannot be the main camera for blitting.");
                return;
            }
            default: {
                axrLogErrorLocation("Unknown window render source type.");
                return;
            }
        }

        const vk::Format srcImageFormat = renderSource.getSwapchainImageFormat();
        constexpr vk::ImageLayout srcFinalImageLayout = vk::ImageLayout::eTransferSrcOptimal;
        const vk::ImageLayout dstFinalImageLayout = m_RenderTarget.getSwapchainImageLayout();
        const vk::Extent2D dstImageExtent = m_RenderTarget.getSwapchainExtent(windowViewIndex);
        vk::ImageMemoryBarrier imageMemoryBarrier;

        for (auto& srcImage : srcImages | std::views::keys) {
            imageMemoryBarrier = vk::ImageMemoryBarrier(
                vk::AccessFlagBits::eNone,
                vk::AccessFlagBits::eTransferRead,
                vk::ImageLayout::eUndefined,
                vk::ImageLayout::eTransferSrcOptimal,
                vk::QueueFamilyIgnored,
                vk::QueueFamilyIgnored,
                srcImage,
                vk::ImageSubresourceRange(
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    0,
                    1
                )
            );

            commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eTransfer,
                static_cast<vk::DependencyFlags>(0),
                0,
                nullptr,
                0,
                nullptr,
                1,
                &imageMemoryBarrier,
                m_Dispatch
            );
        }

        imageMemoryBarrier = vk::ImageMemoryBarrier(
            vk::AccessFlagBits::eNone,
            vk::AccessFlagBits::eTransferWrite,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferDstOptimal,
            vk::QueueFamilyIgnored,
            vk::QueueFamilyIgnored,
            dstImage,
            vk::ImageSubresourceRange(
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            )
        );

        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            static_cast<vk::DependencyFlags>(0),
            0,
            nullptr,
            0,
            nullptr,
            1,
            &imageMemoryBarrier,
            m_Dispatch
        );

        std::array clearColorRanges{
            vk::ImageSubresourceRange(
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            )
        };

        commandBuffer.clearColorImage(
            dstImage,
            vk::ImageLayout::eTransferDstOptimal,
            vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f),
            clearColorRanges,
            m_Dispatch
        );

        auto blitFilter = vk::Filter::eLinear;

        // If linear filter isn't supported, use nearest
        if (!axrAreFormatFeaturesSupported(
            srcImageFormat,
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eBlitSrc | vk::FormatFeatureFlagBits::eSampledImageFilterLinear,
            m_PhysicalDevice,
            m_Dispatch
        )) {
            blitFilter = vk::Filter::eNearest;
        }

        for (int32_t currentIndex = 0; auto [srcImage, srcImageExtent] : srcImages) {
            std::array srcOffsets{
                vk::Offset3D(0, 0, 0),
                vk::Offset3D(
                    static_cast<int32_t>(srcImageExtent.width),
                    static_cast<int32_t>(srcImageExtent.height),
                    1
                ),
            };
            std::array dstOffsets{
                vk::Offset3D(0, 0, 0),
                vk::Offset3D(
                    static_cast<int32_t>(dstImageExtent.width),
                    static_cast<int32_t>(dstImageExtent.height),
                    1
                ),
            };

            if (windowRenderSource == AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_BOTH_EYES) {
                float aspectRatio = std::min(
                    static_cast<float>(dstImageExtent.width) /
                    static_cast<float>(srcImages.size()) /
                    static_cast<float>(srcImageExtent.width),
                    static_cast<float>(dstImageExtent.height) /
                    static_cast<float>(srcImageExtent.height)
                );

                auto srcWidth = static_cast<int32_t>(static_cast<float>(srcImageExtent.width) * aspectRatio);
                auto srcHeight = static_cast<int32_t>(static_cast<float>(srcImageExtent.height) * aspectRatio);

                int32_t xOffset = static_cast<int32_t>(dstImageExtent.width - srcWidth * srcImages.size()) / 2;
                int32_t yOffset = static_cast<int32_t>(dstImageExtent.height - srcHeight) / 2;

                dstOffsets = std::array{
                    vk::Offset3D(xOffset + srcWidth * currentIndex, yOffset, 0),
                    vk::Offset3D(
                        xOffset + srcWidth * (1 + currentIndex),
                        yOffset + srcHeight,
                        1
                    ),
                };
            } else if (windowRenderSource == AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_LEFT_EYE ||
                windowRenderSource == AXR_WINDOW_RENDER_SOURCE_XR_DEVICE_RIGHT_EYE) {
                float aspectRatio = std::min(
                    static_cast<float>(srcImageExtent.width) /
                    static_cast<float>(dstImageExtent.width),
                    static_cast<float>(srcImageExtent.height) /
                    static_cast<float>(dstImageExtent.height)
                );

                auto dstWidth = static_cast<int32_t>(static_cast<float>(dstImageExtent.width) * aspectRatio);
                auto dstHeight = static_cast<int32_t>(static_cast<float>(dstImageExtent.height) * aspectRatio);

                int32_t xOffset = static_cast<int32_t>(srcImageExtent.width - dstWidth) / 2;
                int32_t yOffset = static_cast<int32_t>(srcImageExtent.height - dstHeight) / 2;

                srcOffsets = std::array{
                    vk::Offset3D(xOffset, yOffset, 0),
                    vk::Offset3D(
                        xOffset + dstWidth,
                        yOffset + dstHeight,
                        1
                    ),
                };
            }

            const vk::ImageBlit imageBlit(
                vk::ImageSubresourceLayers(
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    0,
                    1
                ),
                srcOffsets,
                vk::ImageSubresourceLayers(
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    0,
                    1
                ),
                dstOffsets
            );

            commandBuffer.blitImage(
                srcImage,
                vk::ImageLayout::eTransferSrcOptimal,
                dstImage,
                vk::ImageLayout::eTransferDstOptimal,
                1,
                &imageBlit,
                blitFilter,
                m_Dispatch
            );

            currentIndex++;
        }

        if (srcFinalImageLayout != vk::ImageLayout::eTransferSrcOptimal) {
            for (auto srcImage : srcImages | std::views::keys) {
                imageMemoryBarrier = vk::ImageMemoryBarrier(
                    vk::AccessFlagBits::eTransferRead,
                    vk::AccessFlagBits::eNone,
                    vk::ImageLayout::eTransferSrcOptimal,
                    srcFinalImageLayout,
                    vk::QueueFamilyIgnored,
                    vk::QueueFamilyIgnored,
                    srcImage,
                    vk::ImageSubresourceRange(
                        vk::ImageAspectFlagBits::eColor,
                        0,
                        1,
                        0,
                        1
                    )
                );

                commandBuffer.pipelineBarrier(
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eBottomOfPipe,
                    static_cast<vk::DependencyFlags>(0),
                    0,
                    nullptr,
                    0,
                    nullptr,
                    1,
                    &imageMemoryBarrier,
                    m_Dispatch
                );
            }
        }

        if (dstFinalImageLayout != vk::ImageLayout::eTransferDstOptimal) {
            imageMemoryBarrier = vk::ImageMemoryBarrier(
                vk::AccessFlagBits::eTransferWrite,
                vk::AccessFlagBits::eNone,
                vk::ImageLayout::eTransferDstOptimal,
                dstFinalImageLayout,
                vk::QueueFamilyIgnored,
                vk::QueueFamilyIgnored,
                dstImage,
                vk::ImageSubresourceRange(
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    0,
                    1
                )
            );

            commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eBottomOfPipe,
                static_cast<vk::DependencyFlags>(0),
                0,
                nullptr,
                0,
                nullptr,
                1,
                &imageMemoryBarrier,
                m_Dispatch
            );
        }
    }

    /// Present the current frame
    /// @param viewIndex The view index
    /// @returns AXR_SUCCESS if the function succeeded.
    /// @returns AXR_DONT_RENDER if we should skip rendering this frame.
    [[nodiscard]] AxrResult presentFrame(const uint32_t viewIndex) const {
        return m_RenderTarget.presentFrame(viewIndex);
    }

    /// Signal the render target that we're ending the render
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult endRendering() const {
        return m_RenderTarget.endRendering();
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    RenderTarget& m_RenderTarget;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    const vk::DispatchLoaderDynamic& m_Dispatch;
};

#endif
