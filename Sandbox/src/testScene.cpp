﻿#include "testScene.hpp"

TestScene::TestScene(axr::Application& app):
    m_Application(app),
    m_Scene(nullptr) {
}

TestScene::~TestScene() {
}

axr::Result TestScene::setup() {
    if (AXR_FAILED(m_Application.createScene(m_SceneName.c_str()))) return axr::Result::Error;
    m_Scene = m_Application.findScene(m_SceneName.c_str());

    const char* imageName = "UvTesterImage";
    if (AXR_FAILED(
        m_Scene.getAssetCollection().createImage(imageName, axr::EngineAssetEnum::ImageUvTester)
    )) {
        return axr::Result::Error;
    }

    const char* materialName = "MyMaterial";
    if (AXR_FAILED(
        m_Scene.getAssetCollection().createMaterial(
            materialName,
            axr::EngineAssetMaterial_DefaultMaterial(
                imageName,
                axr::engineAssetGetName(axr::EngineAssetEnum::ImageSamplerNearestRepeat)
            )
        )
    )) {
        return axr::Result::Error;
    }

    const char* modelName = "Cube";
    if (AXR_FAILED(m_Scene.getAssetCollection().createModel(modelName, axr::EngineAssetEnum::ModelCube))) {
        return axr::Result::Error;
    }

    m_Entity = m_Scene.createEntity();

    m_Entity.emplace<AxrTransformComponent>(
        AxrTransformComponent{
            .Position = glm::vec3(0.0f, 0.0f, 0.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
            .Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        }
    );
    AxrModelComponent::Mesh::Submesh submesh{
        .MaterialName = {},
    };
    strcpy_s(submesh.MaterialName, materialName);

    // TODO: We need a better solution than this. The entity doesn't clone the data so once this goes out of scope, we need to hold on to the data so it doesn't delete
    //  Maybe we change the components to take std::vector instead of pointers??
    m_ComponentSubmeshes = {
        submesh,
    };
    m_ComponentMeshes = {
        AxrModelComponent::Mesh{
            .SubmeshCount = static_cast<uint32_t>(m_ComponentSubmeshes.size()),
            .Submeshes = m_ComponentSubmeshes.data(),
        }
    };
    AxrModelComponent modelComponent{
        .ModelName = {},
        .MeshCount = static_cast<uint32_t>(m_ComponentMeshes.size()),
        .Meshes = m_ComponentMeshes.data(),
        .PushConstantBufferName = {},
    };
    strcpy_s(modelComponent.ModelName, modelName);
    strcpy_s(
        modelComponent.PushConstantBufferName,
        axr::engineAssetGetName(axr::EngineAssetEnum::PushConstantBufferModelMatrix)
    );

    m_Entity.emplace<AxrModelComponent>(
        modelComponent
    );

    m_CameraEntity = m_Scene.createEntity();

    m_CameraEntity.emplace<AxrTransformComponent>(
        AxrTransformComponent{
            .Position = glm::vec3(0.0f, 0.0f, 2.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
            .Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        }
    );

    m_CameraEntity.emplace<AxrCameraComponent>(
        AxrCameraComponent{
            .Fov = 90.0f,
            .NearPlane = 0.1f,
            .FarPlane = 1000.0f,
        }
    );

    m_Scene.setMainCamera(m_CameraEntity);

    return axr::Result::Success;
}

axr::Result TestScene::loadScene() const {
    return m_Application.loadScene(m_SceneName.c_str());
}

axr::Result TestScene::setAsActiveScene() const {
    return m_Application.setActiveScene(m_SceneName.c_str());
}

void TestScene::update() {
    float deltaTime = m_Application.getDeltaTime();

    m_Entity.patch<AxrTransformComponent>(
        [deltaTime](AxrTransformComponent& transform) {
            transform.Orientation = glm::rotate(
                transform.Orientation,
                0.8f * deltaTime,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
    );
}
