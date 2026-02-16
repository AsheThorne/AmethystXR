#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanQueueFamilies.h"

#include <vulkan/vulkan_core.h>

/// Vulkan environment (XR System, Desktop) related functions
class AxrVulkanEnvironment {
public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    struct SetupConfig {
        VkInstance Instance;
        VkPhysicalDevice PhysicalDevice;
        VkDevice Device;
        VkCommandPool GraphicsCommandPool;
        const AxrVulkanQueueFamilies& QueueFamilies;
        /// Ordered from most desired to the least desired
        const AxrVector_Stack<VkFormat>& SwapchainColorFormatOptions;
        /// Ordered from most desired to the least desired
        const AxrVector_Stack<VkFormat>& SwapchainDepthFormatOptions;
    };

    struct DesktopContext {
        VkInstance Instance = VK_NULL_HANDLE;
        VkSurfaceKHR Surface = VK_NULL_HANDLE;
        VkFormat SwapchainColorFormat = VK_FORMAT_UNDEFINED;
        VkFormat SwapchainDepthFormat = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR SwapchainColorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrVulkanEnvironment() = delete;
    /// Copy Constructor
    /// @param src Source AxrVulkanEnvironment to copy from
    AxrVulkanEnvironment(const AxrVulkanEnvironment& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanEnvironment to move from
    AxrVulkanEnvironment(AxrVulkanEnvironment&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanEnvironment() = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanEnvironment to copy from
    AxrVulkanEnvironment& operator=(const AxrVulkanEnvironment& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanEnvironment to move from
    AxrVulkanEnvironment& operator=(AxrVulkanEnvironment&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the desktop environment context
    /// @param config Setup config
    /// @param context Output desktop environment context
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setupDesktopContext(const SetupConfig& config, DesktopContext& context);
    /// Destroy the given desktop environment context
    /// @param context Desktop environment context to destroy
    static void destroyDesktopContext(DesktopContext& context);

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    // ---- Format ----

    /// Set the color and depth swapchain formats to use for the desktop environment
    /// @param surface Surface to use
    /// @param physicalDevice Physical device to use
    /// @param swapchainColorFormatOptions Swapchain color format options to choose from. Ordered from most desired to
    /// the least desired
    /// @param swapchainDepthFormatOptions Swapchain depth format options to choose from. Ordered from most desired to
    /// the least desired
    /// @param colorFormat Output color format
    /// @param colorSpace Output color space
    /// @param depthFormat Output depth format
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setDesktopSwapchainFormats(
        const VkSurfaceKHR& surface,
        const VkPhysicalDevice& physicalDevice,
        const AxrVector_Stack<VkFormat>& swapchainColorFormatOptions,
        const AxrVector_Stack<VkFormat>& swapchainDepthFormatOptions,
        VkFormat& colorFormat,
        VkColorSpaceKHR& colorSpace,
        VkFormat& depthFormat);

    /// Get the supported formats for the given surface
    /// @param surface Surface to use
    /// @param physicalDevice Physical device to use
    /// @param supportedFormats Output supported formats
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult getSupportedSurfaceFormats(const VkSurfaceKHR& surface,
                                                              const VkPhysicalDevice& physicalDevice,
                                                              AxrVector_Stack<VkSurfaceFormatKHR>& supportedFormats);

    /// Set the color and depth swapchain formats to use
    /// @param physicalDevice Physical device to use
    /// @param swapchainColorFormatOptions Swapchain color format options to choose from. Ordered from most desired to
    /// the least desired
    /// @param swapchainDepthFormatOptions Swapchain depth format options to choose from. Ordered from most desired to
    /// the least desired
    /// @param supportedSwapchainFormats Supported swapchain formats
    /// @param colorFormat Output color format
    /// @param depthFormat Output depth format
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setSwapchainFormats(const VkPhysicalDevice& physicalDevice,
                                                       const AxrVector_Stack<VkFormat>& swapchainColorFormatOptions,
                                                       const AxrVector_Stack<VkFormat>& swapchainDepthFormatOptions,
                                                       const AxrVector_Stack<VkFormat>& supportedSwapchainFormats,
                                                       VkFormat& colorFormat,
                                                       VkFormat& depthFormat);

    /// Check if the given format supports the given features
    /// @param format Format to check against
    /// @param imageTiling Image tiling option
    /// @param features Features to check
    /// @param physicalDevice Physical device to use
    /// @returns True if the given format supports the given features
    [[nodiscard]] static bool areFormatFeaturesSupported(VkFormat format,
                                                         VkImageTiling imageTiling,
                                                         VkFormatFeatureFlags features,
                                                         const VkPhysicalDevice& physicalDevice);
};
