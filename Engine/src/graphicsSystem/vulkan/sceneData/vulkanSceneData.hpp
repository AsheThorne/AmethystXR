#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "vulkanModelData.hpp"
#include "vulkanMaterialLayoutData.hpp"
#include "vulkanMaterialData.hpp"

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
        AxrVulkanSceneData* SharedVulkanSceneData;
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool TransferCommandPool;
        vk::Queue TransferQueue;
        vk::DispatchLoaderDynamic* DispatchHandle;
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

    // ---- Find Assets ----

    /// Find the named shader, including the shared data in the search
    /// @param name The name of the shader
    /// @returns A handle to the found shader. Or nullptr if it wasn't found
    [[nodiscard]] const AxrShader* findShader_shared(const std::string& name) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_SceneName;
    AxrAssetCollection_T m_AssetCollection;
    AxrVulkanSceneData* m_SharedVulkanSceneData;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_TransferCommandPool;
    vk::Queue m_TransferQueue;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    std::unordered_map<std::string, AxrVulkanModelData> m_ModelData;
    std::unordered_map<std::string, AxrVulkanMaterialLayoutData> m_MaterialLayoutData;
    std::unordered_map<std::string, AxrVulkanMaterialData> m_MaterialData;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

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
};

#endif
