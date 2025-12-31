// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "application.h"
#include "axr/logging.h"

#include <cassert>

#include "../platform/platform.h"

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

bool axrApplicationIsRunning() {
    return AxrApplication::get().isRunning();
}

bool axrApplicationProcessEvents() {
    return AxrApplication::get().processEvents();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrApplication& AxrApplication::get() {
    static AxrApplication singleton;
    return singleton;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr application. "
AxrResult AxrApplication::setup(const Config& config) {
    assert(!m_IsSetup);

    m_IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrApplication::shutDown() {
    m_IsSetup = false;
}
bool AxrApplication::isRunning() const {
    return AxrPlatform::get().isWindowOpen();
}

bool AxrApplication::processEvents() const {
    // TODO (Ashe): When we integrate OpenXR, only return false if both OpenXR session and window is closed.
    //  If the user wants to exit if only one exists, then they can manually do that
    return AxrPlatform::get().processEvents();
}
