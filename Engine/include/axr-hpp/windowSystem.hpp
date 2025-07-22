#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/windowSystem.h"
#include "axr-hpp/common/enums.hpp"

namespace axr {
    // ----------------------------------------- //
    // Window Config Definition
    // ----------------------------------------- //

    /// Config for the window system
    struct WindowSystemConfig {
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        uint32_t Width;
        uint32_t Height;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        WindowSystemConfig() :
            Width{},
            Height{} {
        }

        /// Constructor
        /// @param width The window width
        /// @param height The window height
        WindowSystemConfig(
            const uint32_t width,
            const uint32_t height
        ) : Width(width),
            Height{height} {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

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

        /// Check if the window system is valid for use
        /// @returns True if the window system is valid for use
        [[nodiscard]] bool isValid() const {
            return axrWindowSystemIsValid(m_WindowSystem);
        }

        /// Check if the window is open
        /// @returns True if the window is open
        [[nodiscard]] bool isWindowOpen() const {
            return axrWindowSystemIsWindowOpen(m_WindowSystem);
        }

        /// Open the window
        /// @returns AXR_SUCCESS if the function succeeded
        [[nodiscard]] axr::Result openWindow() const {
            return static_cast<axr::Result>(axrWindowSystemOpenWindow(m_WindowSystem));
        }

        /// Signal that we want to close the window
        void closeWindow() const {
            axrWindowSystemCloseWindow(m_WindowSystem);
        }

        /// Check if the cursor is hidden
        /// @returns True if the cursor is hidden
        [[nodiscard]] bool isCursorHidden() const {
            return axrWindowSystemIsCursorHidden(m_WindowSystem);
        }

        /// Unhide the cursor
        void showCursor() const {
            axrWindowSystemShowCursor(m_WindowSystem);
        }

        /// Hide the cursor
        void hideCursor() const {
            axrWindowSystemHideCursor(m_WindowSystem);
        }

        /// Check if the cursor's position is locked
        /// @returns True if the cursor's position is locked
        [[nodiscard]] bool isCursorLocked() const {
            return axrWindowSystemIsCursorLocked(m_WindowSystem);
        }

        /// Lock the cursor's position
        void lockCursor() const {
            axrWindowSystemLockCursor(m_WindowSystem);
        }

        /// Unlock the cursor's position
        void unlockCursor() const {
            axrWindowSystemUnlockCursor(m_WindowSystem);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrWindowSystem_T m_WindowSystem;
    };
}
