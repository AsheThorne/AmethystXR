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
#include "axr/test.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    std::cout << "Hello World!\n";
    std::cout << axr::test();
}
