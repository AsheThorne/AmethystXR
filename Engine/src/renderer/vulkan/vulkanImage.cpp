#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanImage.h"
#include "axr/logging.h"
#include "vulkanUtils.h"

#include <cassert>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrVulkanImage::AxrVulkanImage() = default;

AxrVulkanImage::AxrVulkanImage(const Config& config) :
    m_Config(config) {
}

AxrVulkanImage::AxrVulkanImage(AxrVulkanImage&& src) noexcept {
    m_Config = std::move(src.m_Config);

    m_Image = src.m_Image;
    m_ImageMemory = src.m_ImageMemory;
    m_ImageView = src.m_ImageView;
    m_MipLevelCount = src.m_MipLevelCount;
    m_ImageAspectMask = src.m_ImageAspectMask;
    m_ImageFormat = src.m_ImageFormat;

    src.m_Image = VK_NULL_HANDLE;
    src.m_ImageMemory = VK_NULL_HANDLE;
    src.m_ImageView = VK_NULL_HANDLE;
    src.m_MipLevelCount = {};
    src.m_ImageAspectMask = VK_IMAGE_ASPECT_NONE;
    src.m_ImageFormat = VK_FORMAT_UNDEFINED;
}

AxrVulkanImage::~AxrVulkanImage() {
    cleanup();
}

AxrVulkanImage& AxrVulkanImage::operator=(AxrVulkanImage&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Config = std::move(src.m_Config);

        m_Image = src.m_Image;
        m_ImageMemory = src.m_ImageMemory;
        m_ImageView = src.m_ImageView;
        m_MipLevelCount = src.m_MipLevelCount;
        m_ImageAspectMask = src.m_ImageAspectMask;
        m_ImageFormat = src.m_ImageFormat;

        src.m_Image = VK_NULL_HANDLE;
        src.m_ImageMemory = VK_NULL_HANDLE;
        src.m_ImageView = VK_NULL_HANDLE;
        src.m_MipLevelCount = {};
        src.m_ImageAspectMask = VK_IMAGE_ASPECT_NONE;
        src.m_ImageFormat = VK_FORMAT_UNDEFINED;
    }

    return *this;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

bool AxrVulkanImage::isEmpty() const {
    return m_Image == VK_NULL_HANDLE && m_ImageMemory == VK_NULL_HANDLE && m_ImageView == VK_NULL_HANDLE;
}

const VkImage& AxrVulkanImage::getImage() const {
    return m_Image;
}

const VkImageView& AxrVulkanImage::getImageView() const {
    return m_ImageView;
}

uint32_t AxrVulkanImage::getMipLevelCount() const {
    return m_MipLevelCount;
}

VkFormat AxrVulkanImage::getImageFormat() const {
    return m_ImageFormat;
}

AxrResult AxrVulkanImage::createImage(const VkExtent2D extent,
                                      const VkSampleCountFlagBits sampleCount,
                                      const VkFormat format,
                                      const VkImageTiling imageTiling,
                                      const VkImageUsageFlags imageUsage,
                                      const VkMemoryPropertyFlags memoryProperties,
                                      const VkImageAspectFlags imageAspectMask) {
    AxrResult axrResult = AXR_SUCCESS;
    m_MipLevelCount = 1;
    m_ImageFormat = format;

    axrResult = createImage(m_Config.PhysicalDevice,
                            m_Config.Device,
                            extent,
                            m_MipLevelCount,
                            sampleCount,
                            m_ImageFormat,
                            imageTiling,
                            imageUsage,
                            memoryProperties,
                            m_Image,
                            m_ImageMemory);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyImage();
        return axrResult;
    }

    m_ImageAspectMask = imageAspectMask;

    axrResult = createImageView(m_Config.Device,
                                m_Image,
                                m_ImageFormat,
                                m_ImageAspectMask,
                                m_MipLevelCount,
                                m_ImageView);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyImage();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanImage::destroyImage() {
    m_ImageFormat = VK_FORMAT_UNDEFINED;
    m_MipLevelCount = {};
    m_ImageAspectMask = VK_IMAGE_ASPECT_NONE;
    destroyImageView(m_Config.Device, m_ImageView);
    destroyImage(m_Config.Device, m_Image, m_ImageMemory);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to transition image layout. "
AxrResult AxrVulkanImage::transitionImageLayout(const VkAccessFlags srcAccessMask,
                                                const VkAccessFlags dstAccessMask,
                                                const VkImageLayout oldLayout,
                                                const VkImageLayout newLayout,
                                                const VkPipelineStageFlagBits srcStageMask,
                                                const VkPipelineStageFlagBits dstStageMask) const {
    AxrResult axrResult = AXR_SUCCESS;

    VkCommandBuffer commandBuffer;
    axrResult = axrBeginSingleTimeCommand(m_Config.Device, m_Config.GraphicsCommandPool, commandBuffer);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to begin single time command.");
        return axrResult;
    }

    const VkImageMemoryBarrier imageMemoryBarrier{
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = srcAccessMask,
        .dstAccessMask = dstAccessMask,
        .oldLayout = oldLayout,
        .newLayout = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = m_Image,
        .subresourceRange =
            VkImageSubresourceRange{
                .aspectMask = m_ImageAspectMask,
                .baseMipLevel = 0,
                .levelCount = m_MipLevelCount,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
    };

    constexpr uint32_t imageMemoryBarrierCount = 1;
    vkCmdPipelineBarrier(commandBuffer,
                         srcStageMask,
                         dstStageMask,
                         {},
                         0,
                         nullptr,
                         0,
                         nullptr,
                         imageMemoryBarrierCount,
                         &imageMemoryBarrier);

    axrResult = axrEndSingleTimeCommand(m_Config.Device,
                                        m_Config.GraphicsCommandPool,
                                        m_Config.GraphicsQueue,
                                        commandBuffer);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

// ----------------------------------------- //
// Public Static Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to create vulkan image. "
AxrResult AxrVulkanImage::createImage(const VkPhysicalDevice& physicalDevice,
                                      const VkDevice& device,
                                      const VkExtent2D extent,
                                      const uint32_t mipLevelCount,
                                      const VkSampleCountFlagBits sampleCount,
                                      const VkFormat format,
                                      const VkImageTiling imageTiling,
                                      const VkImageUsageFlags imageUsage,
                                      const VkMemoryPropertyFlags memoryProperties,
                                      VkImage& image,
                                      VkDeviceMemory& imageMemory) {
    assert(physicalDevice != VK_NULL_HANDLE);
    assert(device != VK_NULL_HANDLE);

    if (image != VK_NULL_HANDLE || imageMemory != VK_NULL_HANDLE) [[unlikely]] {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Image already exists.");
        return AXR_SUCCESS;
    }

    const VkImageCreateInfo imageCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = {},
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent =
            VkExtent3D{
                extent.width,
                extent.height,
                1,
            },
        .mipLevels = mipLevelCount,
        .arrayLayers = 1,
        .samples = sampleCount,
        .tiling = imageTiling,
        .usage = imageUsage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    VkResult vkResult = vkCreateImage(device, &imageCreateInfo, nullptr, &image);
    axrLogVkResult(vkResult, "vkCreateImage");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        destroyImage(device, image, imageMemory);
        return AXR_ERROR_VULKAN_ERROR;
    }

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(device, image, &memoryRequirements);

    uint32_t memoryTypeIndex = 0;
    const AxrResult axrResult = axrFindMemoryTypeIndex(physicalDevice,
                                                       memoryRequirements.memoryTypeBits,
                                                       memoryProperties,
                                                       memoryTypeIndex);

    if (AXR_FAILED(axrResult)) [[unlikely]] {
        destroyImage(device, image, imageMemory);
        return axrResult;
    }

    const VkMemoryAllocateInfo memoryAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = memoryTypeIndex,
    };

    vkResult = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &imageMemory);
    axrLogVkResult(vkResult, "vkAllocateMemory");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        destroyImage(device, image, imageMemory);
        return AXR_ERROR_VULKAN_ERROR;
    }

    vkResult = vkBindImageMemory(device, image, imageMemory, 0);
    axrLogVkResult(vkResult, "vkBindImageMemory");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        destroyImage(device, image, imageMemory);
        return AXR_ERROR_VULKAN_ERROR;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanImage::destroyImage(const VkDevice& device, VkImage& image, VkDeviceMemory& imageMemory) {
    if (image != VK_NULL_HANDLE) {
        vkDestroyImage(device, image, nullptr);
        image = VK_NULL_HANDLE;
    }

    if (imageMemory != VK_NULL_HANDLE) {
        vkFreeMemory(device, imageMemory, nullptr);
        imageMemory = VK_NULL_HANDLE;
    }
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create vulkan image view. "
AxrResult AxrVulkanImage::createImageView(const VkDevice& device,
                                          const VkImage& image,
                                          const VkFormat imageFormat,
                                          const VkImageAspectFlags imageAspectMask,
                                          const uint32_t mipLevelCount,
                                          VkImageView& imageView) {
    assert(device != VK_NULL_HANDLE);
    assert(image != VK_NULL_HANDLE);

    if (imageView != VK_NULL_HANDLE) [[unlikely]] {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Image view already exists.");
        return AXR_SUCCESS;
    }

    const VkImageViewCreateInfo imageViewCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = {},
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = imageFormat,
        .components =
            VkComponentMapping{
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
        .subresourceRange =
            VkImageSubresourceRange{
                .aspectMask = imageAspectMask,
                .baseMipLevel = 0,
                .levelCount = mipLevelCount,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
    };

    const VkResult vkResult = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &imageView);
    axrLogVkResult(vkResult, "vkCreateImageView");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanImage::destroyImageView(const VkDevice& device, VkImageView& imageView) {
    if (imageView == VK_NULL_HANDLE)
        return;

    vkDestroyImageView(device, imageView, nullptr);
    imageView = VK_NULL_HANDLE;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrVulkanImage::cleanup() {
    destroyImage();

    m_Config = {};
}

#endif
