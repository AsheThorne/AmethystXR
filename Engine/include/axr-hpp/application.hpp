#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/application.h"
#include "windowSystem.hpp"
#include "graphicsSystem.hpp"
#include "assets.hpp"
#include "scene.hpp"
#include "xrSystem.hpp"
#include "ioActionsSystem.hpp"

namespace axr {
    // ----------------------------------------- //
    // Application Config Definition
    // ----------------------------------------- //

    /// Config for the Application
    struct ApplicationConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        char ApplicationName[AXR_MAX_APPLICATION_NAME_SIZE]{};
        uint32_t ApplicationVersion;
        axr::GraphicsSystemConfig GraphicsSystemConfig;
        const axr::WindowSystemConfig* WindowSystemConfig;
        const axr::XrSystemConfig* XrSystemConfig;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        ApplicationConfig() :
            ApplicationVersion(0),
            GraphicsSystemConfig({}),
            WindowSystemConfig(nullptr),
            XrSystemConfig(nullptr) {
        }

        /// Constructor
        /// @param applicationName The application name
        /// @param applicationVersion The application version
        /// @param graphicsSystemConfig The graphics system config
        /// @param windowSystemConfig The window system config
        /// @param xrSystemConfig The xr system config
        ApplicationConfig(
            const char* applicationName,
            const uint32_t applicationVersion,
            const axr::GraphicsSystemConfig& graphicsSystemConfig,
            const axr::WindowSystemConfig* windowSystemConfig,
            const axr::XrSystemConfig* xrSystemConfig
        ) : ApplicationVersion(applicationVersion),
            GraphicsSystemConfig(graphicsSystemConfig),
            WindowSystemConfig(windowSystemConfig),
            XrSystemConfig(xrSystemConfig) {
            if (applicationName != nullptr) {
                strncpy_s(ApplicationName, applicationName, AXR_MAX_APPLICATION_NAME_SIZE);
            }
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

        /// Get the xr system
        /// @returns A handle to the xr system
        [[nodiscard]] axr::XrSystem getXrSystem() const {
            return axrApplicationGetXrSystem(m_Application);
        }

        /// Get the graphics system
        /// @returns A handle to the graphics system
        [[nodiscard]] axr::GraphicsSystem getGraphicsSystem() const {
            return axrApplicationGetGraphicsSystem(m_Application);
        }

        /// Get the input/output actions system
        /// @returns A handle to the input/output actions system
        [[nodiscard]] axr::IOActionsSystem getIOActionsSystem() const {
            return axrApplicationGetIOActionsSystem(m_Application);
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
            char sceneNameBuffer[AXR_MAX_SCENE_NAME_SIZE]{};
            strncpy_s(sceneNameBuffer, sceneName, AXR_MAX_SCENE_NAME_SIZE);
            return static_cast<axr::Result>(axrApplicationCreateScene(m_Application, sceneNameBuffer));
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
