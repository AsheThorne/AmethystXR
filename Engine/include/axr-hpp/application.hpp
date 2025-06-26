#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/application.h"
#include "windowSystem.hpp"
#include "graphicsSystem.hpp"
#include "assets.hpp"
#include "scene.hpp"

namespace axr {
    // ----------------------------------------- //
    // Application Config Definition
    // ----------------------------------------- //

    /// Config for the Application
    struct ApplicationConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        const char* ApplicationName;
        uint32_t ApplicationVersion;
        axr::WindowSystemConfig WindowSystemConfig;
        axr::GraphicsSystemConfig GraphicsSystemConfig;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ApplicationConfig() :
            ApplicationName{},
            ApplicationVersion{},
            WindowSystemConfig{},
            GraphicsSystemConfig{} {
        }

        /// Constructor
        /// @param applicationName The application name
        /// @param applicationVersion The application version
        /// @param windowSystemConfig The window system config
        /// @param graphicsSystemConfig The graphics system config
        ApplicationConfig(
            const char* applicationName,
            const uint32_t applicationVersion,
            const axr::WindowSystemConfig& windowSystemConfig,
            const axr::GraphicsSystemConfig& graphicsSystemConfig
        ) : ApplicationName(applicationName),
            ApplicationVersion(applicationVersion),
            WindowSystemConfig(windowSystemConfig),
            GraphicsSystemConfig(graphicsSystemConfig) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get a handle to the ApplicationConfig as an AxrApplicationConfig
        /// @returns This as an AxrApplicationConfig
        const AxrApplicationConfig* toRaw() const {
            return reinterpret_cast<const AxrApplicationConfig*>(this);
        }

        /// Get a handle to the ApplicationConfig as an AxrApplicationConfig
        /// @returns This as an AxrApplicationConfig
        AxrApplicationConfig* toRaw() {
            return reinterpret_cast<AxrApplicationConfig*>(this);
        }
    };

    static_assert(
        sizeof(AxrApplicationConfig) == sizeof(axr::ApplicationConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Application Definition
    // ----------------------------------------- //

    /// AmethystXr Application
    class Application {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param config Application config
        Application(const ApplicationConfig& config) {
            m_Application = axrCreateApplication(config.toRaw());
        }

        /// Copy Constructor
        /// @param src Source Application to copy from
        Application(const Application& src) = delete;
        /// Move Constructor
        /// @param src Source Application to move from
        Application(Application&& src) noexcept = delete;

        // ---- Destructor ----

        /// Destructor
        ~Application() {
            axrDestroyApplication(&m_Application);
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source Application to copy from
        Application& operator=(const Application& src) = delete;
        /// Move Assignment Operator
        /// @param src Source Application to move from
        Application& operator=(Application&& src) noexcept = delete;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Set up the application
        [[nodiscard]] axr::Result setup() {
            return static_cast<axr::Result>(axrApplicationSetup(m_Application));
        }

        /// Check if the application is running
        /// @returns True if the application is running
        [[nodiscard]] bool isRunning() const {
            return axrApplicationIsRunning(m_Application);
        }

        /// Process application events
        void processEvents() const {
            axrApplicationProcessEvents(m_Application);
        }

        /// Get the window system
        /// @returns A handle to the window system
        [[nodiscard]] axr::WindowSystem getWindowSystem() const {
            return axrApplicationGetWindowSystem(m_Application);
        }

        /// Get the graphics system
        /// @returns A handle to the graphics system
        [[nodiscard]] axr::GraphicsSystem getGraphicsSystem() const {
            return axrApplicationGetGraphicsSystem(m_Application);
        }

        /// Get the global asset collection
        /// @returns A handle to the global asset collection
        [[nodiscard]] axr::AssetCollection getGlobalAssetCollection() const {
            return axrApplicationGetGlobalAssetCollection(m_Application);
        }

        /// Get the delta time from the last time processEvents() was called
        /// @returns The delta time
        [[nodiscard]] float getDeltaTime() const {
            return axrApplicationGetDeltaTime(m_Application);
        }

        /// Create a new scene
        /// @param sceneName Name of the scene
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result createScene(const char* sceneName) {
            return static_cast<axr::Result>(axrApplicationCreateScene(m_Application, sceneName));
        }

        /// Find the named scene
        /// @param sceneName Name of the scene
        /// @returns A handle to the scene
        [[nodiscard]] axr::Scene findScene(const char* sceneName) {
            return axrApplicationFindScene(m_Application, sceneName);
        }

        /// Load the named scene
        /// @param sceneName Name of the scene
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result loadScene(const char* sceneName) {
            return static_cast<axr::Result>(axrApplicationLoadScene(m_Application, sceneName));
        }

        /// Set the active scene to the named scene
        /// @param sceneName Name of the scene
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result setActiveScene(const char* sceneName) {
            return static_cast<axr::Result>(axrApplicationSetActiveScene(m_Application, sceneName));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrApplication_T m_Application;
    };
}
