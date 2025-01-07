// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/axrApplication.h"
#include "axrApplication.hpp"

#include <iostream>

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //
extern "C" {
    AxrApplication_T axrCreateApplication() {
        return new AxrApplication();
    }

    void axrDestroyApplication(AxrApplication_T* app) {
        if (app == nullptr || *app == nullptr) return;

        delete *app;
        *app = nullptr;
    }

    void axrApplicationSetup(const AxrApplication_T app) {
        if (app == nullptr) return;
        
        app->setup();
    }
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

AxrApplication::AxrApplication() {
    std::cout << "AxrApplication::AxrApplication()" << '\n';
}

AxrApplication::~AxrApplication() {
    std::cout << "AxrApplication::~AxrApplication()" << '\n';
}

void AxrApplication::setup() {
    std::cout << "AxrApplication::setup()" << '\n';
}
