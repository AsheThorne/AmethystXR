#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "vulkanUniformBufferData.hpp"
#include "vulkanModelData.hpp"
#include "vulkanMaterialLayoutData.hpp"
#include "vulkanMaterialData.hpp"
#include "axr/scene.h"
#include "../../../assets/bufferEngineAssets.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <unordered_map>

/// Vulkan scene data
class AxrVulkanSceneData {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan scene data config
    struct Config {
        const char* SceneName;
        AxrAssetCollection_T AssetCollection;
        entt::registry* EcsRegistryHandle;
        AxrVulkanSceneData* GlobalSceneData;
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool TransferCommandPool;
        vk::Queue TransferQueue;
        uint32_t MaxFramesInFlight;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    /// Push constants references for rendering
    struct PushConstantsForRendering {
        const vk::ShaderStageFlags* ShaderStages = nullptr;
        const char* BufferName = "";
        const AxrTransformComponent* TransformComponent = nullptr;
    };

    /// Mesh references for rendering
    struct MeshForRendering {
        const vk::Buffer& Buffer;
        const vk::DeviceSize& BufferIndicesOffset;
        const vk::DeviceSize& BufferVerticesOffset;
        const uint32_t& IndexCount;
        PushConstantsForRendering PushConstants;
    };

    /// Material references for rendering
    struct MaterialForRendering {
        const vk::PipelineLayout& PipelineLayout;
        const vk::Pipeline& WindowPipeline;
        /// One for each frame in flight
        const std::vector<vk::DescriptorSet>& WindowDescriptorSets;
        PushConstantsForRendering PushConstants;
        std::vector<MeshForRendering> Meshes;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Vulkan scene data config
    AxrVulkanSceneData(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanSceneData to copy from
    AxrVulkanSceneData(const AxrVulkanSceneData& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanSceneData to move from
    AxrVulkanSceneData(AxrVulkanSceneData&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanSceneData();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanSceneData to copy from
    AxrVulkanSceneData& operator=(const AxrVulkanSceneData& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanSceneData to move from
    AxrVulkanSceneData& operator=(AxrVulkanSceneData&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the scene name
    /// @returns The scene name
    [[nodiscard]] const char* getSceneName() const;

    /// Load the scene data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadScene();
    /// Unload the scene data
    void unloadScene();

    /// Load the window specific scene data
    /// @param renderPass Render pass to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadWindowData(vk::RenderPass renderPass);
    /// Unload the window specific scene data
    void unloadWindowData();

    /// Get the materials, organized specifically for rendering
    /// @returns The collection of materials for rendering
    [[nodiscard]] const std::unordered_map<std::string, MaterialForRendering>& getMaterialsForRendering() const;

    /// Set window specific named uniform buffer data
    /// @param bufferName Buffer name
    /// @param frameIndex Frame index to use
    /// @param offset Data offset
    /// @param dataSize Data size
    /// @param data Data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setWindowUniformBufferData(
        const std::string& bufferName,
        uint32_t frameIndex,
        vk::DeviceSize offset,
        vk::DeviceSize dataSize,
        const void* data
    ) const;

    /// Set the active scene
    /// @param activeSceneHandle Handle to the new active scene
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult onSetActiveScene(const AxrVulkanSceneData* activeSceneHandle);

    // ---- Find Assets ----

    /// Find the named push constants buffer, including the global data in the search
    /// @param name The name of the push constants buffer
    /// @returns A handle to the found push constants buffer. Or nullptr if it wasn't found
    [[nodiscard]] const AxrPushConstantsBuffer* findPushConstantsBuffer_shared(const std::string& name) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_SceneName;
    AxrAssetCollection_T m_AssetCollection;
    entt::registry* m_EcsRegistryHandle;
    AxrVulkanSceneData* m_GlobalSceneData;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_TransferCommandPool;
    vk::Queue m_TransferQueue;
    uint32_t m_MaxFramesInFlight;
    vk::DispatchLoaderDynamic* m_DispatchHandle;
    bool m_IsWindowDataLoaded;

    /// Window specific engine defined uniform buffers
    std::unordered_map<std::string, AxrVulkanUniformBufferData> m_WindowUniformBufferData;

    std::unordered_map<std::string, AxrVulkanUniformBufferData> m_UniformBufferData;
    std::unordered_map<std::string, AxrVulkanModelData> m_ModelData;
    std::unordered_map<std::string, AxrVulkanMaterialLayoutData> m_MaterialLayoutData;
    std::unordered_map<std::string, AxrVulkanMaterialData> m_MaterialData;
    std::unordered_map<std::string, MaterialForRendering> m_MaterialsForRendering;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Check if this class is the global scene data
    /// @returns True if this class is the global scene data
    [[nodiscard]] bool isThisGlobalSceneData() const;

    /// Check if the given platform's data is loaded
    /// @param platformType Platform type
    /// @returns True if the given platform's data is loaded
    [[nodiscard]] bool isPlatformLoaded(AxrPlatformType platformType) const;

    // ---- Uniform Buffer ----

    /// Destroy all uniform buffer data
    /// @param uniformBufferData Uniform buffer data to destroy
    void destroyUniformBufferData(std::unordered_map<std::string, AxrVulkanUniformBufferData>& uniformBufferData);

    /// Create all uniform buffer data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllUniformBufferData();
    /// Destroy all uniform buffer data
    void destroyAllUniformBufferData();

    /// Initialize all the uniform buffer data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeAllUniformBufferData();
    /// Initialize a single uniform buffer's data. Define either a uniformBufferHandle or a uniformBufferEngineAsset
    /// @param uniformBufferHandle Uniform buffer handle to use
    /// @param uniformBufferEngineAsset Uniform buffer engine asset to use
    /// @param uniformBufferDataCollection Uniform buffer data collection to modify
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeUniformBufferData(
        const AxrUniformBuffer* uniformBufferHandle,
        AxrUniformBufferEngineAssetEnum uniformBufferEngineAsset,
        std::unordered_map<std::string, AxrVulkanUniformBufferData>& uniformBufferDataCollection
    ) const;

    /// Create the given uniform buffer data
    /// @param uniformBufferData Uniform buffer data to create
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createUniformBufferData(AxrVulkanUniformBufferData& uniformBufferData);
    /// Destroy the given uniform buffer data
    /// @param uniformBufferData Uniform buffer data to destroy
    void destroyUniformBufferData(AxrVulkanUniformBufferData& uniformBufferData);

    /// Find the named uniform buffer data, including the global data in the search
    /// @param name The name of the uniform buffer
    /// @param platformType The platform type to check for platform specific uniform buffers
    /// @returns A handle to the found uniform buffer. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanUniformBufferData* findUniformBufferData_shared(
        const std::string& name,
        AxrPlatformType platformType
    ) const;

    /// Create all window uniform buffer data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllWindowUniformBufferData();
    /// Destroy all window uniform buffer data
    void destroyAllWindowUniformBufferData();

    /// Initialize all the window uniform buffer data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeAllWindowUniformBufferData();

    /// Find the named window uniform buffer data, including the global data in the search
    /// @param name The name of the window uniform buffer
    /// @returns A handle to the found window uniform buffer. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanUniformBufferData* findWindowUniformBufferData_shared(const std::string& name) const;

    // ---- Model ----

    /// Create all model data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllModelData();
    /// Destroy all model data
    void destroyAllModelData();

    /// Initialize all the model data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeAllModelData();
    /// Initialize a single model's data for the given model
    /// @param model Model to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeModelData(const AxrModel& model);

    /// Create the given model data
    /// @param modelData Model data to create
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createModelData(AxrVulkanModelData& modelData);
    /// Destroy the given model data
    /// @param modelData Model data to destroy
    void destroyModelData(AxrVulkanModelData& modelData);

    /// Find the named model data, including the global data in the search
    /// @param name The name of the model
    /// @returns A handle to the found model. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanModelData* findModelData_shared(const std::string& name) const;

    // ---- Shader ----

    /// Find the named shader, including the global data in the search
    /// @param name The name of the shader
    /// @returns A handle to the found shader. Or nullptr if it wasn't found
    [[nodiscard]] const AxrShader* findShader_shared(const std::string& name) const;

    // ---- Material Layout ----

    /// Create all material layouts data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllMaterialLayoutData();
    /// Destroy all material layouts data
    void destroyAllMaterialLayoutData();

    /// Initialize all the material layouts
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeAllMaterialLayoutData();
    /// Initialize a single material layout for the given material
    /// @param material Material to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeMaterialLayoutData(const AxrMaterial& material);

    /// Create the given material layout data
    /// @param materialLayoutData Material layout data to create
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData);
    /// Destroy the given material layout data
    /// @param materialLayoutData Material layout data to destroy
    void destroyMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData);

    /// Find the named material layout data
    /// @param name The name of the material layout
    /// @returns A handle to the found material layout. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanMaterialLayoutData* findMaterialLayoutData(const std::string& name) const;

    // ---- Material ----

    /// Create all material data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllMaterialData();
    /// Destroy all material data
    void destroyAllMaterialData();

    /// Initialize all the materials
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeAllMaterialData();
    /// Initialize a single material data for the given material
    /// @param material Material to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeMaterialData(const AxrMaterial& material);

    /// Create the given material data
    /// @param materialData Material data to create
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMaterialData(AxrVulkanMaterialData& materialData);
    /// Destroy the given material data
    /// @param materialData Material data to destroy
    void destroyMaterialData(AxrVulkanMaterialData& materialData);

    /// Create all window specific material data
    /// @param renderPass Render pass to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllWindowMaterialData(vk::RenderPass renderPass);
    /// Destroy all window specific material data
    void destroyAllWindowMaterialData();

    /// Create the given window specific material data
    /// @param renderPass Render pass to use
    /// @param materialData Input/Output material data to use and create window scene data for
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWindowMaterialData(
        vk::RenderPass renderPass,
        AxrVulkanMaterialData& materialData
    );
    /// Destroy the given window specific material data
    /// @param materialData Material data to destroy
    void destroyWindowMaterialData(AxrVulkanMaterialData& materialData);

    /// Find the named material data, including the global data in the search
    /// @param name The name of the material
    /// @returns A handle to the found material. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanMaterialData* findMaterialData_shared(const std::string& name) const;

    // ---- Write Descriptor Sets ----

    /// Write all material descriptor sets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult writeAllWindowDescriptorSets();
    /// Reset all material descriptor sets
    void resetAllWindowDescriptorSets();

    /// Write all scene specific material descriptor sets
    /// @param platformType The platform type to use
    /// @param sceneData Scene data to search for uniform buffers in
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult writeAllSceneSpecificDescriptorSets(
        AxrPlatformType platformType,
        const AxrVulkanSceneData* sceneData
    );

    /// Write the descriptor sets for the given material data
    /// @param platformType The platform type to use
    /// @param bufferScope The buffer scope we're currently writing to
    /// @param sceneData Scene data to search for uniform buffers in
    /// @param materialData The material data to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult writeDescriptorSets(
        AxrPlatformType platformType,
        AxrShaderBufferScopeEnum bufferScope,
        const AxrVulkanSceneData* sceneData,
        AxrVulkanMaterialData& materialData
    ) const;
    /// Reset the descriptor sets for the given material data
    /// @param materialData The material data
    void resetDescriptorSets(AxrVulkanMaterialData& materialData) const;

    // ---- Materials For Rendering ----

    /// Create all materials for rendering
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllMaterialsForRendering();
    /// Destroy all materials for rendering
    void destroyAllMaterialsForRendering();

    /// Add a material for rendering to the given collection
    /// @param transformComponent Transform component
    /// @param modelComponent Model component
    /// @param materialsForRendering Collection to add to
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult addMaterialForRendering(
        const AxrTransformComponent& transformComponent,
        const AxrModelComponent& modelComponent,
        std::unordered_map<std::string, MaterialForRendering>& materialsForRendering
    ) const;
};

#endif
