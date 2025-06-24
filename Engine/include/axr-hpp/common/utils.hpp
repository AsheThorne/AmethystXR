#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/utils.h"

namespace axr {
    // ----------------------------------------- //
    // External Function Definitions
    // ----------------------------------------- //

    /// Check if the given string is empty
    /// @param string String to check
    /// @returns True if the given string is empty
    inline bool stringIsEmpty(const char* string) {
        return axrStringIsEmpty(string);
    }

    /// Clone the given C string
    /// @param string String to clone
    /// @returns The cloned string
    inline char* cloneString(const char* string) {
        return axrCloneString(string);
    }

    /// Destroy the given C string
    /// @param string String to destroy
    inline void destroyString(char** string) {
        return axrDestroyString(string);
    }
}
