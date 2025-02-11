#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanWindowGraphics.hpp"
#include "axr/logger.h"
#include "vulkanUtils.hpp"
#include "../../windowSystem/windowSystem.hpp"

// ---- Special Functions ----

AxrVulkanWindowGraphics::AxrVulkanWindowGraphics(const Config& config):
    m_WindowSystem(config.WindowSystem),
    m_Dispatch(config.Dispatch) {
}

AxrVulkanWindowGraphics::~AxrVulkanWindowGraphics() {
    resetSetup();
}

// ---- Public Functions ----

void AxrVulkanWindowGraphics::addRequiredInstanceExtensions(
    AxrVulkanExtensionCollection<AxrVulkanExtension_T, AxrVulkanExtensionTypeEnum>& extensions
) const {
    auto surfaceExtension = AxrVulkanExtensionSurface{};
    extensions.add(reinterpret_cast<AxrVulkanExtension_T>(&surfaceExtension));

#ifdef AXR_USE_PLATFORM_WIN32
    auto win32SurfaceExtension = AxrVulkanExtensionWin32Surface{};
    extensions.add(reinterpret_cast<AxrVulkanExtension_T>(&win32SurfaceExtension));
#endif
}

void AxrVulkanWindowGraphics::addRequiredDeviceExtensions(
    AxrVulkanExtensionCollection<AxrVulkanExtension_T, AxrVulkanExtensionTypeEnum>& extensions
) const {
    auto swapchainExtension = AxrVulkanExtensionSwapchain{};
    extensions.add(reinterpret_cast<AxrVulkanExtension_T>(&swapchainExtension));
}

AxrResult AxrVulkanWindowGraphics::setup(const SetupConfig& config) {
    AxrResult result = AXR_SUCCESS;

    result = setSetupConfigVariables(config);
    if (AXR_FAILED(result)) {
        resetSetup();
        return result;
    }

    m_WindowSystem.setConfigureWindowGraphicsCallback(this, configureWindowGraphicsCallback);

    return result;
}

void AxrVulkanWindowGraphics::resetSetup() {
    resetPrepareWindowForRendering();

    m_WindowSystem.resetConfigureWindowGraphicsCallback();
    resetSetupConfigVariables();
}

// ---- Private Functions ----

AxrResult AxrVulkanWindowGraphics::setSetupConfigVariables(const SetupConfig& config) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != VK_NULL_HANDLE) {
        axrLogErrorLocation("Instance isn't null.");
        return AXR_ERROR;
    }

    if (config.Instance == VK_NULL_HANDLE) {
        axrLogErrorLocation("Config instance is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    m_Instance = config.Instance;

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSetupConfigVariables() {
    m_Instance = VK_NULL_HANDLE;
}

AxrResult AxrVulkanWindowGraphics::prepareWindowForRendering() {
    AxrResult result = AXR_SUCCESS;

    result = createSurface();
    if (AXR_FAILED(result)) {
        resetPrepareWindowForRendering();
        return result;
    }

    return result;
}

void AxrVulkanWindowGraphics::resetPrepareWindowForRendering() {
    destroySurface();
}

AxrResult AxrVulkanWindowGraphics::createSurface() {
#ifdef AXR_USE_PLATFORM_WIN32
    return createWin32Surface();
#else
    axrLogErrorLocation("Unknown platform.");
    return AXR_ERROR;
#endif
}

void AxrVulkanWindowGraphics::destroySurface() {
    if (m_Surface == VK_NULL_HANDLE) return;

    m_Instance.destroySurfaceKHR(m_Surface, nullptr, m_Dispatch);
    m_Surface = VK_NULL_HANDLE;
}

#ifdef AXR_USE_PLATFORM_WIN32
AxrResult AxrVulkanWindowGraphics::createWin32Surface() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //
    if (m_Surface != VK_NULL_HANDLE) {
        axrLogErrorLocation("Surface already exists.");
        return AXR_ERROR;
    }

    const AxrWin32WindowSystem* win32WindowSystem = m_WindowSystem.getWin32WindowSystem();
    if (win32WindowSystem == nullptr) {
        axrLogErrorLocation("Win32 Window system is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::Win32SurfaceCreateInfoKHR createInfo(
        {},
        win32WindowSystem->getInstance(),
        win32WindowSystem->getWindowHandle()
    );

    const vk::Result vkResult = m_Instance.createWin32SurfaceKHR(&createInfo, nullptr, &m_Surface, m_Dispatch);
    axrLogVkResult(vkResult, "m_Instance.createWin32SurfaceKHR");

    if (axrVkFailed(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

#endif

// ---- Private Static Functions ----

AxrResult AxrVulkanWindowGraphics::configureWindowGraphicsCallback(void* userData, const bool isWindowOpen) {
    if (userData == nullptr) {
        axrLogErrorLocation("userData is null.");
        return AXR_ERROR;
    }

    const auto self = static_cast<AxrVulkanWindowGraphics*>(userData);

    if (isWindowOpen) {
        return self->prepareWindowForRendering();
    } else {
        self->resetPrepareWindowForRendering();
        return AXR_SUCCESS;
    }
}

#endif
