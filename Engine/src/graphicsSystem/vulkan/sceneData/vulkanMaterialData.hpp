#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "vulkanMaterialLayoutData.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan material data
class AxrVulkanMaterialData {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Material data config
    struct Config {
        const AxrShader* VertexShaderHandle;
        const AxrShader* FragmentShaderHandle;
        const AxrMaterial* MaterialHandle;
        const AxrVulkanMaterialLayoutData* MaterialLayoutData;
        uint32_t MaxFramesInFlight;
        vk::Device Device;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanMaterialData();
    /// Constructor
    /// @param config Material data config
    AxrVulkanMaterialData(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanMaterialData to copy from
    AxrVulkanMaterialData(const AxrVulkanMaterialData& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanMaterialData to move from
    AxrVulkanMaterialData(AxrVulkanMaterialData&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanMaterialData();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanMaterialData to copy from
    AxrVulkanMaterialData& operator=(const AxrVulkanMaterialData& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanMaterialData to move from
    AxrVulkanMaterialData& operator=(AxrVulkanMaterialData&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the material name
    /// @returns The material name
    [[nodiscard]] const std::string& getName() const;
    /// Get the material layout data used for this material data
    /// @returns The material layout data
    [[nodiscard]] const AxrVulkanMaterialLayoutData* getMaterialLayoutData() const;
    /// Get the window pipeline
    /// @returns The window pipeline
    [[nodiscard]] const vk::Pipeline& getWindowPipeline() const;
    /// Get the push constant buffer name
    /// @returns The push constant buffer name
    [[nodiscard]] const std::string& getPushConstantBufferName() const;
    /// Get the descriptor sets for the given platform
    /// @param platformType Platform to get the descriptor sets of
    /// @returns The descriptor sets
    [[nodiscard]] const std::vector<vk::DescriptorSet>& getDescriptorSets(AxrPlatformType platformType) const;
    /// Reset the descriptor sets for the given platform
    /// @param platformType Platform to reset the descriptor sets of
    void resetDescriptorSets(AxrPlatformType platformType);
    /// Get a handle to the material this class is built from
    /// @returns A handle to the material this class is built from 
    [[nodiscard]] const AxrMaterial* getMaterial() const;

    /// Check if the data exists
    /// @returns True if the data exists
    [[nodiscard]] bool doesDataExist() const;
    /// Check if the window specific data exists
    /// @returns True if the window specific data exists
    [[nodiscard]] bool doesWindowDataExist() const;

    /// Create the material data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createData();
    /// Destroy the material data
    void destroyData();

    /// Create the window specific material data
    /// @param renderPass Render pass to use
    /// @param msaaSampleCount Msaa sample count
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWindowData(
        vk::RenderPass renderPass,
        vk::SampleCountFlagBits msaaSampleCount
    );
    /// Destroy the window specific material data
    void destroyWindowData();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const AxrShader* m_VertexShaderHandle;
    const AxrShader* m_FragmentShaderHandle;
    const AxrMaterial* m_MaterialHandle;
    const AxrVulkanMaterialLayoutData* m_MaterialLayoutData;
    uint32_t m_MaxFramesInFlight;
    vk::Device m_Device;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    /// These should never be used for anything other than returning a reference to descriptor sets when there's no other option.
    std::vector<vk::DescriptorSet> m_DummyDescriptorSets;
    /// These should never be used for anything other than returning a reference to the name if no name can be found.
    std::string m_DummyName;

    // ---- Window Data ----
    vk::DescriptorPool m_WindowDescriptorPool;
    std::vector<vk::DescriptorSet> m_WindowDescriptorSets;
    vk::Pipeline m_WindowPipeline;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Descriptor Pool ----

    /// Create a descriptor pool
    /// @param viewCount How many views will be used in rendering
    /// @param descriptorPool The output created descriptor pool
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createDescriptorPool(
        uint32_t viewCount,
        vk::DescriptorPool& descriptorPool
    ) const;
    /// Destroy the given descriptor pool
    /// @param descriptorPool The descriptor pool to destroy
    void destroyDescriptorPool(vk::DescriptorPool& descriptorPool) const;

    // ---- Descriptor Sets ----

    /// Create the descriptor sets
    /// @param descriptorPool The descriptor pool to use
    /// @param descriptorSets The output descriptor sets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createDescriptorSets(
        vk::DescriptorPool descriptorPool,
        std::vector<vk::DescriptorSet>& descriptorSets
    ) const;
    /// Reset the given descriptor pool and descriptor sets
    /// @param descriptorPool The descriptor pool to reset
    /// @param descriptorSets The descriptor sets to reset
    void resetDescriptorSets(
        vk::DescriptorPool descriptorPool,
        std::vector<vk::DescriptorSet>& descriptorSets
    ) const;

    // ---- Pipeline ----

    /// Create a pipeline
    /// @param renderPass Render pass to use
    /// @param msaaSampleCount Msaa sample count
    /// @param pipeline Output created pipeline
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createPipeline(
        vk::RenderPass renderPass,
        vk::SampleCountFlagBits msaaSampleCount,
        vk::Pipeline& pipeline
    ) const;
    /// Destroy the given pipeline
    /// @param pipeline Pipeline to destroy
    void destroyPipeline(vk::Pipeline& pipeline);

    /// Clean up the data that's used in a pipeline's creation
    /// @param shaderModules Shader modules
    void cleanupPipelineCreationData(std::vector<vk::ShaderModule>& shaderModules) const;

    /// Create a shader module
    /// @param shaderFileData Shader file data
    /// @param shaderModule Output created shader module
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createShaderModule(
        const std::vector<char>& shaderFileData,
        vk::ShaderModule& shaderModule
    ) const;
    /// Destroy the given shader module
    /// @param shaderModule Shader module to destroy
    void destroyShaderModule(vk::ShaderModule& shaderModule) const;

    /// Get the format for the given vertex attribute enum
    /// @param vertexAttribute Vertex attribute
    /// @returns The format
    [[nodiscard]] vk::Format getVertexAttributeFormat(AxrShaderVertexAttributeEnum vertexAttribute) const;
    /// Get the offset for the given vertex attribute enum
    /// @param vertexAttribute Vertex attribute
    /// @returns The offset
    [[nodiscard]] uint32_t getVertexAttributeOffset(AxrShaderVertexAttributeEnum vertexAttribute) const;
};

#endif
