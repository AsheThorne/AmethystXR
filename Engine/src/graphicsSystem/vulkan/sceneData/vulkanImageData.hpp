#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "../vulkanImage.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan image data
class AxrVulkanImageData {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Image data config
    struct Config {
        std::string Name;
        const AxrImage* ImageHandle;
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool GraphicsCommandPool;
        vk::Queue GraphicsQueue;
        float MaxSamplerAnisotropy;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanImageData();
    /// Constructor
    /// @param config Image data config
    AxrVulkanImageData(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanImageData to copy from
    AxrVulkanImageData(const AxrVulkanImageData& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanImageData to move from
    AxrVulkanImageData(AxrVulkanImageData&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanImageData();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanImageData to copy from
    AxrVulkanImageData& operator=(const AxrVulkanImageData& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanImageData to move from
    AxrVulkanImageData& operator=(AxrVulkanImageData&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the image name
    /// @returns The image name
    [[nodiscard]] const std::string& getName() const;

    /// Check if the data exists
    /// @returns True if the data exists
    [[nodiscard]] bool doesDataExist() const;

    /// Create the image data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createData();
    /// Destroy the image data
    void destroyData();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    const AxrImage* m_ImageHandle;
    vk::Device m_Device;
    float m_MaxSamplerAnisotropy;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    AxrVulkanImage m_Image;
    vk::Sampler m_Sampler;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    /// Create image sampler
    /// @param mipLevelCount Mip level count
    /// @param imageSamplerFilter Image sampler filter 
    /// @param imageSamplerWrapping Image sampler wrapping enum
    /// @param sampler Output image sampler
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createSampler(
        uint32_t mipLevelCount,
        AxrImageSamplerFilterEnum imageSamplerFilter,
        AxrImageSamplerWrappingEnum imageSamplerWrapping,
        vk::Sampler& sampler
    ) const;
    /// Destroy the given image sampler
    /// @param sampler Sampler to destroy
    void destroySampler(vk::Sampler& sampler) const;
};

#endif