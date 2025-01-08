#pragma once

// ----------------------------------------- //
// C Headers
// ----------------------------------------- //
#include <stdint.h>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

// ----------------------------------------- //
// Preprocessor Definitions
// ----------------------------------------- //
#define AXR_MAX_APPLICATION_NAME_SIZE 128

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrApplication Handle
typedef class AxrApplication* AxrApplication_T;

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Config for the AxrApplication
struct AxrApplicationConfig {
    char ApplicationName[AXR_MAX_APPLICATION_NAME_SIZE];
    uint32_t ApplicationVersion;
};

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Create an AxrApplication
    /// @param config A handle to the application config
    /// @returns The created AxrApplication
    AXR_API AxrApplication_T axrCreateApplication(const AxrApplicationConfig* config);
    /// Destroy the given AxrApplication
    /// @param app The AxrApplication to destroy
    AXR_API void axrDestroyApplication(AxrApplication_T* app);

    /// Set up the given axrApplication
    /// @param app The AxrApplication to set up
    AXR_API void axrApplicationSetup(AxrApplication_T app);
}
