// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>

// ----------------------------------------- //
// Standard C++ Library Headers
// ----------------------------------------- //
#include <iostream>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include <axr.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const auto appConfig = axr::ApplicationConfig(
        "Sandbox",
        AXR_MAKE_VERSION(1, 0, 0)
    );
    
    auto app = axr::Application(appConfig);
    app.setup();
}
