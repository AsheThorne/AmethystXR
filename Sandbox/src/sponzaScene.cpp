﻿#include "sponzaScene.hpp"

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
                axr::MaterialBackfaceCullModeEnum::Back,
                axr::MaterialAlphaRenderModeEnum::Opaque,
                "",
                axr::engineAssetGetName(axr::EngineAssetEnum::ImageMissingTexture),
                axr::engineAssetGetName(axr::EngineAssetEnum::ImageSamplerNearestRepeat)
            )
        )
    ))
        return axr::Result::Error;

    const std::string modelName = "SponzaModel";
    const axr::ModelConfig modelConfig(
        modelName.c_str(),
        "sample-models/2.0/Sponza/glTF/Sponza.gltf"
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
    std::vector<std::string> alphaCutoffUniformBufferNames;
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

        axr::MaterialAlphaRenderModeEnum alphaRenderMode;
        switch (modelInfo.Materials[i].AlphaMode) {
            case axr::ModelFileMaterialInfoAlphaModeEnum::Mask:
            case axr::ModelFileMaterialInfoAlphaModeEnum::Opaque: {
                alphaRenderMode = axr::MaterialAlphaRenderModeEnum::Opaque;
                break;
            }
            case axr::ModelFileMaterialInfoAlphaModeEnum::Blend: {
                alphaRenderMode = axr::MaterialAlphaRenderModeEnum::AlphaBlend;
                break;
            }
        }

        if (modelInfo.Materials[i].AlphaCutoff > 0.0f) {
            alphaCutoffUniformBufferNames.emplace_back(
                modelName + "_AlphaCutoff(" + std::to_string(alphaCutoffUniformBufferNames.size() + 1) + ")"
            );
            if (AXR_FAILED(
                    assetCollection.createUniformBuffer(
                        axr::UniformBufferConfig(
                            alphaCutoffUniformBufferNames.back().c_str(),
                            sizeof(float),
                            &modelInfo.Materials[i].AlphaCutoff
                        )
                    )
                )
            )
                return axr::Result::Error;
        }

        if (AXR_FAILED(
            assetCollection.createMaterial(
                materialNames[i].c_str(),
                axr::EngineAssetMaterial_DefaultMaterial(
                    modelInfo.Materials[i].BackfaceCullMode,
                    alphaRenderMode,
                    modelInfo.Materials[i].AlphaCutoff > 0.0f ?
                    alphaCutoffUniformBufferNames.back().c_str() :
                    "",
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

    std::vector<AxrModelComponent::Mesh> meshes(modelInfo.MeshCount);
    for (uint32_t meshIndex = 0; meshIndex < modelInfo.MeshCount; ++meshIndex) {
        std::vector<AxrModelComponent::Submesh> submeshes(modelInfo.Meshes[meshIndex].SubmeshCount);

        for (int submeshIndex = 0; submeshIndex < modelInfo.Meshes[meshIndex].SubmeshCount; ++submeshIndex) {
            const char* materialName = nullptr;

            if (modelInfo.Meshes[meshIndex].Submeshes[submeshIndex].MaterialIndex >= 0) {
                materialName = materialNames[modelInfo.Meshes[meshIndex].Submeshes[submeshIndex].MaterialIndex].c_str();
            } else {
                materialName = defaultMaterialName;
            }

            AxrModelComponent::Submesh submesh{
                .MaterialName = {},
            };
            strcpy_s(submesh.MaterialName, materialName);

            submeshes[submeshIndex] = submesh;
        }

        meshes[meshIndex] = AxrModelComponent::Mesh{
            .Submeshes = submeshes
        };
    }

    AxrModelComponent modelComponent{
        .ModelName = {},
        .Meshes = meshes,
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
                axr::MaterialBackfaceCullModeEnum::Back,
                axr::MaterialAlphaRenderModeEnum::Opaque,
                "",
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

    AxrModelComponent::Submesh testCubeSubmesh{
        .MaterialName = {},
    };
    strcpy_s(testCubeSubmesh.MaterialName, testCubeMaterialName);

    AxrModelComponent headModelComponent{
        .ModelName = {},
        .Meshes = std::vector{
            AxrModelComponent::Mesh{
                .Submeshes = std::vector{
                    testCubeSubmesh,
                },
            },
        },
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

    AxrModelComponent handModelComponent{
        .ModelName = {},
        .Meshes = std::vector{
            AxrModelComponent::Mesh{
                .Submeshes = std::vector{
                    testCubeSubmesh,
                }
            },
        },
        .PushConstantBufferName = {},
    };
    strcpy_s(handModelComponent.ModelName, testCubeModelName);
    strcpy_s(
        handModelComponent.PushConstantBufferName,
        axr::engineAssetGetName(axr::EngineAssetEnum::PushConstantBufferModelMatrix)
    );

    m_XrHandEntity.emplace<AxrModelComponent>(handModelComponent);

    // ---- UI ----

    m_Scene.setBuildUICanvasCallback(
        this,
        [](
        void* userData,
        const axr::PlatformType platformType,
        Clay_Context* context
    ) -> axr::UICanvasConfig {
            const auto scene = static_cast<SponzaScene*>(userData);
            return scene->uiCallback(platformType, context);
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

axr::UICanvasConfig SponzaScene::uiCallback(const axr::PlatformType platformType, Clay_Context* context) const {
    // Only handle UI for the window
    if (platformType != axr::PlatformType::Window) return {};

    Clay_SetCurrentContext(context);
    Clay_BeginLayout();

    CLAY(
        Clay_ElementDeclaration {
        .id = CLAY_ID("OuterContainer"),
        .layout = Clay_LayoutConfig {
        .sizing = Clay_Sizing {
        .width = CLAY_SIZING_FIT(100),
        .height = CLAY_SIZING_FIT(100)},
        .padding = CLAY_PADDING_ALL(16),
        .childGap = 16,
        },
        .backgroundColor = Clay_Color{0.9f,
        0.9f,
        1.0f,
        0.5f},
        }
    ) {
        CLAY(
            {
            .id = CLAY_ID("SideBar"),
            .layout = Clay_LayoutConfig {
            .sizing = Clay_Sizing {
            .width = CLAY_SIZING_FIXED(300),
            .height = CLAY_SIZING_GROW(0) },
            .padding = CLAY_PADDING_ALL(16),
            .childGap = 16,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = Clay_Color{0.2f,
            0.7f,
            0.0f,
            0.9f},
            }
        ) {
        }
        //     CLAY(
        //         { .id = CLAY_ID("ProfilePictureOuter"),
        //         .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) },
        //         .padding = CLAY_PADDING_ALL(16),
        //         .childGap = 16,
        //         .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } } }
        //     ) {
        //         CLAY(
        //             { .id = CLAY_ID("ProfilePicture"),
        //             .layout = { .sizing = { .width = CLAY_SIZING_FIXED(60),
        //             .height = CLAY_SIZING_FIXED(60) }} }
        //         ) {
        //         }
        //         CLAY_TEXT(
        //             CLAY_STRING("Clay - UI Library"),
        //             CLAY_TEXT_CONFIG({ .textColor = {255, 255, 255, 255}, .fontSize = 24 })
        //         );
        //     }
        //
        //     CLAY(
        //         { .id = CLAY_ID("MainContent"),
        //         .layout = { .sizing = { .width = CLAY_SIZING_GROW(0),
        //         .height = CLAY_SIZING_GROW(0) } } }
        //     ) {
        //     }
        // }
    }

    const Clay_RenderCommandArray renderCommands = Clay_EndLayout();

    return axr::UICanvasConfig(true, renderCommands);
}
