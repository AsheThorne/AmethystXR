#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/logging.h"

#ifdef AXR_VULKAN_SUPPORTED
#include <vulkan/vulkan.h>
#endif

#define AXR_VULKAN_CORE_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

#ifdef AXR_VULKAN_SUPPORTED
// ---------------------------------------------------------------------------------- //
//                                     Api Layers                                     //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Vulkan Api Layer Type Enum
enum AxrVulkanApiLayerTypeEnum {
    AXR_VULKAN_API_LAYER_TYPE_UNDEFINED = 0,
    AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION,
    /// This value is used to mark the end of the enum.
    /// It's useful for knowing the max number of api layer types.
    AXR_VULKAN_API_LAYER_TYPE_END
};

constexpr uint32_t AxrVulkanApiLayerMaxCount = AXR_VULKAN_API_LAYER_TYPE_END - 1;

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Vulkan Api Layer Core Validation
struct AxrVulkanApiLayerCoreValidation {
    // Empty on purpose. No data is needed for this api layer
};

/// Vulkan api layer
struct AxrVulkanApiLayer {
    union {
        AxrVulkanApiLayerCoreValidation CoreValidation;
    };
    AxrVulkanApiLayerTypeEnum Type;
};

/// Axr vulkan api layer properties
struct AxrVulkanApiLayerProperties {
    char Name[VK_MAX_EXTENSION_NAME_SIZE];
};

// ----------------------------------------- //
// Properties
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to get vulkan api layer properties. "
/// Get the properties for the given vulkan api layer type
/// @param apiLayerType Api layer type
/// @return Vulkan api layer properties
inline AxrVulkanApiLayerProperties AxrVulkanApiLayerGetProperties(const AxrVulkanApiLayerTypeEnum apiLayerType) {
    switch (apiLayerType) {
        case AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION: {
            return AxrVulkanApiLayerProperties{
                .Name = AXR_VULKAN_CORE_VALIDATION_LAYER_NAME,
            };
        }
        case AXR_VULKAN_API_LAYER_TYPE_END:
        case AXR_VULKAN_API_LAYER_TYPE_UNDEFINED:
        default: {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Unknown vulkan api layer type.");
            return AxrVulkanApiLayerProperties{};
        }
    }
}
#undef AXR_FUNCTION_FAILED_STRING

// ---------------------------------------------------------------------------------- //
//                                     Extensions                                     //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Vulkan Extension Type Enum
enum AxrVulkanExtensionTypeEnum {
    AXR_VULKAN_EXTENSION_TYPE_UNDEFINED = 0,
    AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS,
    AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN,
    AXR_VULKAN_EXTENSION_TYPE_SURFACE,
    // TODO: We probably need the linux one too
    AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE,
    /// This value is used to mark the end of the enum.
    /// It's useful for knowing the max number of extension types.
    AXR_VULKAN_EXTENSION_TYPE_END
};

constexpr uint32_t AxrVulkanExtensionMaxCount = AXR_VULKAN_EXTENSION_TYPE_END - 1;

/// Vulkan Extension Level Enum
enum AxrVulkanExtensionLevelEnum {
    AXR_VULKAN_EXTENSION_LEVEL_UNDEFINED = 0,
    AXR_VULKAN_EXTENSION_LEVEL_INSTANCE,
    AXR_VULKAN_EXTENSION_LEVEL_DEVICE,
};

// Vulkan Extension Debug Utils Severity Flag Bits. See: VkDebugUtilsMessageSeverityFlagBitsEXT
enum AxrVulkanDebugUtilsSeverityFlagBits {
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_VERBOSE_BIT = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_INFO_BIT = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_WARNING_BIT = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_ERROR_BIT = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
};

/// Vulkan Extension Debug Utils Severity Flags Type. See: VkDebugUtilsMessageSeverityFlagsEXT
typedef VkDebugUtilsMessageSeverityFlagsEXT AxrVulkanDebugUtilsSeverityFlags_T;

// Vulkan Extension Debug Utils Type Flag Bits. See: VkDebugUtilsMessageTypeFlagBitsEXT
enum AxrVulkanDebugUtilsTypeFlagBits {
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_GENERAL_BIT = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_VALIDATION_BIT = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_PERFORMANCE_BIT = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_DEVICE_ADDRESS_BINDING_BIT =
        VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
};

/// Vulkan Extension Debug Utils Type Flags Type. See: VkDebugUtilsMessageTypeFlagsEXT
typedef VkDebugUtilsMessageTypeFlagsEXT AxrVulkanDebugUtilsTypeFlags_T;

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Vulkan Extension Debug Utils
struct AxrVulkanExtensionDebugUtils {
    AxrVulkanDebugUtilsSeverityFlags_T SeverityFlags;
    AxrVulkanDebugUtilsTypeFlags_T TypeFlags;
};

/// Vulkan Extension Swapchain
struct AxrVulkanExtensionSwapchain {
    // Empty on purpose. No data is needed for this extension
};

/// Vulkan Extension Surface
struct AxrVulkanExtensionSurface {
    // Empty on purpose. No data is needed for this extension
};

/// Vulkan Extension Win32 Surface
struct AxrVulkanExtensionWin32Surface {
    // Empty on purpose. No data is needed for this extension
};

/// Vulkan api layer
struct AxrVulkanExtension {
    union {
        AxrVulkanExtensionDebugUtils DebugUtils;
        AxrVulkanExtensionSwapchain Swapchain;
        AxrVulkanExtensionSurface Surface;
        AxrVulkanExtensionWin32Surface Win32Surface;
    };
    AxrVulkanExtensionTypeEnum Type;
    bool IsRequired;
};

/// Axr vulkan extension properties
struct AxrVulkanExtensionProperties {
    char Name[VK_MAX_EXTENSION_NAME_SIZE];
    AxrVulkanExtensionLevelEnum Level;
};

// ----------------------------------------- //
// Properties
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to get vulkan extension properties. "
/// Get the properties for the given vulkan extension type
/// @param extensionType Extension type
/// @return Vulkan extension properties
inline AxrVulkanExtensionProperties AxrVulkanExtensionGetProperties(const AxrVulkanExtensionTypeEnum extensionType) {
    switch (extensionType) {
        case AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS: {
            return AxrVulkanExtensionProperties{
                .Name = VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                .Level = AXR_VULKAN_EXTENSION_LEVEL_INSTANCE,
            };
        }
        case AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN: {
            return AxrVulkanExtensionProperties{
                .Name = VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                .Level = AXR_VULKAN_EXTENSION_LEVEL_DEVICE,
            };
        }
        case AXR_VULKAN_EXTENSION_TYPE_SURFACE: {
            return AxrVulkanExtensionProperties{
                .Name = VK_KHR_SURFACE_EXTENSION_NAME,
                .Level = AXR_VULKAN_EXTENSION_LEVEL_INSTANCE,
            };
        }
        case AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE: {
#ifdef VK_USE_PLATFORM_WIN32_KHR
            return AxrVulkanExtensionProperties{
                .Name = VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
                .Level = AXR_VULKAN_EXTENSION_LEVEL_INSTANCE,
            };
#else
            axrLogError(AXR_FUNCTION_FAILED_STRING "Win32 surface extension not supported.");
            return AxrVulkanExtensionProperties{};
#endif
        }
        case AXR_VULKAN_EXTENSION_TYPE_END:
        case AXR_VULKAN_EXTENSION_TYPE_UNDEFINED:
        default: {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Unknown vulkan extension type.");
            return AxrVulkanExtensionProperties{};
        }
    }
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to get vulkan extension type. "
/// Get the vulkan extension type for the given vulkan extension name
/// @param extensionName Extension name
/// @return Vulkan extension type
inline AxrVulkanExtensionTypeEnum AxrVulkanExtensionGetType(const char extensionName[VK_MAX_EXTENSION_NAME_SIZE]) {
    if (strcmp(extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0) {
        return AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS;
    }

    if (strcmp(extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
        return AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN;
    }

    if (strcmp(extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0) {
        return AXR_VULKAN_EXTENSION_TYPE_SURFACE;
    }

#ifdef VK_USE_PLATFORM_WIN32_KHR
    if (strcmp(extensionName, VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0) {
        return AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE;
    }
#endif

    axrLogError(AXR_FUNCTION_FAILED_STRING "Unknown vulkan extension name.");
    return AXR_VULKAN_EXTENSION_TYPE_UNDEFINED;
}
#undef AXR_FUNCTION_FAILED_STRING

#endif

// ---------------------------------------------------------------------------------- //
//                                       Config                                       //
// ---------------------------------------------------------------------------------- //

#ifndef AXR_VULKAN_SUPPORTED
/// Vulkan renderer config
struct AxrVulkanRendererConfig {};
#else
/// Vulkan renderer config
struct AxrVulkanRendererConfig {
    AxrVulkanApiLayer ApiLayers[AxrVulkanApiLayerMaxCount];
    AxrVulkanExtension Extensions[AxrVulkanExtensionMaxCount];
    uint32_t ApiLayerCount;
    uint32_t ExtensionCount;
};
#endif
