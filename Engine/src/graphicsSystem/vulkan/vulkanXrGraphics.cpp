#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanXrGraphics.hpp"

// ---- Special Functions ----

AxrVulkanXrGraphics::AxrVulkanXrGraphics(const Config& config):
    m_XrSystem(config.XrSystem),
    m_Dispatch(config.Dispatch),
    m_Instance(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_IsReady(false) {
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
    m_QueueFamilies = config.QueueFamilies;

    setXrGraphicsBinding();

    m_XrSystem.OnXrSessionStateChangedCallbackGraphics
                  .connect<&AxrVulkanXrGraphics::onXrSessionStateChangedCallback>(this);

    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSetup() {
    resetSetupXrSessionGraphics();
    m_XrSystem.OnXrSessionStateChangedCallbackGraphics.reset();

    m_Instance = VK_NULL_HANDLE;
    m_PhysicalDevice = VK_NULL_HANDLE;
    m_Device = VK_NULL_HANDLE;
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

AxrResult AxrVulkanXrGraphics::setupXrSessionGraphics() {
    m_IsReady = true;
    return AXR_SUCCESS;
}

void AxrVulkanXrGraphics::resetSetupXrSessionGraphics() {
    m_IsReady = false;
}

AxrResult AxrVulkanXrGraphics::onXrSessionStateChangedCallback(const bool isSessionRunning) {
    if (isSessionRunning) {
        return setupXrSessionGraphics();
    }

    resetSetupXrSessionGraphics();
    return AXR_SUCCESS;
}

#endif
