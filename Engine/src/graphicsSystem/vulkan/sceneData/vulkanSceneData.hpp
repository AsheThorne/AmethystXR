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
#include "vulkanImageData.hpp"
#include "vulkanImageSamplerData.hpp"
#include "axr/scene.h"

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
        vk::CommandPool GraphicsCommandPool;
        vk::Queue GraphicsQueue;
        vk::CommandPool TransferCommandPool;
        vk::Queue TransferQueue;
        uint32_t MaxFramesInFlight;
        float MaxSamplerAnisotropy;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    /// Push constant references for rendering
    struct PushConstantForRendering {
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
        PushConstantForRendering PushConstant;
    };

    /// Material references for rendering
    struct MaterialForRendering {
        const vk::PipelineLayout& PipelineLayout;
        const vk::Pipeline& WindowPipeline;
        /// One for each frame in flight
        const std::vector<vk::DescriptorSet>& WindowDescriptorSets;
        PushConstantForRendering PushConstant;
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
    [[nodiscard]] const std::string& getSceneName() const;

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

    // ---- Find Assets ----

    /// Find the named push constant buffer, including the global data in the search
    /// @param name The name of the push constant buffer
    /// @returns A handle to the found push constant buffer. Or nullptr if it wasn't found
    [[nodiscard]] const AxrPushConstantBuffer* findPushConstantBuffer_shared(const std::string& name) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    std::string m_SceneName;
    AxrAssetCollection_T m_AssetCollection;
    entt::registry* m_EcsRegistryHandle;
    AxrVulkanSceneData* m_GlobalSceneData;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_GraphicsCommandPool;
    vk::Queue m_GraphicsQueue;
    vk::CommandPool m_TransferCommandPool;
    vk::Queue m_TransferQueue;
    uint32_t m_MaxFramesInFlight;
    float m_MaxSamplerAnisotropy;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    /// Missing texture image asset
    AxrImage m_MissingTextureImage;

    // ---- Window data ----
    bool m_IsWindowDataLoaded;
    vk::RenderPass m_WindowRenderPass;
    /// Window specific engine defined uniform buffers
    std::unordered_map<std::string, AxrVulkanUniformBufferData> m_WindowUniformBufferData;

    std::unordered_map<std::string, AxrVulkanUniformBufferData> m_UniformBufferData;
    std::unordered_map<std::string, AxrVulkanModelData> m_ModelData;
    std::unordered_map<std::string, AxrVulkanImageData> m_ImageData;
    std::unordered_map<std::string, AxrVulkanImageSamplerData> m_ImageSamplerData;
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

    // ---- Push Constant Buffer ----

    /// Validate all push constant buffers
    /// @returns AXR_SUCCESS if all push constant buffers are valid
    [[nodiscard]] AxrResult validateAllPushConstantBuffers() const;

    /// Validate the given push constant buffer
    /// @param properties Physical device properties to use
    /// @param pushConstantBuffer Push constant buffer to validate
    /// @returns AXR_SUCCESS if the push constant buffer is valid
    [[nodiscard]] AxrResult validatePushConstantBuffer(
        const vk::PhysicalDeviceProperties& properties,
        const AxrPushConstantBuffer& pushConstantBuffer
    ) const;

    /// 'On push constant buffer created' callback for the asset collection
    /// @param pushConstantBuffer Newly created push constant buffer 
    void onPushConstantBufferCreatedCallback(AxrPushConstantBufferConst_T pushConstantBuffer);

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
    /// Initialize a single uniform buffer's data. Define either a uniformBufferHandle or a uniform buffer engineAsset
    /// @param uniformBufferHandle Uniform buffer handle to use
    /// @param engineAsset Uniform buffer engine asset to use
    /// @param uniformBufferDataCollection Uniform buffer data collection to modify
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeUniformBufferData(
        const AxrUniformBuffer* uniformBufferHandle,
        AxrEngineAssetEnum engineAsset,
        std::unordered_map<std::string, AxrVulkanUniformBufferData>& uniformBufferDataCollection
    ) const;

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

    /// 'On uniform buffer created' callback for the asset collection
    /// @param uniformBuffer Newly created uniform buffer 
    void onUniformBufferCreatedCallback(AxrUniformBufferConst_T uniformBuffer);

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

    /// Find the named model data, including the global data in the search
    /// @param name The name of the model
    /// @returns A handle to the found model. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanModelData* findModelData_shared(const std::string& name) const;

    /// 'On model created' callback for the asset collection
    /// @param model Newly created model 
    void onModelCreatedCallback(AxrModelConst_T model);

    // ---- Image ----

    /// Create all image data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllImageData();
    /// Destroy all image data
    void destroyAllImageData();

    /// Initialize all the image data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeAllImageData();
    /// Initialize a single image's data for the given image
    /// @param image Image to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeImageData(const AxrImage& image);

    /// Initialize the 'Missing Texture' image data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeMissingTextureImageData();

    /// Find the named image data, including the global data in the search
    /// @param name The name of the image
    /// @returns A handle to the found image. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanImageData* findImageData_shared(const std::string& name) const;

    /// 'On image created' callback for the asset collection
    /// @param image Newly created image 
    void onImageCreatedCallback(AxrImageConst_T image);

    // ---- Image Sampler ----

    /// Create all image sampler data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllImageSamplerData();
    /// Destroy all image sampler data
    void destroyAllImageSamplerData();

    /// Initialize all the image sampler data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeAllImageSamplerData();
    /// Initialize a single image sampler's data for the given image sampler
    /// @param imageSampler Image sampler to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeImageSamplerData(const AxrImageSampler& imageSampler);

    /// Find the named image sampler data, including the global data in the search
    /// @param name The name of the image sampler
    /// @returns A handle to the found image sampler. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanImageSamplerData* findImageSamplerData_shared(const std::string& name) const;

    /// 'On image sampler created' callback for the asset collection
    /// @param imageSampler Newly created image sampler
    void onImageSamplerCreatedCallback(AxrImageSamplerConst_T imageSampler);

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

    /// Create all window specific material data
    /// @param renderPass Render pass to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllWindowMaterialData(vk::RenderPass renderPass);
    /// Destroy all window specific material data
    void destroyAllWindowMaterialData();

    /// Find the named material data, including the global data in the search
    /// @param name The name of the material
    /// @returns A handle to the found material. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanMaterialData* findMaterialData_shared(const std::string& name) const;

    /// 'On material created' callback for the asset collection
    /// @param material Newly created material
    void onMaterialCreatedCallback(AxrMaterialConst_T material);

    // ---- Write Descriptor Sets ----

    /// Write all material descriptor sets
    /// @param platformType The platform type to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult writeAllDescriptorSets(AxrPlatformType platformType);
    /// Reset all material descriptor sets
    /// @param platformType The platform type to use
    void resetAllDescriptorSets(AxrPlatformType platformType);

    /// Write the descriptor sets for the given material data
    /// @param platformType The platform type to use
    /// @param materialData The material data to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult writeDescriptorSets(
        AxrPlatformType platformType,
        AxrVulkanMaterialData& materialData
    ) const;
    /// Reset the descriptor sets for the given material data
    /// @param platformType The platform type to use
    /// @param materialData The material data
    void resetDescriptorSets(AxrPlatformType platformType, AxrVulkanMaterialData& materialData) const;

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

    /// 'On new renderable entity' callback for the entt registry
    /// @param registry The entt registry 
    /// @param entity The new renderable entity 
    void onNewRenderableEntityCallback(entt::registry& registry, entt::entity entity);
};

#endif
