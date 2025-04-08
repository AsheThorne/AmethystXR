#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "vulkanMaterialLayoutData.hpp"

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
        vk::Device Device;
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
    [[nodiscard]] const char* getSceneName();

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
    vk::Device m_Device;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    std::unordered_map<std::string, AxrVulkanMaterialLayoutData> m_MaterialLayoutData;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

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
    void initializeMaterialLayoutData(const AxrMaterial& material);

    /// Create the given material layout data
    /// @param materialLayoutData Material layout data to create
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData);
    /// Destroy the given material layout data
    /// @param materialLayoutData Material layout data to destroy
    void destroyMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData);

    /// Create all window specific material layout data
    /// @param renderPass Render pass to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllWindowMaterialLayoutData(vk::RenderPass renderPass);
    /// Destroy all window specific material layout data
    void destroyAllWindowMaterialLayoutData();

    /// Create the given window specific material layout data
    /// @param renderPass Render pass to use
    /// @param materialLayoutData Input/Output material layout data to use and create window scene data for
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWindowMaterialLayoutData(
        vk::RenderPass renderPass,
        AxrVulkanMaterialLayoutData& materialLayoutData
    );
    /// Destroy the given window specific material layout data
    /// @param materialLayoutData Material layout data to destroy
    void destroyWindowMaterialLayoutData(AxrVulkanMaterialLayoutData& materialLayoutData);
};

#endif
