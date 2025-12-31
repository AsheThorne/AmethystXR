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

    if (config.WindowEnabled) {
        axrResult = createWindow(config.WindowTitle, config.WindowWidth, config.WindowHeight);
        if (AXR_FAILED(axrResult)) {
            return AXR_ERROR_FALLTHROUGH;
        }
    }

    m_IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrPlatform::shutDown() {
    destroyWindow();
    m_IsSetup = false;
}

bool AxrPlatform::isWindowOpen() const {
    return m_IsWindowOpen;
}

bool AxrPlatform::processEvents() {
    assert(m_IsSetup);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
            handleWindowEvent(event.window);
        } else if (event.type == SDL_EVENT_QUIT) {
            // Signal that the platform layer has requested to exit processing
            return false;
        }
    }

    return true;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create window. "
AxrResult AxrPlatform::createWindow(const char (&title)[AXR_MAX_WINDOW_TITLE_SIZE],
                                    const uint32_t width,
                                    const uint32_t height) {
    assert(!m_IsSetup);

    m_SDLWindow = SDL_CreateWindow("Test", 800, 600, 0);
    if (!m_SDLWindow) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL create window failed: {}.", SDL_GetError());
        return AXR_ERROR_UNKNOWN;
    }

    m_IsWindowOpen = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrPlatform::destroyWindow() {
    if (m_SDLWindow != nullptr) {
        SDL_DestroyWindow(m_SDLWindow);
        m_SDLWindow = nullptr;
    }

    m_IsWindowOpen = false;
}

void AxrPlatform::handleWindowEvent(const SDL_WindowEvent& event) {
    switch (event.type) {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
            destroyWindow();
            break;
        }
        default: {
            // Don't handle any unknown event
            break;
        }
    }
}
