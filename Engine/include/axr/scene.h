#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/assets.h"

// ----------------------------------------- //
// EnTT Headers
// ----------------------------------------- //
#include <entt/entt.hpp>

// ----------------------------------------- //
// GLM Headers
// ----------------------------------------- //
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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
    struct Submesh {
        char MaterialName[AXR_MAX_ASSET_NAME_SIZE];
    };

    struct Mesh {
        std::vector<Submesh> Submeshes;
    };

    char ModelName[AXR_MAX_ASSET_NAME_SIZE];
    std::vector<Mesh> Meshes;

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    char PushConstantBufferName[AXR_MAX_ASSET_NAME_SIZE];
#endif
};

/// Entity camera component
struct AxrCameraComponent {
    float Fov;
    float NearPlane;
    float FarPlane;
};

/// Entity mirror pose input action component
///
/// Requires the AxrTransformComponent to apply the pose transforms to
struct AxrMirrorPoseInputActionComponent {
    char ActionSetName[AXR_MAX_ACTION_SET_NAME_SIZE];
    char PoseInputActionName[AXR_MAX_ACTION_NAME_SIZE];
    glm::vec3 OffsetPosition;
    glm::quat OffsetOrientation;
};

// ---------------------------------------------------------------------------------- //
//                                      Scene                                         //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Types
// ----------------------------------------- //

/// Entity handle
typedef entt::handle AxrEntity_T;
/// Const Entity handle
typedef entt::const_handle AxrEntityConst_T;

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrScene Handle
typedef class AxrScene* AxrScene_T;
/// const AxrScene Handle
typedef const AxrScene* AxrSceneConst_T;

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
    /// Get the scene name
    /// @param scene The scene to use
    /// @returns The name of the scene
    AXR_API const char* axrSceneGetName(AxrSceneConst_T scene);
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

    /// Set the scene's main camera
    /// @param scene The scene to use
    /// @param entity Entity with a camera component
    AXR_API void axrSceneSetMainCamera(AxrScene_T scene, AxrEntityConst_T entity);
}
