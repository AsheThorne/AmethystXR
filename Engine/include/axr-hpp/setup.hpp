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

    // ----------------------------------------- //
    // Function Definitions
    // ----------------------------------------- //

    /// Set up the AmethystXR Engine
    inline void setup(const SetupConfig& config) {
        axrSetup(config.toRaw());
    }
}
