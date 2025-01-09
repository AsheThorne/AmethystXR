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
        axr::WindowPlatformEnum::Win32,
        800,
        600
    );

    const auto appConfig = axr::ApplicationConfig(
        applicationName,
        AXR_MAKE_VERSION(1, 0, 0),
        windowSystemConfig
    );

    auto app = axr::Application(appConfig);
    if (AXR_FAILED(app.setup())) return 0;

    axr::WindowSystem windowSystem = app.getWindowSystem();
    if (AXR_FAILED(windowSystem.openWindow())) return 0;

    while (app.isRunning()) {
        app.processEvents();
    }
}
