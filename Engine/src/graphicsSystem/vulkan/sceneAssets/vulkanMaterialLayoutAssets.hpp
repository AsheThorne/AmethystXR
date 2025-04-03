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

/// Vulkan material layout assets
class AxrVulkanMaterialLayoutAssets {
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

    /// Material layout assets config
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
    AxrVulkanMaterialLayoutAssets();
    /// Constructor
    /// @param config Material layout assets config
    AxrVulkanMaterialLayoutAssets(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanMaterialLayoutAssets to copy from
    AxrVulkanMaterialLayoutAssets(const AxrVulkanMaterialLayoutAssets& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanMaterialLayoutAssets to move from
    AxrVulkanMaterialLayoutAssets(AxrVulkanMaterialLayoutAssets&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanMaterialLayoutAssets();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanMaterialLayoutAssets to copy from
    AxrVulkanMaterialLayoutAssets& operator=(const AxrVulkanMaterialLayoutAssets& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanMaterialLayoutAssets to move from
    AxrVulkanMaterialLayoutAssets& operator=(AxrVulkanMaterialLayoutAssets&& src) noexcept;

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

    /// Check if the assets are empty
    /// @returns True if the assets are empty
    [[nodiscard]] bool areAssetsEmpty() const;
    /// Check if the window specific assets are empty
    /// @returns True if the window specific assets are empty
    [[nodiscard]] bool areWindowAssetsEmpty() const;

    /// Create the material layout assets
    /// @param vertexShader Vertex shader to use
    /// @param fragmentShader Fragment shader to use
    [[nodiscard]] AxrResult createAssets(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader
    );
    /// Destroy the material layout assets
    void destroyAssets();

    /// Create the window specific material layout assets
    /// @param vertexShader Vertex shader to use
    /// @param fragmentShader Fragment shader to use
    [[nodiscard]] AxrResult createWindowAssets(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader
    );
    /// Destroy the window specific material layout assets
    void destroyWindowAssets();

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

    // ---- Assets ----
    std::vector<DescriptorSetItemLocation> m_DescriptorSetItemLocations;
    vk::DescriptorSetLayout m_DescriptorSetLayout;
    vk::PipelineLayout m_PipelineLayout;

    // ---- Window Assets ----
    vk::Pipeline m_WindowPipeline;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- Assets ----

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

    // ---- Window assets ----

    /// Create a pipeline
    /// @param vertexShader Vertex shader to use
    /// @param fragmentShader Fragment shader to use
    /// @param pipeline Output created pipeline
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createPipeline(
        const AxrShader& vertexShader,
        const AxrShader& fragmentShader,
        vk::Pipeline& pipeline
    );
    /// Destroy the given pipeline
    /// @param pipeline Pipeline to destroy
    void destroyPipeline(vk::Pipeline& pipeline);
};

#endif
