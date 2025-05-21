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
        vk::PhysicalDevice PhysicalDevice;
        vk::Device Device;
        vk::CommandPool TransferCommandPool;
        vk::Queue TransferQueue;
        uint32_t MaxFramesInFlight;
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

    /// Check if the loaded scenes have been set up
    /// @returns True if the loaded scenes have been set up
    [[nodiscard]] bool isSetup() const;
    /// Set up the loaded scenes collection
    /// @param config Setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const SetupConfig& config);
    /// Reset the setup() function
    void resetSetup();

    /// Get the global scene data
    /// @returns The global scene data. Or nullptr if it doesn't exist
    [[nodiscard]] AxrVulkanSceneData* getGlobalSceneData() const; 

    /// Load the global scene
    /// @param assetCollection Asset collection to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadGlobalSceneData(
        AxrAssetCollection_T assetCollection
    );
    /// Load a new scene
    /// @param sceneName Name of the scene
    /// @param assetCollection Asset collection to use
    /// @param ecsRegistryHandle ECS registry to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadScene(
        const std::string& sceneName,
        AxrAssetCollection_T assetCollection,
        entt::registry* ecsRegistryHandle
    );
    /// Unload the named scene
    /// @param sceneName Name of the scene 
    void unloadScene(const std::string& sceneName);

    /// Unload and remove all loaded scenes
    void clear();

    /// Find the named loaded scene data
    /// @param sceneName Scene name
    /// @returns The named loaded scene data. Or nullptr if it wasn't found
    [[nodiscard]] AxrVulkanSceneData* findLoadedScene(const std::string& sceneName);

    /// Set the active scene to the named scene
    /// @param sceneName Scene name
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setActiveScene(const std::string& sceneName);
    /// Get the active scene
    /// @returns A handle to the active scene data or nullptr if an active scene isn't set
    [[nodiscard]] AxrVulkanSceneData* getActiveScene() const;

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
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::CommandPool m_TransferCommandPool;
    vk::Queue m_TransferQueue;
    uint32_t m_MaxFramesInFlight;
    vk::DispatchLoaderDynamic* m_Dispatch;

    bool m_IsSetup;
    std::string m_GlobalSceneName = "AXR:SceneGlobal";

    // ---- Setup Window Config ----
    vk::RenderPass m_WindowRenderPass;

    AxrVulkanSceneData* m_ActiveScene;
    // TODO: Check if this needs to be a pointer
    std::vector<AxrVulkanSceneData*> m_LoadedScenes;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Find the named loaded scene data iterator
    /// @param sceneName Scene name
    /// @returns The named loaded scene data iterator. Or m_LoadedScenes.end() if it wasn't found
    [[nodiscard]] std::vector<AxrVulkanSceneData*>::iterator findLoadedSceneIterator(const std::string& sceneName);

    /// Create vulkan scene data
    /// @param sceneName Name of the scene
    /// @param assetCollection Asset collection to use
    /// @param ecsRegistryHandle ECS registry to use
    /// @param globalSceneData Global scene data
    /// @returns A handle to the created vulkan scene data
    [[nodiscard]] AxrVulkanSceneData* createSceneData(
        const std::string& sceneName,
        AxrAssetCollection_T assetCollection,
        entt::registry* ecsRegistryHandle,
        AxrVulkanSceneData* globalSceneData
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
