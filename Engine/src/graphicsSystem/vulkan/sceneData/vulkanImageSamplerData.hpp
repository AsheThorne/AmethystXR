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
        const AxrImageSampler* ImageSamplerHandle;
        vk::PhysicalDevice PhysicalDevice;
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
    /// @param imageFormat The image format that will be used with this sampler
    /// @return The image sampler
    [[nodiscard]] const vk::Sampler& getSampler(vk::Format imageFormat) const;

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
    const AxrImageSampler* m_ImageSamplerHandle;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    float m_MaxSamplerAnisotropy;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    /// These should never be used for anything other than returning a reference to the name if no name can be found.
    std::string m_DummyName;
    /// The main sampler with all requested features
    vk::Sampler m_MainSampler;
    /// The backup sampler to use if the image format isn't compatible with the main sampler
    vk::Sampler m_BackupSampler;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    /// Create the image samplers
    /// @param mainSampler Output main image sampler
    /// @param backupSampler Output backup image sampler
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createSamplers(vk::Sampler& mainSampler, vk::Sampler& backupSampler) const;
    /// Create image sampler
    /// @param magFilter Mag filter to use
    /// @param minFilter Min filter to use
    /// @param mipmapMode Mipmap mode to use
    /// @param sampler Output image sampler
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createSampler(
        vk::Filter magFilter,
        vk::Filter minFilter,
        vk::SamplerMipmapMode mipmapMode,
        vk::Sampler& sampler
    ) const;
    /// Destroy the given image sampler
    /// @param sampler Sampler to destroy
    void destroySampler(vk::Sampler& sampler) const;
};

#endif
