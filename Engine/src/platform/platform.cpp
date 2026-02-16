// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "platform.h"
#include "axr/common/defines.h"
#include "axr/logging.h"

#include <cassert>

#include <SDL3/SDL_vulkan.h>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrPlatform::AxrPlatform() = default;

AxrPlatform::~AxrPlatform() {
    shutDown();
}

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

    if (config.WindowConfig == nullptr) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`config.WindowConfig` is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    AxrResult axrResult = AXR_SUCCESS;

    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL init failed.");
        return AXR_ERROR_UNKNOWN;
    }

    if (config.WindowConfig->Enabled) {
        axrResult = createWindow(config.WindowConfig->Title,
                                 config.WindowConfig->Width,
                                 config.WindowConfig->Height,
                                 config.RendererApiType);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            return axrResult;
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

#ifdef AXR_VULKAN_SUPPORTED
AxrExtensionArray<AxrVulkanExtension, AxrVulkanExtensionMaxCount> AxrPlatform::getRequiredVulkanExtensions() {
    AxrExtensionArray<AxrVulkanExtension, AxrVulkanExtensionMaxCount> extensionsArray;

    uint32_t count;
    char const* const* extensions = SDL_Vulkan_GetInstanceExtensions(&count);

    for (uint32_t i = 0; i < count; ++i) {
        extensionsArray.pushBack(AxrVulkanExtension{
            .Type = AxrVulkanExtensionGetType(extensions[i]),
            .IsRequired = true,
        });
    }

    return extensionsArray;
}

bool AxrPlatform::getVulkanPresentationSupport(const VkInstance& instance,
                                               const VkPhysicalDevice& physicalDevice,
                                               const uint32_t queueFamilyIndex) {
    return SDL_Vulkan_GetPresentationSupport(instance, physicalDevice, queueFamilyIndex);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create vulkan surface. "
AxrResult AxrPlatform::createVulkanSurface(const VkInstance& instance, VkSurfaceKHR& surface) const {
    if (surface != VK_NULL_HANDLE) [[unlikely]] {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Surface already exists.");
        return AXR_SUCCESS;
    }

    if (instance == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Instance is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (!SDL_Vulkan_CreateSurface(m_SDLWindow, instance, nullptr, &surface)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL_Vulkan_CreateSurface failed.");
        return AXR_ERROR_SDL_ERROR;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrPlatform::destroyVulkanSurface(const VkInstance& instance, VkSurfaceKHR& surface) const {
    if (surface == VK_NULL_HANDLE)
        return;

    SDL_Vulkan_DestroySurface(instance, surface, nullptr);
    surface = VK_NULL_HANDLE;
}
#endif

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to create window. "
AxrResult AxrPlatform::createWindow(const char (&title)[AXR_MAX_WINDOW_TITLE_SIZE],
                                    const uint32_t width,
                                    const uint32_t height,
                                    const AxrRendererApiTypeEnum rendererApiType) {
    assert(!m_IsSetup);

    m_SDLWindow =
        SDL_CreateWindow(title, static_cast<int>(width), static_cast<int>(height), getSDLWindowFlags(rendererApiType));
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

SDL_WindowFlags AxrPlatform::getSDLWindowFlags(const AxrRendererApiTypeEnum rendererApiType) {
    SDL_WindowFlags windowFlags = 0;

    switch (rendererApiType) {
        case AXR_RENDERER_API_TYPE_VULKAN: {
#ifdef AXR_VULKAN_SUPPORTED
            windowFlags |= SDL_WINDOW_VULKAN;
#endif
            break;
        }
        case AXR_RENDERER_API_TYPE_UNDEFINED:
        default: {
            break;
        }
    }

    return windowFlags;
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
