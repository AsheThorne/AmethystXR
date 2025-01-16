#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <source_location>

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>
#endif

// ----------------------------------------- //
// Function Definitions
// ----------------------------------------- //

// ---- String Utils ----

/// Convert a const char* to a std::wstring
/// @param string String to convert
/// @returns Converted string
std::wstring axrToWString(const char* string);

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ---- Vulkan Utils ----

/// Check if the vulkan result succeeded
/// @param result Vulkan result
/// @returns True if the result succeeded
bool axrVkSucceeded(vk::Result result);

/// Check if the vulkan result failed
/// @param result Vulkan result
/// @returns True if the result failed
bool axrVkFailed(vk::Result result);

/// Log a vulkan result if it failed
/// @param result Vulkan result
/// @param functionName The name of the function that gave the result
/// @param location Source file location. You don't need to ever change this from the default
void axrLogVkResult(
    vk::Result result,
    const char* functionName,
    const std::source_location& location = std::source_location::current()
);

#endif
