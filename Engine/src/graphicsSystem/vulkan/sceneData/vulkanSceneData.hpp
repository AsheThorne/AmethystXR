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
        AxrAssetCollection_T AssetCollection;
        AxrVulkanSceneData* SharedVulkanSceneData;
    };

    /// Vulkan scene data setup config
    struct SetupConfig {
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

    /// Check if the scene data is set up
    /// @returns True if the scene data is set up
    [[nodiscard]] bool isSetup();
    /// Set up the scene data
    /// @param config Scene data setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const SetupConfig& config);
    /// Reset the setup() function
    void resetSetup();

    /// Load the scene data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadScene();
    /// Unload the scene data
    void unloadScene();

    /// Load the window specific scene data
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadWindowData();
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
    AxrAssetCollection_T m_AssetCollection;
    AxrVulkanSceneData* m_SharedVulkanSceneData;

    // ---- Setup Config Variables ----
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
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createAllWindowMaterialLayoutData();
    /// Destroy all window specific material layout data
    void destroyAllWindowMaterialLayoutData();
};

#endif
