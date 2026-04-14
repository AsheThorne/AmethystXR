#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "defines.h"

#include <cstdint>
#include <cstdlib>

// ---------------------------------------------------------------------------------- //
//                                       AxrID                                        //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrID
typedef uint64_t AxrID_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Generate an Axr ID for the given name
    /// @param name Name to use
    /// @param length Length of the given name
    /// @return Axr ID
    AXR_API AxrID_T axrGenerateID(const char8_t* name, size_t length);
    /// Warning: This is a debug only function. This function will return an empty string if AXR_DEBUG_INFO_ENABLED is
    /// not defined.
    ///
    /// Get the name used during the creation of the given ID
    /// @param id ID to get the name of
    /// @return The name of the given ID. Or an empty string if no name for the ID was found.
    AXR_API const char8_t* axrGetIDName(AxrID_T id);
}

// ----------------------------------------- //
// User defined literals
// ----------------------------------------- //

inline AxrID_T operator""_id(const char8_t* string, const size_t length) {
    return axrGenerateID(string, length);
}
