#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/result.h"
#include "axr/graphicsSystem.h"
#include "axr/windowSystem.h"

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#include "vulkan/vulkanGraphicsSystem.hpp"
#endif

/// Graphics System
class AxrGraphicsSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Graphics System Config
    struct Config {
        const char* ApplicationName;
        uint32_t ApplicationVersion;
        AxrWindowSystem_T WindowSystem;
        AxrGraphicsSystemConfig GraphicsConfig;
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

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    // ---- Vulkan Variables ----
    std::unique_ptr<AxrVulkanGraphicsSystem> m_VulkanGraphicsSystem;
#endif

    // ---- Config Variables ----
    AxrGraphicsApiEnum m_GraphicsApi;
};
