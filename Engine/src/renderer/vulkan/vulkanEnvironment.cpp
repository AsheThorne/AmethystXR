#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanEnvironment.h"

#include "../../memory/allocator.h"
#include "../../platform/platform.h"
#include "vulkanUtils.h"

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to set up desktop context. "
AxrResult AxrVulkanEnvironment::setupDesktopContext(const SetupConfig& config, DesktopContext& context) {
    context.Instance = config.Instance;
    context.Device = config.Device;

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = AxrPlatform::get().createVulkanSurface(config.Instance, context.Surface);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyDesktopContext(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create surface.");
        return axrResult;
    }

    axrResult = setDesktopSwapchainFormats(context.Surface,
                                           config.PhysicalDevice,
                                           config.SwapchainColorFormatOptions,
                                           config.SwapchainDepthFormatOptions,
                                           context.SwapchainColorFormat,
                                           context.SwapchainColorSpace,
                                           context.SwapchainDepthFormat);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyDesktopContext(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to set desktop swapchain formats.");
        return axrResult;
    }

    axrResult = createRenderPass(config.Device,
                                 context.SwapchainColorFormat,
                                 context.SwapchainDepthFormat,
                                 context.MsaaSampleCount,
                                 context.RenderPass);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyDesktopContext(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create render pass.");
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::destroyDesktopContext(DesktopContext& context) {
    destroyRenderPass(context.Device, context.RenderPass);
    resetDesktopSwapchainFormats(context.SwapchainColorFormat,
                                 context.SwapchainColorSpace,
                                 context.SwapchainDepthFormat);
    AxrPlatform::get().destroyVulkanSurface(context.Instance, context.Surface);
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to set desktop swapchain formats. "
AxrResult AxrVulkanEnvironment::setDesktopSwapchainFormats(const VkSurfaceKHR& surface,
                                                           const VkPhysicalDevice& physicalDevice,
                                                           const AxrVector_Stack<VkFormat>& swapchainColorFormatOptions,
                                                           const AxrVector_Stack<VkFormat>& swapchainDepthFormatOptions,
                                                           VkFormat& colorFormat,
                                                           VkColorSpaceKHR& colorSpace,
                                                           VkFormat& depthFormat) {
    AxrVector_Stack<VkSurfaceFormatKHR> supportedSurfaceFormats;
    AxrResult axrResult = getSupportedSurfaceFormats(surface, physicalDevice, supportedSurfaceFormats);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported surface formats.");
        return axrResult;
    }

    AxrVector_Stack<VkFormat> supportedFormats(supportedSurfaceFormats.size() + swapchainDepthFormatOptions.size(),
                                               &AxrAllocator::get().FrameAllocator);
    for (const VkSurfaceFormatKHR& surfaceFormat : supportedSurfaceFormats) {
        supportedFormats.pushBack(surfaceFormat.format);
    }

    // There's no such thing as 'supported depth formats' for the desktop the same way there are supported color
    // formats. So we just add all depth formats.
    for (VkFormat format : swapchainDepthFormatOptions) {
        supportedFormats.pushBack(format);
    }

    axrResult = setSwapchainFormats(physicalDevice,
                                    swapchainColorFormatOptions,
                                    swapchainDepthFormatOptions,
                                    supportedFormats,
                                    colorFormat,
                                    depthFormat);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported surface formats.");
        return axrResult;
    }

    // Set the output color space to the same as the one that the chosen supported color format uses
    for (const VkSurfaceFormatKHR& supportedSurfaceFormat : supportedSurfaceFormats) {
        if (supportedSurfaceFormat.format == colorFormat) {
            colorSpace = supportedSurfaceFormat.colorSpace;
            break;
        }
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::resetDesktopSwapchainFormats(VkFormat& colorFormat,
                                                        VkColorSpaceKHR& colorSpace,
                                                        VkFormat& depthFormat) {
    colorFormat = VK_FORMAT_UNDEFINED;
    colorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;
    depthFormat = VK_FORMAT_UNDEFINED;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to get supported surface formats. "
AxrResult AxrVulkanEnvironment::getSupportedSurfaceFormats(const VkSurfaceKHR& surface,
                                                           const VkPhysicalDevice& physicalDevice,
                                                           AxrVector_Stack<VkSurfaceFormatKHR>& supportedFormats) {
    if (surface == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Surface is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (supportedFormats.allocated()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`supportedFormats` have already been allocated.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    uint32_t surfaceFormatsCount;
    VkResult vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, nullptr);
    axrLogVkResult(vkResult, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    AxrVector_Stack<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatsCount, &AxrAllocator::get().FrameAllocator);
    surfaceFormats.prefillData();
    vkResult =
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, surfaceFormats.data());
    axrLogVkResult(vkResult, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    supportedFormats = std::move(surfaceFormats);
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to set swapchain formats. "
AxrResult AxrVulkanEnvironment::setSwapchainFormats(const VkPhysicalDevice& physicalDevice,
                                                    const AxrVector_Stack<VkFormat>& swapchainColorFormatOptions,
                                                    const AxrVector_Stack<VkFormat>& swapchainDepthFormatOptions,
                                                    const AxrVector_Stack<VkFormat>& supportedSwapchainFormats,
                                                    VkFormat& colorFormat,
                                                    VkFormat& depthFormat) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (swapchainColorFormatOptions.empty()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`swapchainColorFormatOptions` are empty.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (swapchainDepthFormatOptions.empty()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`swapchainDepthFormatOptions` are empty.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (supportedSwapchainFormats.empty()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`supportedSwapchainFormats` are empty.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    // ---- Find the first color format that is supported ----
    VkFormat selectedColorFormat = VK_FORMAT_UNDEFINED;

    // From our list of options, find the first supported color format that supports our desired features
    for (const VkFormat formatOption : swapchainColorFormatOptions) {
        if (!areFormatFeaturesSupported(formatOption,
                                        VK_IMAGE_TILING_OPTIMAL,
                                        VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT |
                                            VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT,
                                        physicalDevice)) {
            continue;
        }

        const auto foundFormatIterator = supportedSwapchainFormats.findFirst(formatOption);
        if (foundFormatIterator != supportedSwapchainFormats.end()) {
            selectedColorFormat = *foundFormatIterator;
            break;
        }
    }

    if (selectedColorFormat == VK_FORMAT_UNDEFINED) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to find a suitable color format to use.");
        return AXR_ERROR_NOT_FOUND;
    }

    // ---- Find the first depth format that is supported ----
    VkFormat selectedDepthFormat = VK_FORMAT_UNDEFINED;

    for (const VkFormat formatOption : swapchainDepthFormatOptions) {
        if (!areFormatFeaturesSupported(formatOption,
                                        VK_IMAGE_TILING_OPTIMAL,
                                        VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT |
                                            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                        physicalDevice)) {
            continue;
        }

        const auto foundFormatIterator = supportedSwapchainFormats.findFirst(formatOption);
        if (foundFormatIterator != supportedSwapchainFormats.end()) {
            selectedDepthFormat = *foundFormatIterator;
            break;
        }
    }

    if (selectedDepthFormat == VK_FORMAT_UNDEFINED) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to find a suitable depth format to use.");
        return AXR_ERROR_NOT_FOUND;
    }

    colorFormat = selectedColorFormat;
    depthFormat = selectedDepthFormat;

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to check supported format features. "
bool AxrVulkanEnvironment::areFormatFeaturesSupported(const VkFormat format,
                                                      const VkImageTiling imageTiling,
                                                      const VkFormatFeatureFlags features,
                                                      const VkPhysicalDevice& physicalDevice) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return false;
    }

    VkFormatProperties properties{};
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

    if (imageTiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
        return true;
    }
    if (imageTiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
        return true;
    }

    return false;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to create render pass. "
AxrResult AxrVulkanEnvironment::createRenderPass(const VkDevice& device,
                                                 const VkFormat colorFormat,
                                                 const VkFormat depthStencilFormat,
                                                 const VkSampleCountFlagBits msaaSampleCount,
                                                 VkRenderPass& renderPass) {
    if (renderPass != VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Render pass already exists.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (device == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Device is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    const bool isMsaaEnabled = msaaSampleCount != VK_SAMPLE_COUNT_1_BIT;

    const VkAttachmentDescription colorAttachment{
        .flags = {},
        .format = colorFormat,
        .samples = msaaSampleCount,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    const VkAttachmentDescription depthStencilAttachment{
        .flags = {},
        .format = depthStencilFormat,
        .samples = msaaSampleCount,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    const VkAttachmentDescription colorResolveAttachment{
        .flags = {},
        .format = colorFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    AxrVector_Stack<VkAttachmentDescription> attachments(3, &AxrAllocator::get().FrameAllocator);
    attachments.append({
        colorAttachment,
        depthStencilAttachment,
    });

    if (isMsaaEnabled) {
        attachments.pushBack(colorResolveAttachment);
    }

    constexpr VkAttachmentReference colorAttachmentRef{
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    constexpr VkAttachmentReference depthStencilAttachmentRef{
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    constexpr VkAttachmentReference colorResolveAttachmentRef{
        .attachment = 2,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpass{
        .flags = {},
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef,
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = &depthStencilAttachmentRef,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };

    if (isMsaaEnabled) {
        subpass.pResolveAttachments = &colorResolveAttachmentRef;
    }

    VkSubpassDependency dependency{
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = {},
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        .dependencyFlags = {},
    };

    if (isMsaaEnabled) {
        dependency.srcAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }

    const VkRenderPassCreateInfo renderPassCreateInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .flags = {},
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency,
    };

    const VkResult vkResult = vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);
    axrLogVkResult(vkResult, "vkCreateRenderPass");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        return AXR_ERROR_VULKAN_ERROR;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::destroyRenderPass(const VkDevice& device, VkRenderPass& renderPass) {
    if (renderPass == VK_NULL_HANDLE)
        return;

    vkDestroyRenderPass(device, renderPass, nullptr);
    renderPass = VK_NULL_HANDLE;
}

#endif
