#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <xtr1common>

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Make `To` the same constness as `From`
/// Remove const from `To` if `From` isn't const
template <typename To, typename From>
struct AxrConstnessAs {
    using Type = std::remove_const_t<To>;
};

/// Make `To` the same constness as `From`
/// Make `To` const if `From` is const and `To` isn't
template <typename To, typename From>
struct AxrConstnessAs<To, const From> {
    using Type = const To;
};

/// Get the type of `To` with the same constantness as `From`
template <typename To, typename From>
using AxrConstnessAs_T = typename AxrConstnessAs<To, From>::Type;

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
