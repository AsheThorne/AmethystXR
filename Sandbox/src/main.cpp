// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const auto engineSetupConfig = axr::SetupConfig(axr::LogLevelEnum::Info);
    axr::setup(engineSetupConfig);

    axr::loggerSetup("Sandbox");
    axr::logInfo("Test: {0}", 5);

    const auto appConfig = axr::ApplicationConfig(
        "Sandbox",
        AXR_MAKE_VERSION(1, 0, 0)
    );

    auto app = axr::Application(appConfig);
    app.setup();
}
