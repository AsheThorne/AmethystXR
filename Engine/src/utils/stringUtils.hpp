#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

/// Convert a const char* to a std::wstring
/// @param string String to convert
/// @returns Converted string
std::wstring axrToWString(const char* string);
