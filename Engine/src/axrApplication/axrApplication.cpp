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

    AxrResult axrApplicationSetup(const AxrApplication_T app) {
        if (app == nullptr) {
            axrLogError("axrApplicationSetup - `app` is null.");
            return AXR_ERROR;
        }

        return app->setup();
    }
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrApplication::AxrApplication(const AxrApplicationConfig& config) :
    m_Config(config) {
    axrLogInfo("AxrApplication::AxrApplication()");
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrResult AxrApplication::setup() {
    AxrResult axrResult = AXR_SUCCESS;
    
    return axrResult;
}
