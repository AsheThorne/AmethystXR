#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "common.hpp"
#include "axr/axrApplication.h"

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
            ApplicationVersion{} {
        }

        /// Constructor
        /// @param applicationName The application name
        /// @param applicationVersion The application version
        ApplicationConfig(
            const char* applicationName,
            const uint32_t applicationVersion
        ) : ApplicationVersion(applicationVersion) {
            if (applicationName != nullptr) {
                strncpy_s(
                    ApplicationName,
                    AXR_MAX_APPLICATION_NAME_SIZE,
                    applicationName,
                    AXR_MAX_APPLICATION_NAME_SIZE
                );
            }
        }

        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        const char* ApplicationName;
        uint32_t ApplicationVersion;

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
            m_AxrApplication = axrCreateApplication(config.toRaw());
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
        [[nodiscard]] axr::Result setup() {
            return static_cast<axr::Result>(axrApplicationSetup(m_AxrApplication));
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrApplication_T m_AxrApplication;
    };
}
