#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "../../../assets/imageSampler.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan image sampler data
class AxrVulkanImageSamplerData {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Image data config
    struct Config {
        std::string Name;
        const AxrImageSampler* ImageSamplerHandle;
        vk::Device Device;
        float MaxSamplerAnisotropy;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanImageSamplerData();
    /// Constructor
    /// @param config Image sampler data config
    AxrVulkanImageSamplerData(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanImageSamplerData to copy from
    AxrVulkanImageSamplerData(const AxrVulkanImageSamplerData& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanImageSamplerData to move from
    AxrVulkanImageSamplerData(AxrVulkanImageSamplerData&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanImageSamplerData();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanImageSamplerData to copy from
    AxrVulkanImageSamplerData& operator=(const AxrVulkanImageSamplerData& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanImageSamplerData to move from
    AxrVulkanImageSamplerData& operator=(AxrVulkanImageSamplerData&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the image sampler name
    /// @returns The image sampler name
    [[nodiscard]] const std::string& getName() const;
    /// Get the image sampler
    /// @return The image sampler
    [[nodiscard]] const vk::Sampler& getSampler() const;

    /// Check if the data exists
    /// @returns True if the data exists
    [[nodiscard]] bool doesDataExist() const;

    /// Create the image sampler data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createData();
    /// Destroy the image sampler data
    void destroyData();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    const AxrImageSampler* m_ImageSamplerHandle;
    vk::Device m_Device;
    float m_MaxSamplerAnisotropy;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    vk::Sampler m_Sampler;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    /// Create image sampler
    /// @param sampler Output image sampler
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createSampler(vk::Sampler& sampler) const;
    /// Destroy the given image sampler
    /// @param sampler Sampler to destroy
    void destroySampler(vk::Sampler& sampler) const;
};

#endif
