#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_set>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/logger.h"
#include "../../common.hpp"
#include "vulkanGraphicsSystem.hpp"
#include "vulkanutils.hpp"
#include "../../utils.hpp"
#include "../../scene/scene.hpp"

// ---- Special Functions ----

AxrVulkanGraphicsSystem::AxrVulkanGraphicsSystem(const Config& config):
    m_ApplicationName(config.ApplicationName),
    m_ApplicationVersion(config.ApplicationVersion),
    m_GlobalAssetCollection(config.GlobalAssetCollection),
    m_SamplerAnisotropyQuality(config.SamplerAnisotropyQuality),
    m_SwapchainColorFormatOptions(
        {
            vk::SurfaceFormatKHR(vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear),
            vk::SurfaceFormatKHR(vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear),
            vk::SurfaceFormatKHR(vk::Format::eR8G8B8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear),
            vk::SurfaceFormatKHR(vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear),
        }
    ),
    m_SwapchainDepthFormatOptions(
        {
            vk::Format::eD32SfloatS8Uint,
            vk::Format::eD24UnormS8Uint,
            vk::Format::eD32Sfloat,
            vk::Format::eD16Unorm,
        }
    ),
    m_Instance(VK_NULL_HANDLE),
    m_DebugUtilsMessenger(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Device(VK_NULL_HANDLE),
    m_GraphicsCommandPool(VK_NULL_HANDLE),
    m_TransferCommandPool(VK_NULL_HANDLE),
    m_MaxFramesInFlight(2),
    m_XrGraphics(nullptr),
    m_WindowGraphics(nullptr) {
    m_Dispatch.init();

    m_ApiLayers.add(config.ApiLayerCount, config.ApiLayers);
    m_Extensions.add(config.ExtensionCount, config.Extensions);

    if (config.WindowSystem != nullptr) {
        if (config.WindowConfig != nullptr) {
            m_WindowGraphics = new AxrVulkanWindowGraphics(
                AxrVulkanWindowGraphics::Config{
                    .WindowSystem = *config.WindowSystem,
                    .Dispatch = m_Dispatch,
                    .LoadedScenes = m_LoadedScenes,
                    .MaxFramesInFlight = m_MaxFramesInFlight,
                    .PresentationMode = config.WindowConfig->PresentationMode,
                    .MaxMsaaSampleCount = config.WindowConfig->MaxMsaaSampleCount,
                }
            );
        } else {
            axrLogErrorLocation("Window config is null.");
        }
    }

    if (config.XrSystem != nullptr) {
        if (config.XrSessionConfig != nullptr) {
            m_XrGraphics = new AxrVulkanXrGraphics(
                AxrVulkanXrGraphics::Config{
                    .XrSystem = *config.XrSystem,
                    .Dispatch = m_Dispatch,
                    .LoadedScenes = m_LoadedScenes,
                    .MaxFramesInFlight = m_MaxFramesInFlight,
                    .MaxMsaaSampleCount = config.XrSessionConfig->MaxMsaaSampleCount,
                }
            );
        } else {
            axrLogErrorLocation("Xr session config is null.");
        }
    }

    addRequiredInstanceExtensions();
    addRequiredDeviceExtensions();
}

AxrVulkanGraphicsSystem::~AxrVulkanGraphicsSystem() {
    resetSetup();

    if (m_WindowGraphics != nullptr) {
        delete m_WindowGraphics;
        m_WindowGraphics = nullptr;
    }

    if (m_XrGraphics != nullptr) {
        delete m_XrGraphics;
        m_XrGraphics = nullptr;
    }

    m_Extensions.clear();
    m_ApiLayers.clear();
}

// ---- Public Functions ----

AxrResult AxrVulkanGraphicsSystem::setup() {
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createInstance();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = createDebugUtils();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = setupPhysicalDevice();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = createLogicalDevice();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = createCommandPools();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = setupSceneData();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = setupWindowGraphics();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = setupXrGraphics();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::drawFrame() const {
    AxrResult axrResult = AXR_SUCCESS;

    if (m_WindowGraphics != nullptr && m_WindowGraphics->isReady()) {
        const AxrVulkanRenderCommands windowRenderCommands(*m_WindowGraphics, m_Device, m_Dispatch);

        axrResult = renderCurrentFrame(windowRenderCommands);
        if (AXR_FAILED(axrResult)) {
            axrLogErrorLocation("Failed to render current frame.");
            return;
        }
    }

    if (m_XrGraphics != nullptr && m_XrGraphics->isReady()) {
        const AxrVulkanRenderCommands xrRenderCommands(*m_XrGraphics, m_Device, m_Dispatch);

        axrResult = renderCurrentFrame(xrRenderCommands);
        if (AXR_FAILED(axrResult)) {
            axrLogErrorLocation("Failed to render current frame.");
            return;
        }
    }
}

void AxrVulkanGraphicsSystem::setClearColor(const glm::vec4& color) const {
    if (m_WindowGraphics != nullptr) {
        m_WindowGraphics->setClearColor(color);
    }

    if (m_XrGraphics != nullptr) {
        m_XrGraphics->setClearColor(color);
    }
}

AxrResult AxrVulkanGraphicsSystem::loadScene(const AxrScene_T scene) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (scene == nullptr) {
        axrLogErrorLocation("Scene is null.");
        return AXR_ERROR;
    }

    if (!m_LoadedScenes.isSetup()) {
        axrLogErrorLocation("Scenes have not been set up.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const AxrResult axrResult = m_LoadedScenes.loadScene(scene);
    if (AXR_FAILED(axrResult)) {
        resetSetupSceneData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanGraphicsSystem::setActiveScene(const std::string& sceneName) {
    return m_LoadedScenes.setActiveScene(sceneName);
}

// ---- Private Functions ----

void AxrVulkanGraphicsSystem::resetSetup() {
    resetSetupXrGraphics();
    resetSetupWindowGraphics();
    resetSetupSceneData();
    destroyCommandPools();
    destroyLogicalDevice();
    resetPhysicalDevice();
    destroyDebugUtils();
    destroyInstance();
}

AxrResult AxrVulkanGraphicsSystem::createInstance() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != VK_NULL_HANDLE) {
        axrLogWarningLocation("Instance already exists.");
        return AXR_SUCCESS;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    const vk::ApplicationInfo appInfo(
        m_ApplicationName.c_str(),
        m_ApplicationVersion,
        AxrEngineName,
        AXR_ENGINE_VERSION,
        // OpenXR will choose the version if this isn't available for it's runtime
        vk::ApiVersion13
    );

    removeUnsupportedApiLayers();
    axrResult = removeUnsupportedInstanceExtensions();
    if (AXR_FAILED(axrResult)) return axrResult;

    const std::vector<const char*> instanceLayers = getAllApiLayerNames();
    const std::vector<const char*> instanceExtensions = getAllInstanceExtensionNames();

    vk::InstanceCreateInfo instanceCreateInfo(
        {},
        &appInfo,
        static_cast<uint32_t>(instanceLayers.size()),
        instanceLayers.data(),
        static_cast<uint32_t>(instanceExtensions.size()),
        instanceExtensions.data()
    );
    instanceCreateInfo = createInstanceChain(instanceCreateInfo).get<vk::InstanceCreateInfo>();

    if (m_XrGraphics != nullptr) {
        axrResult = m_XrGraphics->createVulkanInstance(instanceCreateInfo, m_Instance);
        if (AXR_FAILED(axrResult)) return axrResult;
    } else {
        const vk::Result vkResult = vk::createInstance(
            &instanceCreateInfo,
            nullptr,
            &m_Instance,
            m_Dispatch
        );
        axrLogVkResult(vkResult, "vk::createInstance");
        if (VK_FAILED(vkResult)) return AXR_ERROR;
    }

    m_Dispatch.init(m_Instance);

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyInstance() {
    if (m_Instance != VK_NULL_HANDLE) {
        m_Instance.destroy(nullptr, m_Dispatch);
        m_Instance = VK_NULL_HANDLE;
    }
}

AxrVulkanGraphicsSystem::InstanceChain_T AxrVulkanGraphicsSystem::createInstanceChain(
    const vk::InstanceCreateInfo& instanceCreateInfo
) const {
    InstanceChain_T chain{
        instanceCreateInfo,
        createDebugUtilsCreateInfo()
    };

    if (!m_Extensions.exists(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)) {
        chain.unlink<vk::DebugUtilsMessengerCreateInfoEXT>();
    }

    return chain;
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedInstanceApiLayers() const {
    const auto instanceLayerProperties =
        vk::enumerateInstanceLayerProperties(m_Dispatch);
    axrLogVkResult(instanceLayerProperties.result, "vk::enumerateInstanceLayerProperties");

    if (VK_FAILED(instanceLayerProperties.result)) return {};

    std::vector<std::string> supportedApiLayers(instanceLayerProperties.value.size());

    for (size_t i = 0; i < instanceLayerProperties.value.size(); ++i) {
        supportedApiLayers[i] = instanceLayerProperties.value[i].layerName.data();
    }

    return supportedApiLayers;
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedDeviceApiLayers(
    const vk::PhysicalDevice& physicalDevice
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return {};
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto deviceLayerProperties =
        physicalDevice.enumerateDeviceLayerProperties(m_Dispatch);
    axrLogVkResult(deviceLayerProperties.result, "physicalDevice.enumerateDeviceLayerProperties");

    if (VK_FAILED(deviceLayerProperties.result)) return {};

    std::vector<std::string> supportedApiLayers(deviceLayerProperties.value.size());

    for (size_t i = 0; i < deviceLayerProperties.value.size(); ++i) {
        supportedApiLayers[i] = deviceLayerProperties.value[i].layerName.data();
    }

    return supportedApiLayers;
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedInstanceExtensions() const {
    const auto instanceExtensionProperties =
        vk::enumerateInstanceExtensionProperties(nullptr, m_Dispatch);
    axrLogVkResult(instanceExtensionProperties.result, "vk::enumerateInstanceExtensionProperties");

    if (VK_FAILED(instanceExtensionProperties.result)) return {};

    std::vector<std::string> supportedExtensions(instanceExtensionProperties.value.size());

    for (size_t i = 0; i < instanceExtensionProperties.value.size(); ++i) {
        supportedExtensions[i] = instanceExtensionProperties.value[i].extensionName.data();
    }

    return supportedExtensions;
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedDeviceExtensions(
    const vk::PhysicalDevice& physicalDevice
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return {};
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const auto deviceExtensionProperties =
        physicalDevice.enumerateDeviceExtensionProperties(nullptr, m_Dispatch);
    axrLogVkResult(deviceExtensionProperties.result, "physicalDevice.enumerateDeviceExtensionProperties");

    if (VK_FAILED(deviceExtensionProperties.result)) return {};

    std::vector<std::string> supportedExtensions(deviceExtensionProperties.value.size());

    for (size_t i = 0; i < deviceExtensionProperties.value.size(); ++i) {
        supportedExtensions[i] = deviceExtensionProperties.value[i].extensionName.data();
    }

    return supportedExtensions;
}

void AxrVulkanGraphicsSystem::removeUnsupportedApiLayers() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != VK_NULL_HANDLE) {
        axrLogWarningLocation("Instance already exists. It's too late to remove api layers.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const std::vector<std::string> supportedApiLayers = getSupportedInstanceApiLayers();

    for (auto it = m_ApiLayers.begin(); it != m_ApiLayers.end();) {
        if (*it == nullptr) {
            ++it;
            continue;
        }

        if (!axrContainsString(axrGetVulkanApiLayerName((*it)->Type), supportedApiLayers)) {
            axrLogWarning("Unsupported api layer: {0}", axrGetVulkanApiLayerName((*it)->Type));

            delete *it;
            it = m_ApiLayers.erase(it);
        } else {
            ++it;
        }
    }
}

AxrResult AxrVulkanGraphicsSystem::removeUnsupportedInstanceExtensions() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != VK_NULL_HANDLE) {
        axrLogWarningLocation("Instance already exists. It's too late to remove instance extensions.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    const std::vector<std::string> supportedExtensions = getSupportedInstanceExtensions();

    for (auto it = m_Extensions.begin(); it != m_Extensions.end();) {
        if (*it == nullptr || (*it)->Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) {
            ++it;
            continue;
        }

        if (!axrContainsString(axrGetVulkanExtensionName((*it)->Type), supportedExtensions)) {
            if ((*it)->IsRequired) {
                axrLogErrorLocation(
                    "Unsupported required instance extension: {0}.",
                    axrGetVulkanExtensionName((*it)->Type)
                );
                axrResult = AXR_ERROR;
                continue;
            }

            axrLogWarning("Unsupported instance extension: {0}", axrGetVulkanExtensionName((*it)->Type));

            delete *it;
            it = m_Extensions.erase(it);
        } else {
            ++it;
        }
    }

    return axrResult;
}

AxrResult AxrVulkanGraphicsSystem::removeUnsupportedDeviceExtensions() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device != VK_NULL_HANDLE) {
        axrLogWarningLocation("Device already exists. It's too late to remove device extensions.");
        return AXR_ERROR;
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    const std::vector<std::string> supportedExtensions = getSupportedDeviceExtensions(m_PhysicalDevice);

    for (auto it = m_Extensions.begin(); it != m_Extensions.end();) {
        if (*it == nullptr || (*it)->Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE) {
            ++it;
            continue;
        }

        if (!axrContainsString(axrGetVulkanExtensionName((*it)->Type), supportedExtensions)) {
            if ((*it)->IsRequired) {
                axrLogErrorLocation(
                    "Unsupported required device extension: {0}.",
                    axrGetVulkanExtensionName((*it)->Type)
                );
                axrResult = AXR_ERROR;
                continue;
            }

            axrLogWarning("Unsupported device extension: {0}", axrGetVulkanExtensionName((*it)->Type));

            delete *it;
            it = m_Extensions.erase(it);
        } else {
            ++it;
        }
    }

    return axrResult;
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllApiLayerNames() const {
    std::vector<const char*> apiLayerNames;

    for (const AxrVulkanApiLayerConst_T apiLayer : m_ApiLayers) {
        if (apiLayer == nullptr) continue;

        apiLayerNames.push_back(axrGetVulkanApiLayerName(apiLayer->Type));
    }

    return apiLayerNames;
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllInstanceExtensionNames() const {
    std::vector<const char*> extensionNames;

    for (AxrVulkanExtensionConst_T extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) continue;

        extensionNames.push_back(axrGetVulkanExtensionName(extension->Type));
    }

    return extensionNames;
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllDeviceExtensionNames() const {
    std::vector<const char*> extensionNames;

    for (AxrVulkanExtensionConst_T extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE) continue;

        extensionNames.push_back(axrGetVulkanExtensionName(extension->Type));
    }

    return extensionNames;
}

void AxrVulkanGraphicsSystem::addRequiredInstanceExtensions() {
    if (m_WindowGraphics != nullptr) {
        m_WindowGraphics->addRequiredInstanceExtensions(m_Extensions);
    }

    // OpenXR adds it's extensions automatically when it creates the instance
}

void AxrVulkanGraphicsSystem::addRequiredDeviceExtensions() {
    if (m_WindowGraphics != nullptr) {
        m_WindowGraphics->addRequiredDeviceExtensions(m_Extensions);
    }

    // OpenXR adds it's extensions automatically when it creates the device
}

vk::DebugUtilsMessengerCreateInfoEXT AxrVulkanGraphicsSystem::createDebugUtilsCreateInfo() const {
    auto debugUtilsExtension = reinterpret_cast<AxrVulkanExtensionDebugUtils*>(
        m_Extensions.get(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)
    );

    if (debugUtilsExtension == nullptr) {
        return {};
    }

    return vk::DebugUtilsMessengerCreateInfoEXT{
        {},
        static_cast<vk::DebugUtilsMessageSeverityFlagsEXT>(debugUtilsExtension->SeverityFlags),
        static_cast<vk::DebugUtilsMessageTypeFlagBitsEXT>(debugUtilsExtension->TypeFlags),
        debugUtilsCallback
    };
}

AxrResult AxrVulkanGraphicsSystem::createDebugUtils() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_Extensions.exists(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)) {
        // Debug utils aren't needed to be created
        return AXR_SUCCESS;
    }

    if (m_DebugUtilsMessenger != VK_NULL_HANDLE) {
        axrLogWarningLocation("Debug utils have already been created.");
        return AXR_SUCCESS;
    }

    if (m_Instance == VK_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::DebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo = createDebugUtilsCreateInfo();
    const vk::Result vkResult = m_Instance.createDebugUtilsMessengerEXT(
        &debugUtilsCreateInfo,
        nullptr,
        &m_DebugUtilsMessenger,
        m_Dispatch
    );
    axrLogVkResult(vkResult, "m_Instance.createDebugUtilsMessengerEXT");
    if (VK_FAILED(vkResult)) return AXR_ERROR;

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyDebugUtils() {
    if (m_DebugUtilsMessenger == VK_NULL_HANDLE) return;

    m_Instance.destroyDebugUtilsMessengerEXT(m_DebugUtilsMessenger, nullptr, m_Dispatch);
    m_DebugUtilsMessenger = VK_NULL_HANDLE;
}

AxrResult AxrVulkanGraphicsSystem::setupPhysicalDevice() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_PhysicalDevice != VK_NULL_HANDLE) {
        axrLogWarningLocation("Physical device already setup.");
        return AXR_SUCCESS;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = pickPhysicalDevice(m_PhysicalDevice);
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to pick Physical device.");
        return AXR_ERROR;
    }

    const bool areApiLayersSupported = areApiLayersSupportedForPhysicalDevice(m_PhysicalDevice);
    if (!areApiLayersSupported) {
        axrLogWarning("Not all api layers are supported for the chosen physical device.");
    }

    axrResult = m_QueueFamilies.setQueueFamilyIndices(
        m_PhysicalDevice,
        m_Dispatch
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to set queue family indices.");
        return axrResult;
    }

    axrResult = removeUnsupportedDeviceExtensions();
    if (AXR_FAILED(axrResult)) return axrResult;

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::resetPhysicalDevice() {
    m_QueueFamilies.resetQueueFamilyIndices();
    m_PhysicalDevice = VK_NULL_HANDLE;
}

AxrResult AxrVulkanGraphicsSystem::pickPhysicalDevice(vk::PhysicalDevice& physicalDevice) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == VK_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    if (m_XrGraphics != nullptr) {
        return m_XrGraphics->getVulkanPhysicalDevice(m_Instance, physicalDevice);
    }

    const auto physicalDevices = m_Instance.enumeratePhysicalDevices(m_Dispatch);
    axrLogVkResult(physicalDevices.result, "m_Instance.enumeratePhysicalDevices");
    if (VK_FAILED(physicalDevices.result)) return AXR_ERROR;

    vk::PhysicalDevice chosenPhysicalDevice = VK_NULL_HANDLE;
    uint32_t chosenPhysicalDeviceScore = 0;

    for (const vk::PhysicalDevice& device : physicalDevices.value) {
        const uint32_t currentScore = scorePhysicalDeviceSuitability(device);

        if (currentScore > chosenPhysicalDeviceScore) {
            chosenPhysicalDeviceScore = currentScore;
            chosenPhysicalDevice = device;
        }
    }

    if (chosenPhysicalDevice == VK_NULL_HANDLE) {
        axrLogError("Failed to find a suitable physical device.");
        return AXR_ERROR;
    }

    physicalDevice = chosenPhysicalDevice;
    return AXR_SUCCESS;
}

uint32_t AxrVulkanGraphicsSystem::scorePhysicalDeviceSuitability(const vk::PhysicalDevice& physicalDevice) const {
    const uint32_t queueFamiliesScore = scorePhysicalDeviceQueueFamilies(physicalDevice);
    if (queueFamiliesScore == 0) {
        return 0;
    }

    const uint32_t apiLayersScore = scorePhysicalDeviceApiLayers(physicalDevice);
    if (apiLayersScore == 0) {
        return 0;
    }

    const uint32_t extensionsScore = scorePhysicalDeviceExtensions(physicalDevice);
    if (extensionsScore == 0) {
        return 0;
    }

    const uint32_t featuresScore = scorePhysicalDeviceFeatures(physicalDevice);
    if (featuresScore == 0) {
        return 0;
    }

    const uint32_t propertiesScore = scorePhysicalDeviceProperties(physicalDevice);
    if (featuresScore == 0) {
        return 0;
    }

    return queueFamiliesScore + apiLayersScore + extensionsScore + featuresScore + propertiesScore;
}

uint32_t AxrVulkanGraphicsSystem::scorePhysicalDeviceQueueFamilies(const vk::PhysicalDevice& physicalDevice) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return 0;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrVulkanQueueFamilies queueFamilies;
    const AxrResult axrResult = queueFamilies.setQueueFamilyIndices(
        physicalDevice,
        m_Dispatch
    );

    if (AXR_FAILED(axrResult)) {
        // Failed to find required queue families
        return 0;
    }

    if (!queueFamilies.hasDedicatedTransferQueue()) {
        // We met the minimum requirements.
        // All queue families have been found, but it's not ideal without a dedicated transfer queue family.
        return 1;
    }

    // The ideal case. We have all the queue families and a dedicated transfer queue.
    // '5' is an arbitrary value currently.
    return 5;
}

uint32_t AxrVulkanGraphicsSystem::scorePhysicalDeviceApiLayers(const vk::PhysicalDevice& physicalDevice) const {
    constexpr uint32_t maxScore = 50;

    if (m_ApiLayers.empty()) {
        // All api layers are supported. Cos there are none. duhh.
        return maxScore;
    }

    if (!areApiLayersSupportedForPhysicalDevice(physicalDevice)) {
        // We met the minimum requirements.
        // Missing api layer(s)
        return 1;
    }

    // All api layers are supported
    return maxScore;
}

uint32_t AxrVulkanGraphicsSystem::scorePhysicalDeviceExtensions(const vk::PhysicalDevice& physicalDevice) const {
    // Isolate device level extensions
    std::vector<AxrVulkanExtension_T> deviceExtensions = m_Extensions.getCollection();
    std::erase_if(
        deviceExtensions,
        [](const AxrVulkanExtensionConst_T extension) {
            return extension->Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE;
        }
    );

    constexpr uint32_t maxScore = 50;

    if (deviceExtensions.empty()) {
        return maxScore;
    }

    const std::vector<std::string> supportedExtensions = getSupportedDeviceExtensions(physicalDevice);

    // 50 is the max score if all extensions are supported.
    // So the closer we get to 50 as the final score, the more extensions were found.
    const float extensionWeightedScore = static_cast<float>(maxScore) / static_cast<float>(deviceExtensions.size());
    float score = 0.0f;

    for (const AxrVulkanExtensionConst_T extension : deviceExtensions) {
        if (axrContainsString(axrGetVulkanExtensionName(extension->Type), supportedExtensions)) {
            score += extensionWeightedScore;
        } else if (extension->IsRequired) {
            // This device ia missing a required extension so it is invalid
            return 0;
        }
    }

    // ---- We shouldn't return 0 beyond this point ----

    // We need to return at least 1 to signal that we at least meet the minimum requirements.
    return std::max(static_cast<uint32_t>(1), static_cast<uint32_t>(score));
}

uint32_t AxrVulkanGraphicsSystem::scorePhysicalDeviceFeatures(const vk::PhysicalDevice& physicalDevice) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return 0;
    }
    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t score = 0;
    const vk::PhysicalDeviceFeatures features = physicalDevice.getFeatures(m_Dispatch);

    if (features.samplerAnisotropy) {
        score += 5;
    }
    if (features.sampleRateShading) {
        score += 5;
    }

    return score;
}

uint32_t AxrVulkanGraphicsSystem::scorePhysicalDeviceProperties(const vk::PhysicalDevice& physicalDevice) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return 0;
    }
    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t score = 0;
    const vk::PhysicalDeviceProperties properties = physicalDevice.getProperties(m_Dispatch);

    if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
        score += 1000;
    } else {
        score += 1;
    }

    return score;
}

bool AxrVulkanGraphicsSystem::areApiLayersSupportedForPhysicalDevice(const vk::PhysicalDevice& physicalDevice) const {
    const std::vector<std::string> supportedApiLayers = getSupportedDeviceApiLayers(physicalDevice);

    for (const AxrVulkanApiLayerConst_T apiLayer : m_ApiLayers) {
        if (!axrContainsString(axrGetVulkanApiLayerName(apiLayer->Type), supportedApiLayers)) {
            // Api layer isn't supported
            return false;
        }
    }

    return true;
}

float AxrVulkanGraphicsSystem::getMaxSamplerAnisotropyValue(
    const AxrSamplerAnisotropyQualityEnum anisotropyQuality
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return 1.0f;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::PhysicalDeviceProperties physicalDeviceProperties = m_PhysicalDevice.getProperties(m_Dispatch);

    switch (anisotropyQuality) {
        case AXR_SAMPLER_ANISOTROPY_QUALITY_LOW: {
            return physicalDeviceProperties.limits.maxSamplerAnisotropy / 4.0f;
        }
        case AXR_SAMPLER_ANISOTROPY_QUALITY_MEDIUM: {
            return physicalDeviceProperties.limits.maxSamplerAnisotropy / 2.0f;
        }
        case AXR_SAMPLER_ANISOTROPY_QUALITY_HIGH: {
            return physicalDeviceProperties.limits.maxSamplerAnisotropy;
        }
        default: {
            axrLogErrorLocation("Unknown Anisotropy Quality.");
            // Don't return or break here. Just fall through to the AXR_ANISOTROPY_QUALITY_NONE value
        }
        case AXR_SAMPLER_ANISOTROPY_QUALITY_NONE: {
            return 1.0f;
        }
    }
}

AxrResult AxrVulkanGraphicsSystem::createLogicalDevice() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device != VK_NULL_HANDLE) {
        axrLogWarningLocation("Device already exists.");
        return AXR_SUCCESS;
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return AXR_ERROR;
    }

    if (!m_QueueFamilies.areIndicesValid()) {
        axrLogErrorLocation("Queue family indices are not valid.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const std::unordered_set<uint32_t> uniqueQueueFamilyIndices = m_QueueFamilies.getUniqueQueueFamilyIndices();
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos(uniqueQueueFamilyIndices.size());

    uint32_t index = 0;
    constexpr float queuePriority = 1.0f;
    for (uint32_t queueFamilyIndex : uniqueQueueFamilyIndices) {
        vk::DeviceQueueCreateInfo queueCreateInfo(
            {},
            queueFamilyIndex,
            1,
            &queuePriority
        );
        queueCreateInfos[index] = queueCreateInfo;

        ++index;
    }

    std::vector<const char*> deviceLayers;
    const std::vector<const char*> deviceExtensions = getAllDeviceExtensionNames();

    // If we're missing even one api layer, then pass none to the device create info.
    // We need to either pass all api layers that were passed during instance creation, or none.
    // Reference: "https://docs.vulkan.org/spec/latest/chapters/extensions.html#extendingvulkan-layers-devicelayerdeprecation"
    //
    // "The ppEnabledLayerNames and enabledLayerCount members of VkDeviceCreateInfo are deprecated and their values
    // must be ignored by implementations.
    // However, for compatibility, only an empty list of layers or a list that exactly matches the sequence enabled
    // at instance creation time are valid"
    if (areApiLayersSupportedForPhysicalDevice(m_PhysicalDevice)) {
        deviceLayers = getAllApiLayerNames();
    }

    vk::PhysicalDeviceFeatures supportedDeviceFeatures = m_PhysicalDevice.getFeatures(m_Dispatch);
    vk::PhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = supportedDeviceFeatures.samplerAnisotropy;
    deviceFeatures.sampleRateShading = supportedDeviceFeatures.sampleRateShading;

    vk::DeviceCreateInfo deviceCreateInfo(
        {},
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data(),
        static_cast<uint32_t>(deviceLayers.size()),
        deviceLayers.data(),
        static_cast<uint32_t>(deviceExtensions.size()),
        deviceExtensions.data(),
        &deviceFeatures
    );
    deviceCreateInfo = createDeviceChain(deviceCreateInfo).get<vk::DeviceCreateInfo>();

    if (m_XrGraphics != nullptr) {
        const AxrResult axrResult = m_XrGraphics->createVulkanDevice(m_PhysicalDevice, deviceCreateInfo, m_Device);
        if (AXR_FAILED(axrResult)) {
            return AXR_ERROR;
        }
    } else {
        const vk::Result vkResult = m_PhysicalDevice.createDevice(
            &deviceCreateInfo,
            nullptr,
            &m_Device,
            m_Dispatch
        );
        axrLogVkResult(vkResult, "m_PhysicalDevice.createDevice");
        if (VK_FAILED(vkResult)) return AXR_ERROR;
    }

    const AxrResult axrResult = m_QueueFamilies.setQueueFamilyQueues(m_Device, m_Dispatch);
    if (AXR_FAILED(axrResult)) {
        destroyLogicalDevice();
        axrLogErrorLocation("Failed to set queue family queues.");
        return AXR_ERROR;
    }

    m_Dispatch.init(m_Device);

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyLogicalDevice() {
    m_QueueFamilies.resetQueueFamilyQueues();

    if (m_Device != VK_NULL_HANDLE) {
        m_Device.destroy(nullptr, m_Dispatch);
        m_Device = VK_NULL_HANDLE;
    }
}

AxrVulkanGraphicsSystem::DeviceChain_T AxrVulkanGraphicsSystem::createDeviceChain(
    const vk::DeviceCreateInfo& deviceCreateInfo
) const {
    DeviceChain_T chain{
        deviceCreateInfo
    };

    // No structures exist for the device next chain at the moment

    return chain;
}

AxrResult AxrVulkanGraphicsSystem::createCommandPools() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_GraphicsCommandPool != VK_NULL_HANDLE) {
        axrLogErrorLocation("Graphics command pool already exists.");
        return AXR_ERROR;
    }

    if (m_TransferCommandPool != VK_NULL_HANDLE) {
        axrLogErrorLocation("Transfer command pool already exists.");
        return AXR_ERROR;
    }

    if (!m_QueueFamilies.GraphicsQueueFamilyIndex.has_value()) {
        axrLogErrorLocation("Graphics queue family index does not exist.");
        return AXR_ERROR;
    }

    if (!m_QueueFamilies.TransferQueueFamilyIndex.has_value()) {
        axrLogErrorLocation("Transfer queue family index does not exist.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createCommandPool(
        m_QueueFamilies.GraphicsQueueFamilyIndex.value(),
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        m_GraphicsCommandPool
    );
    if (AXR_FAILED(axrResult)) {
        destroyCommandPools();
        return axrResult;
    }

    axrResult = createCommandPool(
        m_QueueFamilies.TransferQueueFamilyIndex.value(),
        vk::CommandPoolCreateFlagBits::eTransient,
        m_TransferCommandPool
    );
    if (AXR_FAILED(axrResult)) {
        destroyCommandPools();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyCommandPools() {
    destroyCommandPool(m_GraphicsCommandPool);
    destroyCommandPool(m_TransferCommandPool);
}

AxrResult AxrVulkanGraphicsSystem::createCommandPool(
    const uint32_t queueFamilyIndex,
    const vk::CommandPoolCreateFlags commandPoolFlags,
    vk::CommandPool& commandPool
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (commandPool != VK_NULL_HANDLE) {
        axrLogErrorLocation("Command pool already exists.");
        return AXR_ERROR;
    }

    if (m_Device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::CommandPoolCreateInfo commandPoolCreateInfo(
        commandPoolFlags,
        queueFamilyIndex
    );

    const vk::Result vkResult = m_Device.createCommandPool(
        &commandPoolCreateInfo,
        nullptr,
        &commandPool,
        m_Dispatch
    );
    axrLogVkResult(vkResult, "m_Device.createCommandPool");
    if (VK_FAILED(vkResult)) {
        destroyCommandPool(commandPool);
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyCommandPool(vk::CommandPool& commandPool) const {
    if (commandPool == VK_NULL_HANDLE) return;

    m_Device.destroyCommandPool(commandPool, nullptr, m_Dispatch);
    commandPool = VK_NULL_HANDLE;
}

AxrResult AxrVulkanGraphicsSystem::setupSceneData() {
    AxrResult axrResult = AXR_SUCCESS;

    const float maxAnisotropy = getMaxSamplerAnisotropyValue(m_SamplerAnisotropyQuality);

    axrResult = m_LoadedScenes.setup(
        {
            .PhysicalDevice = m_PhysicalDevice,
            .Device = m_Device,
            .GraphicsCommandPool = m_GraphicsCommandPool,
            .GraphicsQueue = m_QueueFamilies.GraphicsQueue,
            .TransferCommandPool = m_TransferCommandPool,
            .TransferQueue = m_QueueFamilies.TransferQueue,
            .MaxFramesInFlight = m_MaxFramesInFlight,
            .MaxSamplerAnisotropy = maxAnisotropy,
            .Dispatch = &m_Dispatch,
        }
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupSceneData();
        return axrResult;
    }

    // ---- Create global scene data ----

    axrResult = m_LoadedScenes.loadGlobalSceneData(
        m_GlobalAssetCollection
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupSceneData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::resetSetupSceneData() {
    m_LoadedScenes.clear();
    m_LoadedScenes.resetSetup();
}

AxrResult AxrVulkanGraphicsSystem::setupWindowGraphics() {
    // Window graphics aren't required
    if (m_WindowGraphics == nullptr) return AXR_SUCCESS;

    const AxrResult axrResult = m_WindowGraphics->setup(
        AxrVulkanWindowGraphics::SetupConfig{
            .Instance = m_Instance,
            .PhysicalDevice = m_PhysicalDevice,
            .Device = m_Device,
            .GraphicsCommandPool = m_GraphicsCommandPool,
            .QueueFamilies = m_QueueFamilies,
            .SwapchainColorFormatOptions = m_SwapchainColorFormatOptions,
            .SwapchainDepthFormatOptions = m_SwapchainDepthFormatOptions,
        }
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupWindowGraphics();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::resetSetupWindowGraphics() {
    if (m_WindowGraphics == nullptr) return;

    m_WindowGraphics->resetSetup();
}

AxrResult AxrVulkanGraphicsSystem::setupXrGraphics() {
    // Xr graphics aren't required
    if (m_XrGraphics == nullptr) return AXR_SUCCESS;

    std::vector<vk::Format> swapchainColorFormatOptions(m_SwapchainColorFormatOptions.size());
    for (uint32_t i = 0; i < m_SwapchainColorFormatOptions.size(); ++i) {
        swapchainColorFormatOptions[i] = m_SwapchainColorFormatOptions[i].format;
    }

    const AxrResult axrResult = m_XrGraphics->setup(
        AxrVulkanXrGraphics::SetupConfig{
            .Instance = m_Instance,
            .PhysicalDevice = m_PhysicalDevice,
            .Device = m_Device,
            .GraphicsCommandPool = m_GraphicsCommandPool,
            .QueueFamilies = m_QueueFamilies,
            .SwapchainColorFormatOptions = swapchainColorFormatOptions,
            .SwapchainDepthFormatOptions = m_SwapchainDepthFormatOptions,
        }
    );
    if (AXR_FAILED(axrResult)) {
        resetSetupXrGraphics();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::resetSetupXrGraphics() {
    if (m_XrGraphics == nullptr) return;

    m_XrGraphics->resetSetup();
}

template <typename RenderTarget>
AxrResult AxrVulkanGraphicsSystem::renderCurrentFrame(
    const AxrVulkanRenderCommands<RenderTarget>& renderCommands
) const {
    AxrVulkanSceneData* sceneData = m_LoadedScenes.getActiveSceneData();
    if (sceneData == nullptr) {
        // Nothing to render
        return AXR_SUCCESS;
    }

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = renderCommands.beginRendering();
    if (axrResult == AXR_DONT_RENDER) return AXR_SUCCESS;
    if (AXR_FAILED(axrResult)) return axrResult;

    for (uint32_t viewIndex = 0; viewIndex < renderCommands.getViewCount(); ++viewIndex) {
        axrResult = renderCommands.updateUniformBuffers(viewIndex, sceneData);
        if (AXR_FAILED(axrResult)) return axrResult;

        axrResult = renderCommands.waitForFrameFence(viewIndex);
        if (AXR_FAILED(axrResult)) return axrResult;

        axrResult = renderCommands.acquireNextSwapchainImage(viewIndex);
        if (axrResult == AXR_DONT_RENDER) return AXR_SUCCESS;
        if (AXR_FAILED(axrResult)) return axrResult;

        axrResult = renderCommands.resetCommandBuffer(viewIndex);
        if (AXR_FAILED(axrResult)) return axrResult;

        axrResult = renderCommands.beginCommandBuffer(viewIndex);
        if (AXR_FAILED(axrResult)) return axrResult;

        renderCommands.beginRenderPass(viewIndex);
        renderCommands.setViewport(viewIndex);
        renderCommands.setScissor(viewIndex);

        for (auto& [materialName, material] : sceneData->getMaterialsForRendering()) {
            renderCommands.bindPipeline(
                viewIndex,
                AxrVulkanRenderCommandPipelines{
                    .WindowPipeline = material.WindowPipeline,
                    .XrSessionPipeline = material.XrSessionPipeline,
                }
            );
            renderCommands.bindDescriptorSets(
                viewIndex,
                material.PipelineLayout,
                AxrVulkanRenderCommandDescriptorSets{
                    .WindowDescriptorSets = material.WindowDescriptorSets,
                    .XrSessionDescriptorSets = material.XrSessionDescriptorSets,
                }
            );
            renderCommands.pushConstants(viewIndex, material.PipelineLayout, material.PushConstant, sceneData);

            for (const AxrVulkanSceneData::MeshForRendering& mesh : material.Meshes) {
                renderCommands.pushConstants(viewIndex, material.PipelineLayout, mesh.PushConstant, sceneData);
                renderCommands.draw(viewIndex, mesh);
            }
        }

        renderCommands.endRenderPass(viewIndex);

        axrResult = renderCommands.endCommandBuffer(viewIndex);
        if (AXR_FAILED(axrResult)) return axrResult;

        axrResult = renderCommands.submitCommandBuffer(viewIndex, m_QueueFamilies.GraphicsQueue);
        if (AXR_FAILED(axrResult)) return axrResult;

        axrResult = renderCommands.presentFrame(viewIndex);
        if (axrResult == AXR_DONT_RENDER) return AXR_SUCCESS;
        if (AXR_FAILED(axrResult)) return axrResult;
    }

    axrResult = renderCommands.endRendering();
    if (AXR_FAILED(axrResult)) return axrResult;

    return AXR_SUCCESS;
}

// ---- Private Static Functions ----

VkBool32 AxrVulkanGraphicsSystem::debugUtilsCallback(
    const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    const VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    AxrLogLevelEnum logLevel = AXR_LOG_LEVEL_ERROR;
    const char* messageSeverityString;
    const char* messageTypeString;

    switch (messageType) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: {
            messageTypeString = "General";
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: {
            messageTypeString = "Validation";
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: {
            messageTypeString = "Performance";
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: {
            messageTypeString = "Device Address Binding";
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT:
        default: {
            messageTypeString = "Unknown Type";
            break;
        }
    }

    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            messageSeverityString = "Verbose";
            logLevel = AXR_LOG_LEVEL_INFO;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            messageSeverityString = "Info";
            logLevel = AXR_LOG_LEVEL_INFO;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            messageSeverityString = "Warning";
            logLevel = AXR_LOG_LEVEL_WARNING;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            messageSeverityString = "Error";
            logLevel = AXR_LOG_LEVEL_ERROR;
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            messageSeverityString = "Unknown Severity";
            logLevel = AXR_LOG_LEVEL_ERROR;
            break;
        }
    }

    axrLog(
        logLevel,
        "[Vulkan | {0} | {1}] : {2}",
        messageTypeString,
        messageSeverityString,
        pCallbackData->pMessage
    );

    return VK_FALSE;
}

#endif
