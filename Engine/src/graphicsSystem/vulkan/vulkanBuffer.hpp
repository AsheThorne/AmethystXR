#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/enums.h"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan Buffer
class AxrVulkanBuffer {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan buffer config
    struct Config {
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
    AxrVulkanBuffer();
    /// Constructor
    /// @param config Vulkan buffer config
    AxrVulkanBuffer(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanBuffer to copy from
    AxrVulkanBuffer(const AxrVulkanBuffer& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanBuffer to move from
    AxrVulkanBuffer(AxrVulkanBuffer&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanBuffer();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanBuffer to copy from
    AxrVulkanBuffer& operator=(const AxrVulkanBuffer& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanBuffer to move from
    AxrVulkanBuffer& operator=(AxrVulkanBuffer&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the buffer exists
    /// @returns True if the buffer doesn't exist
    [[nodiscard]] bool isEmpty() const;
    /// Get the vulkan buffer
    /// @returns The vulkan buffer
    [[nodiscard]] const vk::Buffer& getBuffer() const;

    /// Create the buffer
    /// @param willBufferBeStatic Set to true if the buffer will get converted to a static buffer
    /// @param bufferSize Buffer size
    /// @param bufferUsageFlags Buffer usage flags
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createBuffer(
        bool willBufferBeStatic,
        vk::DeviceSize bufferSize,
        vk::BufferUsageFlags bufferUsageFlags
    );
    /// Destroy the buffer
    void destroyBuffer();

    /// Convert the buffer to a static buffer, meaning the data can no longer be changed
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult convertToStaticBuffer();

    /// Set the buffer data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setBufferData(vk::DeviceSize offset, vk::DeviceSize size, const void* data) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_TransferCommandPool;
    vk::Queue m_TransferQueue;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    bool m_WillBufferBeStatic;
    bool m_IsBufferStatic;
    vk::DeviceSize m_BufferSize;
    vk::BufferUsageFlags m_BufferUsageFlags;
    vk::Buffer m_Buffer;
    vk::DeviceMemory m_BufferMemory;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    /// Create a buffer
    /// @param size Buffer size
    /// @param usage Buffer Usage Flags
    /// @param properties Memory Properties
    /// @param buffer Output created buffer
    /// @param bufferMemory Output buffer memory
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createBuffer(
        vk::DeviceSize size,
        vk::BufferUsageFlags usage,
        vk::MemoryPropertyFlags properties,
        vk::Buffer& buffer,
        vk::DeviceMemory& bufferMemory
    ) const;
    /// Destroy the given buffer
    /// @param buffer Buffer
    /// @param bufferMemory Buffer memory
    void destroyBuffer(
        vk::Buffer& buffer,
        vk::DeviceMemory& bufferMemory
    ) const;
    /// Unmap a given buffer's memory
    /// @param bufferMemory Buffer memory
    /// @param memoryMapped Buffer memory mapped
    void unmapMemory(
        const vk::DeviceMemory& bufferMemory,
        void*& memoryMapped
    ) const;

    /// Copy one buffer's data to another
    /// @param bufferSize Buffer size
    /// @param srcBuffer Source buffer
    /// @param dstBuffer Destination buffer
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult copyBuffer(
        vk::DeviceSize bufferSize,
        vk::Buffer srcBuffer,
        vk::Buffer dstBuffer
    ) const;
};

#endif
