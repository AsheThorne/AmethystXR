#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "../../../assets/model.hpp"
#include "../vulkanBuffer.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan model data
class AxrVulkanModelData {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Model data config
    struct Config {
        std::string Name;
        const AxrModel* ModelHandle;
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool TransferCommandPool;
        vk::Queue TransferQueue;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    /// Model mesh buffer
    struct MeshBuffer {
        AxrVulkanBuffer Buffer;
        vk::DeviceSize IndicesOffset;
        vk::DeviceSize VerticesOffset;
        uint32_t IndexCount;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanModelData();
    /// Constructor
    /// @param config Model data config
    AxrVulkanModelData(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanModelData to copy from
    AxrVulkanModelData(const AxrVulkanModelData& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanModelData to move from
    AxrVulkanModelData(AxrVulkanModelData&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanModelData();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanModelData to copy from
    AxrVulkanModelData& operator=(const AxrVulkanModelData& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanModelData to move from
    AxrVulkanModelData& operator=(AxrVulkanModelData&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the model name
    /// @returns The model name
    [[nodiscard]] const std::string& getName() const;
    /// Get the mesh buffer for the given mesh index
    /// @param meshIndex Mesh index
    /// @returns The mesh buffer
    [[nodiscard]] const vk::Buffer& getMeshBuffer(uint32_t meshIndex) const;
    /// Get the mesh buffer indices offset for the given mesh index
    /// @param meshIndex Mesh index
    /// @returns The mesh buffer indices offset
    [[nodiscard]] const vk::DeviceSize& getMeshBufferIndicesOffset(uint32_t meshIndex) const;
    /// Get the mesh buffer vertices offset for the given mesh index
    /// @param meshIndex Mesh index
    /// @returns The mesh buffer vertices offset
    [[nodiscard]] const vk::DeviceSize& getMeshBufferVerticesOffset(uint32_t meshIndex) const;
    /// Get the mesh index count for the given mesh index
    /// @param meshIndex Mesh index
    /// @returns The mesh index count
    [[nodiscard]] const uint32_t& getMeshIndexCount(uint32_t meshIndex) const;

    /// Check if the data exists
    /// @returns True if the data exists
    [[nodiscard]] bool doesDataExist() const;

    /// Create the model data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createData();
    /// Destroy the model data
    void destroyData();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    const AxrModel* m_ModelHandle;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_TransferCommandPool;
    vk::Queue m_TransferQueue;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    std::vector<MeshBuffer> m_MeshBuffers;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    /// Create all the mesh buffers
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMeshBuffers();
    /// Destroy all mesh buffers
    void destroyMeshBuffers();

    /// Create a mesh buffer for the given mesh
    /// @param mesh Mesh to use
    /// @param meshBuffer Output mesh buffer
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMeshBuffer(const AxrMeshRAII& mesh, MeshBuffer& meshBuffer) const;
    /// Destroy the given mesh buffer
    /// @param meshBuffer Mesh buffer to destroy
    void destroyMeshBuffer(MeshBuffer& meshBuffer) const;
};
