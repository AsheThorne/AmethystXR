// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/lifecycle.h"
#include "application/application.h"
#include "assets/assets.h"
#include "axr/logging.h"
#include "debugInfo/debugInfo.h"
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

    constexpr AxrAllocator::Config allocatorConfig{
        /// 128 Kibibytes
        .FrameAllocatorSize = 131'072,
        .MaxHandleCount = 10'000,
        /// 1 Mebibyte
        .EngineDataAllocatorMainMemorySize = 1'048'576,
        .MaxDebugHandleCount = 100,
        /// 2 Mebibytes
        .DebugInfoAllocatorMainMemorySize = 2'097'152,
    };

    axrResult = AxrAllocator::get().setup(allocatorConfig);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrShutdown();
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrAllocator.setup() failed.");
        return axrResult;
    }

#ifdef AXR_DEBUG_INFO_ENABLED
    constexpr AxrDebugInfo::Config debugInfoConfig{
        .MaxIDCount = 16'384,
    };

    axrResult = AxrDebugInfo::get().setup(debugInfoConfig);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrShutdown();
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrDebugInfo.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logAllAllocatorUsage("Debug Info Setup");
#endif

    axrResult = AxrServer::get().setup(AxrServer::Config{});
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrShutdown();
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrServer.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logAllAllocatorUsage("Server Setup");

    const AxrPlatform::Config platformConfig{
        .WindowConfig = &config->WindowConfig,
        .RendererApiType = config->RendererConfig.ApiType,
    };

    axrResult = AxrPlatform::get().setup(platformConfig);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrShutdown();
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrPlatform.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logAllAllocatorUsage("Platform Setup");

    axrResult = AxrAssets::get().setup(AxrAssets::Config{});
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrShutdown();
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrAssets.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logAllAllocatorUsage("Assets Setup");

    AxrRenderer::Config rendererConfig{
        .RendererConfig = &config->RendererConfig,
        .ApplicationVersion = config->ApplicationConfig.ApplicationVersion,
        .ApplicationName = "",
    };

    std::strncpy(rendererConfig.ApplicationName,
                 config->ApplicationConfig.ApplicationName,
                 AXR_MAX_APPLICATION_NAME_SIZE);

    axrResult = AxrRenderer::get().setup(rendererConfig);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrShutdown();
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrRenderer.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logAllAllocatorUsage("Renderer Setup");

    axrResult = AxrApplication::get().setup(AxrApplication::Config{});
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrShutdown();
        axrLogError(AXR_FUNCTION_FAILED_STRING "AxrApplication.setup() failed.");
        return axrResult;
    }

    AxrAllocator::get().logAllAllocatorUsage("Application Setup");

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void axrShutdown() {
    AxrApplication::get().shutDown();
    AxrRenderer::get().shutDown();
    AxrAssets::get().shutDown();
    AxrPlatform::get().shutDown();
    AxrServer::get().shutDown();
#ifdef AXR_DEBUG_INFO_ENABLED
    AxrDebugInfo::get().shutDown();
#endif
    AxrAllocator::get().shutDown();
}
