#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "axr/graphicsSystem.h"
#include "axr/windowSystem.h"
#include "axr/assets.h"

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
        std::string ApplicationName;
        uint32_t ApplicationVersion;
        AxrWindowSystem_T WindowSystem;
        AxrXrSystem_T XrSystem;
        AxrAssetCollection_T GlobalAssetCollection;
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
    ~AxrGraphicsSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrGraphicsSystem to copy from
    AxrGraphicsSystem& operator=(const AxrGraphicsSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrGraphicsSystem to move from
    AxrGraphicsSystem& operator=(AxrGraphicsSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Draw the current frame
    void drawFrame();

    /// Set the clear color
    /// @param color Clear color
    void setClearColor(const AxrColor& color) const;

    /// Set the window render source
    /// @param renderSource Window render source
    void setWindowRenderSource(AxrWindowRenderSourceEnum renderSource) const;

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the graphics system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Reset setup()
    void resetSetup();

    /// Load the named scene
    /// @param scene Scene to load
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadScene(AxrScene_T scene);
    /// Set the active scene to the named scene
    /// @param sceneName Name of the scene
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setActiveScene(const std::string& sceneName);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    // ---- Vulkan Variables ----
    AxrVulkanGraphicsSystem* m_VulkanGraphicsSystem;
#endif

    // ---- Config Variables ----
    AxrGraphicsApiEnum m_GraphicsApi;
};
