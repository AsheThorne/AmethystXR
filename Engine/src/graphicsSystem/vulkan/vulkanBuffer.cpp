#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanBuffer.hpp"
#include "axr/logger.h"
#include "axr/common/defines.h"
#include "vulkanUtils.hpp"

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrVulkanBuffer::AxrVulkanBuffer():
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_TransferCommandPool(VK_NULL_HANDLE),
    m_TransferQueue(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr),
    m_WillBufferBeStatic(false),
    m_IsBufferStatic(false),
    m_BufferSize(0),
    m_BufferUsageFlags(static_cast<vk::BufferUsageFlagBits>(VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM)),
    m_Buffer(VK_NULL_HANDLE),
    m_BufferMemory(VK_NULL_HANDLE),
    m_MappedMemory(nullptr) {
}

AxrVulkanBuffer::AxrVulkanBuffer(const Config& config):
    m_PhysicalDevice(config.PhysicalDevice),
    m_Device(config.Device),
    m_TransferCommandPool(config.TransferCommandPool),
    m_TransferQueue(config.TransferQueue),
    m_DispatchHandle(config.DispatchHandle),
    m_WillBufferBeStatic(false),
    m_IsBufferStatic(false),
    m_BufferSize(0),
    m_BufferUsageFlags(static_cast<vk::BufferUsageFlagBits>(VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM)),
    m_Buffer(VK_NULL_HANDLE),
    m_BufferMemory(VK_NULL_HANDLE),
    m_MappedMemory(nullptr) {
}

AxrVulkanBuffer::AxrVulkanBuffer(AxrVulkanBuffer&& src) noexcept {
    m_PhysicalDevice = src.m_PhysicalDevice;
    m_Device = src.m_Device;
    m_TransferCommandPool = src.m_TransferCommandPool;
    m_TransferQueue = src.m_TransferQueue;
    m_DispatchHandle = src.m_DispatchHandle;
    m_WillBufferBeStatic = src.m_WillBufferBeStatic;
    m_IsBufferStatic = src.m_IsBufferStatic;
    m_BufferSize = src.m_BufferSize;
    m_BufferUsageFlags = src.m_BufferUsageFlags;
    m_Buffer = src.m_Buffer;
    m_BufferMemory = src.m_BufferMemory;
    m_MappedMemory = src.m_MappedMemory;

    src.m_PhysicalDevice = VK_NULL_HANDLE;
    src.m_Device = VK_NULL_HANDLE;
    src.m_TransferCommandPool = VK_NULL_HANDLE;
    src.m_TransferQueue = VK_NULL_HANDLE;
    src.m_DispatchHandle = nullptr;
    src.m_WillBufferBeStatic = false;
    src.m_IsBufferStatic = false;
    src.m_BufferSize = 0;
    src.m_BufferUsageFlags = static_cast<vk::BufferUsageFlagBits>(VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM);
    src.m_Buffer = VK_NULL_HANDLE;
    src.m_BufferMemory = VK_NULL_HANDLE;
    src.m_MappedMemory = nullptr;
}

AxrVulkanBuffer::~AxrVulkanBuffer() {
    cleanup();
}

AxrVulkanBuffer& AxrVulkanBuffer::operator=(AxrVulkanBuffer&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_PhysicalDevice = src.m_PhysicalDevice;
        m_Device = src.m_Device;
        m_TransferCommandPool = src.m_TransferCommandPool;
        m_TransferQueue = src.m_TransferQueue;
        m_DispatchHandle = src.m_DispatchHandle;
        m_WillBufferBeStatic = src.m_WillBufferBeStatic;
        m_IsBufferStatic = src.m_IsBufferStatic;
        m_BufferSize = src.m_BufferSize;
        m_BufferUsageFlags = src.m_BufferUsageFlags;
        m_Buffer = src.m_Buffer;
        m_BufferMemory = src.m_BufferMemory;
        m_MappedMemory = src.m_MappedMemory;

        src.m_PhysicalDevice = VK_NULL_HANDLE;
        src.m_Device = VK_NULL_HANDLE;
        src.m_TransferCommandPool = VK_NULL_HANDLE;
        src.m_TransferQueue = VK_NULL_HANDLE;
        src.m_DispatchHandle = nullptr;
        src.m_WillBufferBeStatic = false;
        src.m_IsBufferStatic = false;
        src.m_BufferSize = 0;
        src.m_BufferUsageFlags = static_cast<vk::BufferUsageFlagBits>(VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM);
        src.m_Buffer = VK_NULL_HANDLE;
        src.m_BufferMemory = VK_NULL_HANDLE;
        src.m_MappedMemory = nullptr;
    }

    return *this;
}

bool AxrVulkanBuffer::isEmpty() const {
    return m_Buffer == VK_NULL_HANDLE &&
        m_BufferMemory == VK_NULL_HANDLE &&
        m_BufferSize == 0;
}

const vk::Buffer& AxrVulkanBuffer::getBuffer() const {
    return m_Buffer;
}

AxrResult AxrVulkanBuffer::createBuffer(
    const bool willBufferBeStatic,
    const vk::DeviceSize bufferSize,
    const vk::BufferUsageFlags bufferUsageFlags
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!isEmpty()) {
        axrLogErrorLocation("Buffer already exists.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogError("Device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogError("Dispatch handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;
    m_WillBufferBeStatic = willBufferBeStatic;
    m_BufferSize = bufferSize;
    m_BufferUsageFlags = bufferUsageFlags;

    axrResult = createBuffer(
        m_BufferSize,
        // If the buffer will get turned into a static buffer later down the line,
        // ignore the given buffer usage flags.
        // We'll use them for the buffer on the GPU.
        m_WillBufferBeStatic ? vk::BufferUsageFlagBits::eTransferSrc : m_BufferUsageFlags,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        m_Buffer,
        m_BufferMemory
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create buffer.");
        destroyBuffer();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanBuffer::destroyBuffer() {
    destroyBuffer(m_Buffer, m_BufferMemory, m_MappedMemory);
    m_WillBufferBeStatic = false;
    m_IsBufferStatic = false;
    m_BufferSize = 0;
    m_BufferUsageFlags = static_cast<vk::BufferUsageFlagBits>(VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM);
}

AxrResult AxrVulkanBuffer::convertToStaticBuffer() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (isEmpty()) {
        axrLogErrorLocation("Buffer doesn't exist.");
        return AXR_ERROR;
    }

    if (m_IsBufferStatic) {
        axrLogErrorLocation("Buffer is already static.");
        return AXR_ERROR;
    }

    if (!m_WillBufferBeStatic) {
        axrLogErrorLocation("Buffer wasn't created with the intention of it becoming static.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogError("Device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogError("Dispatch handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;
    vk::Buffer oldBuffer = m_Buffer;
    m_Buffer = VK_NULL_HANDLE;
    vk::DeviceMemory oldBufferMemory = m_BufferMemory;
    m_BufferMemory = VK_NULL_HANDLE;

    axrResult = createBuffer(
        m_BufferSize,
        vk::BufferUsageFlagBits::eTransferDst | m_BufferUsageFlags,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_Buffer,
        m_BufferMemory
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create buffer.");
        destroyBuffer(m_Buffer, m_BufferMemory);
        m_Buffer = oldBuffer;
        m_BufferMemory = oldBufferMemory;
        return axrResult;
    }

    axrResult = copyBuffer(
        m_BufferSize,
        oldBuffer,
        m_Buffer
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to copy buffer.");
        destroyBuffer(m_Buffer, m_BufferMemory);
        m_Buffer = oldBuffer;
        m_BufferMemory = oldBufferMemory;
        return axrResult;
    }

    destroyBuffer(oldBuffer, oldBufferMemory);

    m_IsBufferStatic = true;
    return AXR_SUCCESS;
}

AxrResult AxrVulkanBuffer::setBufferData(const vk::DeviceSize offset, const vk::DeviceSize size, const void* data) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_IsBufferStatic) {
        axrLogErrorLocation("Buffer is static and cannot be modified.");
        return AXR_ERROR;
    }

    if (isEmpty()) {
        axrLogErrorLocation("Buffer hasn't been created.");
        return AXR_ERROR;
    }

    if (offset + size > m_BufferSize) {
        axrLogErrorLocation("Buffer data is out of bounds.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogError("Device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogError("Dispatch handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::Result vkResult = m_Device.mapMemory(
        m_BufferMemory,
        offset,
        size,
        {},
        &m_MappedMemory,
        *m_DispatchHandle
    );
    axrLogVkResult(vkResult, "m_Device.mapMemory");
    if (VK_FAILED(vkResult)) {
        unmapMemory(m_BufferMemory, m_MappedMemory);
        return AXR_ERROR;
    }

    const errno_t memcpyError = memcpy_s(
        m_MappedMemory,
        size,
        data,
        size
    );
    unmapMemory(m_BufferMemory, m_MappedMemory);

    if (memcpyError != 0) {
        axrLogErrorLocation("Failed to copy memory.");
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrVulkanBuffer::cleanup() {
    destroyBuffer();

    m_PhysicalDevice = VK_NULL_HANDLE;
    m_TransferCommandPool = VK_NULL_HANDLE;
    m_TransferQueue = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanBuffer::createBuffer(
    const vk::DeviceSize size,
    const vk::BufferUsageFlags usage,
    const vk::MemoryPropertyFlags properties,
    vk::Buffer& buffer,
    vk::DeviceMemory& bufferMemory
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (buffer != VK_NULL_HANDLE) {
        axrLogErrorLocation("Buffer already exists.");
        return AXR_ERROR;
    }

    if (bufferMemory != VK_NULL_HANDLE) {
        axrLogErrorLocation("Buffer already exists.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::BufferCreateInfo bufferCreateInfo(
        {},
        size,
        usage,
        vk::SharingMode::eExclusive
    );

    vk::Result vkResult = m_Device.createBuffer(&bufferCreateInfo, nullptr, &buffer, *m_DispatchHandle);
    axrLogVkResult(vkResult, "device.createBuffer");
    if (VK_FAILED(vkResult)) {
        destroyBuffer(buffer, bufferMemory);
        return AXR_ERROR;
    }

    const vk::MemoryRequirements memoryRequirements = m_Device.getBufferMemoryRequirements(buffer, *m_DispatchHandle);
    uint32_t memoryTypeIndex = 0;
    const AxrResult axrResult = axrFindMemoryTypeIndex(
        m_PhysicalDevice,
        memoryRequirements.memoryTypeBits,
        properties,
        memoryTypeIndex,
        *m_DispatchHandle
    );
    if (AXR_FAILED(axrResult)) {
        destroyBuffer(buffer, bufferMemory);
        return AXR_ERROR;
    }

    const vk::MemoryAllocateInfo memoryAllocateInfo(
        memoryRequirements.size,
        memoryTypeIndex
    );

    vkResult = m_Device.allocateMemory(
        &memoryAllocateInfo,
        nullptr,
        &bufferMemory,
        *m_DispatchHandle
    );
    axrLogVkResult(vkResult, "device.allocateMemory");
    if (VK_FAILED(vkResult)) {
        destroyBuffer(buffer, bufferMemory);
        return AXR_ERROR;
    }

    vkResult = m_Device.bindBufferMemory(
        buffer,
        bufferMemory,
        0,
        *m_DispatchHandle
    );
    axrLogVkResult(vkResult, "device.bindBufferMemory");
    if (VK_FAILED(vkResult)) {
        destroyBuffer(buffer, bufferMemory);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanBuffer::destroyBuffer(
    vk::Buffer& buffer,
    vk::DeviceMemory& bufferMemory,
    void*& memoryMapped
) const {
    unmapMemory(bufferMemory, memoryMapped);
    destroyBuffer(buffer, bufferMemory);
}

void AxrVulkanBuffer::destroyBuffer(
    vk::Buffer& buffer,
    vk::DeviceMemory& bufferMemory
) const {
    if (buffer != VK_NULL_HANDLE) {
        m_Device.destroyBuffer(buffer, nullptr, *m_DispatchHandle);
        buffer = VK_NULL_HANDLE;
    }

    if (bufferMemory != VK_NULL_HANDLE) {
        m_Device.freeMemory(bufferMemory, nullptr, *m_DispatchHandle);
        bufferMemory = VK_NULL_HANDLE;
    }
}

void AxrVulkanBuffer::unmapMemory(const vk::DeviceMemory& bufferMemory, void*& memoryMapped) const {
    if (memoryMapped == nullptr) return;

    m_Device.unmapMemory(bufferMemory, *m_DispatchHandle);
    memoryMapped = nullptr;
}

AxrResult AxrVulkanBuffer::copyBuffer(
    const vk::DeviceSize bufferSize,
    const vk::Buffer srcBuffer,
    const vk::Buffer dstBuffer
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device == VK_NULL_HANDLE) {
        axrLogError("Device is null.");
        return AXR_ERROR;
    }

    if (m_TransferCommandPool == VK_NULL_HANDLE) {
        axrLogError("Transfer command pool is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogError("Dispatch handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    vk::CommandBuffer commandBuffer;
    axrResult = axrBeginSingleTimeCommand(
        m_Device,
        m_TransferCommandPool,
        commandBuffer,
        *m_DispatchHandle
    );
    if (AXR_FAILED(axrResult)) {
        // TOD: Destroy buffer
        return AXR_ERROR;
    }

    const vk::BufferCopy copyRegion(
        0,
        0,
        bufferSize
    );
    commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion, *m_DispatchHandle);

    axrResult = axrEndSingleTimeCommand(
        m_Device,
        m_TransferCommandPool,
        m_TransferQueue,
        commandBuffer,
        *m_DispatchHandle
    );
    if (AXR_FAILED(axrResult)) {
        // TOD: Destroy buffer
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

#endif
