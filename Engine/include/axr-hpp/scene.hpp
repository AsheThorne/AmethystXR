#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/scene.h"

namespace axr {
    // ---------------------------------------------------------------------------------- //
    //                                     Entity                                         //
    // ---------------------------------------------------------------------------------- //

    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// Entity handle
    using Entity_T = AxrEntity_T;
    /// Const Entity handle
    using EntityConst_T = AxrEntityConst_T;

    // ---------------------------------------------------------------------------------- //
    //                                     Scene                                          //
    // ---------------------------------------------------------------------------------- //

    /// Axr Scene
    class Scene {
    public:
        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Constructor
        /// @param scene The scene handle
        Scene(const AxrScene_T scene) {
            m_Scene = scene;
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Get the scene name
        /// @returns The name of the scene
        [[nodiscard]] const char* getName() const {
            return axrSceneGetName(m_Scene);
        }

        /// Get the scene asset collection
        /// @returns The scene asset collection
        [[nodiscard]] axr::AssetCollection getAssetCollection() const {
            return axrSceneGetAssetCollection(m_Scene);
        }

        /// Get the scene ECS registry
        /// @returns The scene ECS registry
        [[nodiscard]] entt::registry* getEcsRegistry() const {
            return axrSceneGetEcsRegistry(m_Scene);
        }

        /// Create a new entity
        /// @returns A handle to the created entity
        axr::Entity_T createEntity() const {
            return {*getEcsRegistry(), axrSceneCreateEntity(m_Scene)};
        }

        /// Set the scene's main camera
        /// @param entity Entity with a camera component
        void setMainCamera(const axr::EntityConst_T entity) const {
            axrSceneSetMainCamera(m_Scene, entity);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        AxrScene_T m_Scene;
    };
}
