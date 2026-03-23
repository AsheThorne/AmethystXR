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
    assert(config.DesktopConfig != nullptr);

    context.Instance = config.Instance;
    context.Device = config.Device;
    context.GraphicsCommandPool = config.GraphicsCommandPool;

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

    axrResult = createDesktopSyncObjects(context.Device,
                                         config.MaxFramesInFlight,
                                         context.ImageAvailableSemaphores,
                                         context.RenderingFinishedSemaphores,
                                         context.RenderingFences);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyDesktopContext(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create desktop sync objects.");
        return axrResult;
    }

    axrResult = createCommandBuffers(context.Device,
                                     context.GraphicsCommandPool,
                                     config.MaxFramesInFlight,
                                     context.RenderingCommandBuffers);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyDesktopContext(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create desktop command buffers.");
        return axrResult;
    }

    axrResult = setupDesktopSwapchain(config.PhysicalDevice,
                                      context.Surface,
                                      config.DesktopConfig->PreferredPresentationMode,
                                      context.SwapchainPresentationMode,
                                      context.SwapchainExtent);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyDesktopContext(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to set up desktop swapchain.");
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::destroyDesktopContext(DesktopContext& context) {
    resetSetupDesktopSwapchain(context.SwapchainPresentationMode, context.SwapchainExtent);
    destroyCommandBuffers(context.Device, context.GraphicsCommandPool, context.RenderingCommandBuffers);
    destroyDesktopSyncObjects(context.Device,
                              context.ImageAvailableSemaphores,
                              context.RenderingFinishedSemaphores,
                              context.RenderingFences);
    destroyRenderPass(context.Device, context.RenderPass);
    resetDesktopSwapchainFormats(context.SwapchainColorFormat,
                                 context.SwapchainColorSpace,
                                 context.SwapchainDepthFormat);
    AxrPlatform::get().destroyVulkanSurface(context.Instance, context.Surface);

    context.Instance = VK_NULL_HANDLE;
    context.Device = VK_NULL_HANDLE;
    context.GraphicsCommandPool = VK_NULL_HANDLE;
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
    assert(surface != VK_NULL_HANDLE);
    assert(physicalDevice != VK_NULL_HANDLE);

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
    assert(physicalDevice != VK_NULL_HANDLE);

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
    assert(device != VK_NULL_HANDLE);

    if (renderPass != VK_NULL_HANDLE) [[unlikely]] {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Render pass already exists.");
        return AXR_SUCCESS;
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

#define AXR_FUNCTION_FAILED_STRING "Failed to create desktop sync objects. "
AxrResult AxrVulkanEnvironment::createDesktopSyncObjects(const VkDevice& device,
                                                         const uint32_t maxFramesInFlight,
                                                         AxrVector_Dynamic<VkSemaphore>& imageAvailableSemaphores,
                                                         AxrVector_Dynamic<VkSemaphore>& renderingFinishedSemaphores,
                                                         AxrVector_Dynamic<VkFence>& renderingFences) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (imageAvailableSemaphores.allocated()) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Image available semaphores already exist.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (renderingFinishedSemaphores.allocated()) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Rendering finished semaphores already exist.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (renderingFences.allocated()) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Rendering fences already exist.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    imageAvailableSemaphores =
        AxrVector_Dynamic<VkSemaphore>(maxFramesInFlight, &AxrAllocator::get().EngineDataAllocator);
    renderingFinishedSemaphores =
        AxrVector_Dynamic<VkSemaphore>(maxFramesInFlight, &AxrAllocator::get().EngineDataAllocator);
    renderingFences = AxrVector_Dynamic<VkFence>(maxFramesInFlight, &AxrAllocator::get().EngineDataAllocator);

    axrResult = createSemaphores(device, imageAvailableSemaphores);
    if (AXR_FAILED(axrResult)) {
        destroyDesktopSyncObjects(device, imageAvailableSemaphores, renderingFinishedSemaphores, renderingFences);
        return axrResult;
    }

    axrResult = createSemaphores(device, renderingFinishedSemaphores);
    if (AXR_FAILED(axrResult)) {
        destroyDesktopSyncObjects(device, imageAvailableSemaphores, renderingFinishedSemaphores, renderingFences);
        return axrResult;
    }

    axrResult = createFences(device, VK_FENCE_CREATE_SIGNALED_BIT, renderingFences);
    if (AXR_FAILED(axrResult)) {
        destroyDesktopSyncObjects(device, imageAvailableSemaphores, renderingFinishedSemaphores, renderingFences);
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::destroyDesktopSyncObjects(const VkDevice& device,
                                                     AxrVector_Dynamic<VkSemaphore>& imageAvailableSemaphores,
                                                     AxrVector_Dynamic<VkSemaphore>& renderingFinishedSemaphores,
                                                     AxrVector_Dynamic<VkFence>& renderingFences) {
    destroySemaphores(device, imageAvailableSemaphores);
    destroySemaphores(device, renderingFinishedSemaphores);
    destroyFences(device, renderingFences);

    // Reset the vectors so the data can be deallocated
    imageAvailableSemaphores = AxrVector_Dynamic<VkSemaphore>();
    renderingFinishedSemaphores = AxrVector_Dynamic<VkSemaphore>();
    renderingFences = AxrVector_Dynamic<VkFence>();
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create semaphores. "
AxrResult AxrVulkanEnvironment::createSemaphores(const VkDevice& device, AxrVector_Dynamic<VkSemaphore>& semaphores) {
    assert(device != VK_NULL_HANDLE);

    if (!semaphores.allocated()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`semaphores` haven't been allocated.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (!semaphores.empty()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`semaphores` already exist.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    constexpr VkSemaphoreCreateInfo semaphoreCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = {},
    };

    VkResult vkResult = VK_SUCCESS;
    semaphores.prefillData(VK_NULL_HANDLE);

    for (VkSemaphore& semaphore : semaphores) {
        vkResult = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphore);
        axrLogVkResult(vkResult, "vkCreateSemaphore");

        if (VK_FAILED(vkResult)) [[unlikely]] {
            break;
        }
    }

    if (VK_FAILED(vkResult)) [[unlikely]] {
        destroySemaphores(device, semaphores);
        return AXR_ERROR_VULKAN_ERROR;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::destroySemaphores(const VkDevice& device, AxrVector_Dynamic<VkSemaphore>& semaphores) {
    for (VkSemaphore& semaphore : semaphores) {
        if (semaphore == VK_NULL_HANDLE) {
            continue;
        }

        vkDestroySemaphore(device, semaphore, nullptr);
        semaphore = VK_NULL_HANDLE;
    }

    semaphores.clear();
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create fences. "
AxrResult AxrVulkanEnvironment::createFences(const VkDevice& device,
                                             const VkFenceCreateFlags flags,
                                             AxrVector_Dynamic<VkFence>& fences) {
    assert(device != VK_NULL_HANDLE);

    if (!fences.allocated()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`fences` haven't been allocated.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (!fences.empty()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`fences` already exist.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    const VkFenceCreateInfo fenceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = flags,
    };

    VkResult vkResult = VK_SUCCESS;
    fences.prefillData(VK_NULL_HANDLE);

    for (VkFence& fence : fences) {
        vkResult = vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
        axrLogVkResult(vkResult, "vkCreateFence");

        if (VK_FAILED(vkResult)) [[unlikely]] {
            break;
        }
    }

    if (VK_FAILED(vkResult)) [[unlikely]] {
        destroyFences(device, fences);
        return AXR_ERROR_VULKAN_ERROR;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::destroyFences(const VkDevice& device, AxrVector_Dynamic<VkFence>& fences) {
    for (VkFence& fence : fences) {
        if (fence == VK_NULL_HANDLE) {
            continue;
        }

        vkDestroyFence(device, fence, nullptr);
        fence = VK_NULL_HANDLE;
    }

    fences.clear();
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create command buffers. "
AxrResult AxrVulkanEnvironment::createCommandBuffers(const VkDevice& device,
                                                     const VkCommandPool& commandPool,
                                                     const uint32_t maxFramesInFlight,
                                                     AxrVector_Dynamic<VkCommandBuffer>& commandBuffers) {
    assert(device != VK_NULL_HANDLE);
    assert(commandPool != VK_NULL_HANDLE);

    if (commandBuffers.allocated() || !commandBuffers.empty()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`commandBuffers` already exist.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    commandBuffers = AxrVector_Dynamic<VkCommandBuffer>(maxFramesInFlight, &AxrAllocator::get().EngineDataAllocator);
    commandBuffers.prefillData(VK_NULL_HANDLE);

    const VkCommandBufferAllocateInfo commandBufferAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = static_cast<uint32_t>(commandBuffers.size()),
    };

    const VkResult vkResult = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers.data());
    axrLogVkResult(vkResult, "vkAllocateCommandBuffers");

    if (VK_FAILED(vkResult)) [[unlikely]] {
        destroyCommandBuffers(device, commandPool, commandBuffers);
        return AXR_ERROR_VULKAN_ERROR;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::destroyCommandBuffers(const VkDevice& device,
                                                 const VkCommandPool& commandPool,
                                                 AxrVector_Dynamic<VkCommandBuffer>& commandBuffers) {
    if (commandBuffers.empty()) {
        return;
    }

    vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    // Reset the vector so the data can be deallocated
    commandBuffers = {};
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up desktop swapchain. "
AxrResult AxrVulkanEnvironment::setupDesktopSwapchain(const VkPhysicalDevice& physicalDevice,
                                                      const VkSurfaceKHR& surface,
                                                      const AxrVulkanPresentationModeEnum preferredPresentationMode,
                                                      VkPresentModeKHR& presentationMode,
                                                      VkExtent2D& extent) {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult =
        setDesktopSwapchainPresentationMode(physicalDevice, surface, preferredPresentationMode, presentationMode);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        resetSetupDesktopSwapchain(presentationMode, extent);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to set swapchain presentation mode.");
        return axrResult;
    }

    axrResult = setDesktopSwapchainExtent(physicalDevice, surface, extent);
    if (AXR_FAILED(axrResult)) {
        resetSetupDesktopSwapchain(presentationMode, extent);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to set swapchain extent.");
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::resetSetupDesktopSwapchain(VkPresentModeKHR& presentationMode, VkExtent2D& extent) {
    resetSwapchainExtent(extent);
    resetDesktopSwapchainPresentationMode(presentationMode);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set desktop swapchain presentation mode. "
AxrResult AxrVulkanEnvironment::setDesktopSwapchainPresentationMode(
    const VkPhysicalDevice& physicalDevice,
    const VkSurfaceKHR& surface,
    const AxrVulkanPresentationModeEnum preferredPresentationMode,
    VkPresentModeKHR& presentationMode) {
    assert(physicalDevice != VK_NULL_HANDLE);
    assert(surface != VK_NULL_HANDLE);

    if (presentationMode != VK_PRESENT_MODE_MAX_ENUM_KHR) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Swapchain presentation mode has already been set.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (preferredPresentationMode == AXR_VULKAN_PRESENTATION_MODE_UNDEFINED) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Preferred presentation mode is undefined.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    AxrVector_Stack<VkPresentModeKHR> supportedPresentationModes;
    const AxrResult axrResult =
        getSupportedSurfacePresentationModes(physicalDevice, surface, supportedPresentationModes);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported surface presentation modes.");
        return axrResult;
    }

    VkPresentModeKHR backupPresentationMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
    const VkPresentModeKHR preferredVkPresentationMode = axrToVkPresentMode(preferredPresentationMode);

    // Find the preferred presentation mode
    for (const VkPresentModeKHR supportedPresentationMode : supportedPresentationModes) {
        if (supportedPresentationMode == preferredVkPresentationMode) {
            presentationMode = supportedPresentationMode;
            return AXR_SUCCESS;
        }

        if (supportedPresentationMode == VK_PRESENT_MODE_FIFO_KHR) {
            backupPresentationMode = supportedPresentationMode;
        }
    }

    // If the preferred presentation mode couldn't be found, but the backup was. Use that
    if (backupPresentationMode != VK_PRESENT_MODE_MAX_ENUM_KHR) {
        presentationMode = backupPresentationMode;
        return AXR_SUCCESS;
    }

    // As a last resort, just use whatever we can that's supported
    presentationMode = supportedPresentationModes[0];

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanEnvironment::resetDesktopSwapchainPresentationMode(VkPresentModeKHR& presentationMode) {
    presentationMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to get supported surface presentation modes. "
AxrResult AxrVulkanEnvironment::getSupportedSurfacePresentationModes(
    const VkPhysicalDevice& physicalDevice,
    const VkSurfaceKHR& surface,
    AxrVector_Stack<VkPresentModeKHR>& supportedPresentationModes) {
    assert(surface != VK_NULL_HANDLE);
    assert(physicalDevice != VK_NULL_HANDLE);

    if (supportedPresentationModes.allocated()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`supportedPresentationModes` have already been allocated.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    uint32_t presentationModesCount;
    VkResult vkResult =
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationModesCount, nullptr);
    axrLogVkResult(vkResult, "vkGetPhysicalDeviceSurfacePresentModesKHR");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    AxrVector_Stack<VkPresentModeKHR> presentationModes(presentationModesCount, &AxrAllocator::get().FrameAllocator);
    presentationModes.prefillData();
    vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                                         surface,
                                                         &presentationModesCount,
                                                         presentationModes.data());
    axrLogVkResult(vkResult, "vkGetPhysicalDeviceSurfacePresentModesKHR");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    supportedPresentationModes = std::move(presentationModes);
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

AxrResult AxrVulkanEnvironment::setDesktopSwapchainExtent(const VkPhysicalDevice& physicalDevice,
                                                          const VkSurfaceKHR& surface,
                                                          VkExtent2D& extent) {
    assert(physicalDevice != VK_NULL_HANDLE);
    assert(surface != VK_NULL_HANDLE);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    const VkResult vkResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
    axrLogVkResult(vkResult, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    // If the current extent width is the max uint32_t value, then we need to get the extent manually
    if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
        extent = surfaceCapabilities.currentExtent;
        return AXR_SUCCESS;
    }

    uint32_t width;
    uint32_t height;
    const AxrResult axrResult = AxrPlatform::get().getWindowSizeInPixels(width, height);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    width = std::clamp(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
    height = std::clamp(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

    extent = VkExtent2D(width, height);
    return AXR_SUCCESS;
}

void AxrVulkanEnvironment::resetSwapchainExtent(VkExtent2D& extent) {
    extent = VkExtent2D{};
}

#endif
