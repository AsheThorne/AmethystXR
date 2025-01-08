// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/axrApplication.h"
#include "axrApplication.hpp"
#include "axr/logger.h"

#include <iostream>

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //
extern "C" {
    AxrApplication_T axrCreateApplication(const AxrApplicationConfig* config) {
        return new AxrApplication(*config);
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

AxrApplication::AxrApplication(const AxrApplicationConfig& config) :
    m_Config(config) {
    std::cout << "AxrApplication::AxrApplication()" << '\n';
}

AxrApplication::~AxrApplication() {
    std::cout << "AxrApplication::~AxrApplication()" << '\n';
}

void AxrApplication::setup() {
    axrLogInfo("Test 2");
    std::cout << "AxrApplication::setup()" << '\n';
    std::cout << "Name: " << m_Config.ApplicationName << '\n';
    std::cout << "Version: "
        << AXR_VERSION_MAJOR(m_Config.ApplicationVersion)
        << "."
        << AXR_VERSION_MINOR(m_Config.ApplicationVersion)
        << "."
        << AXR_VERSION_PATCH(m_Config.ApplicationVersion)
        << '\n';
}
