#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/assets.h"

/// Axr Shader Asset
class AxrShaderAsset {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrShaderAsset();
    /// Constructor
    explicit AxrShaderAsset(const AxrShaderAssetConfig& config);
    /// Copy Constructor
    /// @param src Source AxrShaderAsset to copy from
    AxrShaderAsset(const AxrShaderAsset& src) = delete;
    /// Move Constructor
    /// @param src Source AxrShaderAsset to move from
    AxrShaderAsset(AxrShaderAsset&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrShaderAsset();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrShaderAsset to copy from
    AxrShaderAsset& operator=(const AxrShaderAsset& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrShaderAsset to move from
    AxrShaderAsset& operator=(AxrShaderAsset&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //
    
private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //
    
    /// Clean up this class
    void cleanup();
    /// Move the given AxrShaderAsset to this class
    /// @param src AxrShaderAsset to move
    void move_internal(AxrShaderAsset&& src);
};
