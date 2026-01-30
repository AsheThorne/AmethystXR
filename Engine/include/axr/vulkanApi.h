#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#ifdef AXR_VULKAN_SUPPORTED
#include <vulkan/vulkan_core.h>
#endif

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

// ---------------------------------------------------------------------------------- //
//                                     Extensions                                     //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Vulkan Extension Type Enum
enum AxrVulkanExtensionTypeEnum {
    AXR_VULKAN_EXTENSION_TYPE_UNDEFINED = 0,
    /// Level: AXR_VULKAN_EXTENSION_LEVEL_INSTANCE
    AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS,
    /// Level: AXR_VULKAN_EXTENSION_LEVEL_DEVICE
    AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN,
    /// Level: AXR_VULKAN_EXTENSION_LEVEL_INSTANCE
    AXR_VULKAN_EXTENSION_TYPE_SURFACE,
    /// Level: AXR_VULKAN_EXTENSION_LEVEL_INSTANCE
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
    AxrVulkanExtensionLevelEnum Level;
    bool IsRequired;
};
#endif

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
