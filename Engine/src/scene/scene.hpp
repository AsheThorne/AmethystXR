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
    [[nodiscard]] const std::string& getName() const;
    /// Get the scene asset collection
    /// @returns A handle to the scene asset collection
    [[nodiscard]] AxrAssetCollection_T getAssetCollection();
    /// Get the scene ECS registry
    /// @returns A handle to the scene ECS registry
    [[nodiscard]] entt::registry* getEcsRegistry();

    /// Create a new entity
    /// @returns The entity id
    [[nodiscard]] entt::entity createEntity();

    /// Set the scene's main camera
    /// @param entity Entity with a camera component
    void setMainCamera(AxrEntityConst_T entity);

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Get the main scene's main camera
    /// @returns The main camera
    AxrEntityConst_T getMainCamera() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    std::string m_Name;
    AxrAssetCollection m_AssetCollection;
    entt::registry m_Registry;
    AxrEntityConst_T m_MainCamera;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
};
