#pragma once

// ----------------------------------------- //
// C Headers
// ----------------------------------------- //
#include <cstdint>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"

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
    const char* ApplicationName;
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
    /// @returns AXR_SUCCESS if the function succeeded
    AXR_API AxrResult axrApplicationSetup(AxrApplication_T app);
}
