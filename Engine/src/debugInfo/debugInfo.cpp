#ifdef AXR_DEBUG_INFO_ENABLED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "debugInfo.h"
#include "../memory/allocator.h"
#include "axr/logging.h"

#include <cassert>

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrDebugInfo::AxrDebugInfo() = default;

AxrDebugInfo::~AxrDebugInfo() {
    shutDown();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrDebugInfo& AxrDebugInfo::get() {
    static AxrDebugInfo singleton;
    return singleton;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr debug info. "
AxrResult AxrDebugInfo::setup(const Config& config) {
    assert(!m_IsSetup);

    IDNames = AxrUnorderedMap_Dynamic<AxrID, AxrString>(config.MaxIDCount, &AxrAllocator::get().DebugInfoAllocator);

    m_IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrDebugInfo::shutDown() {
    IDNames.~AxrUnorderedMap_Dynamic();
    m_IsSetup = false;
}

#endif
