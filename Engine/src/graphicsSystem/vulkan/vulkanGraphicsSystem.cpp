// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/logger.h"
#include "vulkanGraphicsSystem.hpp"
#include "../../utils.hpp"

// ---- Special Functions ----

AxrVulkanGraphicsSystem::AxrVulkanGraphicsSystem(const Config& config):
    m_Config(config),
    m_Instance(nullptr) {
}

AxrVulkanGraphicsSystem::~AxrVulkanGraphicsSystem() {
    destroyInstance();
}

// ---- Public Functions ----

AxrResult AxrVulkanGraphicsSystem::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createInstance();
    if (AXR_FAILED(axrResult)) return axrResult;

    return axrResult;
}

// ---- Private Functions ----

AxrResult AxrVulkanGraphicsSystem::createInstance() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //
    
    if (m_Instance != nullptr) {
        axrLogErrorLocation("Instance already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    
    const vk::ApplicationInfo appInfo(
        m_Config.ApplicationName,
        m_Config.ApplicationVersion,
        AxrEngineName,
        AXR_ENGINE_VERSION,
        // TODO: OpenXR decides on the api version when that's in use
        vk::ApiVersion13
    );

    // vk::enumerateInstanceLayerProperties();
    // vk::enumerateInstanceExtensionProperties(nullptr);

    // TODO: Make these options and check against the above functions
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };
    const std::vector<const char*> extensions = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };

    const vk::InstanceCreateInfo instanceCreateInfo(
        {},
        &appInfo,
        static_cast<uint32_t>(validationLayers.size()),
        validationLayers.data(),
        static_cast<uint32_t>(extensions.size()),
        extensions.data()
    );

    const auto createInstanceResult = vk::createInstance(instanceCreateInfo, nullptr);
    axrLogVkResult(createInstanceResult.result, "vk::createInstance");

    if (axrVkFailed(createInstanceResult.result)) {
        return AXR_ERROR;
    }

    m_Instance = createInstanceResult.value;

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyInstance() {
    if (m_Instance != nullptr) {
        m_Instance.destroy();
        m_Instance = nullptr;
    }
}
