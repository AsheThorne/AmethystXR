#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "axr/assets.h"

// ----------------------------------------- //
// EnTT Headers
// ----------------------------------------- //
#include <entt/entt.hpp>

// ----------------------------------------- //
// GLM Headers
// ----------------------------------------- //
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// ---------------------------------------------------------------------------------- //
//                                 Entity Components                                  //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Entity transform component
struct AxrTransformComponent {
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::quat Orientation;
};

/// Entity model component
struct AxrModelComponent {
    const char* ModelName;
    // TODO: Material count must equal the number of meshes in a model
    uint32_t MaterialNamesCount;
    /// One material per mesh
    const char** MaterialNames;
};

// ---------------------------------------------------------------------------------- //
//                                      Scene                                         //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrScene Handle
typedef class AxrScene* AxrScene_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Get the scene name
    /// @param scene The scene to use
    /// @returns The name of the scene
    AXR_API const char* axrSceneGetName(AxrScene_T scene);
    /// Get the scene asset collection
    /// @param scene The scene to use
    /// @returns A handle to the scene asset collection
    AXR_API AxrAssetCollection_T axrSceneGetAssetCollection(AxrScene_T scene);
    /// Get the scene ECS registry
    /// @param scene The scene to use
    /// @returns A handle to the scene ECS registry
    AXR_API entt::registry* axrSceneGetEcsRegistry(AxrScene_T scene);

    /// Create a new entity
    /// @param scene The scene to use
    /// @returns The entity id
    AXR_API entt::entity axrSceneCreateEntity(AxrScene_T scene);
}
