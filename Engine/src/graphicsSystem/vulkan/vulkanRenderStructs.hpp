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

/// Push constant references for rendering
struct AxrVulkanPushConstantForRendering {
    const vk::ShaderStageFlags* ShaderStages = nullptr;
    const char* BufferName = "";
};

/// Mesh references for rendering
struct AxrVulkanMeshForRendering {
    const vk::Buffer* Buffer = nullptr;
    const vk::DeviceSize* BufferIndicesOffset = nullptr;
    const vk::DeviceSize* BufferVerticesOffset = nullptr;
    const uint32_t* IndexCount = nullptr;
    const AxrTransformComponent* TransformComponent = nullptr;
    AxrVulkanPushConstantForRendering PushConstant;
};

/// Material references for rendering
struct AxrVulkanMaterialForRendering {
    std::string MaterialName;
    const vk::PipelineLayout* PipelineLayout;
    const vk::Pipeline* WindowPipeline;
    const vk::Pipeline* XrSessionPipeline;
    /// One for each frame in flight
    const std::vector<vk::DescriptorSet>* WindowDescriptorSets;
    /// One for each frame in flight
    const std::vector<vk::DescriptorSet>* XrSessionDescriptorSets;
    AxrVulkanPushConstantForRendering PushConstant;
    std::vector<AxrVulkanMeshForRendering> Meshes;
    std::vector<uint32_t> DynamicOffsets;
};

#endif
