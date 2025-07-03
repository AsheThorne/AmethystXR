#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/xrSystem.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <source_location>

// ----------------------------------------- //
// OpenXR Headers
// ----------------------------------------- //
#include <openxr/openxr.h>

// ---- Logging ----

/// Log an XrResult if it failed
/// @param xrResult Xr result
/// @param functionName The name of the function that gave the result
/// @param location Source file location. You don't need to ever change this from the default
void axrLogXrResult(
    XrResult xrResult,
    const char* functionName,
    const std::source_location& location = std::source_location::current()
);

/// Convert an XrResult to a string
/// @param xrResult The XrResult
/// @returns The XrResult as a string
const char* axrToString(XrResult xrResult);

// ---- Api Layers / Extensions ----

/// Get the api layer name from the given xr api layer type
/// @param apiLayerType Xr api layer type
/// @returns The api layer name
const char* axrGetXrApiLayerName(AxrXrApiLayerTypeEnum apiLayerType);
/// Get the extension name from the given xr extension type
/// @param extensionType Xr extension type
/// @returns The extension name
const char* axrGetXrExtensionName(AxrXrExtensionTypeEnum extensionType);

// ---- Space ----

/// Convert an AxrXrReferenceSpaceEnum to an XrReferenceSpaceType
/// @param referenceSpace The AxrXrReferenceSpaceEnum
/// @returns The AxrXrReferenceSpaceEnum as an XrReferenceSpaceType
XrReferenceSpaceType axrToReferenceSpace(AxrXrReferenceSpaceEnum referenceSpace);
