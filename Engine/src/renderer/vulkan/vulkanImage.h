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
    // Structs
    // ----------------------------------------- //

    struct Config {
        VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
        VkDevice Device = VK_NULL_HANDLE;
        VkCommandPool GraphicsCommandPool = VK_NULL_HANDLE;
        VkQueue GraphicsQueue = VK_NULL_HANDLE;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanImage();
    /// Constructor
    /// @param config Image config
    explicit AxrVulkanImage(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanImage to copy from
    AxrVulkanImage(const AxrVulkanImage& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanImage to move from
    AxrVulkanImage(AxrVulkanImage&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanImage();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanImage to copy from
    AxrVulkanImage& operator=(const AxrVulkanImage& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanImage to move from
    AxrVulkanImage& operator=(AxrVulkanImage&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the image exists
    /// @returns True if the image doesn't exist
    [[nodiscard]] bool isEmpty() const;
    /// Get the vulkan image
    /// @returns The vulkan image
    [[nodiscard]] const VkImage& getImage() const;
    /// Get the vulkan image view
    /// @returns The vulkan image view
    [[nodiscard]] const VkImageView& getImageView() const;
    /// Get the image mip level count
    /// @returns The image mip level count
    [[nodiscard]] uint32_t getMipLevelCount() const;
    /// Get the image format
    /// @returns The image format
    [[nodiscard]] VkFormat getImageFormat() const;

    /// Create this vulkan image
    /// @param extent Image extent
    /// @param sampleCount Image sample count
    /// @param format Image format
    /// @param imageTiling Image tiling option
    /// @param imageUsage Image usage flags
    /// @param memoryProperties Image memory property flags
    /// @param imageAspectMask Image aspect mask flags
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createImage(VkExtent2D extent,
                                        VkSampleCountFlagBits sampleCount,
                                        VkFormat format,
                                        VkImageTiling imageTiling,
                                        VkImageUsageFlags imageUsage,
                                        VkMemoryPropertyFlags memoryProperties,
                                        VkImageAspectFlags imageAspectMask);
    /// Destroy this image
    void destroyImage();

    /// Transition the image's layout
    /// @param srcAccessMask Image memory barrier src access mask
    /// @param dstAccessMask Image memory barrier dst access mask
    /// @param oldLayout Old image layout
    /// @param newLayout New image layout
    /// @param srcStageMask Pipeline barrier src stage mask
    /// @param dstStageMask Pipeline barrier dst stage mask
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult transitionImageLayout(VkAccessFlags srcAccessMask,
                                                  VkAccessFlags dstAccessMask,
                                                  VkImageLayout oldLayout,
                                                  VkImageLayout newLayout,
                                                  VkPipelineStageFlagBits srcStageMask,
                                                  VkPipelineStageFlagBits dstStageMask) const;

    // ----------------------------------------- //
    // Public Static Functions
    // ----------------------------------------- //

    /// Create a vulkan image
    /// @param device Device to use
    /// @param physicalDevice Physical device to use
    /// @param extent Image extent
    /// @param mipLevelCount Image mip level count
    /// @param sampleCount Image sample count
    /// @param format Image format
    /// @param imageTiling Image tiling
    /// @param imageUsage Image usage
    /// @param memoryProperties Image memory properties
    /// @param image Output created image
    /// @param imageMemory Output image memory
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createImage(const VkPhysicalDevice& physicalDevice,
                                               const VkDevice& device,
                                               VkExtent2D extent,
                                               uint32_t mipLevelCount,
                                               VkSampleCountFlagBits sampleCount,
                                               VkFormat format,
                                               VkImageTiling imageTiling,
                                               VkImageUsageFlags imageUsage,
                                               VkMemoryPropertyFlags memoryProperties,
                                               VkImage& image,
                                               VkDeviceMemory& imageMemory);
    /// Destroy the given vulkan image
    /// @param device Device to use
    /// @param image Image to destroy
    /// @param imageMemory Image memory to destroy
    static void destroyImage(const VkDevice& device, VkImage& image, VkDeviceMemory& imageMemory);

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

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    Config m_Config = {};
    VkImage m_Image = VK_NULL_HANDLE;
    VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
    VkImageView m_ImageView = VK_NULL_HANDLE;
    uint32_t m_MipLevelCount = {};
    VkImageAspectFlags m_ImageAspectMask = VK_IMAGE_ASPECT_NONE;
    VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
    
    /// Move the given AxrVulkanImage to this class
    /// @param src AxrVulkanImage to move
    /// @param useConstructor If true, this function will use the move constructor for non-primitive objects instead of
    /// the move assignment operator when moving variables
    void move_internal(AxrVulkanImage&& src, bool useConstructor);
};

#endif
