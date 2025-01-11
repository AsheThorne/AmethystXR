#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "common.hpp"
#include "axr/application.h"
#include "windowSystem.hpp"
#include "graphicsSystem.hpp"

namespace axr {
    // ----------------------------------------- //
    // Application Config Definition
    // ----------------------------------------- //

    /// Config for the Application
    struct ApplicationConfig {
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
        // Public Variables
        // ----------------------------------------- //
        const char* ApplicationName;
        uint32_t ApplicationVersion;
        axr::WindowSystemConfig WindowSystemConfig;
        axr::GraphicsSystemConfig GraphicsSystemConfig;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        // TODO: When we write tests, we should make a test to make sure that this conversion works without issue.
        //  Even though we have the static_assert below, there is still a chance for bugs if the order of the variables
        //  differ
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
        void processEvents() {
            axrApplicationProcessEvents(m_Application);
        }

        /// Get the window system
        /// @returns A handle to the window system
        [[nodiscard]] axr::WindowSystem getWindowSystem() {
            return axrApplicationGetWindowSystem(m_Application);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrApplication_T m_Application;
    };
}
