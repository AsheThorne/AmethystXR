// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanGraphicsSystem.hpp"

// ---- Special Functions ----

AxrVulkanGraphicsSystem::AxrVulkanGraphicsSystem(const Config& config):
    m_Config(config) {
}

AxrVulkanGraphicsSystem::~AxrVulkanGraphicsSystem() {
}

// ---- Public Headers ----

AxrResult AxrVulkanGraphicsSystem::setup() {
    return AXR_ERROR;
}
