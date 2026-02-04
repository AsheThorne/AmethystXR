// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/lifecycle.h"
#include "application/application.h"
#include "axr/logging.h"
#include "memory/allocator.h"
#include "platform/platform.h"
#include "renderer/renderer.h"
#include "server/server.h"

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr engine. "
AxrResult axrSetup(const AxrEngineConfig* config) {
    axrLoggerSetup(AxrEngineName);

    if (config == nullptr) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`config` is null.");
        return AXR_ERROR_NULLPTR;
    }

    if (AXR_FAILED(AxrAllocator::get().setup(AxrAllocator::Config{}))) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrAllocator.setup() failed.");
        return AXR_ERROR_FALLTHROUGH;
    }

    if (AXR_FAILED(AxrServer::get().setup(AxrServer::Config{}))) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrServer.setup() failed.");
        return AXR_ERROR_FALLTHROUGH;
    }

    const AxrPlatform::Config axrPlatformConfig{
        .WindowConfig = &config->WindowConfig,
        .RendererApiType = config->RendererConfig.ApiType,
    };

    if (AXR_FAILED(AxrPlatform::get().setup(axrPlatformConfig))) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrPlatform.setup() failed.");
        return AXR_ERROR_FALLTHROUGH;
    }

    AxrRenderer::Config axrRendererConfig{
        .RendererConfig = &config->RendererConfig,
        .ApplicationVersion = config->ApplicationConfig.ApplicationVersion,
        .ApplicationName = "",
    };

    std::strncpy(axrRendererConfig.ApplicationName,
                 config->ApplicationConfig.ApplicationName,
                 AXR_MAX_APPLICATION_NAME_SIZE);

    if (AXR_FAILED(AxrRenderer::get().setup(axrRendererConfig))) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrRenderer.setup() failed.");
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
    AxrRenderer::get().shutDown();
    AxrPlatform::get().shutDown();
    AxrServer::get().shutDown();
    AxrAllocator::get().shutDown();
}
