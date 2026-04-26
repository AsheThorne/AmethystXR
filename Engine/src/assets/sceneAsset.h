#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/assets.h"

/// Axr Scene Asset
class AxrSceneAsset {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrSceneAsset();
    /// Constructor
    explicit AxrSceneAsset(const AxrSceneAssetConfig& config);
    /// Copy Constructor
    /// @param src Source AxrSceneAsset to copy from
    AxrSceneAsset(const AxrSceneAsset& src) = delete;
    /// Move Constructor
    /// @param src Source AxrSceneAsset to move from
    AxrSceneAsset(AxrSceneAsset&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrSceneAsset();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrSceneAsset to copy from
    AxrSceneAsset& operator=(const AxrSceneAsset& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrSceneAsset to move from
    AxrSceneAsset& operator=(AxrSceneAsset&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //
    AxrID_T m_ID{};

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
    /// Move the given AxrSceneAsset to this class
    /// @param src AxrSceneAsset to move
    /// @param useConstructor If true, this function will use the move constructor for non-primitive objects instead of
    /// the move assignment operator when moving variables
    void move_internal(AxrSceneAsset&& src, bool useConstructor);
};
