#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdint>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

// TODO: Remove from here or safe guard it with preprocessor
#include <windows.h>
#include <vulkan/vulkan_win32.h>

/// Vulkan Graphics System
class AxrVulkanGraphicsSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan Graphics System Config
    struct Config {
        const char* ApplicationName;
        uint32_t ApplicationVersion;
    };
    
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Vulkan graphics system config
    AxrVulkanGraphicsSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanGraphicsSystem to copy from
    AxrVulkanGraphicsSystem(const AxrVulkanGraphicsSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanGraphicsSystem to move from
    AxrVulkanGraphicsSystem(AxrVulkanGraphicsSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanGraphicsSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanGraphicsSystem to copy from
    AxrVulkanGraphicsSystem& operator=(const AxrVulkanGraphicsSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanGraphicsSystem to move from
    AxrVulkanGraphicsSystem& operator=(AxrVulkanGraphicsSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the vulkan graphics system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    Config m_Config;
    vk::Instance m_Instance;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Create the vulkan instance
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createInstance();
    /// Destroy the vulkan instance
    void destroyInstance();
};

#endif
