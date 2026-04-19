// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "platform.h"
#include "../memory/allocator.h"
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
    assert(config.WindowConfig != nullptr);

    AxrResult axrResult = AXR_SUCCESS;

    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL_Init failed: {}", SDL_GetError());
        return AXR_ERROR_SDL_ERROR;
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

AxrResult AxrPlatform::processEvents() {
    assert(m_IsSetup);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
            const AxrResult axrResult = handleWindowEvent(event.window);

            if (axrResult == AXR_EVENT_WINDOW_CLOSE_REQUESTED) {
                return AXR_EVENT_WINDOW_CLOSE_REQUESTED;
            }
        } else if (event.type == SDL_EVENT_QUIT) {
            return AXR_EVENT_WINDOW_CLOSED;
        }
    }

    return AXR_SUCCESS;
}

void AxrPlatform::destroyWindow() {
    if (m_SDLWindow != nullptr) {
        SDL_DestroyWindow(m_SDLWindow);
        m_SDLWindow = nullptr;
    }

    m_IsWindowOpen = false;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to get window size in pixels. "
AxrResult AxrPlatform::getWindowSizeInPixels(uint32_t& width, uint32_t& height) const {
    int x;
    int y;
    if (!SDL_GetWindowSizeInPixels(m_SDLWindow, &x, &y)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL_GetWindowSizeInPixels failed: {}", SDL_GetError());
        return AXR_ERROR_SDL_ERROR;
    }

    width = static_cast<uint32_t>(x);
    height = static_cast<uint32_t>(y);
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

bool AxrPlatform::pathExists(const char8_t* path) {
    return SDL_GetPathInfo(reinterpret_cast<const char*>(path), nullptr);
}

bool AxrPlatform::isPathRelative(const AxrPath& path) {
    return !isPathAbsolute(path);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to get engine assets path. "
AxrResult AxrPlatform::getEngineAssetsPath(const size_t extraCapacity, AxrPath& path) const {
    const char* basePath = SDL_GetBasePath();
    const auto assetsFolder = u8"axr-assets/";
    auto tempPath = AxrPath(&AxrAllocator::get().EngineDataAllocator);

    const AxrResult axrResult = tempPath.growAllocation(
        std::char_traits<char>::length(basePath) + std::char_traits<char8_t>::length(assetsFolder) + extraCapacity);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to grow AxrPath allocation.");
        return axrResult;
    }

    // Don't need to use appendPath() for the base path because the separator should already be correct for the current
    // platform.
    tempPath.append(basePath);
    tempPath.appendPath(assetsFolder);

    path = std::move(tempPath);

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

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
    assert(instance != VK_NULL_HANDLE);

    if (surface != VK_NULL_HANDLE) [[unlikely]] {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Surface already exists.");
        return AXR_SUCCESS;
    }

    if (!SDL_Vulkan_CreateSurface(m_SDLWindow, instance, nullptr, &surface)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL_Vulkan_CreateSurface failed: {}", SDL_GetError());
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

    m_SDLWindow = SDL_CreateWindow(title,
                                   static_cast<int>(width),
                                   static_cast<int>(height),
                                   getSDLWindowFlags(rendererApiType));
    if (!m_SDLWindow) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "SDL_CreateWindow failed: {}", SDL_GetError());
        return AXR_ERROR_SDL_ERROR;
    }

    m_IsWindowOpen = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

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

AxrResult AxrPlatform::handleWindowEvent(const SDL_WindowEvent& event) {
    switch (event.type) {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
            return AXR_EVENT_WINDOW_CLOSE_REQUESTED;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            // TODO (Ashe): Instead of using callbacks, we could just set a flag here that the window resized. Then on
            //  every frame, in the renderer, we check if the window swapchain needs recreating
            OnWindowResizedRendererCallback(static_cast<uint32_t>(event.data1), static_cast<uint32_t>(event.data2));
            return AXR_SUCCESS;
        }
        default: {
            // Don't handle any unknown event
            break;
        }
    }

    return AXR_SUCCESS;
}
