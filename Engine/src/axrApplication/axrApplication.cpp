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

    AxrWindowSystem_T axrApplicationGetWindowSystem(const AxrApplication_T app) {
        if (app == nullptr) {
            axrLogError("axrApplicationGetWindowSystem - `app` is null.");
            return nullptr;
        }

        return app->getWindowSystem();
    }
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrApplication::AxrApplication(const AxrApplicationConfig& config) :
    m_Config(config),
    m_WindowSystem(
        {
            .ApplicationName = config.ApplicationName,
            .Platform = config.WindowSystemConfig.Platform,
            .Width = config.WindowSystemConfig.Width,
            .Height = config.WindowSystemConfig.Height,
        }
    ) {
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrResult AxrApplication::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = m_WindowSystem.setup();
    if (AXR_FAILED(axrResult)) return axrResult;

    return axrResult;
}

AxrWindowSystem_T AxrApplication::getWindowSystem() {
    return &m_WindowSystem;
}
