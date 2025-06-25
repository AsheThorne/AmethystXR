#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/graphicsSystem.h"

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#include "vulkanApi.hpp"
#endif

namespace axr {
    // ----------------------------------------- //
    // Enums
    // ----------------------------------------- //

    /// Sampler anisotropy quality enum
    enum class SamplerAnisotropyQualityEnum {
        None = AXR_SAMPLER_ANISOTROPY_QUALITY_NONE,
        Low = AXR_SAMPLER_ANISOTROPY_QUALITY_LOW,
        Medium = AXR_SAMPLER_ANISOTROPY_QUALITY_MEDIUM,
        High = AXR_SAMPLER_ANISOTROPY_QUALITY_HIGH,
    };

    /// Graphics api enum
    enum class GraphicsApiEnum {
        Undefined = AXR_GRAPHICS_API_UNDEFINED,
        Vulkan = AXR_GRAPHICS_API_VULKAN,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Config for the graphics system
    struct GraphicsSystemConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        axr::GraphicsApiEnum GraphicsApi;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        axr::VulkanApiConfig* VulkanApiConfig = nullptr;
#endif
        glm::vec4 ClearColor;
        axr::SamplerAnisotropyQualityEnum SamplerAnisotropyQuality;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        GraphicsSystemConfig() :
            GraphicsApi(axr::GraphicsApiEnum::Undefined),
            ClearColor({1.0f}),
            SamplerAnisotropyQuality(axr::SamplerAnisotropyQualityEnum::None) {
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Vulkan Graphics Constructor
        /// @param vulkanApiConfig The vulkan api config
        /// @param clearColor The clear color
        /// @param samplerAnisotropyQuality The sampler anisotropy quality
        GraphicsSystemConfig(
            axr::VulkanApiConfig* vulkanApiConfig,
            const glm::vec4 clearColor,
            const axr::SamplerAnisotropyQualityEnum samplerAnisotropyQuality
        ) : GraphicsApi(axr::GraphicsApiEnum::Vulkan),
            VulkanApiConfig(vulkanApiConfig),
            ClearColor(clearColor),
            SamplerAnisotropyQuality(samplerAnisotropyQuality) {
        }
#endif

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the GraphicsSystemConfig as an AxrGraphicsSystemConfig
        /// @returns This as an AxrGraphicsSystemConfig
        const AxrGraphicsSystemConfig* toRaw() const {
            return reinterpret_cast<const AxrGraphicsSystemConfig*>(this);
        }

        /// Get a handle to the GraphicsSystemConfig as an AxrGraphicsSystemConfig
        /// @returns This as an AxrGraphicsSystemConfig
        AxrGraphicsSystemConfig* toRaw() {
            return reinterpret_cast<AxrGraphicsSystemConfig*>(this);
        }
    };

    static_assert(
        sizeof(AxrGraphicsSystemConfig) == sizeof(axr::GraphicsSystemConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Graphics System Definition
    // ----------------------------------------- //

    /// Graphics System
    class GraphicsSystem {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param graphicsSystem Graphics system handle
        GraphicsSystem(const AxrGraphicsSystem_T graphicsSystem):
            m_GraphicsSystem(graphicsSystem) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Draw the current frame
        void drawFrame() const {
            axrGraphicsSystemDrawFrame(m_GraphicsSystem);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrGraphicsSystem_T m_GraphicsSystem;
    };
}
