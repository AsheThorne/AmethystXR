#include "sponzaScene.hpp"

SponzaScene::SponzaScene(axr::Application& app):
    m_Application(app),
    m_Scene(nullptr) {
}

SponzaScene::~SponzaScene() {
}

axr::Result SponzaScene::setup() {
    if (AXR_FAILED(m_Application.createScene(m_SceneName.c_str()))) return axr::Result::Error;
    m_Scene = m_Application.findScene(m_SceneName.c_str());
    axr::AssetCollection assetCollection = m_Scene.getAssetCollection();    

    const axr::ModelConfig modelConfig(
        m_ModelName.c_str(),
        "P:/C++/AmethystXR/Sandbox/assets/sample-models/2.0/Sponza/glTF/Sponza.gltf"
    );

    if (AXR_FAILED(assetCollection.createModel(modelConfig))) return axr::Result::Error;

    axr::ModelFileInfo modelInfo;
    if (AXR_FAILED(axr::getModelFileData(modelConfig.FilePath, modelInfo))) return axr::Result::Error;

    m_ImageSamplerNames = std::vector<std::string>(modelInfo.ImageSamplerCount, "");
    for (int i = 0; i < modelInfo.ImageSamplerCount; ++i) {
        if (!axrStringIsEmpty(modelInfo.ImageSamplers[i].Name)) {
            m_ImageSamplerNames[i] = modelInfo.ImageSamplers[i].Name;
        } else {
            m_ImageSamplerNames[i] = m_ModelName + "_ImageSampler(" + std::to_string(i + 1) + ")";
        }

        axr::ImageSamplerConfig config(
            m_ImageSamplerNames[i].c_str(),
            modelInfo.ImageSamplers[i].MinFilter,
            modelInfo.ImageSamplers[i].MagFilter,
            modelInfo.ImageSamplers[i].MipmapFilter,
            modelInfo.ImageSamplers[i].WrapU,
            modelInfo.ImageSamplers[i].WrapV
        );
        if (AXR_FAILED(assetCollection.createImageSampler(config))) return axr::Result::Error;
    }

    m_ImageNames = std::vector<std::string>(modelInfo.ImageCount, "");
    for (uint32_t i = 0; i < modelInfo.ImageCount; ++i) {
        if (!axrStringIsEmpty(modelInfo.Images[i].Name)) {
            m_ImageNames[i] = modelInfo.Images[i].Name;
        } else {
            m_ImageNames[i] = m_ModelName + "_Image(" + std::to_string(i + 1) + ")";
        }

        axr::ImageConfig config(
            m_ImageNames[i].c_str(),
            modelInfo.Images[i].FilePath
        );
        if (AXR_FAILED(assetCollection.createImage(config))) return axr::Result::Error;
    }

    m_MaterialNames = std::vector<std::string>(modelInfo.MaterialCount, "");
    for (uint32_t i = 0; i < modelInfo.MaterialCount; ++i) {
        if (!axrStringIsEmpty(modelInfo.Materials[i].Name)) {
            m_MaterialNames[i] = modelInfo.Materials[i].Name;
        } else {
            m_MaterialNames[i] = m_ModelName + "_Material(" + std::to_string(i + 1) + ")";
        }

        const char* imageName = nullptr;
        const char* imageSamplerName = nullptr;
        
        if (modelInfo.Materials[i].ColorImageIndex >= 0) {
            imageName = m_ImageNames[modelInfo.Materials[i].ColorImageIndex].c_str();
        }

        if (modelInfo.Materials[i].ColorImageSamplerIndex >= 0) {
            imageSamplerName = m_ImageSamplerNames[modelInfo.Materials[i].ColorImageSamplerIndex].c_str();
        } else {
            imageSamplerName = axr::engineAssetGetName(axr::EngineAssetEnum::ImageSamplerLinearRepeat);
        }

        if (AXR_FAILED(
            assetCollection.createMaterial(
                m_MaterialNames[i].c_str(),
                axr::EngineAssetMaterial_DefaultMaterial(
                    imageName,
                    imageSamplerName
                )
            )
        )) {
            return axr::Result::Error;
        }
    }

    m_Entity = m_Scene.createEntity();

    m_Entity.emplace<AxrTransformComponent>(
        AxrTransformComponent{
            .Position = glm::vec3(0.0f, 0.0f, 0.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
            .Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        }
    );

    m_ComponentMeshes = std::vector<AxrModelComponent::Mesh>(modelInfo.MeshCount);
    for (uint32_t meshIndex = 0; meshIndex < modelInfo.MeshCount; ++meshIndex) {
        for (int submeshIndex = 0; submeshIndex < modelInfo.Meshes[meshIndex].SubmeshCount; ++submeshIndex) {
            // TODO: We'd probably want to use a default material instead of just skipping
            if (modelInfo.Meshes[meshIndex].Submeshes[submeshIndex].MaterialIndex == -1) continue;

            m_ComponentSubmeshes.emplace_back(
                m_MaterialNames[modelInfo.Meshes[meshIndex].Submeshes[submeshIndex].MaterialIndex].c_str()
            );
        }

        m_ComponentMeshes[meshIndex] = AxrModelComponent::Mesh{
            .SubmeshCount = static_cast<uint32_t>(m_ComponentSubmeshes.size()),
            .Submeshes = m_ComponentSubmeshes.data()
        };
    }

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

axr::Result SponzaScene::loadScene() const {
    return m_Application.loadScene(m_SceneName.c_str());
}

axr::Result SponzaScene::setAsActiveScene() const {
    return m_Application.setActiveScene(m_SceneName.c_str());
}

void SponzaScene::update() {
}
