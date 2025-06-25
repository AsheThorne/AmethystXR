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
        const AxrImage* ImageHandle;
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
    /// Get the image view
    /// @returns The image view
    [[nodiscard]] const vk::ImageView& getImageView() const;

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
    const AxrImage* m_ImageHandle;

    // ---- Data ----
    /// These should never be used for anything other than returning a reference to the name if no name can be found.
    std::string m_DummyName;
    AxrVulkanImage m_Image;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};

#endif