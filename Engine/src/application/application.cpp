// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "application.h"
#include "../memory/allocator.h"
#include "../platform/platform.h"
#include "axr/logging.h"

#include <cassert>

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //

bool axrApplicationIsRunning() {
    return AxrApplication::get().isRunning();
}

AxrResult axrApplicationStartNewFrame() {
    return AxrApplication::get().startNewFrame();
}

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrApplication::AxrApplication() = default;

AxrApplication::~AxrApplication() {
    shutDown();
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

#define AXR_FUNCTION_FAILED_STRING "Failed to start new frame. "
AxrResult AxrApplication::startNewFrame() const {
    // Clear the frame allocator at the start of each frame
    AxrAllocator::get().FrameAllocator.clear();

    const AxrResult axrResult = processEvents();
    if (axrResult == AXR_APPLICATION_CLOSED) [[unlikely]] {
        return axrResult;
    }
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "ProcessEvents() failed.");
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

AxrResult AxrApplication::processEvents() const {
    // TODO (Ashe): When we integrate OpenXR, only return false if both OpenXR session and window is closed.
    //  If the user wants to exit if only one exists, then they can manually do that.
    //  Changed my mind. don't have them manually exit if only one closes. Do it automatically. and maybe have a config
    //  option to change this behaviour

    // If the platform window closed
    if (!AxrPlatform::get().processEvents()) {
        return AXR_APPLICATION_CLOSED;
    }

    return AXR_SUCCESS;
}
