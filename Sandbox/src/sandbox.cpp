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
        .WindowConfig =
            AxrWindowConfig{
                .Width = 800,
                .Height = 600,
                .Title = "Sandbox",
                .Enabled = true,
            },
    };
    axrSetup(&axrEngineConfig);

    axrLoggerSetup("Application");

    while (axrApplicationIsRunning()) {
        if (!axrApplicationProcessEvents()) {
            break;
        }
    }

    axrShutdown();
    return 0;
}
