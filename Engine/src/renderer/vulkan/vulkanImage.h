#pragma once
#ifdef AXR_VULKAN_SUPPORTED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"

#include <vulkan/vulkan_core.h>

/// Axr Vulkan Image
class AxrVulkanImage {
public:
    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Create an image view for the given image
    /// @param device Device to use
    /// @param image Image to create the image view of
    /// @param imageFormat Image format to use
    /// @param imageAspectMask Image aspect mask to use
    /// @param mipLevelCount Image mip level count
    /// @param imageView Output created image view
    [[nodiscard]] static AxrResult createImageView(const VkDevice& device,
                                                   const VkImage& image,
                                                   VkFormat imageFormat,
                                                   VkImageAspectFlags imageAspectMask,
                                                   uint32_t mipLevelCount,
                                                   VkImageView& imageView);
    /// Destroy the given image view
    /// @param device Device to use
    /// @param imageView Image view to destroy
    static void destroyImageView(const VkDevice& device, VkImageView& imageView);
};

#endif
