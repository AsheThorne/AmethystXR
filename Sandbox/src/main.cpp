// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/axr.h"

#ifdef AXR_PLATFORM_WIN32
#    include <windows.h>
#endif

// ----------------------------------------- //
// Entry Point
// ----------------------------------------- //
int start();

#if defined(AXR_PLATFORM_WIN32)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nShowCmd) {
    return start();
}
#elif defined(AXR_PLATFORM_LINUX)
int main() {
    return start();
}
#endif

// ----------------------------------------- //
// Sandbox
// ----------------------------------------- //

int start() {
    axrTest();
    return 0;
}