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
    AxrAssetCollection(AxrAssetCollection&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrAssetCollection();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrAssetCollection to copy from
    AxrAssetCollection& operator=(const AxrAssetCollection& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrAssetCollection to move from
    AxrAssetCollection& operator=(AxrAssetCollection&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Create a new shader
    /// @param shaderConfig Shader config
    /// @returns AXR_SUCCESS if the function succeeded
    AxrResult createShader(AxrShaderConfig shaderConfig);
    /// Create a new engine asset shader
    /// @param engineAssetEnum Engine asset enum
    /// @returns AXR_SUCCESS if the function succeeded
    AxrResult createShader(AxrShaderEngineAssetEnum engineAssetEnum);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    std::unordered_map<std::string, AxrShader> m_Shaders;
};
