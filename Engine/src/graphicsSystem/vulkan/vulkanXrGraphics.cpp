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
    const vk::InstanceCreateInfo* vkInstanceCreateInfo,
    vk::Instance* vkInstance
) const {
    return static_cast<vk::Result>(m_XrSystem.createVulkanInstance(
        vkGetInstanceProcAddr,
        reinterpret_cast<const VkInstanceCreateInfo*>(vkInstanceCreateInfo),
        reinterpret_cast<VkInstance*>(vkInstance)
    ));
}
#endif
