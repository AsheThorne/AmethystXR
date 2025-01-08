#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "logger.h"
#include "common/defines.h"

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Config for axrSetup()
struct AxrSetupConfig {
    AxrLogLevelEnum LogLevel;
};

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

extern "C" {
    /// Set up the AmethystXR Engine
    AXR_API void axrSetup(const AxrSetupConfig* config);
}