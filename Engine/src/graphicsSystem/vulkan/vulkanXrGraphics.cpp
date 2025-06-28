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
    const vk::InstanceCreateInfo* vkInstanceCreateInfo,
    vk::Instance* vkInstance
) const {
    return m_XrSystem.createVulkanInstance(
        vkGetInstanceProcAddr,
        reinterpret_cast<const VkInstanceCreateInfo*>(vkInstanceCreateInfo),
        reinterpret_cast<VkInstance*>(vkInstance)
    );
}

AxrResult AxrVulkanXrGraphics::getVulkanPhysicalDevice(
    const vk::Instance vkInstance,
    vk::PhysicalDevice* vkPhysicalDevice
) const {
    return m_XrSystem.getVulkanPhysicalDevice(vkInstance, reinterpret_cast<VkPhysicalDevice*>(vkPhysicalDevice));
}

#endif
