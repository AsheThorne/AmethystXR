#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
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

    /// Material layout data config
    struct Config {
        std::string Name;
        const AxrShader* VertexShaderHandle;
        const AxrShader* FragmentShaderHandle;
        vk::Device Device;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    /// Descriptor set item location details
    struct DescriptorSetItemLocation {
        AxrShaderBufferLayoutEnum BufferLayoutType;
        uint32_t ShaderBinding;
        uint32_t ItemIndex;
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
    /// Get the pipeline layout
    /// @returns The pipeline layout
    [[nodiscard]] const vk::PipelineLayout& getPipelineLayout() const;
    /// Get the push constant shader stages
    /// @returns The push constant shader stages
    [[nodiscard]] const vk::ShaderStageFlags& getPushConstantShaderStages() const;
    /// Get the descriptor set layout
    /// @returns The descriptor set layout 
    [[nodiscard]] const vk::DescriptorSetLayout& getDescriptorSetLayout() const;
    /// Get the descriptor set item locations
    /// @returns The descriptor set item locations
    [[nodiscard]] const std::vector<DescriptorSetItemLocation>& getDescriptorSetItemLocations() const;

    /// Check if the data exists
    /// @returns True if the data exists
    [[nodiscard]] bool doesDataExist() const;

    /// Create the material layout data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createData();
    /// Destroy the material layout data
    void destroyData();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_Name;
    const AxrShader* m_VertexShaderHandle;
    const AxrShader* m_FragmentShaderHandle;
    vk::Device m_Device;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    // ---- Data ----
    std::vector<DescriptorSetItemLocation> m_DescriptorSetItemLocations;
    vk::DescriptorSetLayout m_DescriptorSetLayout;
    vk::PipelineLayout m_PipelineLayout;
    vk::ShaderStageFlags m_PushConstantShaderStage;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Data ----

    /// Validate the material layout shaders
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult validateMaterialLayoutShaders();

    /// Create the descriptor set layout
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createDescriptorSetLayout();
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
    ) const;

    /// Create the pipeline layout
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createPipelineLayout();
    /// Destroy the pipeline layout
    void destroyPipelineLayout();
};

#endif
