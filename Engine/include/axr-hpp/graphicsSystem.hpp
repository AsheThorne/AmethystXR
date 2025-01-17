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
    enum GraphicsApiEnum {
        Undefined = AXR_GRAPHICS_API_UNDEFINED,
        Vulkan = AXR_GRAPHICS_API_VULKAN,
    };

    // ----------------------------------------- //
    // Graphics Config Definition
    // ----------------------------------------- //

    /// Config for the graphics system
    struct GraphicsSystemConfig {
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        GraphicsSystemConfig() :
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
            VulkanApiConfig(nullptr),
#endif
            GraphicsApi{} {
        }

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        /// Vulkan Graphics Constructor
        /// @param graphicsApi The graphics api
        /// @param vulkanApiConfig The vulkan api config
        GraphicsSystemConfig(
            const axr::GraphicsApiEnum graphicsApi,
            axr::VulkanApiConfig* vulkanApiConfig
        ) : GraphicsApi(graphicsApi),
            VulkanApiConfig(vulkanApiConfig) {
        }
#endif

        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
        axr::VulkanApiConfig* VulkanApiConfig;
#endif
        axr::GraphicsApiEnum GraphicsApi;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        // TODO: When we write tests, we should make a test to make sure that this conversion works without issue.
        //  Even though we have the static_assert below, there is still a chance for bugs if the order of the variables
        //  differ
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
