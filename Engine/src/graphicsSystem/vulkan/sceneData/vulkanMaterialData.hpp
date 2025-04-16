#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "../../../assets/shader.hpp"

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
        std::string Name;
        const AxrShader* VertexShaderHandle;
        const AxrShader* FragmentShaderHandle;
        vk::PipelineLayout PipelineLayout;
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
    /// Get the pipeline layout
    /// @returns The pipeline layout
    [[nodiscard]] const vk::PipelineLayout& getPipelineLayout() const;
    /// Get the window pipeline
    /// @returns The window pipeline
    [[nodiscard]] const vk::Pipeline& getWindowPipeline() const;

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
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWindowData(
        vk::RenderPass renderPass
    );
    /// Destroy the window specific material data
    void destroyWindowData();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    const AxrShader* m_VertexShaderHandle;
    const AxrShader* m_FragmentShaderHandle;
    vk::PipelineLayout m_PipelineLayout;
    vk::Device m_Device;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Window Data ----
    vk::Pipeline m_WindowPipeline;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    // ---- Window Data ----

    /// Create a pipeline
    /// @param renderPass Render pass to use
    /// @param pipeline Output created pipeline
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createPipeline(
        vk::RenderPass renderPass,
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
