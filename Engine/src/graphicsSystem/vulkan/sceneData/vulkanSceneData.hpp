﻿#pragma once
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
#include "../../../xrSystem/xrUtils.hpp"

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
        std::string SceneName;
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

    /// Load window data config
    struct LoadWindowDataConfig {
        vk::RenderPass RenderPass = VK_NULL_HANDLE;
        vk::SampleCountFlagBits MsaaSampleCount = vk::SampleCountFlagBits::e1;
    };

    /// Load xr session data config
    struct LoadXrSessionDataConfig {
        vk::RenderPass RenderPass = VK_NULL_HANDLE;
        vk::SampleCountFlagBits MsaaSampleCount = vk::SampleCountFlagBits::e1;
        uint32_t ViewCount = 0;
    };

    /// Push constant references for rendering
    struct PushConstantForRendering {
        const vk::ShaderStageFlags* ShaderStages = nullptr;
        const char* BufferName = "";
    };

    /// Mesh references for rendering
    struct MeshForRendering {
        const vk::Buffer& Buffer;
        const vk::DeviceSize& BufferIndicesOffset;
        const vk::DeviceSize& BufferVerticesOffset;
        const uint32_t& IndexCount;
        const AxrTransformComponent* TransformComponent = nullptr;
        PushConstantForRendering PushConstant;
    };

    /// Material references for rendering
    struct MaterialForRendering {
        std::string MaterialName;
        const vk::PipelineLayout& PipelineLayout;
        const vk::Pipeline& WindowPipeline;
        const vk::Pipeline& XrSessionPipeline;
        /// One for each frame in flight
        const std::vector<vk::DescriptorSet>& WindowDescriptorSets;
        const std::vector<vk::DescriptorSet>& XrSessionDescriptorSets;
        PushConstantForRendering PushConstant;
        std::vector<MeshForRendering> Meshes;
        std::vector<uint32_t> DynamicOffsets;
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
    /// Get the ecs registry handle
    /// @returns The ecs registry handle
    [[nodiscard]] entt::registry* getEcsRegistryHandle() const;

    /// Load the scene data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadScene();
    /// Unload the scene data
    void unloadScene();

    /// Load the window specific scene data
    /// @param config Load window data config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadWindowData(const LoadWindowDataConfig& config);
    /// Unload the window specific scene data
    void unloadWindowData();

    /// Load the xr session specific scene data
    /// @param config Load xr session data config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadXrSessionData(const LoadXrSessionDataConfig& config);
    /// Unload the xr session specific scene data
    void unloadXrSessionData();

    /// Get the materials, organized specifically for rendering
    /// @param alphaRenderMode Alpha render mode to get the materials for
    /// @returns The collection of materials for rendering
    [[nodiscard]] const std::vector<MaterialForRendering>& getMaterialsForRendering(
        AxrMaterialAlphaRenderModeEnum alphaRenderMode
    ) const;

    /// Set platform specific uniform buffer data
    /// @param platformType Platform type
    /// @param bufferName Buffer name
    /// @param frameIndex Frame index to use
    /// @param viewIndex View index
    /// @param offset Data offset
    /// @param dataSize Data size
    /// @param data Data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setPlatformUniformBufferData(
        AxrPlatformType platformType,
        const std::string& bufferName,
        uint32_t frameIndex,
        uint32_t viewIndex,
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

    // ---- Window data ----
    bool m_IsWindowDataLoaded;
    LoadWindowDataConfig m_LoadWindowDataConfig;
    /// Window specific engine defined uniform buffers
    std::unordered_map<std::string, AxrVulkanUniformBufferData> m_WindowUniformBufferData;

    // ---- Xr session data ----
    bool m_IsXrSessionDataLoaded;
    LoadXrSessionDataConfig m_LoadXrSessionDataConfig;
    /// Xr session specific engine defined uniform buffers.
    /// There's one uniform buffer per view.
    std::unordered_map<std::string, std::array<AxrVulkanUniformBufferData, AXR_MAX_XR_VIEWS>>
    m_XrSessionUniformBufferData;

    std::unordered_map<std::string, AxrVulkanUniformBufferData> m_UniformBufferData;
    std::unordered_map<std::string, AxrVulkanModelData> m_ModelData;
    std::unordered_map<std::string, AxrVulkanImageData> m_ImageData;
    std::unordered_map<std::string, AxrVulkanImageSamplerData> m_ImageSamplerData;
    std::unordered_map<std::string, AxrVulkanMaterialLayoutData> m_MaterialLayoutData;
    std::unordered_map<std::string, AxrVulkanMaterialData> m_MaterialData;
    std::vector<MaterialForRendering> m_OpaqueMaterialsForRendering;
    std::vector<MaterialForRendering> m_AlphaBlendMaterialsForRendering;
    std::vector<MaterialForRendering> m_OITMaterialsForRendering;

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
    void onPushConstantBufferCreatedCallback(AxrPushConstantBufferConst_T pushConstantBuffer) const;

    // ---- Uniform Buffer ----

    /// Destroy all uniform buffer data
    /// @param uniformBufferData Uniform buffer data to destroy
    void destroyUniformBufferData(std::unordered_map<std::string, AxrVulkanUniformBufferData>& uniformBufferData);
    /// Destroy all uniform buffer data
    /// @param uniformBufferData Uniform buffer data to destroy
    void destroyUniformBufferData(
        std::unordered_map<std::string, std::array<AxrVulkanUniformBufferData, AXR_MAX_XR_VIEWS>>& uniformBufferData
    );

    /// Create all uniform buffer data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllUniformBufferData();
    /// Destroy all uniform buffer data
    void destroyAllUniformBufferData();

    /// Initialize a single uniform buffer's data. Define either a uniformBufferHandle or a uniform buffer engineAsset
    /// @param uniformBufferHandle Uniform buffer handle to use
    /// @param engineAsset Uniform buffer engine asset to use
    /// @param uniformBufferData Output Uniform buffer data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeUniformBufferData(
        const AxrUniformBuffer* uniformBufferHandle,
        AxrEngineAssetEnum engineAsset,
        AxrVulkanUniformBufferData& uniformBufferData
    ) const;

    /// Find the named uniform buffer data, including the global data in the search
    /// @param name The name of the uniform buffer
    /// @param platformType The platform type to check for platform specific uniform buffers
    /// @param viewIndex The view index
    /// @returns A handle to the found uniform buffer. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanUniformBufferData* findUniformBufferData_shared(
        const std::string& name,
        AxrPlatformType platformType,
        uint32_t viewIndex
    ) const;

    /// Create all window uniform buffer data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllWindowUniformBufferData();
    /// Destroy all window uniform buffer data
    void destroyAllWindowUniformBufferData();

    /// Find the named window uniform buffer data, including the global data in the search
    /// @param name The name of the window uniform buffer
    /// @returns A handle to the found window uniform buffer. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanUniformBufferData* findWindowUniformBufferData_shared(const std::string& name) const;

    /// Create all xr session uniform buffer data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllXrSessionUniformBufferData();
    /// Destroy all xr session uniform buffer data
    void destroyAllXrSessionUniformBufferData();

    /// Find the named xr session uniform buffer data, including the global data in the search
    /// @param name The name of the xr session uniform buffer
    /// @param viewIndex The xr view index
    /// @returns A handle to the found xr session uniform buffer. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanUniformBufferData* findXrSessionUniformBufferData_shared(
        const std::string& name,
        uint32_t viewIndex
    ) const;

    /// 'On uniform buffer created' callback for the asset collection
    /// @param uniformBuffer Newly created uniform buffer 
    void onUniformBufferCreatedCallback(AxrUniformBufferConst_T uniformBuffer);

    // ---- Model ----

    /// Create all model data
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllModelData();
    /// Destroy all model data
    void destroyAllModelData();

    /// Initialize a single model's data for the given model
    /// @param model Model to use
    /// @param modelData Output model data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeModelData(const AxrModel& model, AxrVulkanModelData& modelData) const;

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

    /// Initialize a single image's data for the given image
    /// @param image Image to use
    /// @param imageData Output image data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeImageData(const AxrImage& image, AxrVulkanImageData& imageData) const;

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

    /// Initialize a single image sampler's data for the given image sampler
    /// @param imageSampler Image sampler to use
    /// @param imageSamplerData Output image sampler data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeImageSamplerData(
        const AxrImageSampler& imageSampler,
        AxrVulkanImageSamplerData& imageSamplerData
    ) const;

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

    /// Initialize a single material layout for the given material
    /// @param material Material to use
    /// @param materialLayoutData Output material layout data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeMaterialLayoutData(
        const AxrMaterial& material,
        AxrVulkanMaterialLayoutData& materialLayoutData
    ) const;

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

    /// Initialize a single material data for the given material
    /// @param material Material to use
    /// @param materialData Output material data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeMaterialData(const AxrMaterial& material, AxrVulkanMaterialData& materialData) const;

    /// Create all window specific material data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllWindowMaterialData();
    /// Destroy all window specific material data
    void destroyAllWindowMaterialData();

    /// Create all xr session specific material data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllXrSessionMaterialData();
    /// Destroy all xr session specific material data
    void destroyAllXrSessionMaterialData();

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
    /// @param viewCount The number of views for the xr device
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult writeAllDescriptorSets(AxrPlatformType platformType, uint32_t viewCount);
    /// Reset all material descriptor sets
    /// @param platformType The platform type to use
    void resetAllDescriptorSets(AxrPlatformType platformType);

    /// Write the descriptor sets for the given material data
    /// @param platformType The platform type to use
    /// @param viewCount The number of views for the xr device
    /// @param materialData The material data to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult writeDescriptorSets(
        AxrPlatformType platformType,
        uint32_t viewCount,
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

    /// Add a material for rendering to either m_OpaqueMaterialsForRendering, m_AlphaBlendMaterialsForRendering
    /// or m_OITMaterialsForRendering depending on it's alpha rendering mode. 
    /// @param transformComponent Transform component
    /// @param modelComponent Model component
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult addMaterialForRendering(
        const AxrTransformComponent& transformComponent,
        const AxrModelComponent& modelComponent
    );

    /// 'On new renderable entity' callback for the entt registry
    /// @param registry The entt registry 
    /// @param entity The new renderable entity 
    void onNewRenderableEntityCallback(entt::registry& registry, entt::entity entity);

    /// Find the named material in the given collection of 'materials for rendering'
    /// @param materials 'Materials for rendering' collection
    /// @param materialName Material name
    /// @returns A handle to the found material or nullptr if it wasn't found.
    [[nodiscard]] MaterialForRendering* findMaterialForRendering(
        std::vector<MaterialForRendering>& materials,
        const std::string& materialName
    ) const;
};

#endif
