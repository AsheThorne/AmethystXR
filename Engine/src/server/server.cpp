// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "server.h"
#include "axr/logging.h"

#include <cassert>

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrServer::AxrServer() = default;

AxrServer::~AxrServer() {
    shutDown();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrServer& AxrServer::get() {
    static AxrServer singleton;
    return singleton;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr server. "
AxrResult AxrServer::setup(const Config& config) {
    assert(!m_IsSetup);

    m_IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrServer::shutDown() {
    m_IsSetup = false;
}
