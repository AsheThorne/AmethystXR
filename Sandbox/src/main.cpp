// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
    const std::string applicationName = "Sandbox";

    const auto engineSetupConfig = axr::SetupConfig(axr::LogLevelEnum::Info);
    axr::setup(engineSetupConfig);
    axr::loggerSetup(applicationName);

    axr::VulkanWindowConfig vulkanWindowConfig(axr::VulkanPresentationModeEnum::Mailbox);

    axr::VulkanApiConfig vulkanApiConfig(&vulkanWindowConfig);

#ifndef NDEBUG
    vulkanApiConfig.addApiLayerCoreValidation();
    vulkanApiConfig.addExtensionDebugUtils(
        // axr::VulkanDebugUtilsSeverityFlagBits::Verbose |
        // axr::VulkanDebugUtilsSeverityFlagBits::Info |
        axr::VulkanDebugUtilsSeverityFlagBits::Warning |
        axr::VulkanDebugUtilsSeverityFlagBits::Error,
        axr::VulkanDebugUtilsTypeFlagBits::General |
        axr::VulkanDebugUtilsTypeFlagBits::Validation |
        axr::VulkanDebugUtilsTypeFlagBits::Performance |
        axr::VulkanDebugUtilsTypeFlagBits::DeviceAddressBinding
    );
#endif

    const axr::GraphicsSystemConfig graphicsSystemConfig(
        &vulkanApiConfig,
        axr::SamplerAnisotropyQualityEnum::High
    );

    const axr::WindowSystemConfig windowSystemConfig(
        800,
        600
    );

    const auto appConfig = axr::ApplicationConfig(
        applicationName.c_str(),
        AXR_MAKE_VERSION(1, 0, 0),
        windowSystemConfig,
        graphicsSystemConfig
    );

    auto app = axr::Application(appConfig);

    const std::string imageName = "UvTesterImage";
    const std::string imageSamplerName = "ImageSampler";
    const std::string materialName = "MyMaterial";
    const std::string modelName = "Cube";

    const axr::ImageSamplerConfig imageSamplerConfig(
        imageSamplerName.c_str(),
        axr::ImageSamplerFilterEnum::Nearest,
        axr::ImageSamplerFilterEnum::Nearest,
        axr::ImageSamplerFilterEnum::Nearest,
        axr::ImageSamplerWrappingEnum::Repeat,
        axr::ImageSamplerWrappingEnum::Repeat
    );

    axr::AssetCollection globalAssetCollection = app.getGlobalAssetCollection();
    if (AXR_FAILED(
        globalAssetCollection.createImage(imageName.c_str(), axr::EngineAssetEnum::ImageUvTester)
    ))
        return -1;
    if (AXR_FAILED(globalAssetCollection.createImageSampler(imageSamplerConfig))) return -1;
    if (AXR_FAILED(globalAssetCollection.createShader(axr::EngineAssetEnum::ShaderDefaultFrag))) return -1;
    if (AXR_FAILED(globalAssetCollection.createShader(axr::EngineAssetEnum::ShaderDefaultVert))) return -1;
    if (AXR_FAILED(
        globalAssetCollection.createMaterial(
            materialName.c_str(),
            axr::EngineAssetMaterial_DefaultMaterial(imageName.c_str(), imageSamplerName.c_str())
        )
    )) {
        return -1;
    }

    if (AXR_FAILED(
        globalAssetCollection.createModel(modelName.c_str(), axr::EngineAssetEnum::ModelCube)
    ))
        return -1;

    const std::string scene1Name = "Scene1";
    if (AXR_FAILED(app.createScene(scene1Name.c_str()))) return -1;
    const axr::Scene scene1 = app.findScene(scene1Name.c_str());
    const axr::Entity_T entity = scene1.createEntity();

    entity.emplace<AxrTransformComponent>(
        AxrTransformComponent{
            .Position = glm::vec3(0.0f, 0.0f, 0.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
            .Orientation = glm::quat(1.f, 0.f, 0.f, 0.f),
        }
    );
    AxrModelComponent::Mesh::Submesh submesh{
        .MaterialName = materialName.c_str(),
    };
    AxrModelComponent::Mesh mesh{
        .SubmeshCount = 1,
        .Submeshes = &submesh,
    };
    entity.emplace<AxrModelComponent>(
        AxrModelComponent{
            .ModelName = modelName.c_str(),
            .MeshCount = 1,
            .Meshes = &mesh,
            .PushConstantBufferName = axr::engineAssetGetName(
                axr::EngineAssetEnum::PushConstantBufferModelMatrix
            ),
        }
    );

    if (AXR_FAILED(app.setup())) return -1;

    if (AXR_FAILED(app.loadScene(scene1Name.c_str()))) return -1;
    if (AXR_FAILED(app.setActiveScene(scene1Name.c_str()))) return -1;

    axr::WindowSystem windowSystem = app.getWindowSystem();
    if (AXR_FAILED(windowSystem.openWindow())) return -1;

    const axr::GraphicsSystem graphicsSystem = app.getGraphicsSystem();

    while (app.isRunning()) {
        app.processEvents();

        entity.patch<AxrTransformComponent>(
            [](AxrTransformComponent& transform) {
                //TODO: Get and use delta time
                transform.Orientation = glm::rotate(transform.Orientation, 0.0001f, glm::vec3(0.0f, 1.0f, 0.0f));
            }
        );

        graphicsSystem.drawFrame();
    }

    return 0;
}
