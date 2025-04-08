#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_map>
#include <string>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "shader.hpp"
#include "material.hpp"

/// Axr Asset Collection
class AxrAssetCollection {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrAssetCollection();
    /// Copy Constructor
    /// @param src Source AxrAssetCollection to copy from
    AxrAssetCollection(const AxrAssetCollection& src) = delete;
    /// Move Constructor
    /// @param src Source AxrAssetCollection to move from
    AxrAssetCollection(AxrAssetCollection&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrAssetCollection();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrAssetCollection to copy from
    AxrAssetCollection& operator=(const AxrAssetCollection& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrAssetCollection to move from
    AxrAssetCollection& operator=(AxrAssetCollection&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    // ---- All Assets ----

    /// Check if all the assets in the collection have been loaded
    /// @returns True if all assets in the collection have been loaded
    [[nodiscard]] bool isLoaded();
    /// Load all assets in the collection
    /// @param graphicsApi Graphics api to use with these assets
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult loadAssets(AxrGraphicsApiEnum graphicsApi);
    /// Unload all assets in the collection
    void unloadAssets();

    // ---- Shader ----

    /// Create a new shader
    /// @param shaderConfig Shader config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createShader(const AxrShaderConfig& shaderConfig);
    /// Create a new engine asset shader
    /// @param engineAssetEnum Engine asset enum
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createShader(AxrShaderEngineAssetEnum engineAssetEnum);

    // ---- Material ----

    /// Create a new material
    /// @param materialConfig Material config
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMaterial(const AxrMaterialConfig& materialConfig);
    /// Create a material using the engine defined material named 'Default Material'
    /// @param materialName Material name
    /// @param materialValues Material values
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createMaterial(
        const char* materialName,
        AxrMaterialEngineAsset_DefaultMaterial materialValues
    );

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Find the named shader
    /// @param name Name of the shader
    /// @returns A handle to the shader if it was found. nullptr if it wasn't found
    [[nodiscard]] const AxrShader* findShader(const std::string& name);

    /// Get the shaders
    /// @returns A map of the shaders 
    [[nodiscard]] const std::unordered_map<std::string, AxrShader>& getShaders();
    /// Get the materials
    /// @returns A map of the materials
    [[nodiscard]] const std::unordered_map<std::string, AxrMaterial>& getMaterials();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    std::unordered_map<std::string, AxrShader> m_Shaders;
    std::unordered_map<std::string, AxrMaterial> m_Materials;
};
