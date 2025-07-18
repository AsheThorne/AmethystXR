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

    const char* defaultMaterialName = "SponzaDefaultMaterial";
    if (AXR_FAILED(
        assetCollection.createMaterial(
            defaultMaterialName,
            axr::EngineAssetMaterial_DefaultMaterial(
                axr::engineAssetGetName(axr::EngineAssetEnum::ImageMissingTexture),
                axr::engineAssetGetName(axr::EngineAssetEnum::ImageSamplerNearestRepeat)
            )
        )
    ))
        return axr::Result::Error;

    const std::string modelName = "SponzaModel";
    const axr::ModelConfig modelConfig(
        modelName.c_str(),
        "P:/C++/AmethystXR/Sandbox/assets/sample-models/2.0/Sponza/glTF/Sponza.gltf"
    );

    if (AXR_FAILED(assetCollection.createModel(modelConfig))) return axr::Result::Error;

    axr::ModelFileInfo modelInfo;
    if (AXR_FAILED(axr::getModelFileData(modelConfig.FilePath, modelInfo))) return axr::Result::Error;

    std::vector<std::string> imageSamplerNames(modelInfo.ImageSamplerCount, "");
    for (int i = 0; i < modelInfo.ImageSamplerCount; ++i) {
        if (!axrStringIsEmpty(modelInfo.ImageSamplers[i].Name)) {
            imageSamplerNames[i] = modelInfo.ImageSamplers[i].Name;
        } else {
            imageSamplerNames[i] = modelName + "_ImageSampler(" + std::to_string(i + 1) + ")";
        }

        axr::ImageSamplerConfig config(
            imageSamplerNames[i].c_str(),
            modelInfo.ImageSamplers[i].MinFilter,
            modelInfo.ImageSamplers[i].MagFilter,
            modelInfo.ImageSamplers[i].MipmapFilter,
            modelInfo.ImageSamplers[i].WrapU,
            modelInfo.ImageSamplers[i].WrapV
        );
        if (AXR_FAILED(assetCollection.createImageSampler(config))) return axr::Result::Error;
    }

    std::vector<std::string> imageNames(modelInfo.ImageCount, "");
    for (uint32_t i = 0; i < modelInfo.ImageCount; ++i) {
        if (!axrStringIsEmpty(modelInfo.Images[i].Name)) {
            imageNames[i] = modelInfo.Images[i].Name;
        } else {
            imageNames[i] = modelName + "_Image(" + std::to_string(i + 1) + ")";
        }

        axr::ImageConfig config(
            imageNames[i].c_str(),
            modelInfo.Images[i].FilePath
        );
        if (AXR_FAILED(assetCollection.createImage(config))) return axr::Result::Error;
    }

    std::vector<std::string> materialNames(modelInfo.MaterialCount, "");
    for (uint32_t i = 0; i < modelInfo.MaterialCount; ++i) {
        if (!axrStringIsEmpty(modelInfo.Materials[i].Name)) {
            materialNames[i] = modelInfo.Materials[i].Name;
        } else {
            materialNames[i] = modelName + "_Material(" + std::to_string(i + 1) + ")";
        }

        const char* imageName = nullptr;
        const char* imageSamplerName = nullptr;

        if (modelInfo.Materials[i].ColorImageIndex >= 0) {
            imageName = imageNames[modelInfo.Materials[i].ColorImageIndex].c_str();
        }

        if (modelInfo.Materials[i].ColorImageSamplerIndex >= 0) {
            imageSamplerName = imageSamplerNames[modelInfo.Materials[i].ColorImageSamplerIndex].c_str();
        } else {
            imageSamplerName = axr::engineAssetGetName(axr::EngineAssetEnum::ImageSamplerLinearRepeat);
        }

        if (AXR_FAILED(
            assetCollection.createMaterial(
                materialNames[i].c_str(),
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
            const char* materialName = nullptr;

            if (modelInfo.Meshes[meshIndex].Submeshes[submeshIndex].MaterialIndex >= 0) {
                materialName = materialNames[modelInfo.Meshes[meshIndex].Submeshes[submeshIndex].MaterialIndex].c_str();
            } else {
                materialName = defaultMaterialName;
            }

            AxrModelComponent::Mesh::Submesh submesh{
                .MaterialName = {},
            };
            strcpy_s(submesh.MaterialName, materialName);

            m_ComponentSubmeshes.emplace_back(submesh);
        }

        m_ComponentMeshes[meshIndex] = AxrModelComponent::Mesh{
            .SubmeshCount = static_cast<uint32_t>(m_ComponentSubmeshes.size()),
            .Submeshes = m_ComponentSubmeshes.data()
        };
    }

    AxrModelComponent modelComponent{
        .ModelName = {},
        .MeshCount = static_cast<uint32_t>(m_ComponentMeshes.size()),
        .Meshes = m_ComponentMeshes.data(),
        .PushConstantBufferName = {},
    };
    strcpy_s(modelComponent.ModelName, modelName.c_str());
    strcpy_s(
        modelComponent.PushConstantBufferName,
        axr::engineAssetGetName(axr::EngineAssetEnum::PushConstantBufferModelMatrix)
    );

    m_Entity.emplace<AxrModelComponent>(modelComponent);

    m_CameraEntity = m_Scene.createEntity();

    m_CameraEntity.emplace<AxrTransformComponent>(
        AxrTransformComponent{
            .Position = glm::vec3(0.0f, 1.0f, -0.5f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
            .Orientation = glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f)),
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

    // ---- XR Head ----

    const char* testCubeImageName = "UvTesterImage";
    if (AXR_FAILED(
        m_Scene.getAssetCollection().createImage(testCubeImageName, axr::EngineAssetEnum::ImageUvTester)
    )) {
        return axr::Result::Error;
    }

    const char* testCubeMaterialName = "HandMaterial";
    if (AXR_FAILED(
        assetCollection.createMaterial(
            testCubeMaterialName,
            axr::EngineAssetMaterial_DefaultMaterial(
                testCubeImageName,
                axr::engineAssetGetName(axr::EngineAssetEnum::ImageSamplerNearestRepeat)
            )
        )
    )) {
        return axr::Result::Error;
    }

    const char* testCubeModelName = "TestCube";
    if (AXR_FAILED(assetCollection.createModel(testCubeModelName, axr::EngineAssetEnum::ModelCube))) {
        return axr::Result::Error;
    }

    m_XrHeadEntity = m_Scene.createEntity();
    m_XrHeadEntity.emplace<AxrTransformComponent>(
        AxrTransformComponent{
            .Position = glm::vec3(0.0f, 0.0f, 0.0f),
            .Scale = glm::vec3(0.3f, 0.3f, 0.3f),
            .Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        }
    );
    m_XrHeadEntity.emplace<AxrMirrorPoseInputActionComponent>(
        AxrMirrorPoseInputActionComponent{
            .ActionSetName = "test",
            .PoseInputActionName = "head",
            .OffsetPosition = glm::vec3(0.0f, 0.0f, 0.0f),
            .OffsetOrientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        }
    );

    AxrModelComponent::Mesh::Submesh headSubmesh{
        .MaterialName = {},
    };
    strcpy_s(headSubmesh.MaterialName, testCubeMaterialName);

    m_HeadComponentSubmeshes = {
        headSubmesh,
    };
    m_HeadComponentMeshes = {
        AxrModelComponent::Mesh{
            .SubmeshCount = static_cast<uint32_t>(m_HeadComponentSubmeshes.size()),
            .Submeshes = m_HeadComponentSubmeshes.data(),
        }
    };

    AxrModelComponent headModelComponent{
        .ModelName = {},
        .MeshCount = static_cast<uint32_t>(m_HeadComponentMeshes.size()),
        .Meshes = m_HeadComponentMeshes.data(),
        .PushConstantBufferName = {},
    };
    strcpy_s(headModelComponent.ModelName, testCubeModelName);
    strcpy_s(
        headModelComponent.PushConstantBufferName,
        axr::engineAssetGetName(axr::EngineAssetEnum::PushConstantBufferModelMatrix)
    );

    m_XrHeadEntity.emplace<AxrModelComponent>(headModelComponent);

    // ---- XR Hand ----

    m_XrHandEntity = m_Scene.createEntity();
    m_XrHandEntity.emplace<AxrTransformComponent>(
        AxrTransformComponent{
            .Position = glm::vec3(0.0f, 0.0f, 0.0f),
            .Scale = glm::vec3(0.1f, 0.1f, 0.1f),
            .Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        }
    );
    m_XrHandEntity.emplace<AxrMirrorPoseInputActionComponent>(
        AxrMirrorPoseInputActionComponent{
            .ActionSetName = "test",
            .PoseInputActionName = "righthand",
            .OffsetPosition = glm::vec3(0.0f, 0.2f, 0.0f),
            .OffsetOrientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        }
    );

    AxrModelComponent::Mesh::Submesh handSubmesh{
        .MaterialName = {},
    };
    strcpy_s(handSubmesh.MaterialName, testCubeMaterialName);

    m_HandComponentSubmeshes = {
        handSubmesh,
    };
    m_HandComponentMeshes = {
        AxrModelComponent::Mesh{
            .SubmeshCount = static_cast<uint32_t>(m_HandComponentSubmeshes.size()),
            .Submeshes = m_HandComponentSubmeshes.data(),
        }
    };

    AxrModelComponent handModelComponent{
        .ModelName = {},
        .MeshCount = static_cast<uint32_t>(m_HandComponentMeshes.size()),
        .Meshes = m_HandComponentMeshes.data(),
        .PushConstantBufferName = {},
    };
    strcpy_s(handModelComponent.ModelName, testCubeModelName);
    strcpy_s(
        handModelComponent.PushConstantBufferName,
        axr::engineAssetGetName(axr::EngineAssetEnum::PushConstantBufferModelMatrix)
    );

    m_XrHandEntity.emplace<AxrModelComponent>(handModelComponent);

    return axr::Result::Success;
}

axr::Result SponzaScene::loadScene() const {
    return m_Application.loadScene(m_SceneName.c_str());
}

axr::Result SponzaScene::setAsActiveScene() const {
    return m_Application.setActiveScene(m_SceneName.c_str());
}

void SponzaScene::update() {
    const axr::ActionSystem actionSystem = m_Application.getActionSystem();
    const axr::ActionSet actionSet = actionSystem.getActionSet("test");
    const axr::BoolInputAction keyAction = actionSet.getBoolInputAction("click");
    const axr::PoseInputAction headAction = actionSet.getPoseInputAction("righthand");

    if (keyAction.valueChanged() && keyAction.getValue()) {
        if (headAction.isEnabled()) {
            headAction.disable();
        } else {
            headAction.enable();
        }
    }
}
