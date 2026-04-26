#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../common/containers/unorderedMap_dynamic.h"
#include "../common/id.h"
#include "axr/assets.h"
#include "axr/common/enums.h"
#include "sceneAsset.h"
#include "shaderAsset.h"

/// Axr Assets
class AxrAssets {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrAssets to copy from
    AxrAssets(const AxrAssets& src) = delete;
    /// Move Constructor
    /// @param src Source AxrAssets to move from
    AxrAssets(AxrAssets&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrAssets to copy from
    AxrAssets& operator=(const AxrAssets& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrAssets to move from
    AxrAssets& operator=(AxrAssets&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrAssets();

    // ---- Destructor ----

    /// Destructor
    ~AxrAssets();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrAssets Config
    struct Config {};

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrAssets singleton
    /// @return A reference to the AxrAssets singleton
    static AxrAssets& get();

    /// Set up the assets
    /// @param config Assets config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the assets
    void shutDown();

    /// Get the scene asset registry
    /// @return The scene asset registry
    [[nodiscard]] const AxrUnorderedMap_Dynamic<AxrID, AxrSceneAsset>& getSceneAssetRegistry() const;
    /// Get the shader asset registry
    /// @return The shader asset registry
    [[nodiscard]] const AxrUnorderedMap_Dynamic<AxrID, AxrShaderAsset>& getShaderAssetRegistry() const;

    /// Register a new scene asset
    /// @param config Scene asset config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult registerSceneAsset(const AxrSceneAssetConfig& config);
    /// Register a new shader asset
    /// @param config Shader asset config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult registerShaderAsset(const AxrShaderAssetConfig& config);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrUnorderedMap_Dynamic<AxrID, AxrSceneAsset> m_SceneRegistry;
    AxrUnorderedMap_Dynamic<AxrID, AxrShaderAsset> m_ShaderRegistry;
    bool m_IsSetup = false;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Set up all registries
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupRegistries();
    /// Clean up all registries
    void cleanupRegistries();

    /// Register all engine assets
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult registerEngineAssets();

    // ---- Scene Asset ----

    /// Check if the given scene asset config is valid
    /// @param config Config to check
    /// @return AXR_SUCCESS if the config is valid
    [[nodiscard]] static AxrResult isSceneAssetConfigValid(const AxrSceneAssetConfig& config);

    // ---- Shader Asset ----

    /// Get the AxrShaderAssetConfig for the given shader engine asset
    /// @param engineAsset Engine asset
    /// @return The engine asset AxrShaderAssetConfig
    [[nodiscard]] static AxrShaderAssetConfig getEngineAssetShaderConfig(AxrEngineAssetEnum engineAsset);
    /// Check if the given shader asset config is valid
    /// @param config Config to check
    /// @return AXR_SUCCESS if the config is valid
    [[nodiscard]] static AxrResult isShaderAssetConfigValid(const AxrShaderAssetConfig& config);
};
