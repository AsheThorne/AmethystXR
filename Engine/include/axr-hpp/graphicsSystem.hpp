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
    // Graphics Api Enum Definition
    // ----------------------------------------- //

    /// Graphics api enum
    enum class GraphicsApiEnum {
        Undefined = AXR_GRAPHICS_API_UNDEFINED,
        Vulkan = AXR_GRAPHICS_API_VULKAN,
    };

    // ----------------------------------------- //
    // Graphics Config Definition
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

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        GraphicsSystemConfig() :
            GraphicsApi{} {
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Vulkan Graphics Constructor
        /// @param vulkanApiConfig The vulkan api config
        GraphicsSystemConfig(
            axr::VulkanApiConfig* vulkanApiConfig
        ) : GraphicsApi(axr::GraphicsApiEnum::Vulkan),
            VulkanApiConfig(vulkanApiConfig) {
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

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrGraphicsSystem_T m_GraphicsSystem;
    };
}
