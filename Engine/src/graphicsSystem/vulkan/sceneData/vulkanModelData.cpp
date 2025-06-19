#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanModelData.hpp"
#include "axr/common/enums.h"
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
    m_ModelBuffer = std::move(src.m_ModelBuffer);
    m_MeshBufferLocations = std::move(src.m_MeshBufferLocations);

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
        m_ModelBuffer = std::move(src.m_ModelBuffer);
        m_MeshBufferLocations = std::move(src.m_MeshBufferLocations);

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

const vk::Buffer& AxrVulkanModelData::getModelBuffer() const {
    return m_ModelBuffer.getBuffer();
}

const vk::DeviceSize& AxrVulkanModelData::getSubmeshBufferIndicesOffset(const uint32_t meshIndex, const uint32_t submeshIndex) const {
    return m_MeshBufferLocations[meshIndex].SubmeshLocations[submeshIndex].IndicesOffset;
}

const vk::DeviceSize& AxrVulkanModelData::getSubmeshBufferVerticesOffset(const uint32_t meshIndex, const uint32_t submeshIndex) const {
    return m_MeshBufferLocations[meshIndex].SubmeshLocations[submeshIndex].VerticesOffset;
}

const uint32_t& AxrVulkanModelData::getSubmeshIndexCount(const uint32_t meshIndex, const uint32_t submeshIndex) const {
    return m_MeshBufferLocations[meshIndex].SubmeshLocations[submeshIndex].IndexCount;
}

bool AxrVulkanModelData::doesDataExist() const {
    return !m_ModelBuffer.isEmpty();
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

    axrResult = createModelBuffer();
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create mesh buffers.");
        destroyData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanModelData::destroyData() {
    destroyModelBuffer();
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

AxrResult AxrVulkanModelData::createModelBuffer() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_ModelBuffer.isEmpty()) {
        axrLogErrorLocation("Model buffer already exists.");
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
            destroyModelBuffer();
            return axrResult;
        }
    }

    const std::vector<AxrMeshRAII>& meshes = m_ModelHandle->getMeshes();
    m_ModelBuffer = AxrVulkanBuffer(
        {
            .PhysicalDevice = m_PhysicalDevice,
            .Device = m_Device,
            .TransferCommandPool = m_TransferCommandPool,
            .TransferQueue = m_TransferQueue,
            .DispatchHandle = m_DispatchHandle
        }
    );

    vk::DeviceSize indexBufferSize = 0;
    vk::DeviceSize vertexBufferSize = 0;

    for (const AxrMeshRAII& mesh : meshes) {
        for (const AxrSubmeshRAII& submesh : mesh.Submeshes) {
            indexBufferSize += sizeof(uint32_t) * submesh.Indices.size();
            vertexBufferSize += sizeof(AxrVertex) * submesh.Vertices.size();
        }
    }

    axrResult = m_ModelBuffer.createBuffer(
        // NOTE: We only have static meshes right now. In the future, we will have dynamic ones though
        //  But if we have dynamic ones, does that mean we need a new buffer for each frame in flight!?!?! I hope not.
        //  But if we don't, then why do we for uniform buffers?
        true,
        indexBufferSize + vertexBufferSize,
        vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eVertexBuffer
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create mesh buffer.");
        destroyModelBuffer();
        return axrResult;
    }

    vk::DeviceSize indicesOffset = 0;
    vk::DeviceSize verticesOffset = indexBufferSize;

    m_MeshBufferLocations.resize(meshes.size());
    for (size_t i = 0; i < meshes.size(); ++i) {
        m_MeshBufferLocations[i].SubmeshLocations.resize(meshes[i].Submeshes.size());

        for (size_t j = 0; j < meshes[i].Submeshes.size(); ++j) {
            SubmeshBufferLocation& submeshBufferLocation = m_MeshBufferLocations[i].SubmeshLocations[j];
            submeshBufferLocation.IndicesOffset = indicesOffset;
            submeshBufferLocation.VerticesOffset = verticesOffset;
            submeshBufferLocation.IndexCount = static_cast<uint32_t>(meshes[i].Submeshes[j].Indices.size());

            const vk::DeviceSize submeshIndicesSize = sizeof(uint32_t) * meshes[i].Submeshes[j].Indices.size();
            const vk::DeviceSize submeshVerticesSize = sizeof(AxrVertex) * meshes[i].Submeshes[j].Vertices.size();

            axrResult = m_ModelBuffer.setBufferData(
                submeshBufferLocation.IndicesOffset,
                submeshIndicesSize,
                meshes[i].Submeshes[j].Indices.data()
            );
            if (AXR_FAILED(axrResult)) {
                axrLogErrorLocation("Failed to set submesh indices.");
                destroyModelBuffer();
                return axrResult;
            }

            axrResult = m_ModelBuffer.setBufferData(
                submeshBufferLocation.VerticesOffset,
                submeshVerticesSize,
                meshes[i].Submeshes[j].Vertices.data()
            );
            if (AXR_FAILED(axrResult)) {
                axrLogErrorLocation("Failed to set submesh indices.");
                destroyModelBuffer();
                return axrResult;
            }

            indicesOffset += submeshIndicesSize;
            verticesOffset += submeshVerticesSize;
        }
    }

    axrResult = m_ModelBuffer.convertToStaticBuffer();
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to convert to static buffer.");
        destroyModelBuffer();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanModelData::destroyModelBuffer() {
    m_ModelBuffer.destroyBuffer();
    m_MeshBufferLocations.clear();
}

#endif
