#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanModelData.hpp"
#include "axr/common/result.h"
#include "axr/logger.h"

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrVulkanModelData::AxrVulkanModelData():
    m_ModelHandle(nullptr),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_TransferCommandPool(VK_NULL_HANDLE),
    m_TransferQueue(VK_NULL_HANDLE),
    m_DispatchHandle(nullptr) {
}

AxrVulkanModelData::AxrVulkanModelData(const Config& config):
    m_Name(config.Name),
    m_ModelHandle(config.ModelHandle),
    m_PhysicalDevice(config.PhysicalDevice),
    m_Device(config.Device),
    m_TransferCommandPool(config.TransferCommandPool),
    m_TransferQueue(config.TransferQueue),
    m_DispatchHandle(config.DispatchHandle) {
}

AxrVulkanModelData::AxrVulkanModelData(AxrVulkanModelData&& src) noexcept {
    m_Name = std::move(src.m_Name);
    m_MeshBuffers = std::move(src.m_MeshBuffers);

    m_ModelHandle = src.m_ModelHandle;
    m_PhysicalDevice = src.m_PhysicalDevice;
    m_Device = src.m_Device;
    m_TransferCommandPool = src.m_TransferCommandPool;
    m_TransferQueue = src.m_TransferQueue;
    m_DispatchHandle = src.m_DispatchHandle;

    src.m_ModelHandle = nullptr;
    src.m_PhysicalDevice = VK_NULL_HANDLE;
    src.m_Device = VK_NULL_HANDLE;
    src.m_TransferCommandPool = VK_NULL_HANDLE;
    src.m_TransferQueue = VK_NULL_HANDLE;
    src.m_DispatchHandle = nullptr;
}

AxrVulkanModelData::~AxrVulkanModelData() {
    cleanup();
}

AxrVulkanModelData& AxrVulkanModelData::operator=(AxrVulkanModelData&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Name = std::move(src.m_Name);
        m_MeshBuffers = std::move(src.m_MeshBuffers);

        m_ModelHandle = src.m_ModelHandle;
        m_PhysicalDevice = src.m_PhysicalDevice;
        m_Device = src.m_Device;
        m_TransferCommandPool = src.m_TransferCommandPool;
        m_TransferQueue = src.m_TransferQueue;
        m_DispatchHandle = src.m_DispatchHandle;

        src.m_ModelHandle = nullptr;
        src.m_PhysicalDevice = VK_NULL_HANDLE;
        src.m_Device = VK_NULL_HANDLE;
        src.m_TransferCommandPool = VK_NULL_HANDLE;
        src.m_TransferQueue = VK_NULL_HANDLE;
        src.m_DispatchHandle = nullptr;
    }

    return *this;
}

// ---- Public Functions ----

const std::string& AxrVulkanModelData::getName() const {
    return m_Name;
}

bool AxrVulkanModelData::doesDataExist() const {
    return !m_MeshBuffers.empty();
}

AxrResult AxrVulkanModelData::createData() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (doesDataExist()) {
        axrLogErrorLocation("Model data already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createMeshBuffers();
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create mesh buffers.");
        destroyData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanModelData::destroyData() {
    destroyMeshBuffers();
}

// ---- Private Functions ----

void AxrVulkanModelData::cleanup() {
    destroyData();

    m_Name.clear();
    m_ModelHandle = nullptr;
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_TransferCommandPool = VK_NULL_HANDLE;
    m_TransferQueue = VK_NULL_HANDLE;
    m_DispatchHandle = nullptr;
}

AxrResult AxrVulkanModelData::createMeshBuffers() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_MeshBuffers.empty()) {
        axrLogErrorLocation("Mesh buffers already exist.");
        return AXR_ERROR;
    }

    if (m_ModelHandle == nullptr) {
        axrLogErrorLocation("Model handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    if (!m_ModelHandle->isLoaded()) {
        axrResult = m_ModelHandle->loadFile();
        if (AXR_FAILED(axrResult)) {
            axrLogErrorLocation("Failed to load model named: {0}.", m_ModelHandle->getName());
            destroyMeshBuffers();
            return axrResult;
        }
    }

    const std::vector<AxrMeshRAII>& meshes = m_ModelHandle->getMeshes();
    m_MeshBuffers.resize(meshes.size());

    for (size_t i = 0; i < m_MeshBuffers.size(); ++i) {
        MeshBuffer meshBuffer{
            .Buffer = AxrVulkanBuffer(
                AxrVulkanBuffer::Config{
                    .PhysicalDevice = m_PhysicalDevice,
                    .Device = m_Device,
                    .TransferCommandPool = m_TransferCommandPool,
                    .TransferQueue = m_TransferQueue,
                    .DispatchHandle = m_DispatchHandle
                }
            ),
            .IndicesOffset = 0,
            .VerticesOffset = 0
        };
        m_MeshBuffers[i] = std::move(meshBuffer);

        axrResult = createMeshBuffer(meshes[i], m_MeshBuffers[i]);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create mesh buffers.");
        destroyMeshBuffers();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanModelData::destroyMeshBuffers() {
    for (MeshBuffer& meshBuffer : m_MeshBuffers) {
        destroyMeshBuffer(meshBuffer);
    }
    m_MeshBuffers.clear();
}

AxrResult AxrVulkanModelData::createMeshBuffer(const AxrMeshRAII& mesh, MeshBuffer& meshBuffer) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!meshBuffer.Buffer.isEmpty()) {
        axrLogErrorLocation("Mesh buffer already exist.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    if (m_DispatchHandle == nullptr) {
        axrLogErrorLocation("Dispatch handle is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    const vk::DeviceSize indexBufferSize = sizeof(uint32_t) * mesh.Indices.size();
    const vk::DeviceSize vertexBufferSize = sizeof(AxrVertex) * mesh.Vertices.size();

    meshBuffer.IndicesOffset = 0;
    meshBuffer.VerticesOffset = indexBufferSize;

    axrResult = meshBuffer.Buffer.createBuffer(
        // NOTE: We only have static meshes right now. In the future, we will have dynamic ones though
        true,
        indexBufferSize + vertexBufferSize,
        vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eVertexBuffer
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create mesh buffer.");
        meshBuffer.Buffer.destroyBuffer();
        return axrResult;
    }

    axrResult = meshBuffer.Buffer.setBufferData(
        meshBuffer.IndicesOffset,
        indexBufferSize,
        mesh.Indices.data()
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to set mesh indices.");
        meshBuffer.Buffer.destroyBuffer();
        return axrResult;
    }

    axrResult = meshBuffer.Buffer.setBufferData(
        meshBuffer.VerticesOffset,
        vertexBufferSize,
        mesh.Vertices.data()
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to set mesh indices.");
        meshBuffer.Buffer.destroyBuffer();
        return axrResult;
    }

    axrResult = meshBuffer.Buffer.convertToStaticBuffer();
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to convert to static buffer.");
        meshBuffer.Buffer.destroyBuffer();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanModelData::destroyMeshBuffer(MeshBuffer& meshBuffer) const {
    meshBuffer.Buffer.destroyBuffer();
    meshBuffer.IndicesOffset = 0;
    meshBuffer.VerticesOffset = 0;
}

#endif
