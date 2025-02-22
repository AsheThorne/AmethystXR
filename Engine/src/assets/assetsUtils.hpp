#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <filesystem>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/result.h"

/// Read file bytes on disk
/// @param path Path to the file
/// @param data Output file data
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrReadFileBytes(const std::filesystem::path& path, std::vector<char>& data);

/// Get the absolute file path for the given path to the assets directory
/// @param path Path to use
/// @returns The absolute path
std::filesystem::path axrGetAbsolutePathToAsset(const std::filesystem::path& path);
/// Get the 'assets' directory path
/// @returns Path to the 'assets' directory
std::filesystem::path axrGetAssetsDirectoryPath();
/// Get the 'axr-assets' directory path
/// @returns Path to the 'axr-assets' directory
std::filesystem::path axrGetEngineAssetsDirectoryPath();
/// Get the directory where the executable file sits
/// @returns Path to the executable file directory
std::filesystem::path axrGetExecutableFileDirectoryPath();