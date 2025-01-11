#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/graphicsSystem.h"

namespace axr {
    // ----------------------------------------- //
    // Graphics Api Enum Definition
    // ----------------------------------------- //

    /// Graphics api enum
    enum GraphicsApiEnum {
        Undefined = AXR_GRAPHICS_API_UNDEFINED,
        Vulkan = AXR_GRAPHICS_API_VULKAN,
    };

    static_assert(
        sizeof(AxrGraphicsApiEnum) == sizeof(axr::GraphicsApiEnum),
        "Original type and wrapper have different size!"
    );

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
            GraphicsApi{} {
        }

        /// Constructor
        /// @param graphicsApi The graphics api
        GraphicsSystemConfig(
            const axr::GraphicsApiEnum graphicsApi
        ) : GraphicsApi(graphicsApi) {
        }

        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
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
