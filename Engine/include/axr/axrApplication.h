#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

// ----------------------------------------- //
// Forward Declarations
// ----------------------------------------- //
typedef class AxrApplication* AxrApplication_T;

// ----------------------------------------- //
// Definitions
// ----------------------------------------- //
extern "C" {
    /// Create an AxrApplication
    /// @returns The created AxrApplication
    AXR_API AxrApplication_T axrCreateApplication();
    /// Destroy the given AxrApplication
    /// @param app The AxrApplication to destroy
    AXR_API void axrDestroyApplication(AxrApplication_T* app);

    /// Set up the given axrApplication
    /// @param app The AxrApplication to set up
    AXR_API void axrApplicationSetup(AxrApplication_T app);
}
