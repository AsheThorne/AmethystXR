#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan_core.h>

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

// ---- Api Layer Enums ----

/// Vulkan Api Layer Type
enum AxrVulkanApiLayerTypeEnum {
    AXR_VULKAN_API_LAYER_TYPE_UNDEFINED = 0,
    AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION = 1,
    /// This value is used to mark the end of the enum.
    /// It's useful for knowing the max number of api layer types.
    AXR_VULKAN_API_LAYER_TYPE_END
};

// ---- Extension Enums ----

/// Vulkan Extension Type
enum AxrVulkanExtensionTypeEnum {
    AXR_VULKAN_EXTENSION_TYPE_UNDEFINED = 0,
    AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS = 1,
    AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN = 2,
    AXR_VULKAN_EXTENSION_TYPE_SURFACE = 3,
    AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE = 4,
    /// This value is used to mark the end of the enum.
    /// It's useful for knowing the max number of extension types.
    AXR_VULKAN_EXTENSION_TYPE_END
};

/// Vulkan Extension Level
enum AxrVulkanExtensionLevelEnum {
    AXR_VULKAN_EXTENSION_LEVEL_UNDEFINED = 0,
    AXR_VULKAN_EXTENSION_LEVEL_INSTANCE = 1,
    AXR_VULKAN_EXTENSION_LEVEL_DEVICE = 2,
};

// Vulkan Extension Debug Utils Severity Flag Bits. See: VkDebugUtilsMessageSeverityFlagBitsEXT
enum AxrVulkanDebugUtilsSeverityFlagBits {
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_VERBOSE_BIT = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_INFO_BIT = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_WARNING_BIT = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_SEVERITY_ERROR_BIT = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
};

/// Vulkan Extension Debug Utils Severity Flags Type
typedef VkDebugUtilsMessageSeverityFlagsEXT AxrVulkanDebugUtilsSeverityFlags_T;

// Vulkan Extension Debug Utils Type Flag Bits. See: VkDebugUtilsMessageTypeFlagBitsEXT
enum AxrVulkanDebugUtilsTypeFlagBits {
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_GENERAL_BIT = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_VALIDATION_BIT = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_PERFORMANCE_BIT = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    AXR_VULKAN_EXTENSION_DEBUG_UTILS_TYPE_DEVICE_ADDRESS_BINDING_BIT =
    VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
};

/// Vulkan Extension Debug Utils Type Flags Type
typedef VkDebugUtilsMessageTypeFlagsEXT AxrVulkanDebugUtilsTypeFlags_T;


// ----------------------------------------- //
// Structs
// ----------------------------------------- //

// ---- Api Layer Structs ----

/// Vulkan Api Layer Base Structure
struct AxrVulkanApiLayerStructure {
    AxrVulkanApiLayerTypeEnum Type = AXR_VULKAN_API_LAYER_TYPE_UNDEFINED;
};

/// AxrVulkanApiLayerStructure Handle Type
using AxrVulkanApiLayer_T = AxrVulkanApiLayerStructure*;

/// Vulkan Api Layer Core Validation Structure
struct AxrVulkanApiLayerCoreValidation {
    const AxrVulkanApiLayerTypeEnum Type = AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION;
};

// ---- Extension Structs ----

/// Vulkan Extension Base Structure
struct AxrVulkanExtensionStructure {
    AxrVulkanExtensionTypeEnum Type = AXR_VULKAN_EXTENSION_TYPE_UNDEFINED;
    AxrVulkanExtensionLevelEnum Level = AXR_VULKAN_EXTENSION_LEVEL_UNDEFINED;
};

/// AxrVulkanExtensionStructure Handle Type
using AxrVulkanExtension_T = AxrVulkanExtensionStructure*;

/// Vulkan Extension Debug Utils
struct AxrVulkanExtensionDebugUtils {
    const AxrVulkanExtensionTypeEnum Type = AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS;
    const AxrVulkanExtensionLevelEnum Level = AXR_VULKAN_EXTENSION_LEVEL_INSTANCE;
    AxrVulkanDebugUtilsSeverityFlags_T SeverityFlags;
    AxrVulkanDebugUtilsTypeFlags_T TypeFlags;
};

/// Vulkan Extension Swapchain
struct AxrVulkanExtensionSwapchain {
    const AxrVulkanExtensionTypeEnum Type = AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN;
    const AxrVulkanExtensionLevelEnum Level = AXR_VULKAN_EXTENSION_LEVEL_DEVICE;
};

/// Vulkan Extension Surface
struct AxrVulkanExtensionSurface {
    const AxrVulkanExtensionTypeEnum Type = AXR_VULKAN_EXTENSION_TYPE_SURFACE;
    const AxrVulkanExtensionLevelEnum Level = AXR_VULKAN_EXTENSION_LEVEL_INSTANCE;
};

// TODO: Should this be guarded by the AXR_PLATFORM_WIN32 preprocessor? 
/// Vulkan Extension Win32 Surface
struct AxrVulkanExtensionWin32Surface {
    const AxrVulkanExtensionTypeEnum Type = AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE;
    const AxrVulkanExtensionLevelEnum Level = AXR_VULKAN_EXTENSION_LEVEL_INSTANCE;
};

// ---- Vulkan Config ----

/// Vulkan Api Graphics System Config
struct AxrVulkanApiConfig {
    uint32_t ApiLayersCount;
    AxrVulkanApiLayer_T* ApiLayers;
    uint32_t ExtensionsCount;
    AxrVulkanExtension_T* Extensions;
};

// TODO: Change all `using` to `typedef` like this one in the plain c headers
/// Vulkan Api Config Type
typedef AxrVulkanApiConfig* AxrVulkanApiConfig_T;

#endif
