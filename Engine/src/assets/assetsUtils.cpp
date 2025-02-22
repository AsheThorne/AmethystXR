// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <fstream>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "assetsUtils.hpp"
#include "axr/logger.h"

#ifdef AXR_USE_PLATFORM_WIN32
// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>
#endif

AxrResult axrReadFileBytes(const std::filesystem::path& path, std::vector<char>& data) {
    const std::filesystem::path fullPath = axrGetAbsolutePathToAsset(path);

    // Open the file at the end so we can get the file size easily
    std::ifstream file(fullPath.c_str(), std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        axrLogErrorLocation("Failed to open file");
        return AXR_ERROR;
    }

    data = std::vector<char>(file.tellg());
    file.seekg(0);
    file.read(data.data(), static_cast<std::streamsize>(data.size()));
    file.close();

    return AXR_SUCCESS;
}

std::filesystem::path axrGetAbsolutePathToAsset(const std::filesystem::path& path) {
    if (path.is_absolute()) {
        return path;
    }

    // We can assume any path that isn't absolute, is meant to point to the application 'assets' folder.
    // Engine defined asset paths are all absolute and will already point to the 'axr-assets' folder
    return axrGetAssetsDirectoryPath().append(path.c_str());
}

std::filesystem::path axrGetAssetsDirectoryPath() {
    return axrGetExecutableFileDirectoryPath().append("assets");
}

std::filesystem::path axrGetEngineAssetsDirectoryPath() {
    return axrGetExecutableFileDirectoryPath().append("axr-assets");
}

std::filesystem::path axrGetExecutableFileDirectoryPath() {
    std::wstring filePath;

#ifdef AXR_USE_PLATFORM_WIN32
    filePath.resize(MAX_PATH);
    GetModuleFileNameW(nullptr, filePath.data(), MAX_PATH);
    // Remove the executable file from the path
    filePath = filePath.substr(0, filePath.find_last_of(L"\\/") + 1);
#else
    axrLogErrorLocation("Unknown platform.")
    return {};
#endif

    return filePath;
}
