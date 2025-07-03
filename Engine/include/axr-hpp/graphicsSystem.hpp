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

    /// Multisample Anti-Aliasing sample count enum
    enum class MsaaSampleCountEnum {
        SampleCount1 = AXR_MSAA_SAMPLE_COUNT_1,
        SampleCount2 = AXR_MSAA_SAMPLE_COUNT_2,
        SampleCount4 = AXR_MSAA_SAMPLE_COUNT_4,
        SampleCount8 = AXR_MSAA_SAMPLE_COUNT_8,
        SampleCount16 = AXR_MSAA_SAMPLE_COUNT_16,
        SampleCount32 = AXR_MSAA_SAMPLE_COUNT_32,
        SampleCount64 = AXR_MSAA_SAMPLE_COUNT_64,
    };

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Config for the window graphics
    struct GraphicsWindowConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        axr::MsaaSampleCountEnum MaxMsaaSampleCount;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        GraphicsWindowConfig() :
            MaxMsaaSampleCount(axr::MsaaSampleCountEnum::SampleCount1) {
        }

        /// Constructor
        /// @param maxMsaaSampleCount Max msaa sample count to use
        explicit GraphicsWindowConfig(const axr::MsaaSampleCountEnum maxMsaaSampleCount) :
            MaxMsaaSampleCount(maxMsaaSampleCount) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the GraphicsWindowConfig as an AxrGraphicsWindowConfig
        /// @returns This as an AxrGraphicsWindowConfig
        const AxrGraphicsWindowConfig* toRaw() const {
            return reinterpret_cast<const AxrGraphicsWindowConfig*>(this);
        }

        /// Get a handle to the GraphicsWindowConfig as an AxrGraphicsWindowConfig
        /// @returns This as an AxrGraphicsWindowConfig
        AxrGraphicsWindowConfig* toRaw() {
            return reinterpret_cast<AxrGraphicsWindowConfig*>(this);
        }
    };

    static_assert(
        sizeof(AxrGraphicsWindowConfig) == sizeof(axr::GraphicsWindowConfig),
        "Original type and wrapper have different size!"
    );

    /// Config for the xr session graphics
    struct GraphicsXrSessionConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        axr::MsaaSampleCountEnum MaxMsaaSampleCount;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        GraphicsXrSessionConfig() :
            MaxMsaaSampleCount(axr::MsaaSampleCountEnum::SampleCount1) {
        }

        /// Constructor
        /// @param maxMsaaSampleCount Max msaa sample count to use
        explicit GraphicsXrSessionConfig(const axr::MsaaSampleCountEnum maxMsaaSampleCount) :
            MaxMsaaSampleCount(maxMsaaSampleCount) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the GraphicsXrSessionConfig as an AxrGraphicsXrSessionConfig
        /// @returns This as an AxrGraphicsXrSessionConfig
        const AxrGraphicsXrSessionConfig* toRaw() const {
            return reinterpret_cast<const AxrGraphicsXrSessionConfig*>(this);
        }

        /// Get a handle to the GraphicsXrSessionConfig as an AxrGraphicsXrSessionConfig
        /// @returns This as an AxrGraphicsXrSessionConfig
        AxrGraphicsXrSessionConfig* toRaw() {
            return reinterpret_cast<AxrGraphicsXrSessionConfig*>(this);
        }
    };

    static_assert(
        sizeof(AxrGraphicsXrSessionConfig) == sizeof(axr::GraphicsXrSessionConfig),
        "Original type and wrapper have different size!"
    );

    /// Config for the graphics system
    struct GraphicsSystemConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        axr::GraphicsApiEnum GraphicsApi;
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        axr::VulkanApiConfig* VulkanApiConfig = nullptr;
#endif
        const axr::GraphicsWindowConfig* WindowConfig;
        const axr::GraphicsXrSessionConfig* XrSessionConfig;
        axr::SamplerAnisotropyQualityEnum SamplerAnisotropyQuality;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        GraphicsSystemConfig() :
            GraphicsApi(axr::GraphicsApiEnum::Undefined),
            WindowConfig(nullptr),
            XrSessionConfig(nullptr),
            SamplerAnisotropyQuality(axr::SamplerAnisotropyQualityEnum::None) {
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Vulkan Graphics Constructor
        /// @param vulkanApiConfig The vulkan api config
        /// @param windowConfig The window graphics config
        /// @param xrSessionConfig The xr session graphics config
        /// @param samplerAnisotropyQuality The sampler anisotropy quality
        GraphicsSystemConfig(
            axr::VulkanApiConfig* vulkanApiConfig,
            const axr::GraphicsWindowConfig* windowConfig,
            const axr::GraphicsXrSessionConfig* xrSessionConfig,
            const axr::SamplerAnisotropyQualityEnum samplerAnisotropyQuality
        ) : GraphicsApi(axr::GraphicsApiEnum::Vulkan),
            VulkanApiConfig(vulkanApiConfig),
            WindowConfig(windowConfig),
            XrSessionConfig(xrSessionConfig),
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

        /// Set the clear color
        /// @param color Clear color
        void setClearColor(const glm::vec4& color) const {
            axrGraphicsSystemSetClearColor(m_GraphicsSystem, color);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrGraphicsSystem_T m_GraphicsSystem;
    };
}
