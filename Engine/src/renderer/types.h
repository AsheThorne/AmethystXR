#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkan/vulkanRenderer.h"

/// Renderer context api type
enum AxrRendererContextApiTypeEnum {
    AXR_RENDERER_CONTEXT_API_TYPE_UNDEFINED = 0,
    AXR_RENDERER_CONTEXT_API_TYPE_VULKAN = 1,
};

/// Renderer context
struct AxrRendererContext {
    union {
        // Don't wrap in an AXR_VULKAN_SUPPORTED preprocessor.
        // An empty AxrVulkanRenderer::Context struct is declared if AXR_VULKAN_SUPPORTED isn't defined
        AxrVulkanRenderer::Context Vulkan;
    };
    AxrRendererContextApiTypeEnum ApiType{};
};
