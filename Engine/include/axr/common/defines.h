#pragma once

// ----------------------------------------- //
// AXR Macros
// ----------------------------------------- //
#ifdef AXR_BUILD_DLL
#define AXR_API __declspec(dllexport)
#else
#define AXR_API __declspec(dllimport)
#endif

#define AXR_SUCCEEDED(result) ((result) >= 0)
#define AXR_FAILED(result) ((result) < 0)

#define AXR_MAKE_VERSION(major, minor, patch) \
    ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

#define AXR_VERSION_MAJOR(version) (((uint32_t)(version) >> 22U) & 0x7FU)
#define AXR_VERSION_MINOR(version) (((uint32_t)(version) >> 12U) & 0x3FFU)
#define AXR_VERSION_PATCH(version) ((uint32_t)(version) & 0xFFFU)

// ----------------------------------------- //
// AXR Engine Data
// ----------------------------------------- //
inline const char* AxrEngineName = "AmethystXR";
#define AXR_ENGINE_VERSION AXR_MAKE_VERSION(@AmethystXR_V1_VERSION_MAJOR @, @AmethystXR_V1_VERSION_MINOR @, @AmethystXR_V1_VERSION_PATCH @)

// ----------------------------------------- //
// Vulkan Macros
// ----------------------------------------- //
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#define VK_SUCCEEDED(result) ((result) >= 0)
#define VK_FAILED(result) ((result) < 0)
#endif
