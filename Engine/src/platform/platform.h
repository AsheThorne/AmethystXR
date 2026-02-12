#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../common/extensionArray.h"
#include "axr/common/defines.h"
#include "axr/common/enums.h"
#include "axr/lifecycle.h"

#include <SDL3/SDL.h>

/// AXR Platform singleton class
class AxrPlatform {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrPlatform to copy from
    AxrPlatform(const AxrPlatform& src) = delete;
    /// Move Constructor
    /// @param src Source AxrPlatform to move from
    AxrPlatform(AxrPlatform&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrPlatform to copy from
    AxrPlatform& operator=(const AxrPlatform& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrPlatform to move from
    AxrPlatform& operator=(AxrPlatform&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrPlatform();

    // ---- Destructor ----

    /// Destructor
    ~AxrPlatform();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrPlatform Config
    struct Config {
        const AxrWindowConfig* WindowConfig;
        AxrRendererApiTypeEnum RendererApiType;
    };

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrPlatform singleton
    /// @return A reference to the AxrPlatform singleton
    static AxrPlatform& get();

    /// Set up the platform
    /// @param config Platform config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the platform
    void shutDown();

    /// Check if the platform window is open
    /// @return True if the platform window is open
    [[nodiscard]] bool isWindowOpen() const;
    /// Process all platform events
    /// @return False if the window closed
    [[nodiscard]] bool processEvents();

#ifdef AXR_VULKAN_SUPPORTED
    /// Get the required vulkan platform extensions
    /// @return Required vulkan platform extensions
    [[nodiscard]] static AxrExtensionArray<AxrVulkanExtension, AxrVulkanExtensionMaxCount> getRequiredVulkanExtensions();
    /// Check if the given queue family index supported presentation on the current platform
    /// @param instance VkInstance to use
    /// @param physicalDevice VkPhysicalDevice to use
    /// @param queueFamilyIndex Queue family index to check
    /// @return True if the given queue family index supports presentation
    [[nodiscard]] static bool getVulkanPresentationSupport(const VkInstance& instance,
                                                           const VkPhysicalDevice& physicalDevice,
                                                           uint32_t queueFamilyIndex);
#endif

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    SDL_Window* m_SDLWindow = nullptr;
    bool m_IsSetup = false;
    bool m_IsWindowOpen = false;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Create the desktop window
    /// @param title Window title
    /// @param width Window width
    /// @param height Window height
    /// @param rendererApiType Renderer Api type
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createWindow(const char (&title)[AXR_MAX_WINDOW_TITLE_SIZE],
                                         uint32_t width,
                                         uint32_t height,
                                         AxrRendererApiTypeEnum rendererApiType);
    /// Destroy the desktop window
    void destroyWindow();

    /// Get the SDL window flags to use
    /// @param rendererApiType Renderer api type
    /// @return SDL window flags
    [[nodiscard]] static SDL_WindowFlags getSDLWindowFlags(AxrRendererApiTypeEnum rendererApiType);

    /// Handle the given window event
    /// @param event Window event to handle
    void handleWindowEvent(const SDL_WindowEvent& event);
};
