// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/lifecycle.h"
#include "application/application.h"
#include "axr/logging.h"
#include "memory/allocator.h"
#include "platform/platform.h"

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr engine. "
AxrResult axrSetup(const AxrEngineConfig* config) {
    // NOTE (Ashe): If we decide to use the AxrAllocator for the logger too then we obviously cannot set up the logger
    //  first. One issue with that though, is we can't do logging until that's set up.
    axrLoggerSetup(AxrEngineName);

    if (config == nullptr) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`config` is null.");
        return AXR_ERROR_NULLPTR;
    }

    if (AXR_FAILED(AxrAllocator::get().setup(AxrAllocator::Config{}))) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrAllocator.setup() failed.");
        return AXR_ERROR_FALLTHROUGH;
    }

    AxrPlatform::Config axrPlatformConfig{
        .WindowWidth = config->WindowConfig.Width,
        .WindowHeight = config->WindowConfig.Height,
        .WindowEnabled = config->WindowConfig.Enabled,
    };

    std::strncpy(axrPlatformConfig.WindowTitle, config->WindowConfig.Title, AXR_MAX_WINDOW_TITLE_SIZE);

    if (AXR_FAILED(AxrPlatform::get().setup(AxrPlatform::Config{axrPlatformConfig}))) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrPlatform.setup() failed.");
        return AXR_ERROR_FALLTHROUGH;
    }

    if (AXR_FAILED(AxrApplication::get().setup(AxrApplication::Config{}))) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrApplication.setup() failed.");
        return AXR_ERROR_FALLTHROUGH;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void axrShutdown() {
    AxrApplication::get().shutDown();
    AxrPlatform::get().shutDown();
    AxrAllocator::get().shutDown();
}
