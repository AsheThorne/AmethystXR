// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "application.h"
#include "axr/logging.h"

#include <cassert>

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
