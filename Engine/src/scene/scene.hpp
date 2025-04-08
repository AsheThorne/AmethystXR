#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/scene.h"
#include "../assets/assetCollection.hpp"

/// Axr Scene
class AxrScene {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrScene();
    /// Constructor
    /// @param name Name of the scene
    AxrScene(const char* name);
    /// Copy Constructor
    /// @param src Source AxrScene to copy from
    AxrScene(const AxrScene& src) = delete;
    /// Move Constructor
    /// @param src Source AxrScene to move from
    AxrScene(AxrScene&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrScene();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrScene to copy from
    AxrScene& operator=(const AxrScene& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrScene to move from
    AxrScene& operator=(AxrScene&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the scene name
    /// @returns The name of the scene
    [[nodiscard]] const char* getName() const;
    /// Get the scene asset collection
    /// @returns A handle to the scene asset collection
    [[nodiscard]] AxrAssetCollection_T getAssetCollection();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    const char* m_Name;
    AxrAssetCollection m_AssetCollection;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
