#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "logger.hpp"
#include "axr/setup.h"

namespace axr {
    // ----------------------------------------- //
    // Setup() Config Definition
    // ----------------------------------------- //

    /// Config for Setup()
    struct SetupConfig {
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        SetupConfig() :
            LogLevel{LogLevelEnum::Undefined} {
        }

        /// Constructor
        /// @param logLevel The logging level
        SetupConfig(const axr::LogLevelEnum logLevel) :
            LogLevel(logLevel) {
        }

        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        axr::LogLevelEnum LogLevel;

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        // TODO: When we write tests, we should make a test to make sure that this conversion works without issue.
        //  Even though we have the static_assert below, there is still a chance for bugs if the order of the variables
        //  differ
        /// Get a handle to the SetupConfig as an AxrSetupConfig
        /// @returns This as an AxrSetupConfig
        const AxrSetupConfig* toRaw() const {
            return reinterpret_cast<const AxrSetupConfig*>(this);
        }

        /// Get a handle to the SetupConfig as an AxrSetupConfig
        /// @returns This as an AxrSetupConfig
        AxrSetupConfig* toRaw() {
            return reinterpret_cast<AxrSetupConfig*>(this);
        }
    };

    static_assert(
        sizeof(AxrSetupConfig) == sizeof(axr::SetupConfig),
        "Original type and wrapper have different size!"
    );

    /// Set up the AmethystXR Engine
    inline void setup(const SetupConfig& config) {
        axrSetup(config.toRaw());
    }
}
