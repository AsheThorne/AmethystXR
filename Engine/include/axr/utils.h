#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

extern "C" {
    /// Check if the given string is empty
    /// @param string String to check
    /// @returns True if the given string is empty
    AXR_API bool axrStringIsEmpty(const char* string);
    /// Clone the given C string
    /// @param string String to clone
    /// @returns The cloned string
    AXR_API char* axrCloneString(const char* string);
    /// Destroy the given C string
    /// @param string String to destroy
    AXR_API void axrDestroyString(char** string);
}
