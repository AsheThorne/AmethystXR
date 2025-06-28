#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanXrGraphics.hpp"

// ---- Special Functions ----

AxrVulkanXrGraphics::AxrVulkanXrGraphics(const Config& config):
    m_XrSystem(config.XrSystem) {
}

AxrVulkanXrGraphics::~AxrVulkanXrGraphics() {
}

// ---- Public Functions ----

vk::Result AxrVulkanXrGraphics::createVulkanInstance(
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

#endif
