#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/scene.h"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Mesh references for rendering
struct AxrVulkanMeshForRendering {
    const vk::Buffer* Buffer = nullptr;
    const vk::DeviceSize* BufferIndicesOffset = nullptr;
    const vk::DeviceSize* BufferVerticesOffset = nullptr;
    const uint32_t* IndexCount = nullptr;
    const AxrTransformComponent* TransformComponent = nullptr;
    const vk::ShaderStageFlags PushConstantShaderStages = static_cast<vk::ShaderStageFlags>(
        VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM
    );
    const char* PushConstantBufferName = "";
};

/// Material references for rendering
struct AxrVulkanMaterialForRendering {
    std::string MaterialName;
    const vk::PipelineLayout* PipelineLayout = nullptr;
    const vk::Pipeline* WindowPipeline = nullptr;
    const vk::Pipeline* XrSessionPipeline = nullptr;
    /// One for each frame in flight
    const std::vector<vk::DescriptorSet>* WindowDescriptorSets = nullptr;
    /// One for each frame in flight
    const std::vector<vk::DescriptorSet>* XrSessionDescriptorSets = nullptr;
    std::vector<AxrVulkanMeshForRendering> Meshes;
    std::vector<uint32_t> DynamicOffsets;
};

#endif
