#include "testScene.hpp"

TestScene::TestScene(axr::Application& app):
    m_Application(app),
    m_Scene(nullptr) {
}

TestScene::~TestScene() {
}

axr::Result TestScene::setup() {
    if (AXR_FAILED(m_Application.createScene(m_SceneName.c_str()))) return axr::Result::Error;
    m_Scene = m_Application.findScene(m_SceneName.c_str());

    if (AXR_FAILED(
        m_Scene.getAssetCollection().createImage(m_ImageName.c_str(), axr::EngineAssetEnum::ImageUvTester)
    )) {
        return axr::Result::Error;
    }

    const axr::ImageSamplerConfig imageSamplerConfig(
        m_ImageSamplerName.c_str(),
        axr::ImageSamplerFilterEnum::Nearest,
        axr::ImageSamplerFilterEnum::Nearest,
        axr::ImageSamplerFilterEnum::Nearest,
        axr::ImageSamplerWrappingEnum::Repeat,
        axr::ImageSamplerWrappingEnum::Repeat
    );
    if (AXR_FAILED(m_Scene.getAssetCollection().createImageSampler(imageSamplerConfig))) return axr::Result::Error;


    if (AXR_FAILED(
        m_Scene.getAssetCollection().createMaterial(
            m_MaterialName.c_str(),
            axr::EngineAssetMaterial_DefaultMaterial(
                m_ImageName.c_str(),
                m_ImageSamplerName.c_str())
        )
    )) {
        return axr::Result::Error;
    }

    if (AXR_FAILED(m_Scene.getAssetCollection().createModel(m_ModelName.c_str(), axr::EngineAssetEnum::ModelCube))) {
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
    // TODO: We need a better solution than this. The entity doesn't clone the data so once this goes out of scope, we need to hold on to the data so it doesn't delete 
    m_ComponentSubmeshes = {
        AxrModelComponent::Mesh::Submesh{
            .MaterialName = m_MaterialName.c_str(),
        },
    };
    m_ComponentMeshes = {
        AxrModelComponent::Mesh{
            .SubmeshCount = static_cast<uint32_t>(m_ComponentSubmeshes.size()),
            .Submeshes = m_ComponentSubmeshes.data(),
        }
    };
    m_Entity.emplace<AxrModelComponent>(
        AxrModelComponent{
            .ModelName = m_ModelName.c_str(),
            .MeshCount = static_cast<uint32_t>(m_ComponentMeshes.size()),
            .Meshes = m_ComponentMeshes.data(),
            .PushConstantBufferName = axr::engineAssetGetName(
                axr::EngineAssetEnum::PushConstantBufferModelMatrix
            ),
        }
    );

    return axr::Result::Success;
}

axr::Result TestScene::loadScene() const {
    return m_Application.loadScene(m_SceneName.c_str());
}

axr::Result TestScene::setAsActiveScene() const {
    return m_Application.setActiveScene(m_SceneName.c_str());
}

void TestScene::update() {
    m_Entity.patch<AxrTransformComponent>(
        [](AxrTransformComponent& transform) {
            //TODO: Get and use delta time
            transform.Orientation = glm::rotate(transform.Orientation, 0.0001f, glm::vec3(0.0f, 1.0f, 0.0f));
        }
    );
}
