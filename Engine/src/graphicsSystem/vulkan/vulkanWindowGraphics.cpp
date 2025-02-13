#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanWindowGraphics.hpp"
#include "axr/logger.h"
#include "vulkanUtils.hpp"
#include "../../windowSystem/windowSystem.hpp"
#include "vulkanSurfaceDetails.hpp"

// ---- Special Functions ----

AxrVulkanWindowGraphics::AxrVulkanWindowGraphics(const Config& config):
    m_WindowSystem(config.WindowSystem),
    m_Dispatch(config.Dispatch),
    m_PreferredPresentationMode(config.PresentationMode),
    m_Instance(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Surface(VK_NULL_HANDLE),
    m_SwapchainColorFormat(vk::Format::eUndefined),
    m_SwapchainDepthFormat(vk::Format::eUndefined),
    m_SwapchainPresentationMode(static_cast<vk::PresentModeKHR>(VK_PRESENT_MODE_MAX_ENUM_KHR)),
    m_SwapchainExtent(0, 0) {
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
    resetWindowConfiguration();

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

    if (m_PhysicalDevice != VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device isn't null.");
        return AXR_ERROR;
    }

    if (config.PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Config physical device is null.");
        return AXR_ERROR;
    }

    if (!m_SwapchainColorFormatOptions.empty()) {
        axrLogErrorLocation("Swapchain color format options aren't empty.");
        return AXR_ERROR;
    }

    if (config.SwapchainColorFormatOptions.empty()) {
        axrLogErrorLocation("Config swapchain color format options are empty.");
        return AXR_ERROR;
    }

    if (!m_SwapchainDepthFormatOptions.empty()) {
        axrLogErrorLocation("Swapchain depth format options aren't empty.");
        return AXR_ERROR;
    }

    if (config.SwapchainDepthFormatOptions.empty()) {
        axrLogErrorLocation("Config swapchain depth format options are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    m_Instance = config.Instance;
    m_PhysicalDevice = config.PhysicalDevice;
    m_SwapchainColorFormatOptions = config.SwapchainColorFormatOptions;
    m_SwapchainDepthFormatOptions = config.SwapchainDepthFormatOptions;

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSetupConfigVariables() {
    m_Instance = VK_NULL_HANDLE;
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_SwapchainColorFormatOptions.clear();
    m_SwapchainDepthFormatOptions.clear();
}

AxrResult AxrVulkanWindowGraphics::configureWindowGraphics() {
    AxrResult result = AXR_SUCCESS;

    result = createSurface();
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    const auto surfaceDetails = AxrVulkanSurfaceDetails(m_PhysicalDevice, m_Surface, m_Dispatch);
    if (!surfaceDetails.isValid()) {
        return AXR_ERROR;
    }

    result = setSwapchainFormats(surfaceDetails.Formats);
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    result = setSwapchainPresentationMode(surfaceDetails.PresentationModes);
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    result = setSwapchainExtent(surfaceDetails.Capabilities);
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    return result;
}

void AxrVulkanWindowGraphics::resetWindowConfiguration() {
    resetSwapchainExtent();
    resetSwapchainPresentationMode();
    resetSwapchainFormats();
    destroySurface();
}

AxrResult AxrVulkanWindowGraphics::createSurface() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_ERROR;

#ifdef AXR_USE_PLATFORM_WIN32
    axrResult = createWin32Surface();
#else
    axrLogErrorLocation("Unknown platform.");
#endif

    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::destroySurface() {
    if (m_Surface != VK_NULL_HANDLE) {
        m_Instance.destroySurfaceKHR(m_Surface, nullptr, m_Dispatch);
        m_Surface = VK_NULL_HANDLE;
    }
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

AxrResult AxrVulkanWindowGraphics::setSwapchainFormats(const std::vector<vk::SurfaceFormatKHR>& surfaceFormats) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_SwapchainColorFormat != vk::Format::eUndefined) {
        axrLogErrorLocation("Swapchain color format has already been set.");
        return AXR_ERROR;
    }

    if (m_SwapchainDepthFormat != vk::Format::eUndefined) {
        axrLogErrorLocation("Swapchain depth format has already been set.");
        return AXR_ERROR;
    }

    if (m_SwapchainColorFormatOptions.empty()) {
        axrLogErrorLocation("Swapchain color format options are empty.");
        return AXR_ERROR;
    }

    if (m_SwapchainDepthFormatOptions.empty()) {
        axrLogErrorLocation("Swapchain depth format options are empty.");
        return AXR_ERROR;
    }

    if (surfaceFormats.empty()) {
        axrLogErrorLocation("Surface formats are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto foundFormatIt = std::find_first_of(
        m_SwapchainColorFormatOptions.begin(),
        m_SwapchainColorFormatOptions.end(),
        surfaceFormats.begin(),
        surfaceFormats.end()
    );

    if (foundFormatIt != m_SwapchainColorFormatOptions.end()) {
        m_SwapchainColorFormat = *foundFormatIt;
    } else {
        // None of our ideal swapchain color formats could be found.
        // It should still be ok to just pick what ever the first supported surface format is but there may be issues.
        m_SwapchainColorFormat = surfaceFormats[0];
    }

    // We don't need to check surface compatibility for the depth format.
    // As long as the options we're given are supported by the physical device, that's all we need.
    // And we just pick the first option since they're already ordered from most desired to least desired.
    m_SwapchainDepthFormat = m_SwapchainDepthFormatOptions[0];

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSwapchainFormats() {
    m_SwapchainColorFormat = vk::Format::eUndefined;
    m_SwapchainDepthFormat = vk::Format::eUndefined;
}

AxrResult AxrVulkanWindowGraphics::setSwapchainPresentationMode(
    const std::vector<vk::PresentModeKHR>& surfacePresentationModes
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_SwapchainPresentationMode != static_cast<vk::PresentModeKHR>(VK_PRESENT_MODE_MAX_ENUM_KHR)) {
        axrLogErrorLocation("Swapchain presentation mode has already been set.");
        return AXR_ERROR;
    }

    if (m_PreferredPresentationMode == AXR_VULKAN_PRESENTATION_MODE_UNDEFINED) {
        axrLogErrorLocation("Preferred presentation mode is undefined.");
        return AXR_ERROR;
    }

    if (surfacePresentationModes.empty()) {
        axrLogErrorLocation("Surface presentation modes are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    // Find the preferred presentation mode
    for (const vk::PresentModeKHR presentationMode : surfacePresentationModes) {
        if (presentationMode == axrToVkPresentMode(m_PreferredPresentationMode)) {
            m_SwapchainPresentationMode = presentationMode;
            return AXR_SUCCESS;
        }
    }

    // Find FIFO presentation mode as a backup if preferred presentation mode isn't supported
    for (const vk::PresentModeKHR presentationMode : surfacePresentationModes) {
        if (presentationMode == vk::PresentModeKHR::eFifo) {
            m_SwapchainPresentationMode = presentationMode;
            return AXR_SUCCESS;
        }
    }

    // Just use what ever we can that's supported
    m_SwapchainPresentationMode = surfacePresentationModes[0];

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSwapchainPresentationMode() {
    m_SwapchainPresentationMode = static_cast<vk::PresentModeKHR>(VK_PRESENT_MODE_MAX_ENUM_KHR);
}

AxrResult AxrVulkanWindowGraphics::setSwapchainExtent(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities) {
    // If the current extent width is the max uint32_t value, then we need to get the extent manually
    if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
        m_SwapchainExtent = surfaceCapabilities.currentExtent;
        return AXR_SUCCESS;
    }

    uint32_t width;
    uint32_t height;
    const AxrResult axrResult = m_WindowSystem.getClientSize(width, height);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    width = std::clamp(
        width,
        surfaceCapabilities.minImageExtent.width,
        surfaceCapabilities.maxImageExtent.width
    );
    height = std::clamp(
        height,
        surfaceCapabilities.minImageExtent.height,
        surfaceCapabilities.maxImageExtent.height
    );

    m_SwapchainExtent = vk::Extent2D(width, height);
    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSwapchainExtent() {
    m_SwapchainExtent.width = 0;
    m_SwapchainExtent.height = 0;
}

// ---- Private Static Functions ----

AxrResult AxrVulkanWindowGraphics::configureWindowGraphicsCallback(void* userData, const bool isWindowOpen) {
    if (userData == nullptr) {
        axrLogErrorLocation("userData is null.");
        return AXR_ERROR;
    }

    const auto self = static_cast<AxrVulkanWindowGraphics*>(userData);

    if (isWindowOpen) {
        return self->configureWindowGraphics();
    } else {
        self->resetWindowConfiguration();
        return AXR_SUCCESS;
    }
}

#endif
