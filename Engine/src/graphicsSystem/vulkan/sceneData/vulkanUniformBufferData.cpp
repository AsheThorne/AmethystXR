#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanUniformBufferData.hpp"
#include "axr/logger.h"
#include "../../../assets/engineAssets.hpp"

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrVulkanUniformBufferData::AxrVulkanUniformBufferData():
    m_UniformBufferHandle(nullptr),
    m_MaxFramesInFlight(0),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_TransferCommandPool(VK_NULL_HANDLE),
    m_TransferQueue(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr),
    m_UniformBufferAlignment(0) {
}

AxrVulkanUniformBufferData::AxrVulkanUniformBufferData(const Config& config):
    m_UniformBufferHandle(config.UniformBufferHandle),
    m_MaxFramesInFlight(config.MaxFramesInFlight),
    m_PhysicalDevice(config.PhysicalDevice),
    m_Device(config.Device),
    m_TransferCommandPool(config.TransferCommandPool),
    m_TransferQueue(config.TransferQueue),
    m_DispatchHandle(config.DispatchHandle),
    m_UniformBufferAlignment(0) {
}

AxrVulkanUniformBufferData::AxrVulkanUniformBufferData(AxrVulkanUniformBufferData&& src) noexcept {
    m_UniformBuffers = std::move(src.m_UniformBuffers);

    m_UniformBufferHandle = src.m_UniformBufferHandle;
    m_MaxFramesInFlight = src.m_MaxFramesInFlight;
    m_PhysicalDevice = src.m_PhysicalDevice;
    m_Device = src.m_Device;
    m_TransferCommandPool = src.m_TransferCommandPool;
    m_TransferQueue = src.m_TransferQueue;
    m_DispatchHandle = src.m_DispatchHandle;
    m_UniformBufferAlignment = src.m_UniformBufferAlignment;

    src.m_UniformBufferHandle = nullptr;
    src.m_MaxFramesInFlight = 0;
    src.m_PhysicalDevice = VK_NULL_HANDLE;
    src.m_Device = VK_NULL_HANDLE;
    src.m_TransferCommandPool = VK_NULL_HANDLE;
    src.m_TransferQueue = VK_NULL_HANDLE;
    src.m_DispatchHandle = nullptr;
    src.m_UniformBufferAlignment = 0;
}

AxrVulkanUniformBufferData::~AxrVulkanUniformBufferData() {
    cleanup();
}

AxrVulkanUniformBufferData& AxrVulkanUniformBufferData::operator=(AxrVulkanUniformBufferData&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_UniformBuffers = std::move(src.m_UniformBuffers);

        m_UniformBufferHandle = src.m_UniformBufferHandle;
        m_MaxFramesInFlight = src.m_MaxFramesInFlight;
        m_PhysicalDevice = src.m_PhysicalDevice;
        m_Device = src.m_Device;
        m_TransferCommandPool = src.m_TransferCommandPool;
        m_TransferQueue = src.m_TransferQueue;
        m_DispatchHandle = src.m_DispatchHandle;
        m_UniformBufferAlignment = src.m_UniformBufferAlignment;

        src.m_UniformBufferHandle = nullptr;
        src.m_MaxFramesInFlight = 0;
        src.m_PhysicalDevice = VK_NULL_HANDLE;
        src.m_Device = VK_NULL_HANDLE;
        src.m_TransferCommandPool = VK_NULL_HANDLE;
        src.m_TransferQueue = VK_NULL_HANDLE;
        src.m_DispatchHandle = nullptr;
        src.m_UniformBufferAlignment = 0;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanUniformBufferData::getName() const {
    if (m_UniformBufferHandle == nullptr) {
        return m_DummyName;
    }

    return m_UniformBufferHandle->getName();
}

const AxrVulkanBuffer& AxrVulkanUniformBufferData::getBuffer(const uint32_t frameIndex) const {
    return m_UniformBuffers[frameIndex];
}

vk::DeviceSize AxrVulkanUniformBufferData::getBufferSize() const {
    if (m_UniformBufferHandle == nullptr) {
        return 0;
    }

    return m_UniformBufferHandle->getDataSize();
}

vk::DeviceSize AxrVulkanUniformBufferData::getInstanceSize() const {
    if (m_UniformBufferHandle == nullptr) {
        return 0;
    }

    return m_UniformBufferHandle->getInstanceSize();
}

AxrUniformBufferTypeEnum AxrVulkanUniformBufferData::getBufferType() const {
    if (m_UniformBufferHandle == nullptr) {
        return AXR_UNIFORM_BUFFER_TYPE_UNDEFINED;
    }

    return m_UniformBufferHandle->getBufferType();
}

bool AxrVulkanUniformBufferData::doesDataExist() const {
    return !m_UniformBuffers.empty();
}

AxrResult AxrVulkanUniformBufferData::createData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_UniformBuffers.empty()) {
        axrLogErrorLocation("Uniform buffers already exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = setAlignment();
    if (AXR_FAILED(axrResult)) {
        destroyData();
        return axrResult;
    }

    m_UniformBuffers.resize(m_MaxFramesInFlight);

    for (uint32_t i = 0; i < m_MaxFramesInFlight; ++i) {
        auto buffer = AxrVulkanBuffer(
            AxrVulkanBuffer::Config{
                .PhysicalDevice = m_PhysicalDevice,
                .Device = m_Device,
                .TransferCommandPool = m_TransferCommandPool,
                .TransferQueue = m_TransferQueue,
                .DispatchHandle = m_DispatchHandle
            }
        );
        m_UniformBuffers[i] = std::move(buffer);

        axrResult = createUniformBuffer(m_UniformBuffers[i]);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        destroyData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanUniformBufferData::destroyData() {
    for (AxrVulkanBuffer& uniformBuffer : m_UniformBuffers) {
        destroyUniformBuffer(uniformBuffer);
    }
    m_UniformBuffers.clear();
    resetAlignment();
}

AxrResult AxrVulkanUniformBufferData::setData(
    const uint32_t index,
    const vk::DeviceSize offset,
    const vk::DeviceSize size,
    const void* data
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (data == nullptr) {
        axrLogErrorLocation("Data is null.");
        return AXR_ERROR;
    }

    if (index > m_UniformBuffers.size() - 1) {
        axrLogErrorLocation("Index out of bounds.");
        return AXR_ERROR;
    }

    if (m_UniformBuffers[index].isEmpty()) {
        axrLogErrorLocation("Buffer is empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    return m_UniformBuffers[index].setBufferData(offset, size, data);
}

// ---- Private Functions ----

void AxrVulkanUniformBufferData::cleanup() {
    destroyData();

    m_UniformBufferHandle = nullptr;
    m_MaxFramesInFlight = 0;
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_TransferCommandPool = VK_NULL_HANDLE;
    m_TransferQueue = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
    m_UniformBufferAlignment = 0;
}

AxrResult AxrVulkanUniformBufferData::setAlignment() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch handle is null.");
        return AXR_ERROR;
    }

    if (m_UniformBufferHandle == nullptr) {
        axrLogErrorLocation("Uniform buffer handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::PhysicalDeviceProperties properties = m_PhysicalDevice.getProperties(*m_DispatchHandle);
    const uint64_t minUniformBufferOffsetAlignment = properties.limits.minUniformBufferOffsetAlignment;

    const uint64_t instanceSize = m_UniformBufferHandle->getInstanceSize();

    m_UniformBufferAlignment = instanceSize;

    if (minUniformBufferOffsetAlignment > 0) {
        m_UniformBufferAlignment =
            (m_UniformBufferAlignment + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1);
    }

    return AXR_SUCCESS;
}

void AxrVulkanUniformBufferData::resetAlignment() {
    m_UniformBufferAlignment = 0;
}

AxrResult AxrVulkanUniformBufferData::createUniformBuffer(AxrVulkanBuffer& buffer) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!buffer.isEmpty()) {
        axrLogErrorLocation("Uniform buffer already exists.");
        return AXR_ERROR;
    }

    if (m_UniformBufferHandle == nullptr) {
        axrLogErrorLocation("Uniform buffer handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    const uint64_t instanceSize = m_UniformBufferHandle->getInstanceSize();
    const bool isDataAligned = instanceSize == m_UniformBufferAlignment;
    const bool alignData = m_UniformBufferHandle->getBufferType() == AXR_UNIFORM_BUFFER_TYPE_DYNAMIC && !isDataAligned;
    uint64_t size;
    if (alignData) {
        size = m_UniformBufferHandle->getInstanceCount() * m_UniformBufferAlignment;
    } else {
        size = m_UniformBufferHandle->getDataSize();
    }

    axrResult = buffer.createBuffer(
        false,
        size,
        vk::BufferUsageFlagBits::eUniformBuffer
    );
    if (AXR_FAILED(axrResult)) {
        destroyUniformBuffer(buffer);
        return axrResult;
    }

    const void* srcData = m_UniformBufferHandle->getData();

    if (srcData != nullptr) {
        if (alignData) {
            void* alignedData = calloc(1, size);

            for (int i = 0; i < m_UniformBufferHandle->getInstanceCount(); ++i) {
                memcpy_s(
                    static_cast<int8_t*>(alignedData) + m_UniformBufferAlignment * i,
                    size - m_UniformBufferAlignment * i,
                    static_cast<const int8_t*>(srcData) + m_UniformBufferAlignment * i,
                    instanceSize
                );
            }

            axrResult = buffer.setBufferData(0, size, alignedData);
            free(alignedData);
        } else {
            axrResult = buffer.setBufferData(0, size, srcData);
        }

        if (AXR_FAILED(axrResult)) {
            destroyUniformBuffer(buffer);
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

void AxrVulkanUniformBufferData::destroyUniformBuffer(AxrVulkanBuffer& buffer) const {
    buffer.destroyBuffer();
}

#endif
