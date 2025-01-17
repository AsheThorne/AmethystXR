#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/windowSystem.h"

namespace axr {
    // ----------------------------------------- //
    // Window Platform Enum Definition
    // ----------------------------------------- //

    /// Window platform enum
    enum class WindowPlatformEnum {
        Undefined = AXR_WINDOW_PLATFORM_UNDEFINED,
        Win32 = AXR_WINDOW_PLATFORM_WIN32,
    };

    // ----------------------------------------- //
    // Window Config Definition
    // ----------------------------------------- //

    /// Config for the window system
    struct WindowSystemConfig {
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        WindowSystemConfig() :
            Platform{},
            Width{},
            Height{} {
        }

        /// Constructor
        /// @param platform The window platform
        /// @param width The window width
        /// @param height The window height
        WindowSystemConfig(
            const axr::WindowPlatformEnum platform,
            const uint32_t width,
            const uint32_t height
        ) : Platform(platform),
            Width(width),
            Height{height} {
        }

        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        axr::WindowPlatformEnum Platform;
        uint32_t Width;
        uint32_t Height;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        // TODO: When we write tests, we should make a test to make sure that this conversion works without issue.
        //  Even though we have the static_assert below, there is still a chance for bugs if the order of the variables
        //  differ
        /// Get a handle to the WindowSystemConfig as an AxrWindowSystemConfig
        /// @returns This as an AxrWindowSystemConfig
        const AxrWindowSystemConfig* toRaw() const {
            return reinterpret_cast<const AxrWindowSystemConfig*>(this);
        }

        /// Get a handle to the WindowSystemConfig as an AxrWindowSystemConfig
        /// @returns This as an AxrWindowSystemConfig
        AxrWindowSystemConfig* toRaw() {
            return reinterpret_cast<AxrWindowSystemConfig*>(this);
        }
    };

    static_assert(
        sizeof(AxrWindowSystemConfig) == sizeof(axr::WindowSystemConfig),
        "Original type and wrapper have different size!"
    );

    // ----------------------------------------- //
    // Window System Definition
    // ----------------------------------------- //

    /// Window System
    class WindowSystem {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param windowSystem Window system handle
        WindowSystem(const AxrWindowSystem_T windowSystem):
            m_WindowSystem(windowSystem) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if the window is open
        /// @returns True if the window is open
        [[nodiscard]] bool isWindowOpen() const {
            return axrWindowSystemIsWindowOpen(m_WindowSystem);
        }

        /// Open the window
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result openWindow() {
            return static_cast<axr::Result>(axrWindowSystemOpenWindow(m_WindowSystem));
        }

        /// Signal that we want to close the window
        void closeWindow() {
            axrWindowSystemCloseWindow(m_WindowSystem);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrWindowSystem_T m_WindowSystem;
    };
}
