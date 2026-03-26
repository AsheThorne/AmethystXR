#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../common/containers/vector_dynamic.h"
#include "vulkanExtensions.h"
#include "vulkanImage.h"
#include "vulkanQueueFamilies.h"

#ifdef AXR_VULKAN_SUPPORTED
#include <vulkan/vulkan_core.h>
#endif

#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Forward Declarations
// ----------------------------------------- //

/// Vulkan renderer context
struct AxrVulkanRendererContext;

// ---------------------------------------------------------------------------------- //
//                        Vulkan Desktop Environment Context                          //
// ---------------------------------------------------------------------------------- //

/// Vulkan renderer desktop environment swapchain context
struct AxrVulkanRendererDesktopSwapchainContext {
    AxrVector_Dynamic<VkImage> ColorImages = AxrVector_Dynamic<VkImage>();
    AxrVector_Dynamic<VkImageView> ColorImageViews = AxrVector_Dynamic<VkImageView>();
    AxrVector_Dynamic<AxrVulkanImage> DepthImages = AxrVector_Dynamic<AxrVulkanImage>();
    AxrVector_Dynamic<AxrVulkanImage> MsaaImages = AxrVector_Dynamic<AxrVulkanImage>();
    VkSwapchainKHR Swapchain = VK_NULL_HANDLE;
    VkExtent2D Extent = {};
    VkFormat ColorFormat = VK_FORMAT_UNDEFINED;
    VkFormat DepthFormat = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR ColorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;
    AxrVulkanPresentationModeEnum PreferredPresentationMode = AXR_VULKAN_PRESENTATION_MODE_UNDEFINED;
    VkPresentModeKHR PresentationMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
};

/// Vulkan renderer desktop environment context
struct AxrVulkanRendererDesktopContext {
    const AxrVulkanQueueFamilies* QueueFamilies = nullptr;
    AxrVector_Dynamic<VkSemaphore> ImageAvailableSemaphores = AxrVector_Dynamic<VkSemaphore>();
    AxrVector_Dynamic<VkSemaphore> RenderingFinishedSemaphores = AxrVector_Dynamic<VkSemaphore>();
    AxrVector_Dynamic<VkFence> RenderingFences = AxrVector_Dynamic<VkFence>();
    AxrVector_Dynamic<VkCommandBuffer> RenderingCommandBuffers = AxrVector_Dynamic<VkCommandBuffer>();
    AxrVector_Dynamic<VkFramebuffer> Framebuffers = AxrVector_Dynamic<VkFramebuffer>();
    AxrVulkanRendererDesktopSwapchainContext SwapchainContext{};
    VkInstance Instance = VK_NULL_HANDLE;
    VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
    VkDevice Device = VK_NULL_HANDLE;
    VkCommandPool GraphicsCommandPool = VK_NULL_HANDLE;
    VkSurfaceKHR Surface = VK_NULL_HANDLE;
    VkRenderPass RenderPass = VK_NULL_HANDLE;
    VkSampleCountFlagBits MsaaSampleCount = VK_SAMPLE_COUNT_1_BIT;
    bool IsSwapchainOutOfDate = false;
    bool IsSetup = false;
};

#endif

// ---------------------------------------------------------------------------------- //
//                               Vulkan Render Context                                //
// ---------------------------------------------------------------------------------- //

#ifndef AXR_VULKAN_SUPPORTED
/// Vulkan renderer context
struct AxrVulkanRendererContext {};
#else
/// Vulkan renderer context
struct AxrVulkanRendererContext {
    AxrVulkanRendererDesktopContext DesktopEnvironmentContext{};
    AxrVulkanExtensions::ApiLayersArray_T ApiLayers{};
    AxrVulkanExtensions::ExtensionsArray_T Extensions{};
    AxrVulkanQueueFamilies QueueFamilies{};
    VkInstance Instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT DebugUtilsMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
    VkDevice Device = VK_NULL_HANDLE;
    VkCommandPool GraphicsCommandPool = VK_NULL_HANDLE;
    VkCommandPool TransferCommandPool = VK_NULL_HANDLE;
    VkPhysicalDeviceMultiviewFeatures EnabledDeviceMultiviewFeatures{};
    VkPhysicalDeviceFeatures EnabledDeviceFeatures{};
    bool IsSetup = false;
};
#endif

// ---------------------------------------------------------------------------------- //
//                               Vulkan Render Surface                                //
// ---------------------------------------------------------------------------------- //

#ifndef AXR_VULKAN_SUPPORTED
/// Vulkan render surface
struct AxrVulkanRenderSurface {};
#else
/// Vulkan render surface
struct AxrVulkanRenderSurface {};
#endif
