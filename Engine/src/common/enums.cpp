// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

const char* axrResultToString(const AxrResult axrResult) {
    switch (axrResult) {
        case AXR_SUCCESS: {
            return "AXR_SUCCESS";
        }
        case AXR_EVENT_APPLICATION_CLOSED: {
            return "AXR_EVENT_APPLICATION_CLOSED";
        }
        case AXR_EVENT_WINDOW_CLOSE_REQUESTED: {
            return "AXR_EVENT_WINDOW_CLOSE_REQUESTED";
        }
        case AXR_EVENT_WINDOW_CLOSED: {
            return "AXR_EVENT_WINDOW_CLOSED";
        }
        case AXR_DONT_RENDER: {
            return "AXR_DONT_RENDER";
        }
        case AXR_ERROR_UNKNOWN: {
            return "AXR_ERROR_UNKNOWN";
        }
        case AXR_ERROR_DUPLICATE: {
            return "AXR_ERROR_DUPLICATE";
        }
        case AXR_ERROR_NOT_FOUND: {
            return "AXR_ERROR_NOT_FOUND";
        }
        case AXR_ERROR_OUT_OF_MEMORY: {
            return "AXR_ERROR_OUT_OF_MEMORY";
        }
        case AXR_ERROR_NOT_SUPPORTED: {
            return "AXR_ERROR_NOT_SUPPORTED";
        }
        case AXR_ERROR_VALIDATION_FAILED: {
            return "AXR_ERROR_VALIDATION_FAILED";
        }
        case AXR_ERROR_INVALID_PATH: {
            return "AXR_ERROR_INVALID_PATH";
        }
        case AXR_ERROR_VULKAN_ERROR: {
            return "AXR_ERROR_VULKAN_ERROR";
        }
        case AXR_ERROR_SDL_ERROR: {
            return "AXR_ERROR_SDL_ERROR";
        }
        default: {
            return "Unknown AxrResult";
        }
    }
}
