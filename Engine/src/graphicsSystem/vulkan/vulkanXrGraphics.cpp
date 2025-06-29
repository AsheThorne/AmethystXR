#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanXrGraphics.hpp"
#include "vulkanImage.hpp"
#include "vulkanUtils.hpp"
#include "vulkanSharedFunctions.hpp"

// ---- Special Functions ----

AxrVulkanXrGraphics::AxrVulkanXrGraphics(const Config& config):
    m_XrSystem(config.XrSystem),
    m_Dispatch(config.Dispatch),
    m_MaxFramesInFlight(config.MaxFramesInFlight),
    m_Instance(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_GraphicsCommandPool(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_IsReady(false),
    m_SwapchainColorFormat(vk::Format::eUndefined),
    m_SwapchainDepthFormat(vk::Format::eUndefined),
    m_SwapchainImageLayout(vk::ImageLayout::eColorAttachmentOptimal),
    m_RenderPass(VK_NULL_HANDLE) {
}

AxrVulkanXrGraphics::~AxrVulkanXrGraphics() {
    resetSetup();
}

// ---- Public Functions ----

AxrResult AxrVulkanXrGraphics::setup(const SetupConfig& config) {
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

    if (m_GraphicsCommandPool != VK_NULL_HANDLE) {
        axrLogErrorLocation("Graphics command pool isn't null.");
        return AXR_ERROR;
    }

    if (config.GraphicsCommandPool == VK_NULL_HANDLE) {
        axrLogErrorLocation("Config graphics command pool is null.");
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

    const AxrResult axrResult = setSwapchainFormatOptions(
        config.PhysicalDevice,
        config.SwapchainColorFormatOptions,
        config.SwapchainDepthFormatOptions
    );
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    setXrGraphicsBinding();

    m_XrSystem.OnXrSessionStateChangedCallbackGraphics
              .connect<&AxrVulkanXrGraphics::onXrSessionStateChangedCallback>(this);

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSetup() {
    resetSetupXrSessionGraphics();
    m_XrSystem.OnXrSessionStateChangedCallbackGraphics.reset();

    resetXrGraphicsBinding();
    resetSwapchainFormatOptions();
    m_Instance = VK_NULL_HANDLE;
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
    m_GraphicsCommandPool = VK_NULL_HANDLE;
    m_QueueFamilies.reset();
}

bool AxrVulkanXrGraphics::isReady() const {
    return m_IsReady;
}

AxrResult AxrVulkanXrGraphics::createVulkanInstance(
    const vk::InstanceCreateInfo& createInfo,
    vk::Instance& vkInstance
) const {
    return m_XrSystem.createVulkanInstance(
        vkGetInstanceProcAddr,
        createInfo,
        reinterpret_cast<VkInstance&>(vkInstance)
    );
}

AxrResult AxrVulkanXrGraphics::getVulkanPhysicalDevice(
    const vk::Instance vkInstance,
    vk::PhysicalDevice& vkPhysicalDevice
) const {
    return m_XrSystem.getVulkanPhysicalDevice(vkInstance, reinterpret_cast<VkPhysicalDevice&>(vkPhysicalDevice));
}

AxrResult AxrVulkanXrGraphics::createVulkanDevice(
    const vk::PhysicalDevice vkPhysicalDevice,
    const vk::DeviceCreateInfo& createInfo,
    vk::Device& vkDevice
) const {
    return m_XrSystem.createVulkanDevice(
        vkGetInstanceProcAddr,
        vkPhysicalDevice,
        createInfo,
        reinterpret_cast<VkDevice&>(vkDevice)
    );
}

// ---- Private Functions ----

void AxrVulkanXrGraphics::setXrGraphicsBinding() const {
    if (!m_QueueFamilies.GraphicsQueueFamilyIndex.has_value()) {
        axrLogErrorLocation("Graphics queue family index is null.");
        return;
    }

    const XrGraphicsBindingVulkan2KHR graphicsBinding{
        .type = XR_TYPE_GRAPHICS_BINDING_VULKAN2_KHR,
        .next = nullptr,
        .instance = m_Instance,
        .physicalDevice = m_PhysicalDevice,
        .device = m_Device,
        .queueFamilyIndex = m_QueueFamilies.GraphicsQueueFamilyIndex.value(),
        .queueIndex = 0
    };
    m_XrSystem.setGraphicsBinding(graphicsBinding);
}

void AxrVulkanXrGraphics::resetXrGraphicsBinding() const {
    m_XrSystem.resetGraphicsBinding();
}

AxrResult AxrVulkanXrGraphics::setupXrSessionGraphics() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = setSwapchainFormats();
    if (AXR_FAILED(axrResult)) {
        resetSetupXrSessionGraphics();
        return axrResult;
    }

    axrResult = createRenderPass();
    if (AXR_FAILED(axrResult)) {
        resetSetupXrSessionGraphics();
        return axrResult;
    }

    axrResult = setupAllViews();
    if (AXR_FAILED(axrResult)) {
        resetSetupXrSessionGraphics();
        return axrResult;
    }

    m_IsReady = true;
    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSetupXrSessionGraphics() {
    m_IsReady = false;

    resetSetupAllViews();
    destroyRenderPass();
    resetSwapchainFormats();
}

AxrResult AxrVulkanXrGraphics::setSwapchainFormatOptions(
    const vk::PhysicalDevice& physicalDevice,
    const std::vector<vk::Format>& swapchainColorFormatOptions,
    const std::vector<vk::Format>& swapchainDepthFormatOptions
) {
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

    if (swapchainColorFormatOptions.empty()) {
        axrLogErrorLocation("Swapchain color format options are empty.");
        return AXR_ERROR;
    }

    if (swapchainDepthFormatOptions.empty()) {
        axrLogErrorLocation("Swapchain depth format options are empty.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    for (const vk::Format format : swapchainColorFormatOptions) {
        if (axrAreFormatFeaturesSupported(
            format,
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eSampledImage &
            vk::FormatFeatureFlagBits::eColorAttachment,
            physicalDevice,
            m_Dispatch
        )) {
            m_SwapchainColorFormatOptions.push_back(format);
        }
    }

    for (const vk::Format format : swapchainDepthFormatOptions) {
        if (axrAreFormatFeaturesSupported(
            format,
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eSampledImage &
            vk::FormatFeatureFlagBits::eDepthStencilAttachment,
            physicalDevice,
            m_Dispatch
        )) {
            m_SwapchainDepthFormatOptions.push_back(format);
        }
    }

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSwapchainFormatOptions() {
    m_SwapchainColorFormatOptions.clear();
    m_SwapchainDepthFormatOptions.clear();
}

AxrResult AxrVulkanXrGraphics::setSwapchainFormats() {
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

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    std::vector<int64_t> supportedSwapchainFormats;
    const AxrResult axrResult = m_XrSystem.getSupportedSwapchainFormats(supportedSwapchainFormats);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to get supported swapchain formats.");
        return AXR_ERROR;
    }

    std::vector<vk::Format> supportedSwapchainVkFormats(supportedSwapchainFormats.size());
    for (size_t i = 0; i < supportedSwapchainVkFormats.size(); ++i) {
        supportedSwapchainVkFormats[i] = static_cast<vk::Format>(supportedSwapchainFormats[i]);
    }

    // ---- Find color format ----

    auto foundFormatIt = std::find_first_of(
        m_SwapchainColorFormatOptions.begin(),
        m_SwapchainColorFormatOptions.end(),
        supportedSwapchainVkFormats.begin(),
        supportedSwapchainVkFormats.end()
    );

    if (foundFormatIt == m_SwapchainColorFormatOptions.end()) {
        axrLogErrorLocation("Failed to find a supported swapchain color format.");
        resetSwapchainFormats();
        return AXR_ERROR;
    }

    m_SwapchainColorFormat = *foundFormatIt;

    // ---- Find depth format ----

    foundFormatIt = std::find_first_of(
        m_SwapchainDepthFormatOptions.begin(),
        m_SwapchainDepthFormatOptions.end(),
        supportedSwapchainVkFormats.begin(),
        supportedSwapchainVkFormats.end()
    );

    if (foundFormatIt == m_SwapchainDepthFormatOptions.end()) {
        axrLogErrorLocation("Failed to find a supported swapchain color format.");
        resetSwapchainFormats();
        return AXR_ERROR;
    }

    m_SwapchainDepthFormat = *foundFormatIt;

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSwapchainFormats() {
    m_SwapchainColorFormat = vk::Format::eUndefined;
    m_SwapchainDepthFormat = vk::Format::eUndefined;
}

AxrResult AxrVulkanXrGraphics::setupSwapchain(const AxrXrSystem::View& xrView, View& view) {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = setSwapchainExtent(xrView, view);
    if (AXR_FAILED(axrResult)) {
        resetSetupSwapchain(view);
        return axrResult;
    }

    axrResult = createSwapchain(view);
    if (AXR_FAILED(axrResult)) {
        resetSetupSwapchain(view);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSetupSwapchain(View& view) {
    destroySwapchain(view);
    resetSwapchainExtent(view);
}

AxrResult AxrVulkanXrGraphics::setSwapchainExtent(const AxrXrSystem::View& xrView, View& view) const {
    view.SwapchainExtent = vk::Extent2D(
        xrView.ViewConfigurationView.recommendedImageRectWidth,
        xrView.ViewConfigurationView.recommendedImageRectHeight
    );

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSwapchainExtent(View& view) const {
    view.SwapchainExtent = vk::Extent2D(0, 0);
}

AxrResult AxrVulkanXrGraphics::createSwapchain(View& view) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (view.ColorSwapchain.Swapchain != XR_NULL_HANDLE) {
        axrLogErrorLocation("Color swapchain already exists.");
        return AXR_ERROR;
    }

    if (view.DepthSwapchain.Swapchain != XR_NULL_HANDLE) {
        axrLogErrorLocation("Depth swapchain already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    // ---- Color swapchain ----

    axrResult = m_XrSystem.createSwapchain(
        XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT,
        static_cast<int64_t>(m_SwapchainColorFormat),
        // TODO: Multisample
        1,
        view.SwapchainExtent.width,
        view.SwapchainExtent.height,
        view.ColorSwapchain.Swapchain
    );
    if (AXR_FAILED(axrResult)) {
        destroySwapchain(view);
        return axrResult;
    }

    axrResult = setupSwapchainImages(
        view.ColorSwapchain.Swapchain,
        vk::ImageAspectFlagBits::eColor,
        m_SwapchainColorFormat,
        view.ColorSwapchain.Images,
        view.ColorSwapchain.ImageViews
    );
    if (AXR_FAILED(axrResult)) {
        destroySwapchain(view);
        return axrResult;
    }

    // ---- Depth swapchain ----

    axrResult = m_XrSystem.createSwapchain(
        XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        static_cast<int64_t>(m_SwapchainDepthFormat),
        // TODO: Multisample
        1,
        view.SwapchainExtent.width,
        view.SwapchainExtent.height,
        view.DepthSwapchain.Swapchain
    );
    if (AXR_FAILED(axrResult)) {
        destroySwapchain(view);
        return axrResult;
    }

    vk::ImageAspectFlags depthImageAspectFlags = vk::ImageAspectFlagBits::eDepth;
    if (axrFormatHasStencilComponent(m_SwapchainDepthFormat)) {
        depthImageAspectFlags |= vk::ImageAspectFlagBits::eStencil;
    }

    axrResult = setupSwapchainImages(
        view.DepthSwapchain.Swapchain,
        depthImageAspectFlags,
        m_SwapchainDepthFormat,
        view.DepthSwapchain.Images,
        view.DepthSwapchain.ImageViews
    );
    if (AXR_FAILED(axrResult)) {
        destroySwapchain(view);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::destroySwapchain(View& view) const {
    resetSetupSwapchainImages(view.DepthSwapchain.Images, view.DepthSwapchain.ImageViews);
    m_XrSystem.destroySwapchain(view.DepthSwapchain.Swapchain);
    resetSetupSwapchainImages(view.ColorSwapchain.Images, view.ColorSwapchain.ImageViews);
    m_XrSystem.destroySwapchain(view.ColorSwapchain.Swapchain);
}

AxrResult AxrVulkanXrGraphics::setupSwapchainImages(
    const XrSwapchain swapchain,
    const vk::ImageAspectFlags imageAspectFlags,
    const vk::Format imageFormat,
    std::vector<vk::Image>& images,
    std::vector<vk::ImageView>& imageViews
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (swapchain == XR_NULL_HANDLE) {
        axrLogErrorLocation("Swapchain is null.");
        return AXR_ERROR;
    }

    if (!images.empty()) {
        axrLogErrorLocation("Images already exist.");
        return AXR_ERROR;
    }

    if (!imageViews.empty()) {
        axrLogErrorLocation("Image views already exist.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = m_XrSystem.getVulkanSwapchainImages(
        swapchain,
        *reinterpret_cast<std::vector<VkImage>*>(&images)
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupSwapchainImages(images, imageViews);
        return axrResult;
    }

    imageViews.resize(images.size());
    for (int i = 0; i < images.size(); ++i) {
        axrResult = AxrVulkanImage::createImageView(
            m_Device,
            images[i],
            imageFormat,
            imageAspectFlags,
            1,
            imageViews[i],
            m_Dispatch
        );
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        resetSetupSwapchainImages(images, imageViews);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSetupSwapchainImages(
    std::vector<vk::Image>& images,
    std::vector<vk::ImageView>& imageViews
) const {
    for (vk::ImageView& imageView : imageViews) {
        AxrVulkanImage::destroyImageView(m_Device, imageView, m_Dispatch);
    }
    imageViews.clear();

    images.clear();
}

AxrResult AxrVulkanXrGraphics::createRenderPass() {
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
        m_SwapchainColorFormat,
        m_SwapchainDepthFormat,
        m_SwapchainImageLayout,
        // TODO: multisample
        vk::SampleCountFlagBits::e1,
        m_RenderPass,
        m_Dispatch
    );
}

void AxrVulkanXrGraphics::destroyRenderPass() {
    axrDestroyRenderPass(m_Device, m_RenderPass, m_Dispatch);
}

AxrResult AxrVulkanXrGraphics::setupAllViews() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_Views.empty()) {
        axrLogErrorLocation("Views already exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    const std::vector<AxrXrSystem::View> xrViews = m_XrSystem.getViews();
    m_Views.resize(xrViews.size());

    for (size_t i = 0; i < m_Views.size(); ++i) {
        axrResult = setupView(xrViews[i], m_Views[i]);
        if (AXR_FAILED(axrResult)) {
            break;
        }
    }

    if (AXR_FAILED(axrResult)) {
        resetSetupAllViews();
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSetupAllViews() {
    for (View& view : m_Views) {
        resetSetupView(view);
    }

    m_Views.clear();
}

AxrResult AxrVulkanXrGraphics::setupView(const AxrXrSystem::View& xrView, View& view) {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createSyncObjects(view);
    if (AXR_FAILED(axrResult)) {
        resetSetupView(view);
        return axrResult;
    }

    axrResult = createCommandBuffers(view);
    if (AXR_FAILED(axrResult)) {
        resetSetupView(view);
        return axrResult;
    }

    axrResult = setupSwapchain(xrView, view);
    if (AXR_FAILED(axrResult)) {
        resetSetupView(view);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSetupView(View& view) {
    resetSetupSwapchain(view);
    destroyCommandBuffers(view);
    destroySyncObjects(view);
}

AxrResult AxrVulkanXrGraphics::createSyncObjects(View& view) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!view.RenderingFinishedSemaphores.empty()) {
        axrLogErrorLocation("Rendering finished semaphores already exist.");
        return AXR_ERROR;
    }

    if (!view.RenderingFences.empty()) {
        axrLogErrorLocation("Rendering fences already exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = axrCreateSemaphores(m_Device, m_MaxFramesInFlight, view.RenderingFinishedSemaphores, m_Dispatch);
    if (AXR_FAILED(axrResult)) {
        destroySyncObjects(view);
        return axrResult;
    }

    axrResult = axrCreateFences(m_Device, m_MaxFramesInFlight, view.RenderingFences, m_Dispatch);
    if (AXR_FAILED(axrResult)) {
        destroySyncObjects(view);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::destroySyncObjects(View& view) const {
    axrDestroySemaphores(m_Device, view.RenderingFinishedSemaphores, m_Dispatch);
    axrDestroyFences(m_Device, view.RenderingFences, m_Dispatch);
}

AxrResult AxrVulkanXrGraphics::createCommandBuffers(View& view) const {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = axrCreateCommandBuffers(
        m_Device,
        m_GraphicsCommandPool,
        m_MaxFramesInFlight,
        view.RenderingCommandBuffers,
        m_Dispatch
    );
    if (AXR_FAILED(axrResult)) {
        destroyCommandBuffers(view);
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::destroyCommandBuffers(View& view) const {
    axrDestroyCommandBuffers(m_Device, m_GraphicsCommandPool, view.RenderingCommandBuffers, m_Dispatch);
}

AxrResult AxrVulkanXrGraphics::onXrSessionStateChangedCallback(const bool isSessionRunning) {
    if (isSessionRunning) {
        return setupXrSessionGraphics();
    }

    resetSetupXrSessionGraphics();
    return AXR_SUCCESS;
}

#endif
