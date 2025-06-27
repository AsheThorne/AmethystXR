// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "xrSystem.hpp"
#include "axr/logger.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrXrSystemIsXrSessionRunning(const AxrXrSystemConst_T xrSystem) {
    if (xrSystem == nullptr) {
        axrLogErrorLocation("`xrSystem` is null.");
        return false;
    }

    return xrSystem->isXrSessionRunning();
}

AxrResult axrXrSystemStartXrSession(const AxrXrSystem_T xrSystem) {
    if (xrSystem == nullptr) {
        axrLogErrorLocation("`xrSystem` is null.");
        return AXR_ERROR;
    }

    return xrSystem->startXrSession();
}

void axrXrSystemStopXrSession(const AxrXrSystem_T xrSystem) {
    if (xrSystem == nullptr) {
        axrLogErrorLocation("`xrSystem` is null.");
        return;
    }

    xrSystem->stopXrSession();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrXrSystem::AxrXrSystem(const Config& config):
    m_ApplicationName(config.ApplicationName) {
}

AxrXrSystem::~AxrXrSystem() {
}

// ---- Public Functions ----

bool AxrXrSystem::isXrSessionRunning() const {
    return false;
}

AxrResult AxrXrSystem::startXrSession() {
    return AXR_ERROR;
}

void AxrXrSystem::stopXrSession() {
}
