// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/setup.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

extern "C" {
    void axrSetup(const AxrSetupConfig* config) {
        axrLoggerSetup(AxrEngineName);
        axrSetLoggerLevel(config->LogLevel);
    }
}
