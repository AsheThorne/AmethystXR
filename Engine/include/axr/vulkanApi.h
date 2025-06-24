#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan_core.h>

// ---------------------------------------------------------------------------------- //
//                                     Api Layers                                     //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Vulkan Api Layer Type
enum AxrVulkanApiLayerTypeEnum {
    AXR_VULKAN_API_LAYER_TYPE_UNDEFINED = 0,
    AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION,
    /// This value is used to mark the end of the enum.
    /// It's useful for knowing the max number of api layer types.
    AXR_VULKAN_API_LAYER_TYPE_END
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Vulkan Api Layer Base Structure
struct AxrVulkanApiLayerStructure {
    AxrVulkanApiLayerTypeEnum Type = AXR_VULKAN_API_LAYER_TYPE_UNDEFINED;
};

/// AxrVulkanApiLayerStructure Handle Type
typedef AxrVulkanApiLayerStructure* AxrVulkanApiLayer_T;

/// Vulkan Api Layer Core Validation Structure
struct AxrVulkanApiLayerCoreValidation {
    const AxrVulkanApiLayerTypeEnum Type = AXR_VULKAN_API_LAYER_TYPE_CORE_VALIDATION;
};

// ---------------------------------------------------------------------------------- //
//                                     Extensions                                     //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Vulkan Extension Type
enum AxrVulkanExtensionTypeEnum {
    AXR_VULKAN_EXTENSION_TYPE_UNDEFINED = 0,
    AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS,
    AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN,
    AXR_VULKAN_EXTENSION_TYPE_SURFACE,
    AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE,
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

/// Vulkan Extension Base Structure
struct AxrVulkanExtensionStructure {
    AxrVulkanExtensionTypeEnum Type = AXR_VULKAN_EXTENSION_TYPE_UNDEFINED;
    AxrVulkanExtensionLevelEnum Level = AXR_VULKAN_EXTENSION_LEVEL_UNDEFINED;
};

/// AxrVulkanExtensionStructure Handle Type
typedef AxrVulkanExtensionStructure* AxrVulkanExtension_T;

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
#ifdef AXR_USE_PLATFORM_WIN32
/// Vulkan Extension Win32 Surface
struct AxrVulkanExtensionWin32Surface {
    const AxrVulkanExtensionTypeEnum Type = AXR_VULKAN_EXTENSION_TYPE_WIN32_SURFACE;
    const AxrVulkanExtensionLevelEnum Level = AXR_VULKAN_EXTENSION_LEVEL_INSTANCE;
};
#endif

// ---------------------------------------------------------------------------------- //
//                                   Vulkan Config                                    //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Vulkan Presentation Mode Enum
enum AxrVulkanPresentationModeEnum {
    AXR_VULKAN_PRESENTATION_MODE_UNDEFINED = 0,
    AXR_VULKAN_PRESENTATION_MODE_IMMEDIATE,
    AXR_VULKAN_PRESENTATION_MODE_MAILBOX,
    AXR_VULKAN_PRESENTATION_MODE_FIFO,
    AXR_VULKAN_PRESENTATION_MODE_FIFO_RELAXED,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Vulkan Window Graphics Config
struct AxrVulkanWindowConfig {
    AxrVulkanPresentationModeEnum PresentationMode;
};

/// Vulkan Api Graphics System Config
struct AxrVulkanApiConfig {
    uint32_t ApiLayerCount;
    AxrVulkanApiLayer_T* ApiLayers;
    uint32_t ExtensionCount;
    AxrVulkanExtension_T* Extensions;
    AxrVulkanWindowConfig* WindowConfig;
};

/// AxrVulkanApiConfig Handle
typedef AxrVulkanApiConfig* AxrVulkanApiConfig_T;

#endif
