// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "application.h"
#include "../memory/allocator.h"
#include "../platform/platform.h"
#include "../renderer/renderer.h"
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

    AxrResult axrResult = processEvents();
    if (axrResult == AXR_EVENT_APPLICATION_CLOSED) [[unlikely]] {
        return axrResult;
    }
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "ProcessEvents() failed.");
        return axrResult;
    }

    axrResult = AxrRenderer::get().renderScene();
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to render scene.");
        return axrResult;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

AxrResult AxrApplication::processEvents() const {
    AxrResult axrResult = AxrPlatform::get().processEvents();

    if (axrResult == AXR_EVENT_WINDOW_CLOSE_REQUESTED) [[unlikely]] {
        AxrRenderer::get().destroyDesktopResources();
        AxrPlatform::get().destroyWindow();

        // After we've cleaned up all resources, process events again so we can trigger and process the
        // AXR_EVENT_WINDOW_CLOSED event before returning control to whoever called this function
        axrResult = AxrPlatform::get().processEvents();
    }

    // TODO (Ashe): When we integrate OpenXR, return AXR_EVENT_APPLICATION_CLOSED if either of them are destroyed. Maybe
    //  have a config option to only close the application if both of them are closed though.
    if (axrResult == AXR_EVENT_WINDOW_CLOSED) [[unlikely]] {
        return AXR_EVENT_APPLICATION_CLOSED;
    }

    return AXR_SUCCESS;
}
