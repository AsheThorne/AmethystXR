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
    m_UniformBufferEngineAsset(AXR_ENGINE_ASSET_UNDEFINED),
    m_MaxFramesInFlight(0),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_TransferCommandPool(VK_NULL_HANDLE),
    m_TransferQueue(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr) {
}

AxrVulkanUniformBufferData::AxrVulkanUniformBufferData(const Config& config):
    m_UniformBufferHandle(config.UniformBufferHandle),
    m_UniformBufferEngineAsset(config.UniformBufferEngineAsset),
    m_MaxFramesInFlight(config.MaxFramesInFlight),
    m_PhysicalDevice(config.PhysicalDevice),
    m_Device(config.Device),
    m_TransferCommandPool(config.TransferCommandPool),
    m_TransferQueue(config.TransferQueue),
    m_DispatchHandle(config.DispatchHandle) {
    if (m_UniformBufferHandle != nullptr) {
        m_Name = m_UniformBufferHandle->getName();
    } else if (m_UniformBufferEngineAsset != AXR_ENGINE_ASSET_UNDEFINED) {
        m_Name = axrEngineAssetGetUniformBufferName(m_UniformBufferEngineAsset);
    }
}

AxrVulkanUniformBufferData::AxrVulkanUniformBufferData(AxrVulkanUniformBufferData&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_UniformBuffers = std::move(src.m_UniformBuffers);

    m_UniformBufferHandle = src.m_UniformBufferHandle;
    m_UniformBufferEngineAsset = src.m_UniformBufferEngineAsset;
    m_MaxFramesInFlight = src.m_MaxFramesInFlight;
    m_PhysicalDevice = src.m_PhysicalDevice;
    m_Device = src.m_Device;
    m_TransferCommandPool = src.m_TransferCommandPool;
    m_TransferQueue = src.m_TransferQueue;
    m_DispatchHandle = src.m_DispatchHandle;

    src.m_UniformBufferHandle = nullptr;
    src.m_UniformBufferEngineAsset = AXR_ENGINE_ASSET_UNDEFINED;
    src.m_MaxFramesInFlight = 0;
    src.m_PhysicalDevice = VK_NULL_HANDLE;
    src.m_Device = VK_NULL_HANDLE;
    src.m_TransferCommandPool = VK_NULL_HANDLE;
    src.m_TransferQueue = VK_NULL_HANDLE;
    src.m_DispatchHandle = nullptr;
}

AxrVulkanUniformBufferData::~AxrVulkanUniformBufferData() {
    cleanup();
}

AxrVulkanUniformBufferData& AxrVulkanUniformBufferData::operator=(AxrVulkanUniformBufferData&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_UniformBuffers = std::move(src.m_UniformBuffers);

        m_UniformBufferHandle = src.m_UniformBufferHandle;
        m_UniformBufferEngineAsset = src.m_UniformBufferEngineAsset;
        m_MaxFramesInFlight = src.m_MaxFramesInFlight;
        m_PhysicalDevice = src.m_PhysicalDevice;
        m_Device = src.m_Device;
        m_TransferCommandPool = src.m_TransferCommandPool;
        m_TransferQueue = src.m_TransferQueue;
        m_DispatchHandle = src.m_DispatchHandle;

        src.m_UniformBufferHandle = nullptr;
        src.m_UniformBufferEngineAsset = AXR_ENGINE_ASSET_UNDEFINED;
        src.m_MaxFramesInFlight = 0;
        src.m_PhysicalDevice = VK_NULL_HANDLE;
        src.m_Device = VK_NULL_HANDLE;
        src.m_TransferCommandPool = VK_NULL_HANDLE;
        src.m_TransferQueue = VK_NULL_HANDLE;
        src.m_DispatchHandle = nullptr;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanUniformBufferData::getName() const {
    return m_Name;
}

const AxrVulkanBuffer& AxrVulkanUniformBufferData::getBuffer(const uint32_t frameIndex) const {
    return m_UniformBuffers[frameIndex];
}

vk::DeviceSize AxrVulkanUniformBufferData::getBufferSize() const {
    if (m_UniformBufferHandle != nullptr) {
        return m_UniformBufferHandle->getDataSize();
    }

    return axrEngineAssetGetUniformBufferSize(m_UniformBufferEngineAsset);
}

vk::DeviceSize AxrVulkanUniformBufferData::getInstanceSize() const {
    if (m_UniformBufferHandle != nullptr) {
        return m_UniformBufferHandle->getInstanceSize();
    }

    return axrEngineAssetGetUniformBufferInstanceSize(m_UniformBufferEngineAsset);
}

AxrUniformBufferTypeEnum AxrVulkanUniformBufferData::getBufferType() const {
    if (m_UniformBufferHandle != nullptr) {
        return m_UniformBufferHandle->getBufferType();
    }

    return axrEngineAssetGetUniformBufferType(m_UniformBufferEngineAsset);

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
    m_Name.clear();
    m_UniformBufferHandle = nullptr;
    m_MaxFramesInFlight = 0;
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_TransferCommandPool = VK_NULL_HANDLE;
    m_TransferQueue = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanUniformBufferData::createUniformBuffer(AxrVulkanBuffer& buffer) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!buffer.isEmpty()) {
        axrLogErrorLocation("Uniform buffer already exists.");
        return AXR_ERROR;
    }

    if (m_UniformBufferHandle == nullptr && m_UniformBufferEngineAsset == AXR_ENGINE_ASSET_UNDEFINED) {
        axrLogErrorLocation(
            "Uniform buffer handle is null and uniform buffer engine asset is undefined. One of these must be defined."
        );
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    uint64_t size = 0;
    const void* data = nullptr;

    if (m_UniformBufferHandle != nullptr) {
        size = m_UniformBufferHandle->getDataSize();
        data = m_UniformBufferHandle->getData();
    } else {
        size = axrEngineAssetGetUniformBufferSize(m_UniformBufferEngineAsset);
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

    if (data != nullptr) {
        axrResult = buffer.setBufferData(0, size, data);
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
