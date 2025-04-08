#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <vector>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanSceneData.hpp"

/// Vulkan loaded scenes collection
class AxrVulkanLoadedScenesCollection {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// AxrVulkanLoadedScenesCollection setup config
    struct SetupConfig {
        vk::Device Device;
        vk::DispatchLoaderDynamic* Dispatch;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrVulkanLoadedScenesCollection();
    /// Copy Constructor
    /// @param src Source AxrVulkanLoadedScenesCollection to copy from
    AxrVulkanLoadedScenesCollection(const AxrVulkanLoadedScenesCollection& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanLoadedScenesCollection to move from
    AxrVulkanLoadedScenesCollection(AxrVulkanLoadedScenesCollection&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanLoadedScenesCollection();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanLoadedScenesCollection to copy from
    AxrVulkanLoadedScenesCollection& operator=(const AxrVulkanLoadedScenesCollection& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanLoadedScenesCollection to move from
    AxrVulkanLoadedScenesCollection& operator=(AxrVulkanLoadedScenesCollection&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Find the named loaded scene data
    /// @param sceneName Scene name
    /// @returns The named loaded scene data. Or nullptr if it wasn't found
    [[nodiscard]] const AxrVulkanSceneData* findLoadedScene(const char* sceneName);

    /// Set up the loaded scenes collection
    /// @param config Setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const SetupConfig& config);
    /// Reset the setup() function
    void resetSetup();

    /// Load a new scene
    /// @param sceneName Name of the scene
    /// @param assetCollection Asset collection to use
    /// @param sharedSceneData Shared scene assets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadScene(
        const char* sceneName,
        AxrAssetCollection_T assetCollection,
        AxrVulkanSceneData* sharedSceneData
    );
    /// Unload the named scene
    /// @param sceneName Name of the scene 
    void unloadScene(const char* sceneName);

    /// Unload and remove all loaded scenes
    void clear();

    /// Set up the window data for all scenes and load all window specific scene data
    /// @param renderPass Render pass to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupWindowData(vk::RenderPass renderPass);
    /// Reset the setupWindowData() function
    void resetSetupWindowData();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Setup Config ----
    vk::Device m_Device;
    vk::DispatchLoaderDynamic* m_Dispatch;

    // ---- Setup Window Config ----
    vk::RenderPass m_WindowRenderPass;

    std::vector<AxrVulkanSceneData*> m_LoadedScenes;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Find the named loaded scene data iterator
    /// @param sceneName Scene name
    /// @returns The named loaded scene data iterator. Or m_LoadedScenes.end() if it wasn't found
    [[nodiscard]] std::vector<AxrVulkanSceneData*>::iterator findLoadedSceneIterator(const char* sceneName);

    /// Create vulkan scene data
    /// @param sceneName Name of the scene
    /// @param assetCollection Asset collection to use
    /// @param sharedSceneData Shared scene data
    /// @returns A handle to the created vulkan scene data
    [[nodiscard]] AxrVulkanSceneData* createSceneData(
        const char* sceneName,
        AxrAssetCollection_T assetCollection,
        AxrVulkanSceneData* sharedSceneData
    ) const;
    /// Destroy the given vulkan scene data
    void destroySceneData(AxrVulkanSceneData*& sceneData) const;

    // ---- Window Scene Data ----

    /// Check if the window scene data is ready
    /// @returns True if teh window scene data is ready
    [[nodiscard]] bool isWindowReady() const;

    /// Load all window specific scene data for the loaded scenes
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadAllWindowSceneData() const;
    /// Unload all window specific scene data for the loaded scenes
    void unloadAllWindowSceneData() const;
};

#endif
