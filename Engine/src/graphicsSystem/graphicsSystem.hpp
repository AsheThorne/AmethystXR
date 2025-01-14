#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "axr/graphicsSystem.h"
#include "vulkan/vulkanGraphicsSystem.hpp"

/// Graphics System
class AxrGraphicsSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Graphics System Config
    struct Config {
        AxrGraphicsApiEnum GraphicsApi;
        const char* ApplicationName;
        uint32_t ApplicationVersion;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Graphics system config
    AxrGraphicsSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrGraphicsSystem to copy from
    AxrGraphicsSystem(const AxrGraphicsSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrGraphicsSystem to move from
    AxrGraphicsSystem(AxrGraphicsSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrGraphicsSystem() = default;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrGraphicsSystem to copy from
    AxrGraphicsSystem& operator=(const AxrGraphicsSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrGraphicsSystem to move from
    AxrGraphicsSystem& operator=(AxrGraphicsSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Headers
    // ----------------------------------------- //

    /// Set up the graphics system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Vulkan Variables ----
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    AxrVulkanGraphicsSystem m_VulkanGraphicsSystem;
#endif
    
    Config m_Config;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    // ---- Vulkan Functions ----
    
    /// Set up the vulkan graphics
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupVulkan();
};
