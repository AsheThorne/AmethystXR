// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "inputSystem.hpp"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrInputSystem::AxrInputSystem() {
}

AxrInputSystem::~AxrInputSystem() {
    resetSetup();
}

AxrResult AxrInputSystem::setup() {
    return AXR_SUCCESS;
}

void AxrInputSystem::resetSetup() {
}

// ---- Public Functions ----
