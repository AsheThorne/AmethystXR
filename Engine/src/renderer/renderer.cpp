// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "renderer.h"
#include "axr/logging.h"

#include <cassert>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrRenderer::AxrRenderer() = default;

AxrRenderer::~AxrRenderer() {
    shutDown();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrRenderer& AxrRenderer::get() {
    static AxrRenderer singleton;
    return singleton;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr renderer. "
AxrResult AxrRenderer::setup(const Config& config) {
    assert(!m_IsSetup);

    m_IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrRenderer::shutDown() {
    m_IsSetup = false;
}
