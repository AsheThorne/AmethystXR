#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/axrApplication.h"

namespace axr {
    /// AmethystXr Application
    class Application {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        Application() {
            m_AxrApplication = axrCreateApplication();
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
            axrDestroyApplication(&m_AxrApplication);
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source Application to copy from
        Application& operator=(const Application& src) = delete;
        /// Move Assignment Operator
        /// @param src Source Application to move from
        Application& operator=(Application&& src) noexcept = delete;

        /// Set up the application
        void setup() {
            axrApplicationSetup(m_AxrApplication);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrApplication_T m_AxrApplication;
    };
}
