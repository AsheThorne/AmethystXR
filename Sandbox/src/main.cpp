// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
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
        axr::GraphicsApiEnum::Vulkan,
        &vulkanApiConfig
    );

    const auto appConfig = axr::ApplicationConfig(
        applicationName,
        AXR_MAKE_VERSION(1, 0, 0),
        windowSystemConfig,
        graphicsSystemConfig
    );

    auto app = axr::Application(appConfig);
    if (AXR_FAILED(app.setup())) return 0;

    axr::AssetCollection globalAssetCollection = app.getGlobalAssetCollection();

    axr::VertexShaderProperties vertexShaderProperties;
    const axr::ShaderConfig shader1Config("VertexShader", "shaders/shader.vert", vertexShaderProperties);
    if (!shader1Config.isValid()) return 0;
    if (AXR_FAILED(globalAssetCollection.createShader(shader1Config))) return 0;

    axr::FragmentShaderProperties fragmentShaderProperties;
    const axr::ShaderConfig shader2Config("FragmentShader", "shaders/shader.frag", fragmentShaderProperties);
    if (!shader2Config.isValid()) return 0;
    if (AXR_FAILED(globalAssetCollection.createShader(shader2Config))) return 0;

    axr::WindowSystem windowSystem = app.getWindowSystem();
    if (AXR_FAILED(windowSystem.openWindow())) return 0;

    while (app.isRunning()) {
        app.processEvents();
    }

    return 0;
}
