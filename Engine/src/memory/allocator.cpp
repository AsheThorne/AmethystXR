// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "allocator.h"

#include <cassert>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrAllocator::AxrAllocator() = default;

AxrAllocator::~AxrAllocator() {
    shutDown();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrAllocator& AxrAllocator::get() {
    static AxrAllocator singleton;
    return singleton;
}

AxrResult AxrAllocator::setup(const Config& config) {
    assert(!m_IsSetup);

    m_IsSetup = true;
    return AXR_SUCCESS;
}

void AxrAllocator::shutDown() {
    m_IsSetup = false;
}
