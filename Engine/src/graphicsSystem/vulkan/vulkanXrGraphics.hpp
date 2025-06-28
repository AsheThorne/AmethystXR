#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "../../xrSystem/xrSystem.hpp"

/// Vulkan Xr Graphics
class AxrVulkanXrGraphics {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan xr graphics config
    struct Config {
        AxrXrSystem& XrSystem;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Xr graphics config
    AxrVulkanXrGraphics(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanXrGraphics to copy from
    AxrVulkanXrGraphics(const AxrVulkanXrGraphics& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanXrGraphics to move from
    AxrVulkanXrGraphics(AxrVulkanXrGraphics&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanXrGraphics();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanXrGraphics to copy from
    AxrVulkanXrGraphics& operator=(const AxrVulkanXrGraphics& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanXrGraphics to move from
    AxrVulkanXrGraphics& operator=(AxrVulkanXrGraphics&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config ----
    AxrXrSystem& m_XrSystem;
};
