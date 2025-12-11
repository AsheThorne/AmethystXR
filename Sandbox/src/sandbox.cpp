// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "sandbox.h"
#include "axr/axr.h"

// ----------------------------------------- //
// Sandbox
// ----------------------------------------- //

int start() {
    const AxrEngineConfig axrEngineConfig{};
    axrSetup(&axrEngineConfig);

    // (Ashe): The logger might use the AxrAllocator in the future so we should wait until the engine start up is
    // successful before creating the application logger.
    axrLoggerSetup("Application");

    axrShutdown();
    return 0;
}
