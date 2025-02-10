#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/windowSystem.h"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan Window Graphics
class AxrVulkanWindowGraphics {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrVulkanWindowGraphics Config
    struct Config {
        AxrWindowSystem& WindowSystem;
        vk::DispatchLoaderDynamic& Dispatch;
    };

    /// AxrVulkanWindowGraphics Setup Config
    struct SetupConfig {
        vk::Instance Instance;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrVulkanWindowGraphics(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanWindowGraphics to copy from
    AxrVulkanWindowGraphics(const AxrVulkanWindowGraphics& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanWindowGraphics to move from
    AxrVulkanWindowGraphics(AxrVulkanWindowGraphics&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanWindowGraphics();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanWindowGraphics to copy from
    AxrVulkanWindowGraphics& operator=(const AxrVulkanWindowGraphics& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanWindowGraphics to move from
    AxrVulkanWindowGraphics& operator=(AxrVulkanWindowGraphics&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up vulkan window graphics
    /// @param config Setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const SetupConfig& config);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config ----
    AxrWindowSystem& m_WindowSystem;
    vk::DispatchLoaderDynamic& m_Dispatch;

    // ---- Setup Config ----
    vk::Instance m_Instance;

    vk::SurfaceKHR m_Surface;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up the window graphics
    void cleanup();

    /// Set the SetupConfig variables
    /// @param config Setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSetupConfigVariables(const SetupConfig& config);
    /// Reset the SetupConfig variables
    void resetSetupConfigVariables();

    /// Prepare the window for rendering
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult prepareWindowForRendering();
    /// Destroy everything that gets created in prepareWindowForRendering 
    void destroyWindowRenderingConfig();

    // ---- Surface ----

    /// Create the vulkan surface
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createSurface();
    /// Destroy the vulkan surface
    void destroySurface();

#ifdef AXR_USE_PLATFORM_WIN32
    /// Create the vulkan win32 surface
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWin32Surface();
#endif

    // ----------------------------------------- //
    // Private Static Functions
    // ----------------------------------------- //

    /// 'Configure window graphics' callback function
    /// @param userData User data
    /// @param isWindowOpen If true,the graphics should be set up. If false, the graphics should be cleaned up.
    /// @returns AXR_SUCCESS if the function succeeded
    static AxrResult configureWindowGraphicsCallback(void* userData, bool isWindowOpen);
};


#endif
