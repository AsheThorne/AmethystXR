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
std::wstring axrToWString(const std::string& string);

/// Check if a string is within the given collection
/// @param string The string to look for
/// @param stringCollection The collection of strings to search
bool axrContainsString(
    const std::string& string,
    const std::vector<std::string>& stringCollection
);
