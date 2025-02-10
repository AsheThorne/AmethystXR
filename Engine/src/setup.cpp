// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/setup.h"
#include "common.hpp"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

void axrSetup(const AxrSetupConfig* config) {
    axrLoggerSetup(AxrEngineName);
    axrSetLoggerLevel(config->LogLevel);
}
