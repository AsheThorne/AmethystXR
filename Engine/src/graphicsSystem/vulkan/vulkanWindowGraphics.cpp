#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanWindowGraphics.hpp"
#include "axr/logger.h"
#include "vulkanUtils.hpp"
#include "../../windowSystem/windowSystem.hpp"
#include "vulkanSharedFunctions.hpp"
#include "vulkanImage.hpp"

// ---- Special Functions ----

AxrVulkanWindowGraphics::AxrVulkanWindowGraphics(const Config& config):
    m_WindowSystem(config.WindowSystem),
    m_Dispatch(config.Dispatch),
    m_LoadedScenes(config.LoadedScenes),
    m_PreferredPresentationMode(config.PresentationMode),
    m_MaxFramesInFlight(config.MaxFramesInFlight),
    m_Instance(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_GraphicsCommandPool(VK_NULL_HANDLE),
    m_SwapchainImageLayout(vk::ImageLayout::ePresentSrcKHR),
    m_Surface(VK_NULL_HANDLE),
    m_SwapchainColorFormat(vk::Format::eUndefined),
    m_SwapchainDepthFormat(vk::Format::eUndefined),
    m_SwapchainPresentationMode(static_cast<vk::PresentModeKHR>(VK_PRESENT_MODE_MAX_ENUM_KHR)),
    m_RenderPass(VK_NULL_HANDLE),
    m_SwapchainExtent(0, 0),
    m_Swapchain(VK_NULL_HANDLE),
    m_IsReady(false),
    m_CurrentImageIndex(0),
    m_CurrentFrame(0),
    m_IsSwapchainOutOfDate(false) {
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

    m_WindowSystem.OnWindowOpenStateChangedCallbackGraphics = AxrCallback(this, onWindowOpenStateChangedCallback);

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSetup() {
    resetWindowConfiguration();

    m_WindowSystem.OnWindowOpenStateChangedCallbackGraphics = {};
    resetSetupConfigVariables();
}

bool AxrVulkanWindowGraphics::isReady() const {
    return m_IsReady;
}

vk::RenderPass AxrVulkanWindowGraphics::getRenderPass() const {
    return m_RenderPass;
}

vk::Framebuffer AxrVulkanWindowGraphics::getFramebuffer() const {
    return m_SwapchainFramebuffers[m_CurrentImageIndex];
}

vk::Extent2D AxrVulkanWindowGraphics::getSwapchainExtent() const {
    return m_SwapchainExtent;
}

vk::ClearColorValue AxrVulkanWindowGraphics::getClearColorValue() const {
    // TODO: Don't hard code this
    return vk::ClearColorValue(0.2f, 0.05f, 0.2f, 1.0f);
}

vk::CommandBuffer AxrVulkanWindowGraphics::getRenderingCommandBuffer() const {
    return m_RenderingCommandBuffers[m_CurrentFrame];
}

std::vector<vk::Semaphore> AxrVulkanWindowGraphics::getRenderingWaitSemaphores() const {
    return {m_ImageAvailableSemaphores[m_CurrentFrame]};
}

std::vector<vk::PipelineStageFlags> AxrVulkanWindowGraphics::getRenderingWaitStages() const {
    return {vk::PipelineStageFlagBits::eColorAttachmentOutput};
}

std::vector<vk::Semaphore> AxrVulkanWindowGraphics::getRenderingSignalSemaphores() const {
    return {m_RenderingFinishedSemaphores[m_CurrentFrame]};
}

vk::Fence AxrVulkanWindowGraphics::getRenderingFence() const {
    return {m_RenderingFences[m_CurrentFrame]};
}

uint32_t AxrVulkanWindowGraphics::getCurrentRenderingFrame() const {
    return m_CurrentFrame;
}

AxrResult AxrVulkanWindowGraphics::acquireNextSwapchainImage() {
    if (m_IsSwapchainOutOfDate && AXR_FAILED(recreateSwapchain())) {
        axrLogErrorLocation("Failed to recreate swapchain.");
        return AXR_ERROR;
    }

    const vk::Result vkResult = m_Device.acquireNextImageKHR(
        m_Swapchain,
        UINT64_MAX,
        m_ImageAvailableSemaphores[m_CurrentFrame],
        VK_NULL_HANDLE,
        &m_CurrentImageIndex,
        m_Dispatch
    );
    axrLogVkResult(vkResult, "m_Device.acquireNextImageKHR");

    if (vkResult == vk::Result::eErrorOutOfDateKHR || vkResult == vk::Result::eSuboptimalKHR) {
        if (AXR_FAILED(recreateSwapchain())) {
            axrLogErrorLocation("Failed to recreate swapchain.");
            return AXR_ERROR;
        }
    }

    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanWindowGraphics::presentFrame() {
    const std::vector<vk::Semaphore> waitSemaphores = getRenderingSignalSemaphores();

    const vk::PresentInfoKHR presentInfo(
        static_cast<uint32_t>(waitSemaphores.size()),
        waitSemaphores.data(),
        1,
        &m_Swapchain,
        &m_CurrentImageIndex,
        nullptr
    );

    const vk::Result vkResult = m_QueueFamilies.PresentationQueue.presentKHR(
        &presentInfo,
        m_Dispatch
    );
    axrLogVkResult(vkResult, "PresentationQueue.presentKHR");

    if (vkResult == vk::Result::eErrorOutOfDateKHR || vkResult == vk::Result::eSuboptimalKHR) {
        if (AXR_FAILED(recreateSwapchain())) {
            axrLogErrorLocation("Failed to recreate swapchain.");
            return AXR_ERROR;
        }
    } else {
        if (AXR_FAILED(vkResult)) {
            return AXR_ERROR;
        }
    }

    m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight;

    return AXR_SUCCESS;
}

AxrResult AxrVulkanWindowGraphics::updateSceneDataUniformBuffer(const AxrVulkanSceneData* sceneData) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (sceneData == nullptr) {
        axrLogErrorLocation("Scene data is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    // TODO: Use the active camera's properties
    AxrEngineAssetUniformBuffer_SceneData sceneDataEngineAsset{
        .ViewMatrix = glm::lookAt(
            glm::vec3(2.0f, 0.5f, 2.0f),
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        ),
        .ProjectionMatrix = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(m_SwapchainExtent.width) / static_cast<float>(m_SwapchainExtent.height),
            0.1f,
            100.0f
        )
    };
    sceneDataEngineAsset.ProjectionMatrix[1][1] *= -1.0f;

    // TODO: I don't think we should set the whole scene data object in the window graphics class.
    //  We just need to get the view matrix and projection matrix for the window.
    return sceneData->setWindowUniformBufferData(
        axrEngineAssetGetUniformBufferName(AXR_ENGINE_ASSET_UNIFORM_BUFFER_SCENE_DATA),
        m_CurrentFrame,
        0,
        sizeof(sceneDataEngineAsset),
        &sceneDataEngineAsset
    );
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

    if (m_Device != VK_NULL_HANDLE) {
        axrLogErrorLocation("Logical device isn't null.");
        return AXR_ERROR;
    }

    if (config.Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Config logical device is null.");
        return AXR_ERROR;
    }

    if (m_GraphicsCommandPool != VK_NULL_HANDLE) {
        axrLogErrorLocation("Graphics command pool isn't null.");
        return AXR_ERROR;
    }

    if (config.GraphicsCommandPool == VK_NULL_HANDLE) {
        axrLogErrorLocation("Config graphics command pool is null.");
        return AXR_ERROR;
    }

    if (m_QueueFamilies.isValid()) {
        axrLogErrorLocation("Queue families are already set.");
        return AXR_ERROR;
    }

    if (!config.QueueFamilies.isValid()) {
        axrLogErrorLocation("Config queue families aren't valid.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    m_Instance = config.Instance;
    m_PhysicalDevice = config.PhysicalDevice;
    m_Device = config.Device;
    m_GraphicsCommandPool = config.GraphicsCommandPool;
    m_QueueFamilies = config.QueueFamilies;

    // TODO: Don't take the whole config object. only take what it needs
    const AxrResult axrResult = setSwapchainFormatOptions(config);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSetupConfigVariables() {
    resetSwapchainFormatOptions();

    m_Instance = VK_NULL_HANDLE;
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_GraphicsCommandPool = VK_NULL_HANDLE;
    m_QueueFamilies.reset();
}

AxrResult AxrVulkanWindowGraphics::setSwapchainFormatOptions(const SetupConfig& config) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_SwapchainColorFormatOptions.empty()) {
        axrLogErrorLocation("Swapchain color format options aren't empty.");
        return AXR_ERROR;
    }

    if (!m_SwapchainDepthFormatOptions.empty()) {
        axrLogErrorLocation("Swapchain depth format options aren't empty.");
        return AXR_ERROR;
    }

    if (config.SwapchainColorFormatOptions.empty()) {
        axrLogErrorLocation("Config swapchain color format options are empty.");
        return AXR_ERROR;
    }

    if (config.SwapchainDepthFormatOptions.empty()) {
        axrLogErrorLocation("Config swapchain depth format options are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    for (const vk::SurfaceFormatKHR surfaceFormat : config.SwapchainColorFormatOptions) {
        if (axrAreFormatFeaturesSupported(
            surfaceFormat.format,
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eSampledImage &
            vk::FormatFeatureFlagBits::eColorAttachment,
            // TODO: Check if we're rendering directly to the surface first for asking for these.
            // vk::FormatFeatureFlagBits::eBlitDst &
            // vk::FormatFeatureFlagBits::eTransferDst,
            config.PhysicalDevice,
            m_Dispatch
        )) {
            m_SwapchainColorFormatOptions.push_back(surfaceFormat);
        }
    }

    for (const vk::Format format : config.SwapchainDepthFormatOptions) {
        if (axrAreFormatFeaturesSupported(
            format,
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eSampledImage &
            vk::FormatFeatureFlagBits::eDepthStencilAttachment,
            config.PhysicalDevice,
            m_Dispatch
        )) {
            m_SwapchainDepthFormatOptions.push_back(format);
        }
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSwapchainFormatOptions() {
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

    result = createRenderPass();
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    result = createSyncObjects();
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    result = createCommandBuffers();
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    result = setupSwapchain(surfaceDetails);
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    result = m_LoadedScenes.setupWindowData(m_RenderPass);
    if (AXR_FAILED(result)) {
        resetWindowConfiguration();
        return result;
    }

    m_IsReady = true;
    m_WindowSystem.OnWindowResizedCallbackGraphics = AxrCallback(this, onWindowResizedCallback);

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetWindowConfiguration() {
    m_IsReady = false;
    m_WindowSystem.OnWindowResizedCallbackGraphics = {};

    m_LoadedScenes.resetSetupWindowData();
    resetSetupSwapchain();
    destroyCommandBuffers();
    destroySyncObjects();
    destroyRenderPass();
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

    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}
#endif

AxrResult AxrVulkanWindowGraphics::setupSwapchain(const AxrVulkanSurfaceDetails& surfaceDetails) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = setSwapchainPresentationMode(surfaceDetails.PresentationModes);
    if (AXR_FAILED(axrResult)) {
        resetWindowConfiguration();
        return axrResult;
    }

    axrResult = setSwapchainExtent(surfaceDetails.Capabilities);
    if (AXR_FAILED(axrResult)) {
        resetWindowConfiguration();
        return axrResult;
    }

    axrResult = createSwapchain(surfaceDetails.Capabilities);
    if (AXR_FAILED(axrResult)) {
        resetWindowConfiguration();
        return axrResult;
    }

    axrResult = getSwapchainImages();
    if (AXR_FAILED(axrResult)) {
        resetWindowConfiguration();
        return axrResult;
    }

    axrResult = createFramebuffers();
    if (AXR_FAILED(axrResult)) {
        resetWindowConfiguration();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSetupSwapchain() {
    destroyFramebuffers();
    resetSwapchainImages();
    destroySwapchain();
    resetSwapchainExtent();
    resetSwapchainPresentationMode();
}

AxrResult AxrVulkanWindowGraphics::recreateSwapchain() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    // Don't bother recreating the swapchain if the window isn't visible
    uint32_t width = 0;
    uint32_t height = 0;
    axrResult = m_WindowSystem.getClientSize(width, height);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to get client size.");
        return axrResult;
    }

    if (width == 0 || height == 0) {
        m_IsSwapchainOutOfDate = true;
        return AXR_SUCCESS;
    }

    const vk::Result vkResult = m_Device.waitIdle(m_Dispatch);
    axrLogVkResult(vkResult, "m_Device.waitIdle");
    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }


    resetSetupSwapchain();

    const auto surfaceDetails = AxrVulkanSurfaceDetails(m_PhysicalDevice, m_Surface, m_Dispatch);
    if (!surfaceDetails.isValid()) {
        return AXR_ERROR;
    }

    axrResult = setupSwapchain(surfaceDetails);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to setup swapchain.");
        return axrResult;
    }

    m_IsSwapchainOutOfDate = false;

    return AXR_SUCCESS;
}

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

AxrResult AxrVulkanWindowGraphics::createSwapchain(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Swapchain != VK_NULL_HANDLE) {
        axrLogErrorLocation("Swapchain already exists.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Logical device is null.");
        return AXR_ERROR;
    }

    if (m_Surface == VK_NULL_HANDLE) {
        axrLogErrorLocation("Surface is null.");
        return AXR_ERROR;
    }

    if (!m_QueueFamilies.GraphicsQueueFamilyIndex.has_value()) {
        axrLogErrorLocation("Graphics queue family index is undefined.");
        return AXR_ERROR;
    }

    if (!m_QueueFamilies.PresentationQueueFamilyIndex.has_value()) {
        axrLogErrorLocation("Presentation queue family index is undefined.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;

    // 0 is a special value that indicates that there is no maximum
    if (surfaceCapabilities.maxImageCount > 0 && minImageCount > surfaceCapabilities.
        maxImageCount) {
        minImageCount = surfaceCapabilities.maxImageCount;
    }

    // TODO: Use vk::ImageUsageFlagBits::eTransferDst if we aren't rendering directly to the surface
    constexpr vk::ImageUsageFlags imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    vk::SwapchainCreateInfoKHR swapchainCreateInfo(
        {},
        m_Surface,
        minImageCount,
        m_SwapchainColorFormat.format,
        m_SwapchainColorFormat.colorSpace,
        m_SwapchainExtent,
        1,
        imageUsage,
        vk::SharingMode::eExclusive,
        0,
        nullptr,
        surfaceCapabilities.currentTransform,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        m_SwapchainPresentationMode,
        vk::True,
        // TODO: Add old swapchain if it's being recreated
        // oldSwapchain
        VK_NULL_HANDLE
    );

    const uint32_t graphicsFamilyIndex = m_QueueFamilies.GraphicsQueueFamilyIndex.value();
    const uint32_t presentationFamilyIndex = m_QueueFamilies.PresentationQueueFamilyIndex.value();
    const uint32_t queueFamilyIndices[] = {graphicsFamilyIndex, presentationFamilyIndex};

    if (graphicsFamilyIndex != presentationFamilyIndex) {
        swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    const vk::Result vkResult = m_Device.createSwapchainKHR(
        &swapchainCreateInfo,
        nullptr,
        &m_Swapchain,
        m_Dispatch
    );
    axrLogVkResult(vkResult, "m_Device.createSwapchainKHR");

    if (VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::destroySwapchain() {
    if (m_Swapchain != VK_NULL_HANDLE) {
        m_Device.destroySwapchainKHR(m_Swapchain, nullptr, m_Dispatch);
        m_Swapchain = VK_NULL_HANDLE;
    }
}

AxrResult AxrVulkanWindowGraphics::getSwapchainImages() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_SwapchainColorImages.empty()) {
        axrLogErrorLocation("Swapchain color images already exist.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Logical device is null.");
        return AXR_ERROR;
    }

    if (m_Swapchain == VK_NULL_HANDLE) {
        axrLogErrorLocation("Swapchain is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto swapchainImagesResult = m_Device.getSwapchainImagesKHR(m_Swapchain, m_Dispatch);
    axrLogVkResult(swapchainImagesResult.result, "m_Device.getSwapchainImagesKHR");
    if (VK_FAILED(swapchainImagesResult.result)) {
        resetSwapchainImages();
        return AXR_ERROR;
    }

    m_SwapchainColorImages = swapchainImagesResult.value;
    m_SwapchainColorImageViews.resize(m_SwapchainColorImages.size());
    AxrResult axrResult = AXR_SUCCESS;

    for (size_t i = 0; i < m_SwapchainColorImageViews.size(); ++i) {
        axrResult = AxrVulkanImage::createImageView(
            m_Device,
            m_SwapchainColorImages[i],
            m_SwapchainColorFormat.format,
            vk::ImageAspectFlagBits::eColor,
            1,
            m_SwapchainColorImageViews[i],
            m_Dispatch
        );
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        resetSwapchainImages();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::resetSwapchainImages() {
    for (vk::ImageView& imageView : m_SwapchainColorImageViews) {
        AxrVulkanImage::destroyImageView(m_Device, imageView, m_Dispatch);
    }
    m_SwapchainColorImageViews.clear();
}

AxrResult AxrVulkanWindowGraphics::createRenderPass() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_RenderPass != VK_NULL_HANDLE) {
        axrLogErrorLocation("Render pass already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    return axrCreateRenderPass(
        m_Device,
        m_SwapchainColorFormat.format,
        m_SwapchainDepthFormat,
        m_SwapchainImageLayout,
        m_RenderPass,
        m_Dispatch
    );
}

void AxrVulkanWindowGraphics::destroyRenderPass() {
    axrDestroyRenderPass(m_Device, m_RenderPass, m_Dispatch);
}

AxrResult AxrVulkanWindowGraphics::createSyncObjects() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_ImageAvailableSemaphores.empty()) {
        axrLogErrorLocation("Image available semaphores already exist.");
        return AXR_ERROR;
    }

    if (!m_RenderingFinishedSemaphores.empty()) {
        axrLogErrorLocation("Rendering finished semaphores already exist.");
        return AXR_ERROR;
    }

    if (!m_RenderingFences.empty()) {
        axrLogErrorLocation("Rendering fences already exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = axrCreateSemaphores(m_Device, m_MaxFramesInFlight, m_ImageAvailableSemaphores, m_Dispatch);
    if (AXR_FAILED(axrResult)) {
        destroySyncObjects();
        return axrResult;
    }

    axrResult = axrCreateSemaphores(m_Device, m_MaxFramesInFlight, m_RenderingFinishedSemaphores, m_Dispatch);
    if (AXR_FAILED(axrResult)) {
        destroySyncObjects();
        return axrResult;
    }

    axrResult = axrCreateFences(m_Device, m_MaxFramesInFlight, m_RenderingFences, m_Dispatch);
    if (AXR_FAILED(axrResult)) {
        destroySyncObjects();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::destroySyncObjects() {
    axrDestroySemaphores(m_Device, m_ImageAvailableSemaphores, m_Dispatch);
    axrDestroySemaphores(m_Device, m_RenderingFinishedSemaphores, m_Dispatch);
    axrDestroyFences(m_Device, m_RenderingFences, m_Dispatch);
}

AxrResult AxrVulkanWindowGraphics::createCommandBuffers() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = axrCreateCommandBuffers(
        m_Device,
        m_GraphicsCommandPool,
        m_MaxFramesInFlight,
        m_RenderingCommandBuffers,
        m_Dispatch
    );
    if (AXR_FAILED(axrResult)) {
        destroyCommandBuffers();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::destroyCommandBuffers() {
    axrDestroyCommandBuffers(m_Device, m_GraphicsCommandPool, m_RenderingCommandBuffers, m_Dispatch);
}

AxrResult AxrVulkanWindowGraphics::createFramebuffers() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_SwapchainFramebuffers.empty()) {
        axrLogErrorLocation("Swapchain framebuffers already exist.");
        return AXR_ERROR;
    }

    if (m_SwapchainColorImageViews.empty()) {
        axrLogErrorLocation("Swapchain color image views don't exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const AxrResult axrResult = axrCreateFramebuffers(
        m_Device,
        m_RenderPass,
        m_SwapchainExtent,
        m_SwapchainColorImageViews,
        m_SwapchainFramebuffers,
        m_Dispatch
    );

    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to create swapchain framebuffers.");
        destroyFramebuffers();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanWindowGraphics::destroyFramebuffers() {
    axrDestroyFramebuffers(m_Device, m_SwapchainFramebuffers, m_Dispatch);
}

// ---- Private Static Functions ----

AxrResult AxrVulkanWindowGraphics::onWindowOpenStateChangedCallback(void* userData, const bool isWindowOpen) {
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

void AxrVulkanWindowGraphics::onWindowResizedCallback(void* userData, uint32_t width, uint32_t height) {
    if (userData == nullptr) {
        axrLogErrorLocation("userData is null.");
        return;
    }

    const auto self = static_cast<AxrVulkanWindowGraphics*>(userData);
    if (AXR_FAILED(self->recreateSwapchain())) {
        axrLogErrorLocation("Failed to recreate swapchain.");
        return;
    }
}

#endif
