#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <vector>

// ----------------------------------------- //
// Function Definitions
// ----------------------------------------- //

// ---- String Utils ----

/// Convert a const char* to a std::wstring
/// @param string String to convert
/// @returns Converted string
std::wstring axrToWString(const char* string);

/// Check if a string is within the given collection
/// @param string The string to look for
/// @param stringCollection The collection of strings to search
bool axrContainsString(
    const char* string,
    const std::vector<std::string>& stringCollection
);

/// Check if the given string is empty
/// @param string String to check
/// @returns True if the given string is empty
bool axrStringIsEmpty(const char* string);