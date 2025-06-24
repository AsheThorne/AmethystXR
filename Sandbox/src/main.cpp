// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr.hpp>
#include "sponzaScene.hpp"

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

    axr::AssetCollection globalAssetCollection = app.getGlobalAssetCollection();
    if (AXR_FAILED(globalAssetCollection.createShader(axr::EngineAssetEnum::ShaderDefaultFrag))) return -1;
    if (AXR_FAILED(globalAssetCollection.createShader(axr::EngineAssetEnum::ShaderDefaultVert))) return -1;

    if (AXR_FAILED(app.setup())) return -1;

    SponzaScene sponzaScene(app);
    if (AXR_FAILED(sponzaScene.setup())) return -1;

    if (AXR_FAILED(sponzaScene.loadScene())) return -1;
    if (AXR_FAILED(sponzaScene.setAsActiveScene())) return -1;

    axr::WindowSystem windowSystem = app.getWindowSystem();
    if (AXR_FAILED(windowSystem.openWindow())) return -1;

    const axr::GraphicsSystem graphicsSystem = app.getGraphicsSystem();

    while (app.isRunning()) {
        app.processEvents();

        sponzaScene.update();

        graphicsSystem.drawFrame();
    }

    return 0;
}
