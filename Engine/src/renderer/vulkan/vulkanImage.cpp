#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanImage.h"
#include "axr/logging.h"
#include "vulkanUtils.h"

#include <cassert>

// ----------------------------------------- //
// Public Static Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to create image view. "
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

#endif
