// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/lifecycle.h"
#include "application/application.h"
#include "assets/assets.h"
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
    assert(config != nullptr);

    AxrResult axrResult = AXR_SUCCESS;

    constexpr AxrAllocator::Config axrAllocatorConfig{
        /// 512 Kibibytes (Half a Mebibyte)
        .FrameAllocatorSize = 524'288,
        .MaxHandleCount = 10'000,
        /// 1 Mebibyte
        .EngineDataAllocatorMainMemorySize = 1'048'576,
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
    AxrAllocator::get().logHandlesAllocatorUsage("Server Setup");
    AxrAllocator::get().logEngineDataAllocatorUsage("Server Setup");

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
    AxrAllocator::get().logHandlesAllocatorUsage("Platform Setup");
    AxrAllocator::get().logEngineDataAllocatorUsage("Platform Setup");

    axrResult = AxrAssets::get().setup(AxrAssets::Config{});
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrAssets.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logFrameAllocatorUsage("Assets Setup");
    AxrAllocator::get().logHandlesAllocatorUsage("Assets Setup");
    AxrAllocator::get().logEngineDataAllocatorUsage("Assets Setup");

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
    AxrAllocator::get().logHandlesAllocatorUsage("Renderer Setup");
    AxrAllocator::get().logEngineDataAllocatorUsage("Renderer Setup");

    axrResult = AxrApplication::get().setup(AxrApplication::Config{});
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrApplication.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logFrameAllocatorUsage("Application Setup");
    AxrAllocator::get().logHandlesAllocatorUsage("Application Setup");
    AxrAllocator::get().logEngineDataAllocatorUsage("Application Setup");

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void axrShutdown() {
    AxrApplication::get().shutDown();
    AxrRenderer::get().shutDown();
    AxrAssets::get().shutDown();
    AxrPlatform::get().shutDown();
    AxrServer::get().shutDown();
    AxrAllocator::get().shutDown();
}
