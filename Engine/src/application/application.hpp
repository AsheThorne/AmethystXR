#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/application.h"
#include "../windowSystem/windowSystem.hpp"
#include "../graphicsSystem/graphicsSystem.hpp"
#include "../assets/assetCollection.hpp"
#include "../scene/scene.hpp"

/// AmethystXr Application
class AxrApplication {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Application config
    AxrApplication(const AxrApplicationConfig& config);
    /// Copy Constructor
    /// @param src Source AxrApplication to copy from
    AxrApplication(const AxrApplication& src) = delete;
    /// Move Constructor
    /// @param src Source AxrApplication to move from
    AxrApplication(AxrApplication&& src) = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrApplication();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrApplication to copy from
    AxrApplication& operator=(const AxrApplication& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrApplication to move from
    AxrApplication& operator=(AxrApplication&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the AxrApplication class
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Check if the application is running
    /// @returns True if the application is running
    [[nodiscard]] bool isRunning() const;
    /// Process application events
    void processEvents();

    /// Get the window system
    /// @returns A handle to the window system
    [[nodiscard]] AxrWindowSystem_T getWindowSystem();
    /// Get the graphics system
    /// @returns A handle to the graphics system
    [[nodiscard]] AxrGraphicsSystem_T getGraphicsSystem();
    /// Get the global asset collection
    /// @returns A handle to the global asset collection
    [[nodiscard]] AxrAssetCollection_T getGlobalAssetCollection();

    /// Create a new scene
    /// @param sceneName Name of the scene
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createScene(const char* sceneName);
    /// Find the named scene
    /// @param sceneName Name of the scene
    /// @returns A handle to the scene
    [[nodiscard]] AxrScene_T findScene(const char* sceneName);
    /// Load the named scene
    /// @param sceneName Name of the scene
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadScene(const char* sceneName);
    /// Set the active scene to the named scene
    /// @param sceneName Name of the scene
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setActiveScene(const char* sceneName);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_ApplicationName;
    uint32_t m_ApplicationVersion;

    AxrWindowSystem m_WindowSystem;
    AxrGraphicsSystem m_GraphicsSystem;
    AxrAssetCollection m_GlobalAssetCollection;
    std::unordered_map<std::string, AxrScene> m_Scenes;
};
