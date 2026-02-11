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

    AxrResult axrResult = AXR_SUCCESS;

    constexpr AxrAllocator::Config axrAllocatorConfig{
        /// 1 Mebibyte
        .FrameAllocatorSize = 1'048'576,
    };

    axrResult = AxrAllocator::get().setup(axrAllocatorConfig);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrAllocator.setup() failed.");
        return axrResult;
    }

    axrResult = AxrServer::get().setup(AxrServer::Config{});
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrServer.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logFrameAllocatorUsage("Server Setup");

    const AxrPlatform::Config axrPlatformConfig{
        .WindowConfig = &config->WindowConfig,
        .RendererApiType = config->RendererConfig.ApiType,
    };

    axrResult = AxrPlatform::get().setup(axrPlatformConfig);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrPlatform.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logFrameAllocatorUsage("Platform Setup");

    AxrRenderer::Config axrRendererConfig{
        .RendererConfig = &config->RendererConfig,
        .ApplicationVersion = config->ApplicationConfig.ApplicationVersion,
        .ApplicationName = "",
    };

    std::strncpy(axrRendererConfig.ApplicationName,
                 config->ApplicationConfig.ApplicationName,
                 AXR_MAX_APPLICATION_NAME_SIZE);

    axrResult = AxrRenderer::get().setup(axrRendererConfig);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrRenderer.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logFrameAllocatorUsage("Renderer Setup");

    axrResult = AxrApplication::get().setup(AxrApplication::Config{});
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrApplication.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logFrameAllocatorUsage("Application Setup");

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
