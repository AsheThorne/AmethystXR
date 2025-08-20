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
        const AxrModel* ModelHandle;
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool TransferCommandPool;
        vk::Queue TransferQueue;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    /// Submesh buffer location
    struct SubmeshBufferLocation {
        vk::DeviceSize IndicesOffset;
        vk::DeviceSize VerticesOffset;
        uint32_t IndexCount;
    };

    /// Mesh buffer location
    struct MeshBufferLocation {
        std::vector<SubmeshBufferLocation> SubmeshLocations;
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
    /// Get the model buffer
    /// @returns The model buffer
    [[nodiscard]] const vk::Buffer& getModelBuffer() const;
    /// Get the model mesh count
    /// @returns The model mesh count
    [[nodiscard]] uint32_t getMeshCount() const;
    /// Get the submesh count for the given mesh index
    /// @param meshIndex Mesh index to use
    /// @returns The submesh count
    [[nodiscard]] uint32_t getSubmeshCount(uint32_t meshIndex) const;
    /// Get the submesh buffer indices offset for the given mesh and submesh index
    /// @param meshIndex Mesh index
    /// @param submeshIndex Submesh index
    /// @returns The submesh buffer indices offset
    [[nodiscard]] const vk::DeviceSize& getSubmeshBufferIndicesOffset(uint32_t meshIndex, uint32_t submeshIndex) const;
    /// Get the submesh buffer vertices offset for the given mesh and submesh index
    /// @param meshIndex Mesh index
    /// @param submeshIndex Submesh index
    /// @returns The submesh buffer vertices offset
    [[nodiscard]] const vk::DeviceSize& getSubmeshBufferVerticesOffset(uint32_t meshIndex, uint32_t submeshIndex) const;
    /// Get the submesh index count for the given mesh and submesh index
    /// @param meshIndex Mesh index
    /// @param submeshIndex Submesh index
    /// @returns The submesh index count
    [[nodiscard]] const uint32_t& getSubmeshIndexCount(uint32_t meshIndex, uint32_t submeshIndex) const;

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
    const AxrModel* m_ModelHandle;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_TransferCommandPool;
    vk::Queue m_TransferQueue;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    /// These should never be used for anything other than returning a reference to the name if no name can be found.
    std::string m_DummyName;
    AxrVulkanBuffer m_ModelBuffer;
    std::vector<MeshBufferLocation> m_MeshBufferLocations;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    /// Create the model buffer
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createModelBuffer();
    /// Destroy the model buffer
    void destroyModelBuffer();
};
