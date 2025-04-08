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

/// Vulkan material layout data
class AxrVulkanMaterialLayoutData {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Descriptor set item location details
    struct DescriptorSetItemLocation {
        AxrShaderBufferLayoutEnum BufferLayoutType;
        uint32_t ShaderBinding;
        uint32_t ItemIndex;
    };

    /// Material layout data config
    struct Config {
        std::string Name;
        std::string VertexShaderName;
        std::string FragmentShaderName;
        vk::Device Device;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanMaterialLayoutData();
    /// Constructor
    /// @param config Material layout data config
    AxrVulkanMaterialLayoutData(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanMaterialLayoutData to copy from
    AxrVulkanMaterialLayoutData(const AxrVulkanMaterialLayoutData& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanMaterialLayoutData to move from
    AxrVulkanMaterialLayoutData(AxrVulkanMaterialLayoutData&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanMaterialLayoutData();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanMaterialLayoutData to copy from
    AxrVulkanMaterialLayoutData& operator=(const AxrVulkanMaterialLayoutData& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanMaterialLayoutData to move from
    AxrVulkanMaterialLayoutData& operator=(AxrVulkanMaterialLayoutData&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the material layout name
    /// @returns The material layout name
    [[nodiscard]] const std::string& getName() const;
    /// Get the vertex shader name
    /// @returns The vertex shader name
    [[nodiscard]] const std::string& getVertexShaderName() const;
    /// Get the fragment shader name
    /// @returns The fragment shader name
    [[nodiscard]] const std::string& getFragmentShaderName() const;

    /// Check if the data exists
    /// @returns True if the data exists
    [[nodiscard]] bool doesDataExist() const;
    /// Check if the window specific data exists
    /// @returns True if the window specific data exists
    [[nodiscard]] bool doesWindowDataExist() const;

    /// Create the material layout data
    /// @param vertexShader Vertex shader to use
    /// @param fragmentShader Fragment shader to use
    [[nodiscard]] AxrResult createData(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader
    );
    /// Destroy the material layout data
    void destroyData();

    /// Create the window specific material layout data
    /// @param vertexShader Vertex shader to use
    /// @param fragmentShader Fragment shader to use
    /// @param renderPass Render pass to use
    [[nodiscard]] AxrResult createWindowData(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader,
        vk::RenderPass renderPass
    );
    /// Destroy the window specific material layout data
    void destroyWindowData();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    std::string m_VertexShaderName;
    std::string m_FragmentShaderName;
    vk::Device m_Device;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    std::vector<DescriptorSetItemLocation> m_DescriptorSetItemLocations;
    vk::DescriptorSetLayout m_DescriptorSetLayout;
    vk::PipelineLayout m_PipelineLayout;

    // ---- Window Data ----
    vk::Pipeline m_WindowPipeline;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    /// Validate the material layout shaders
    /// @param vertexShader Vertex shader to check
    /// @param fragmentShader Fragment shader to check
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult validateMaterialLayoutShaders(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader
    );

    /// Create the descriptor set layout
    /// @param vertexShader Vertex shader to use
    /// @param fragmentShader Fragment shader to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createDescriptorSetLayout(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader
    );
    /// Destroy the descriptor set layout
    void destroyDescriptorSetLayout();

    /// Add a new descriptor set layout item
    /// @param binding Item binding
    /// @param descriptorType Item descriptor type
    /// @param stageFlag Item stage
    /// @param bindings Output descriptor set layout item bindings collection to append
    /// @param descriptorSetItemLocations Output descriptor set item locations collection to append
    void addDescriptorSetLayoutItem(
        uint32_t binding,
        vk::DescriptorType descriptorType,
        vk::ShaderStageFlagBits stageFlag,
        std::vector<vk::DescriptorSetLayoutBinding>& bindings,
        std::vector<DescriptorSetItemLocation>& descriptorSetItemLocations
    );

    /// Create the pipeline layout
    /// @param vertexShader Vertex shader to use
    /// @param fragmentShader Fragment shader to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createPipelineLayout(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader
    );
    /// Destroy the pipeline layout
    void destroyPipelineLayout();

    // ---- Window Data ----

    /// Create a pipeline
    /// @param vertexShader Vertex shader to use
    /// @param fragmentShader Fragment shader to use
    /// @param renderPass Render pass to use
    /// @param pipeline Output created pipeline
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createPipeline(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader,
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
};

#endif
