// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/setup.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

void axrSetup(const AxrSetupConfig* config) {
    axrLoggerSetup(AxrEngineName);
    axrSetLoggerLevel(config->LogLevel);
}
