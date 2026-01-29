// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "sandbox.h"
#include "axr/axr.h"

// ----------------------------------------- //
// Sandbox
// ----------------------------------------- //

int start() {
    const AxrEngineConfig axrEngineConfig{
        .ApplicationConfig =
            AxrApplicationConfig{
                .ApplicationVersion = AXR_MAKE_VERSION(0, 1, 0),
                .ApplicationName = "Sandbox",
            },
        .WindowConfig =
            AxrWindowConfig{
                .Width = 800,
                .Height = 600,
                .Title = "Sandbox",
                .Enabled = true,
            },
        .RendererConfig =
            AxrRendererConfig{
                .ApiType = AXR_RENDERER_API_TYPE_VULKAN,
            },
    };
    axrSetup(&axrEngineConfig);

    axrLoggerSetup("Sandbox");

    while (axrApplicationIsRunning()) {
        if (!axrApplicationProcessEvents()) {
            break;
        }
    }

    axrShutdown();
    return 0;
}
