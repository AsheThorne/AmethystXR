#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "../../assets/image.hpp"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan Image
class AxrVulkanImage {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan buffer config
    struct Config {
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool GraphicsCommandPool;
        vk::Queue GraphicsQueue;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanImage();
    /// Constructor
    /// @param config Vulkan image config
    AxrVulkanImage(const Config& config);
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
    [[nodiscard]] const vk::Image& getImage() const;
    /// Get the vulkan image view
    /// @returns The vulkan image view
    [[nodiscard]] const vk::ImageView& getImageView() const;
    /// Get the image mip level count
    /// @returns The image mip level count
    [[nodiscard]] uint32_t getMipLevelCount() const;

    /// Create the image
    /// @param image Source image
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createImage(AxrImageConst_T image);
    /// Destroy the image
    void destroyImage();

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
    /// @param dispatch Dispatch to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createImage(
        vk::Device device,
        vk::PhysicalDevice physicalDevice,
        vk::Extent2D extent,
        uint32_t mipLevelCount,
        vk::SampleCountFlagBits sampleCount,
        vk::Format format,
        vk::ImageTiling imageTiling,
        vk::ImageUsageFlags imageUsage,
        vk::MemoryPropertyFlags memoryProperties,
        vk::Image& image,
        vk::DeviceMemory& imageMemory,
        const vk::DispatchLoaderDynamic& dispatch
    );
    /// Destroy a vulkan image
    /// @param device Device to use
    /// @param image Image to destroy
    /// @param imageMemory Image memory to destroy
    /// @param dispatch Dispatch to use
    static void destroyImage(
        vk::Device device,
        vk::Image& image,
        vk::DeviceMemory& imageMemory,
        const vk::DispatchLoaderDynamic& dispatch
    );

    /// Create an image view for the given image
    /// @param device Device to use
    /// @param image Image to create image view from
    /// @param format Format to use
    /// @param aspectMask Aspect mask to use
    /// @param mipLevelCount Mip level count to use
    /// @param imageView Output image view
    /// @param dispatch Dispatch to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createImageView(
        vk::Device device,
        const vk::Image& image,
        vk::Format format,
        vk::ImageAspectFlags aspectMask,
        uint32_t mipLevelCount,
        vk::ImageView& imageView,
        const vk::DispatchLoaderDynamic& dispatch
    );
    /// Destroy the given image view
    /// @param device Device to use
    /// @param imageView Image view to destroy
    /// @param dispatch Dispatch to use
    static void destroyImageView(
        vk::Device device,
        vk::ImageView& imageView,
        const vk::DispatchLoaderDynamic& dispatch
    );

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_GraphicsCommandPool;
    vk::Queue m_GraphicsQueue;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    vk::Image m_Image;
    vk::ImageView m_ImageView;
    vk::DeviceMemory m_ImageMemory;
    uint32_t m_MipLevelCount;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    /// Get the image format for the given color channels
    /// @param colorChannels Color channels
    /// @returns The image format
    [[nodiscard]] vk::Format getImageFormat(uint32_t colorChannels) const;
    /// Count the number of mip levels for the given dimensions
    /// @param width Image width
    /// @param height Image height
    /// @returns The number of mip levels
    [[nodiscard]] uint32_t countImageMipLevels(uint32_t width, uint32_t height) const;

    /// Copy the given buffer to the given image
    /// @param buffer Buffer
    /// @param image Image
    /// @param imageWidth Image width
    /// @param imageHeight Image height
    /// @param mipLevelCount Mip level count
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult copyBufferToImage(
        vk::Buffer buffer,
        vk::Image image,
        uint32_t imageWidth,
        uint32_t imageHeight,
        uint32_t mipLevelCount
    ) const;
};

#endif
