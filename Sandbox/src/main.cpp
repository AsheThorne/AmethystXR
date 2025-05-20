// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
    const char* applicationName = "Sandbox";

    const auto engineSetupConfig = axr::SetupConfig(axr::LogLevelEnum::Info);
    axr::setup(engineSetupConfig);
    axr::loggerSetup(applicationName);

    const axr::WindowSystemConfig windowSystemConfig(
        800,
        600
    );

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
        &vulkanApiConfig
    );

    const auto appConfig = axr::ApplicationConfig(
        applicationName,
        AXR_MAKE_VERSION(1, 0, 0),
        windowSystemConfig,
        graphicsSystemConfig
    );

    auto app = axr::Application(appConfig);

    axr::AssetCollection globalAssetCollection = app.getGlobalAssetCollection();

    axr::VertexShaderProperties vertexShaderProperties;
    vertexShaderProperties.addVertexAttribute(axr::ShaderVertexAttributeEnum::Position, 0, 0);
    vertexShaderProperties.addVertexAttribute(axr::ShaderVertexAttributeEnum::Color, 0, 1);
    vertexShaderProperties.addUniformBufferLayout(0, axr::engineAssetGetUniformBufferSize(axr::EngineAssetEnum::UniformBufferSceneData));
    vertexShaderProperties.addPushConstantBufferLayout(axr::engineAssetGetPushConstantBufferSize(axr::EngineAssetEnum::PushConstantBufferModelMatrix));
    const axr::ShaderConfig vertexShaderConfig("VertexShader", "shaders/shader.vert", vertexShaderProperties);
    if (!vertexShaderConfig.isValid()) return 0;
    if (AXR_FAILED(globalAssetCollection.createShader(vertexShaderConfig))) return 0;

    const axr::FragmentShaderProperties fragmentShaderProperties;
    const axr::ShaderConfig fragmentShaderConfig("FragmentShader", "shaders/shader.frag", fragmentShaderProperties);
    if (!fragmentShaderConfig.isValid()) return 0;
    if (AXR_FAILED(globalAssetCollection.createShader(fragmentShaderConfig))) return 0;

    const auto defaultModelMatrixBuffer = axr::EngineAssetPushConstantBuffer_ModelMatrix(glm::mat4(1.0f));

    const axr::PushConstantBufferConfig defaultModelMatrixBufferConfig(
        "DefaultModelMatrix",
        axr::engineAssetGetPushConstantBufferSize(axr::EngineAssetEnum::PushConstantBufferModelMatrix),
        &defaultModelMatrixBuffer
    );
    if (AXR_FAILED(globalAssetCollection.createPushConstantBuffer(defaultModelMatrixBufferConfig))) return 0;

    axr::ShaderValues vertexShaderValues;
    vertexShaderValues.addUniformBufferLink(
        0,
        axr::engineAssetGetName(axr::EngineAssetEnum::UniformBufferSceneData)
    );
    const axr::ShaderValues fragmentShaderValues;
    const char* materialName = "MyMaterial";
    const axr::MaterialConfig materialConfig(
        materialName,
        vertexShaderConfig.Name,
        fragmentShaderConfig.Name,
        defaultModelMatrixBufferConfig.Name,
        vertexShaderValues,
        fragmentShaderValues
    );
    if (!materialConfig.isValid()) return 0;
    if (AXR_FAILED(globalAssetCollection.createMaterial(materialConfig))) return 0;

    if (AXR_FAILED(globalAssetCollection.createModel("Triangle", axr::EngineAssetEnum::ModelTriangle))) return 0;

    const char* scene1Name = "Scene1";
    if (AXR_FAILED(app.createScene(scene1Name))) return 0;
    const axr::Scene scene1 = app.findScene(scene1Name);
    const axr::Entity_T triangleEntity = scene1.createEntity();

    triangleEntity.emplace<AxrTransformComponent>(
        AxrTransformComponent{
            .Position = glm::vec3(0.0f, 0.0f, 0.0f),
            .Scale = glm::vec3(1.0f, 1.0f, 1.0f),
            .Orientation = glm::quat(1.f, 0.f, 0.f, 0.f),
        }
    );
    AxrModelComponent::Mesh mesh{
        .MaterialName = materialName,
    };
    triangleEntity.emplace<AxrModelComponent>(
        AxrModelComponent{
            .ModelName = "Triangle",
            .MeshCount = 1,
            .Meshes = &mesh,
            .PushConstantBufferName = axr::engineAssetGetName(
                axr::EngineAssetEnum::PushConstantBufferModelMatrix
            ),
        }
    );

    if (AXR_FAILED(app.setup())) return 0;

    if (AXR_FAILED(app.loadScene(scene1Name))) return 0;
    if (AXR_FAILED(app.setActiveScene(scene1Name))) return 0;

    axr::WindowSystem windowSystem = app.getWindowSystem();
    if (AXR_FAILED(windowSystem.openWindow())) return 0;

    const axr::GraphicsSystem graphicsSystem = app.getGraphicsSystem();

    while (app.isRunning()) {
        app.processEvents();

        graphicsSystem.drawFrame();
    }

    return 0;
}
