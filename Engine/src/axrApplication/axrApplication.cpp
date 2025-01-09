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
    axrLogInfo("AxrApplication::AxrApplication()");
}

AxrApplication::~AxrApplication() {
    axrLogInfo("AxrApplication::~AxrApplication()");
}

void AxrApplication::setup() {
    axrLogInfo("AxrApplication::setup()");
    axrLogInfo("Name: {0}", m_Config.ApplicationName);
    axrLogInfo(
        "Version: {0}.{1}.{2}",
        AXR_VERSION_MAJOR(m_Config.ApplicationVersion),
        AXR_VERSION_MINOR(m_Config.ApplicationVersion),
        AXR_VERSION_PATCH(m_Config.ApplicationVersion)
    );
}
