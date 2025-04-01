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

// ---- Special Functions ----

AxrVulkanGraphicsSystem::AxrVulkanGraphicsSystem(const Config& config):
    m_ApplicationName(config.ApplicationName),
    m_ApplicationVersion(config.ApplicationVersion),
    m_SwapchainColorFormatOptions(
        {
            vk::SurfaceFormatKHR(vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear),
            vk::SurfaceFormatKHR(vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear),
            vk::SurfaceFormatKHR(vk::Format::eR8G8B8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear),
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
    m_GlobalSceneAssets(
        {
            .AssetCollection = config.GlobalAssetCollection,
            .SharedVulkanSceneAssets = nullptr,
        }
    ) {
    if (config.VulkanConfig == nullptr) {
        axrLogErrorLocation("Vulkan config is null.");
        return;
    }

    m_DynamicDispatchLoader.init();

    m_ApiLayers.add(config.VulkanConfig->ApiLayersCount, config.VulkanConfig->ApiLayers);
    m_Extensions.add(config.VulkanConfig->ExtensionsCount, config.VulkanConfig->Extensions);

    addRequiredInstanceExtensions();
    addRequiredDeviceExtensions();

    if (config.WindowSystem != nullptr) {
        if (config.VulkanConfig->WindowConfig != nullptr) {
            m_WindowGraphics = new AxrVulkanWindowGraphics(
                {
                    .WindowSystem = *config.WindowSystem,
                    .Dispatch = m_DynamicDispatchLoader,
                    .PresentationMode = config.VulkanConfig->WindowConfig->PresentationMode
                }
            );
        } else {
            axrLogErrorLocation("Vulkan window config is null.");
        }
    }
}

AxrVulkanGraphicsSystem::~AxrVulkanGraphicsSystem() {
    resetSetup();

    if (m_WindowGraphics != nullptr) {
        delete m_WindowGraphics;
        m_WindowGraphics = nullptr;
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

    axrResult = m_GlobalSceneAssets.setup(
        {
            .Device = m_Device,
            .DispatchHandle = &m_DynamicDispatchLoader
        }
    );
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = m_GlobalSceneAssets.loadAssets();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    if (m_WindowGraphics != nullptr) {
        axrResult = m_WindowGraphics->setup(
            {
                .Instance = m_Instance,
                .PhysicalDevice = m_PhysicalDevice,
                .Device = m_Device,
                .QueueFamilies = m_QueueFamilies,
                .SwapchainColorFormatOptions = m_SwapchainColorFormatOptions,
                .SwapchainDepthFormatOptions = m_SwapchainDepthFormatOptions,
            }
        );
        if (AXR_FAILED(axrResult)) {
            resetSetup();
            return axrResult;
        }
    }

    return AXR_SUCCESS;
}

// ---- Private Functions ----

void AxrVulkanGraphicsSystem::resetSetup() {
    if (m_WindowGraphics != nullptr) {
        m_WindowGraphics->resetSetup();
    }

    m_GlobalSceneAssets.unloadAssets();
    m_GlobalSceneAssets.resetSetup();
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

    const vk::ApplicationInfo appInfo(
        m_ApplicationName,
        m_ApplicationVersion,
        AxrEngineName,
        AXR_ENGINE_VERSION,
        // TODO: OpenXR decides on the api version when that's in use
        vk::ApiVersion13
    );

    removeUnsupportedApiLayers();
    removeUnsupportedInstanceExtensions();

    const std::vector<const char*> instanceLayers = getAllApiLayerNames();
    const std::vector<const char*> instanceExtensions = getAllInstanceExtensionNames();

    const vk::InstanceCreateInfo instanceCreateInfo(
        {},
        &appInfo,
        static_cast<uint32_t>(instanceLayers.size()),
        instanceLayers.data(),
        static_cast<uint32_t>(instanceExtensions.size()),
        instanceExtensions.data()
    );

    const vk::Result vkResult = vk::createInstance(
        &createInstanceChain(instanceCreateInfo).get<vk::InstanceCreateInfo>(),
        nullptr,
        &m_Instance,
        m_DynamicDispatchLoader
    );
    axrLogVkResult(vkResult, "vk::createInstance");
    if (VK_FAILED(vkResult)) return AXR_ERROR;

    m_DynamicDispatchLoader.init(m_Instance);

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyInstance() {
    if (m_Instance != VK_NULL_HANDLE) {
        m_Instance.destroy(nullptr, m_DynamicDispatchLoader);
        m_Instance = VK_NULL_HANDLE;
    }
}

AxrVulkanGraphicsSystem::InstanceChain_T AxrVulkanGraphicsSystem::createInstanceChain(
    const vk::InstanceCreateInfo& instanceCreateInfo
) const {
    InstanceChain_T chain{
        instanceCreateInfo,
        createDebugUtilsCreateInto()
    };

    if (!m_Extensions.exists(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)) {
        chain.unlink<vk::DebugUtilsMessengerCreateInfoEXT>();
    }

    return chain;
}

std::vector<std::string> AxrVulkanGraphicsSystem::getSupportedInstanceApiLayers() const {
    const auto instanceLayerProperties =
        vk::enumerateInstanceLayerProperties(m_DynamicDispatchLoader);
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
        physicalDevice.enumerateDeviceLayerProperties(m_DynamicDispatchLoader);
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
        vk::enumerateInstanceExtensionProperties(nullptr, m_DynamicDispatchLoader);
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
        physicalDevice.enumerateDeviceExtensionProperties(nullptr, m_DynamicDispatchLoader);
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

        if (!axrContainsString(axrGetApiLayerName((*it)->Type), supportedApiLayers)) {
            axrLogWarning("Unsupported api layer: {0}", axrGetApiLayerName((*it)->Type));

            delete *it;
            it = m_ApiLayers.erase(it);
        } else {
            ++it;
        }
    }
}

void AxrVulkanGraphicsSystem::removeUnsupportedInstanceExtensions() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != VK_NULL_HANDLE) {
        axrLogWarningLocation("Instance already exists. It's too late to remove instance extensions.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const std::vector<std::string> supportedExtensions = getSupportedInstanceExtensions();

    // TODO: Some extensions may be required and this function should fail if it doesn't have it.
    for (auto it = m_Extensions.begin(); it != m_Extensions.end();) {
        if (*it == nullptr || (*it)->Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) {
            ++it;
            continue;
        }

        if (!axrContainsString(axrGetExtensionName((*it)->Type), supportedExtensions)) {
            axrLogWarning("Unsupported instance extension: {0}", axrGetExtensionName((*it)->Type));

            delete *it;
            it = m_Extensions.erase(it);
        } else {
            ++it;
        }
    }
}

void AxrVulkanGraphicsSystem::removeUnsupportedDeviceExtensions() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Device != VK_NULL_HANDLE) {
        axrLogWarningLocation("Device already exists. It's too late to remove device extensions.");
        return;
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const std::vector<std::string> supportedExtensions = getSupportedDeviceExtensions(m_PhysicalDevice);

    for (auto it = m_Extensions.begin(); it != m_Extensions.end();) {
        if (*it == nullptr || (*it)->Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE) {
            ++it;
            continue;
        }

        if (!axrContainsString(axrGetExtensionName((*it)->Type), supportedExtensions)) {
            axrLogWarning("Unsupported device extension: {0}", axrGetExtensionName((*it)->Type));

            delete *it;
            it = m_Extensions.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllApiLayerNames() const {
    std::vector<const char*> apiLayerNames;

    for (const AxrVulkanApiLayer_T apiLayer : m_ApiLayers) {
        if (apiLayer == nullptr) continue;

        apiLayerNames.push_back(axrGetApiLayerName(apiLayer->Type));
    }

    return apiLayerNames;
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllInstanceExtensionNames() const {
    std::vector<const char*> extensionNames;

    for (AxrVulkanExtension_T extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) continue;

        extensionNames.push_back(axrGetExtensionName(extension->Type));
    }

    return extensionNames;
}

std::vector<const char*> AxrVulkanGraphicsSystem::getAllDeviceExtensionNames() const {
    std::vector<const char*> extensionNames;

    for (AxrVulkanExtension_T extension : m_Extensions) {
        if (extension == nullptr || extension->Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE) continue;

        extensionNames.push_back(axrGetExtensionName(extension->Type));
    }

    return extensionNames;
}

void AxrVulkanGraphicsSystem::addRequiredInstanceExtensions() {
    if (m_WindowGraphics != nullptr) {
        m_WindowGraphics->addRequiredInstanceExtensions(m_Extensions);
    }

    // TODO: Add required instance extensions for OpenXR
}

void AxrVulkanGraphicsSystem::addRequiredDeviceExtensions() {
    if (m_WindowGraphics != nullptr) {
        m_WindowGraphics->addRequiredDeviceExtensions(m_Extensions);
    }

    // TODO: Add required device extensions for OpenXR
}

vk::DebugUtilsMessengerCreateInfoEXT AxrVulkanGraphicsSystem::createDebugUtilsCreateInto() const {
    auto debugUtilsExtension = reinterpret_cast<AxrVulkanExtensionDebugUtils*>(
        m_Extensions.get(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)
    );

    if (debugUtilsExtension == nullptr) {
        return {};
    }

    return {
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

    const vk::DebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo = createDebugUtilsCreateInto();
    const vk::Result vkResult = m_Instance.createDebugUtilsMessengerEXT(
        &debugUtilsCreateInfo,
        nullptr,
        &m_DebugUtilsMessenger,
        m_DynamicDispatchLoader
    );
    axrLogVkResult(vkResult, "m_Instance.createDebugUtilsMessengerEXT");
    if (VK_FAILED(vkResult)) return AXR_ERROR;

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyDebugUtils() {
    if (m_DebugUtilsMessenger == VK_NULL_HANDLE) return;

    m_Instance.destroyDebugUtilsMessengerEXT(m_DebugUtilsMessenger, nullptr, m_DynamicDispatchLoader);
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

    m_PhysicalDevice = pickPhysicalDevice();
    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Failed to pick Physical device.");
        return AXR_ERROR;
    }

    const bool areApiLayersSupported = areApiLayersSupportedForPhysicalDevice(m_PhysicalDevice);
    if (!areApiLayersSupported) {
        axrLogWarning("Not all api layers are supported for the chosen physical device.");
    }

    const AxrResult axrResult = m_QueueFamilies.setQueueFamilyIndices(
        m_PhysicalDevice,
        m_DynamicDispatchLoader
    );
    if (AXR_FAILED(axrResult)) {
        axrLogErrorLocation("Failed to set queue family indices.");
        return axrResult;
    }

    removeUnsupportedDeviceExtensions();

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::resetPhysicalDevice() {
    m_QueueFamilies.resetQueueFamilyIndices();
    m_PhysicalDevice = VK_NULL_HANDLE;
}

vk::PhysicalDevice AxrVulkanGraphicsSystem::pickPhysicalDevice() const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == VK_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return VK_NULL_HANDLE;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    // TODO: OpenXR chooses the physical device if that's set up.

    const auto physicalDevices = m_Instance.enumeratePhysicalDevices(m_DynamicDispatchLoader);
    axrLogVkResult(physicalDevices.result, "m_Instance.enumeratePhysicalDevices");
    if (VK_FAILED(physicalDevices.result)) return VK_NULL_HANDLE;

    vk::PhysicalDevice chosenPhysicalDevice = VK_NULL_HANDLE;
    uint32_t chosenPhysicalDeviceScore = 0;

    for (const vk::PhysicalDevice& physicalDevice : physicalDevices.value) {
        const uint32_t currentScore = scorePhysicalDeviceSuitability(physicalDevice);

        if (currentScore > chosenPhysicalDeviceScore) {
            chosenPhysicalDeviceScore = currentScore;
            chosenPhysicalDevice = physicalDevice;
        }
    }

    if (chosenPhysicalDevice == VK_NULL_HANDLE) {
        axrLogError("Failed to find a suitable physical device.");
        return VK_NULL_HANDLE;
    }

    return chosenPhysicalDevice;
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
        m_DynamicDispatchLoader
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
        [](const AxrVulkanExtension_T extension) {
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

    for (const AxrVulkanExtension_T extension : deviceExtensions) {
        // TODO: Some extensions may be required and this function should fail if it doesn't have it.
        if (axrContainsString(axrGetExtensionName(extension->Type), supportedExtensions)) {
            score += extensionWeightedScore;
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
    const vk::PhysicalDeviceFeatures features = physicalDevice.getFeatures(m_DynamicDispatchLoader);

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
    const vk::PhysicalDeviceProperties properties = physicalDevice.getProperties(m_DynamicDispatchLoader);

    if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
        score += 1000;
    } else {
        score += 1;
    }

    return score;
}

bool AxrVulkanGraphicsSystem::areApiLayersSupportedForPhysicalDevice(const vk::PhysicalDevice& physicalDevice) const {
    const std::vector<std::string> supportedApiLayers = getSupportedDeviceApiLayers(physicalDevice);

    for (const AxrVulkanApiLayer_T apiLayer : m_ApiLayers) {
        if (!axrContainsString(axrGetApiLayerName(apiLayer->Type), supportedApiLayers)) {
            // Api layer isn't supported
            return false;
        }
    }

    return true;
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

    vk::PhysicalDeviceFeatures supportedDeviceFeatures = m_PhysicalDevice.getFeatures(m_DynamicDispatchLoader);
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

    const vk::Result vkResult = m_PhysicalDevice.createDevice(
        &createDeviceChain(deviceCreateInfo).get<vk::DeviceCreateInfo>(),
        nullptr,
        &m_Device,
        m_DynamicDispatchLoader
    );
    axrLogVkResult(vkResult, "m_PhysicalDevice.createDevice");
    if (VK_FAILED(vkResult)) return AXR_ERROR;

    const AxrResult axrResult = m_QueueFamilies.setQueueFamilyQueues(m_Device, m_DynamicDispatchLoader);
    if (AXR_FAILED(axrResult)) {
        destroyLogicalDevice();
        axrLogErrorLocation("Failed to set queue family queues.");
        return AXR_ERROR;
    }

    m_DynamicDispatchLoader.init(m_Device);

    return AXR_SUCCESS;
}

void AxrVulkanGraphicsSystem::destroyLogicalDevice() {
    m_QueueFamilies.resetQueueFamilyQueues();

    if (m_Device != VK_NULL_HANDLE) {
        m_Device.destroy(nullptr, m_DynamicDispatchLoader);
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
