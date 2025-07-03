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
        // axr::VulkanDebugUtilsSeverityFlagBits::VerboseBit |
        // axr::VulkanDebugUtilsSeverityFlagBits::InfoBit |
        axr::VulkanDebugUtilsSeverityFlagBits::WarningBit |
        axr::VulkanDebugUtilsSeverityFlagBits::ErrorBit,
        axr::VulkanDebugUtilsTypeFlagBits::GeneralBit |
        axr::VulkanDebugUtilsTypeFlagBits::ValidationBit |
        axr::VulkanDebugUtilsTypeFlagBits::PerformanceBit |
        axr::VulkanDebugUtilsTypeFlagBits::DeviceAddressBindingBit
    );
#endif

    const axr::GraphicsWindowConfig windowGraphicsConfig(axr::MsaaSampleCountEnum::SampleCount64);
    const axr::GraphicsXrSessionConfig xrSessionGraphicsConfig(axr::MsaaSampleCountEnum::SampleCount1);

    const axr::GraphicsSystemConfig graphicsSystemConfig(
        &vulkanApiConfig,
        &windowGraphicsConfig,
        &xrSessionGraphicsConfig,
        axr::SamplerAnisotropyQualityEnum::High
    );

    const axr::WindowSystemConfig windowSystemConfig(
        800,
        600
    );

    axr::XrSystemConfig xrSystemConfig(axr::XrReferenceSpaceEnum::Stage);

#ifndef NDEBUG
    xrSystemConfig.addApiLayerCoreValidation();
    xrSystemConfig.addExtensionDebugUtils(
        // axr::XrDebugUtilsSeverityFlagBits::VerboseBit |
        // axr::XrDebugUtilsSeverityFlagBits::InfoBit |
        axr::XrDebugUtilsSeverityFlagBits::WarningBit |
        axr::XrDebugUtilsSeverityFlagBits::ErrorBit,
        axr::XrDebugUtilsTypeFlagBits::GeneralBit |
        axr::XrDebugUtilsTypeFlagBits::ValidationBit |
        axr::XrDebugUtilsTypeFlagBits::PerformanceBit |
        axr::XrDebugUtilsTypeFlagBits::ConformanceBit
    );
#endif

    const auto appConfig = axr::ApplicationConfig(
        applicationName.c_str(),
        AXR_MAKE_VERSION(1, 0, 0),
        graphicsSystemConfig,
        &windowSystemConfig,
        &xrSystemConfig
    );

    auto app = axr::Application(appConfig);
    if (AXR_FAILED(app.setup())) return -1;

    axr::AssetCollection globalAssetCollection = app.getGlobalAssetCollection();
    if (AXR_FAILED(globalAssetCollection.createShader(axr::EngineAssetEnum::ShaderDefaultFrag))) return -1;
    if (AXR_FAILED(globalAssetCollection.createShader(axr::EngineAssetEnum::ShaderDefaultVert))) return -1;

    SponzaScene scene(app);
    if (AXR_FAILED(scene.setup())) return -1;

    if (AXR_FAILED(scene.loadScene())) return -1;
    if (AXR_FAILED(scene.setAsActiveScene())) return -1;

    axr::WindowSystem windowSystem = app.getWindowSystem();
    if (windowSystem.isValid()) {
        if (AXR_FAILED(windowSystem.openWindow())) return -1;
    }

    axr::XrSystem xrSystem = app.getXrSystem();
    if (xrSystem.isValid()) {
        if (AXR_FAILED(xrSystem.startXrSession())) return -1;
    }

    axr::GraphicsSystem graphicsSystem = app.getGraphicsSystem();
    graphicsSystem.setClearColor(glm::vec4(0.2f, 0.05f, 0.2f, 1.0f));

    while (app.isRunning()) {
        app.processEvents();

        if (!windowSystem.isWindowOpen()) {
            xrSystem.stopXrSession();
            app.processEvents();
            if (!app.isRunning()) {
                break;
            }
        }

        scene.update();

        graphicsSystem.drawFrame();
    }

    return 0;
}
