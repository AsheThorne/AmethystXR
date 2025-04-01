#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "vulkanMaterialLayoutAssets.hpp"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <string>
#include <unordered_map>

/// Vulkan scene assets
class AxrVulkanSceneAssets {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan scene assets config
    struct Config {
        AxrAssetCollection_T AssetCollection;
        AxrVulkanSceneAssets* SharedVulkanSceneAssets;
    };

    /// Vulkan scene assets setup config
    struct SetupConfig {
        vk::Device Device;
        vk::DispatchLoaderDynamic* DispatchHandle;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Vulkan scene assets config
    AxrVulkanSceneAssets(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanSceneAssets to copy from
    AxrVulkanSceneAssets(const AxrVulkanSceneAssets& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanSceneAssets to move from
    AxrVulkanSceneAssets(AxrVulkanSceneAssets&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanSceneAssets();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanSceneAssets to copy from
    AxrVulkanSceneAssets& operator=(const AxrVulkanSceneAssets& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanSceneAssets to move from
    AxrVulkanSceneAssets& operator=(AxrVulkanSceneAssets&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the scene assets are set up
    /// @returns True if the scene assets are set up
    [[nodiscard]] bool isSetup();
    /// Set up the scene assets
    /// @param config Scene assets setup config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const SetupConfig& config);
    /// Reset the setup() function
    void resetSetup();

    /// Load the scene assets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadAssets();
    /// Unload the scene assets
    void unloadAssets();

    /// Load the window specific scene assets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadWindowAssets();
    /// Unload the window specific scene assets
    void unloadWindowAssets();

    // ---- Find Assets ----

    /// Find the named shader, including the shared assets in the search
    /// @param name The name of the shader
    /// @returns A handle to the found shader. Or nullptr if it wasn't found
    [[nodiscard]] const AxrShader* findShader_shared(const std::string& name) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    AxrAssetCollection_T m_AssetCollection;
    AxrVulkanSceneAssets* m_SharedVulkanSceneAssets;

    // ---- Setup Config Variables ----
    vk::Device m_Device;
    vk::DispatchLoaderDynamic* m_DispatchHandle;

    std::unordered_map<std::string, AxrVulkanMaterialLayoutAssets> m_MaterialLayoutAssets;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //
    
    // ---- Material Layout ----

    /// Load the material layout assets
    /// @results AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadMaterialLayoutAssets();
    /// Unload the material layout assets
    void unloadMaterialLayoutAssets();

    /// Initialize all the material layout assets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult initializeMaterialLayoutAssets();
    /// Initialize a single material layout asset for the given material
    /// @param material Material to use
    /// @returns AXR_SUCCESS if the function succeeded
    void initializeMaterialLayoutAsset(const AxrMaterial& material);

    /// Load the given material layout assets
    /// @param materialLayoutAssets Material layout assets to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadMaterialLayoutAsset(AxrVulkanMaterialLayoutAssets& materialLayoutAssets);
    /// Unload the given material layout assets
    /// @param materialLayoutAssets Material layout assets to use
    void unloadMaterialLayoutAsset(AxrVulkanMaterialLayoutAssets& materialLayoutAssets);

    /// Load the window specific material layout assets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadWindowMaterialLayoutAssets();
    /// Unload the window specific material layout assets
    void unloadWindowMaterialLayoutAssets();
};

#endif
