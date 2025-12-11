#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/enums.h"

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Engine Config
struct AxrEngineConfig {};

// ----------------------------------------- //
// External Function Declarations
// ----------------------------------------- //
extern "C" {
    /// Set up the AmethystXR engine
    /// @param config Handle to the engine config
    /// @return AXR_SUCCESS if the function succeeded. AXR_ERROR_NULLPTR if `config` is null.
    AXR_API AxrResult axrSetup(const AxrEngineConfig* config);
    /// Shut down the AmethystXR engine
    AXR_API void axrShutdown();
}
