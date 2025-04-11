// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd) {
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

    const axr::VertexShaderProperties vertexShaderProperties;
    const axr::ShaderConfig vertexShaderConfig("VertexShader", "shaders/shader.vert", vertexShaderProperties);
    if (!vertexShaderConfig.isValid()) return 0;
    if (AXR_FAILED(globalAssetCollection.createShader(vertexShaderConfig))) return 0;

    const axr::FragmentShaderProperties fragmentShaderProperties;
    const axr::ShaderConfig fragmentShaderConfig("FragmentShader", "shaders/shader.frag", fragmentShaderProperties);
    if (!fragmentShaderConfig.isValid()) return 0;
    if (AXR_FAILED(globalAssetCollection.createShader(fragmentShaderConfig))) return 0;

    const axr::ShaderValues vertexShaderValues;
    const axr::ShaderValues fragmentShaderValues;
    const char* materialName = "MyMaterial";
    const axr::MaterialConfig materialConfig(
        materialName,
        vertexShaderConfig.Name,
        fragmentShaderConfig.Name,
        vertexShaderValues,
        fragmentShaderValues
    );
    if (!materialConfig.isValid()) return 0;
    if (AXR_FAILED(globalAssetCollection.createMaterial(materialConfig))) return 0;

    if (AXR_FAILED(globalAssetCollection.createModel("Triangle", axr::ModelEngineAssetEnum::Triangle))) return 0;

    const char* scene1Name = "Scene1";
    if (AXR_FAILED(app.createScene(scene1Name))) return 0;
    const axr::Scene scene1 = app.findScene(scene1Name);
    entt::registry* scene1Registry = scene1.getEcsRegistry();
    const entt::entity triangleEntity = scene1Registry->create();
    scene1Registry->emplace<AxrModelComponent>(
        triangleEntity,
        AxrModelComponent{
            .ModelName = "Triangle",
            .MaterialNamesCount = 1,
            .MaterialNames = &materialName,
        }
    );

    if (AXR_FAILED(app.setup())) return 0;

    axr::WindowSystem windowSystem = app.getWindowSystem();
    if (AXR_FAILED(windowSystem.openWindow())) return 0;

    while (app.isRunning()) {
        app.processEvents();
    }

    return 0;
}
