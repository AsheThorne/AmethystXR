#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "../../../assets/uniformBuffer.hpp"
#include "../vulkanBuffer.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan uniform buffer data
class AxrVulkanUniformBufferData {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Uniform buffer data config
    struct Config {
        /// We don't define both UniformBufferHandle and UniformBufferEngineAsset. It's one or the other.
        /// If UniformBufferHandle is used instead of UniformBufferEngineAsset, Then we don't need to manually update the uniform buffer
        /// when the AxrUniformBuffer updates.
        const AxrUniformBuffer* UniformBufferHandle;
        /// We don't define both UniformBufferHandle and UniformBufferEngineAsset. It's one or the other.
        AxrEngineAssetEnum UniformBufferEngineAsset;
        uint32_t MaxFramesInFlight;
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool TransferCommandPool;
        vk::Queue TransferQueue;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanUniformBufferData();
    /// Constructor
    /// @param config Uniform buffer data config
    AxrVulkanUniformBufferData(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanUniformBufferData to copy from
    AxrVulkanUniformBufferData(const AxrVulkanUniformBufferData& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanUniformBufferData to move from
    AxrVulkanUniformBufferData(AxrVulkanUniformBufferData&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanUniformBufferData();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanUniformBufferData to copy from
    AxrVulkanUniformBufferData& operator=(const AxrVulkanUniformBufferData& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanUniformBufferData to move from
    AxrVulkanUniformBufferData& operator=(AxrVulkanUniformBufferData&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the uniform buffer name
    /// @returns The uniform buffer name
    [[nodiscard]] const std::string& getName() const;
    /// Get the vulkan buffer at the given frame
    /// @param frameIndex Frame index to use
    /// @returns The vulkan buffer
    [[nodiscard]] const AxrVulkanBuffer& getBuffer(uint32_t frameIndex) const;
    /// Get the uniform buffer size
    /// @returns The uniform buffer size
    [[nodiscard]] vk::DeviceSize getBufferSize() const;

    /// Check if the data exists
    /// @returns True if the data exists
    [[nodiscard]] bool doesDataExist() const;

    /// Create the uniform buffer data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createData();
    /// Destroy the uniform buffer data
    void destroyData();

    /// Set the buffer data
    /// @param index Frame in flight index
    /// @param offset Offset of the data to set
    /// @param size Size of the data to set
    /// @param data Data to set
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setData(uint32_t index, vk::DeviceSize offset, vk::DeviceSize size, const void* data) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    // TODO: Check if these "vulkan*data" files need the name here. Can't we just get it from the buffer handle?
    //  It is needed here since a uniformBufferHandle isn't required. but other classes probably don't need it.
    std::string m_Name;
    const AxrUniformBuffer* m_UniformBufferHandle;
    AxrEngineAssetEnum m_UniformBufferEngineAsset;
    uint32_t m_MaxFramesInFlight;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_TransferCommandPool;
    vk::Queue m_TransferQueue;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    /// One buffer per frame in flight
    std::vector<AxrVulkanBuffer> m_UniformBuffers;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    /// Create a vulkan buffer to use as the uniform buffer
    /// @param buffer Output vulkan buffer
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createUniformBuffer(AxrVulkanBuffer& buffer) const;
    /// Destroy a vulkan buffer
    void destroyUniformBuffer(AxrVulkanBuffer& buffer) const;
};

#endif
