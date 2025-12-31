// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "platform.h"
#include "axr/common/defines.h"
#include "axr/logging.h"

#include <cassert>

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrPlatform& AxrPlatform::get() {
    static AxrPlatform singleton;
    return singleton;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr platform. "
AxrResult AxrPlatform::setup(const Config& config) {
    assert(!m_IsSetup);
    AxrResult axrResult = AXR_SUCCESS;

    if (!SDL_Init(SDL_INIT_EVENTS)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL init failed.");
        return AXR_ERROR_UNKNOWN;
    }

    axrResult = createWindow();
    if (AXR_FAILED(axrResult)) {
        return AXR_ERROR_FALLTHROUGH;
    }

    m_IsRunning = true;
    m_IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrPlatform::shutDown() {
    destroyWindow();
    m_IsRunning = false;
    m_IsSetup = false;
}

bool AxrPlatform::isRunning() const {
    return m_IsRunning;
}

void AxrPlatform::processEvents() {
    assert(m_IsSetup);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
            // HandleWindowEvent(event.window);
        } else if (event.type == SDL_EVENT_QUIT) {
            m_IsRunning = false;
        }
    }
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create window. "
AxrResult AxrPlatform::createWindow() {
    assert(!m_IsSetup);

    m_SDLWindow = SDL_CreateWindow("Test", 800, 600, 0);
    if (!m_SDLWindow) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL create window failed: {}.", SDL_GetError());
        return AXR_ERROR_UNKNOWN;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrPlatform::destroyWindow() {
    if (m_SDLWindow == nullptr)
        return;

    SDL_DestroyWindow(m_SDLWindow);
    m_SDLWindow = nullptr;
}
