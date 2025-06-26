// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr.hpp>
#include "sponzaScene.hpp"
#include "testScene.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
    const std::string applicationName = "Sandbox";

    const auto engineSetupConfig = axr::SetupConfig(axr::LogLevelEnum::Info);
    axr::setup(engineSetupConfig);
    axr::loggerSetup(applicationName);

    const axr::VulkanWindowConfig vulkanWindowConfig(axr::VulkanPresentationModeEnum::Mailbox);

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

    const axr::GraphicsWindowConfig windowGraphicsConfig(axr::MsaaSampleCountEnum::SampleCount1);

    const axr::GraphicsSystemConfig graphicsSystemConfig(
        &vulkanApiConfig,
        &windowGraphicsConfig,
        glm::vec4(0.2f, 0.05f, 0.2f, 1.0f),
        axr::SamplerAnisotropyQualityEnum::High
    );

    // TODO: make window config optional. if window config is null, there's no window
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
    if (AXR_FAILED(app.setup())) return -1;

    axr::AssetCollection globalAssetCollection = app.getGlobalAssetCollection();
    if (AXR_FAILED(globalAssetCollection.createShader(axr::EngineAssetEnum::ShaderDefaultFrag))) return -1;
    if (AXR_FAILED(globalAssetCollection.createShader(axr::EngineAssetEnum::ShaderDefaultVert))) return -1;

    TestScene scene(app);
    if (AXR_FAILED(scene.setup())) return -1;

    if (AXR_FAILED(scene.loadScene())) return -1;
    if (AXR_FAILED(scene.setAsActiveScene())) return -1;

    axr::WindowSystem windowSystem = app.getWindowSystem();
    if (AXR_FAILED(windowSystem.openWindow())) return -1;

    const axr::GraphicsSystem graphicsSystem = app.getGraphicsSystem();

    while (app.isRunning()) {
        app.processEvents();

        scene.update();

        graphicsSystem.drawFrame();
    }

    return 0;
}
