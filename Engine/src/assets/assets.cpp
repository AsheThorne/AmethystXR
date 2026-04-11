// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "assets.h"
#include "../memory/allocator.h"
#include "axr/logging.h"

#include <cassert>

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrAssets::AxrAssets() = default;

AxrAssets::~AxrAssets() {
    shutDown();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrAssets& AxrAssets::get() {
    static AxrAssets singleton;
    return singleton;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr assets. "
AxrResult AxrAssets::setup(const Config& config) {
    assert(!m_IsSetup);

#ifdef AXR_DEBUG_INFO_ENABLED
    m_IDMap = AxrUnorderedMap_Dynamic<AxrID, AxrString>(config.MaxIDCount, &AxrAllocator::get().DebugInfoAllocator);
#endif

    m_IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrAssets::shutDown() {
    m_IDMap = {};

    m_IsSetup = false;
}
