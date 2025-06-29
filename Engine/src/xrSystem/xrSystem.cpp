// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "xrSystem.hpp"
#include "axr/logger.h"
#include "xrUtils.hpp"
#include "../common.hpp"
#include "../utils.hpp"
#include "xrExtensionFunctions.hpp"

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
#include "../graphicsSystem/vulkan/vulkanUtils.hpp"
#endif

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

bool axrXrSystemIsXrSessionRunning(const AxrXrSystemConst_T xrSystem) {
    if (xrSystem == nullptr) {
        axrLogErrorLocation("`xrSystem` is null.");
        return false;
    }

    return xrSystem->isXrSessionRunning();
}

AxrResult axrXrSystemStartXrSession(const AxrXrSystem_T xrSystem) {
    if (xrSystem == nullptr) {
        axrLogErrorLocation("`xrSystem` is null.");
        return AXR_ERROR;
    }

    return xrSystem->startXrSession();
}

void axrXrSystemStopXrSession(const AxrXrSystem_T xrSystem) {
    if (xrSystem == nullptr) {
        axrLogErrorLocation("`xrSystem` is null.");
        return;
    }

    xrSystem->stopXrSession();
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

// ---- Special Functions ----

AxrXrSystem::AxrXrSystem(const Config& config):
    m_ApplicationName(config.ApplicationName),
    m_ApplicationVersion(0),
    m_GraphicsApi(config.GraphicsApi),
    m_StageReferenceSpace(config.StageReferenceSpace),
    m_Instance(XR_NULL_HANDLE),
    m_DebugUtilsMessenger(XR_NULL_HANDLE),
    m_SystemId(XR_NULL_SYSTEM_ID),
    m_SupportedViewConfigurationTypes(
        {
            XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
            XR_VIEW_CONFIGURATION_TYPE_PRIMARY_MONO,
        }
    ),
    m_ViewConfigurationType(XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM),
    m_SupportedEnvironmentBlendModes(
        {
            XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
            XR_ENVIRONMENT_BLEND_MODE_ADDITIVE,
        }
    ),
    m_EnvironmentBlendMode(XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM),
    m_GraphicsBinding(nullptr),
    m_IsSessionRunning(false),
    m_Session(XR_NULL_HANDLE),
    m_SessionState(XR_SESSION_STATE_UNKNOWN) {
    m_ApiLayers.add(config.ApiLayerCount, config.ApiLayers);
    m_Extensions.add(config.ExtensionCount, config.Extensions);

    addRequiredExtensions();
}

AxrXrSystem::~AxrXrSystem() {
    resetSetup();

    m_Extensions.clear();
    m_ApiLayers.clear();
}

// ---- Public Functions ----

bool AxrXrSystem::isXrSessionRunning() const {
    return m_IsSessionRunning;
}

AxrResult AxrXrSystem::startXrSession() {
    if (m_IsSessionRunning) {
        return AXR_SUCCESS;
    }

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = createSession();
    if (AXR_FAILED(axrResult)) {
        destroySessionData();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrXrSystem::stopXrSession() {
    if (!m_IsSessionRunning) {
        return;
    }

    if (m_Session == XR_NULL_HANDLE) {
        axrLogErrorLocation("Session is null.");
        return;
    }

    const XrResult xrResult = xrRequestExitSession(m_Session);
    axrLogXrResult(xrResult, "xrRequestExitSession");
}

AxrResult AxrXrSystem::setup() {
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

    axrResult = setSystemId();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    logSystemDetails();

    axrResult = setViewConfiguration();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    axrResult = setEnvironmentBlendMode();
    if (AXR_FAILED(axrResult)) {
        resetSetup();
        return axrResult;
    }

    return AXR_SUCCESS;
}

void AxrXrSystem::resetSetup() {
    destroySessionData();

    resetEnvironmentBlendMode();
    resetViewConfiguration();
    resetSystemId();
    destroyDebugUtils();
    destroyInstance();
}

void AxrXrSystem::processEvents() {
    XrEventDataBuffer eventData{.type = XR_TYPE_EVENT_DATA_BUFFER};
    auto xrPollEvents = [&]() -> bool {
        eventData = {.type = XR_TYPE_EVENT_DATA_BUFFER};
        return xrPollEvent(m_Instance, &eventData) == XR_SUCCESS;
    };

    while (xrPollEvents()) {
        // NOLINTNEXTLINE(clang-diagnostic-switch-enum)
        switch (eventData.type) {
            case XR_TYPE_EVENT_DATA_EVENTS_LOST: {
                xrEvent_EventsLost(*reinterpret_cast<XrEventDataEventsLost*>(&eventData));
                break;
            }
            case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
                xrEvent_InstanceLossPending(*reinterpret_cast<XrEventDataInstanceLossPending*>(&eventData));
                break;
            }
            case XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED: {
                xrEvent_InteractionProfileChanged(*reinterpret_cast<XrEventDataInteractionProfileChanged*>(&eventData));
                break;
            }
            case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING: {
                xrEvent_ReferenceSpaceChangePending(
                    *reinterpret_cast<XrEventDataReferenceSpaceChangePending*>(&eventData)
                );
                break;
            }
            case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
                xrEvent_SessionStateChanged(*reinterpret_cast<XrEventDataSessionStateChanged*>(&eventData));
                break;
            }
            default: {
                break;
            }
        }
    }
}

AxrResult AxrXrSystem::getSupportedSwapchainFormats(std::vector<int64_t>& formats) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Session == XR_NULL_HANDLE) {
        axrLogErrorLocation("Session is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t availableSwapchainFormatsCount;
    XrResult xrResult = xrEnumerateSwapchainFormats(
        m_Session,
        0,
        &availableSwapchainFormatsCount,
        nullptr
    );
    axrLogXrResult(xrResult, "xrEnumerateSwapchainFormats");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    std::vector<int64_t> availableSwapchainFormats(availableSwapchainFormatsCount);
    xrResult = xrEnumerateSwapchainFormats(
        m_Session,
        availableSwapchainFormatsCount,
        &availableSwapchainFormatsCount,
        availableSwapchainFormats.data()
    );
    axrLogXrResult(xrResult, "xrEnumerateSwapchainFormats");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    formats = availableSwapchainFormats;
    return AXR_SUCCESS;
}

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrResult AxrXrSystem::createVulkanInstance(
    const PFN_vkGetInstanceProcAddr pfnGetInstanceProcAddr,
    const VkInstanceCreateInfo& createInfo,
    VkInstance& vkInstance
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("System ID is null.");
        return AXR_ERROR;
    }

    if (createInfo.pApplicationInfo == nullptr) {
        axrLogErrorLocation("VkInstanceCreateInfo.pApplicationInfo is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t vulkanApiVersion;
    const AxrResult axrResult = chooseVulkanApiVersion(
        createInfo.pApplicationInfo->apiVersion,
        vulkanApiVersion
    );
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    const VkApplicationInfo appInfo{
        .sType = createInfo.pApplicationInfo->sType,
        .pNext = createInfo.pApplicationInfo->pNext,
        .pApplicationName = createInfo.pApplicationInfo->pApplicationName,
        .applicationVersion = createInfo.pApplicationInfo->applicationVersion,
        .pEngineName = createInfo.pApplicationInfo->pEngineName,
        .engineVersion = createInfo.pApplicationInfo->engineVersion,
        .apiVersion = vulkanApiVersion,
    };

    const VkInstanceCreateInfo vkInstanceCreateInfo{
        .sType = createInfo.sType,
        .pNext = createInfo.pNext,
        .flags = createInfo.flags,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = createInfo.enabledLayerCount,
        .ppEnabledLayerNames = createInfo.ppEnabledLayerNames,
        .enabledExtensionCount = createInfo.enabledExtensionCount,
        .ppEnabledExtensionNames = createInfo.ppEnabledExtensionNames,
    };

    const XrVulkanInstanceCreateInfoKHR xrVulkanInstanceCreateInfo{
        .type = XR_TYPE_VULKAN_INSTANCE_CREATE_INFO_KHR,
        .next = nullptr,
        .systemId = m_SystemId,
        .createFlags = {},
        .pfnGetInstanceProcAddr = pfnGetInstanceProcAddr,
        .vulkanCreateInfo = &vkInstanceCreateInfo,
        .vulkanAllocator = nullptr,
    };

    VkResult vkResult = VK_ERROR_UNKNOWN;
    const XrResult xrResult = xrCreateVulkanInstanceKHR(
        m_Instance,
        &xrVulkanInstanceCreateInfo,
        &vkInstance,
        &vkResult
    );
    axrLogXrResult(xrResult, "xrCreateVulkanInstanceKHR");
    if (XR_FAILED(xrResult) || VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrXrSystem::getVulkanPhysicalDevice(const VkInstance vkInstance, VkPhysicalDevice& vkPhysicalDevice) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("System ID is null.");
        return AXR_ERROR;
    }

    if (vkInstance == VK_NULL_HANDLE) {
        axrLogErrorLocation("vkInstance is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const XrVulkanGraphicsDeviceGetInfoKHR graphicsDeviceGetInfo{
        .type = XR_TYPE_VULKAN_GRAPHICS_DEVICE_GET_INFO_KHR,
        .next = nullptr,
        .systemId = m_SystemId,
        .vulkanInstance = vkInstance,
    };

    const XrResult xrResult = xrGetVulkanGraphicsDevice2KHR(m_Instance, &graphicsDeviceGetInfo, &vkPhysicalDevice);
    axrLogXrResult(xrResult, "xrGetVulkanGraphicsDevice2KHR");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrXrSystem::createVulkanDevice(
    const PFN_vkGetInstanceProcAddr pfnGetInstanceProcAddr,
    const VkPhysicalDevice vkPhysicalDevice,
    const VkDeviceCreateInfo& createInfo,
    VkDevice& vkDevice
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("System ID is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const XrVulkanDeviceCreateInfoKHR xrVulkanDeviceCreateInfo{
        .type = XR_TYPE_VULKAN_DEVICE_CREATE_INFO_KHR,
        .next = nullptr,
        .systemId = m_SystemId,
        .createFlags = {},
        .pfnGetInstanceProcAddr = pfnGetInstanceProcAddr,
        .vulkanPhysicalDevice = vkPhysicalDevice,
        .vulkanCreateInfo = &createInfo,
        .vulkanAllocator = nullptr,
    };

    VkResult vkResult = VK_ERROR_UNKNOWN;
    const XrResult xrResult = xrCreateVulkanDeviceKHR(m_Instance, &xrVulkanDeviceCreateInfo, &vkDevice, &vkResult);
    axrLogXrResult(xrResult, "xrCreateVulkanDeviceKHR");
    if (XR_FAILED(xrResult) || VK_FAILED(vkResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrXrSystem::setGraphicsBinding(const XrGraphicsBindingVulkan2KHR& graphicsBinding) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_GraphicsBinding != nullptr) {
        axrLogErrorLocation("Graphics binding already exists.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    m_GraphicsBinding = reinterpret_cast<XrBaseInStructure*>(new XrGraphicsBindingVulkan2KHR(graphicsBinding));
}
#endif

// ---- Private Functions ----

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
AxrResult AxrXrSystem::chooseVulkanApiVersion(const uint32_t desiredApiVersion, uint32_t& apiVersion) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("System ID is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    XrGraphicsRequirementsVulkan2KHR graphicsRequirements{
        .type = XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN2_KHR
    };
    const XrResult xrResult = xrGetVulkanGraphicsRequirements2KHR(m_Instance, m_SystemId, &graphicsRequirements);
    axrLogXrResult(xrResult, "xrGetVulkanGraphicsRequirements2KHR");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    const uint32_t minSupportedApiVersionMajor = XR_VERSION_MAJOR(graphicsRequirements.minApiVersionSupported);
    const uint32_t minSupportedApiVersionMinor = XR_VERSION_MINOR(graphicsRequirements.minApiVersionSupported);
    const uint32_t maxSupportedApiVersionMajor = XR_VERSION_MAJOR(graphicsRequirements.maxApiVersionSupported);
    const uint32_t maxSupportedApiVersionMinor = XR_VERSION_MINOR(graphicsRequirements.maxApiVersionSupported);

    const uint32_t desiredApiVersionMajor = VK_VERSION_MAJOR(desiredApiVersion);
    const uint32_t desiredApiVersionMinor = VK_VERSION_MINOR(desiredApiVersion);

    if (desiredApiVersionMajor < minSupportedApiVersionMajor ||
        desiredApiVersionMinor < minSupportedApiVersionMinor) {
        apiVersion = VK_MAKE_API_VERSION(0, minSupportedApiVersionMajor, minSupportedApiVersionMinor, 0);
        return AXR_SUCCESS;
    }

    if (desiredApiVersionMajor > maxSupportedApiVersionMajor ||
        desiredApiVersionMinor > maxSupportedApiVersionMinor) {
        apiVersion = VK_MAKE_API_VERSION(0, maxSupportedApiVersionMajor, maxSupportedApiVersionMinor, 0);
        return AXR_SUCCESS;
    }

    apiVersion = VK_MAKE_API_VERSION(0, desiredApiVersionMajor, desiredApiVersionMinor, 0);
    return AXR_SUCCESS;
}
#endif

AxrResult AxrXrSystem::createInstance() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance != XR_NULL_HANDLE) {
        axrLogWarningLocation("Instance already exists.");
        return AXR_SUCCESS;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    removeUnsupportedApiLayers();
    removeUnsupportedExtensions();

    const std::vector<const char*> layers = getAllApiLayerNames();
    const std::vector<const char*> extensions = getAllExtensionNames();
    XrBaseOutStructure* instanceStructureChain = createInstanceChain();

    XrApplicationInfo applicationInfo{};
    // TODO: Change AXR api/includes to no longer take const char* or char* for strings. Use char[MAX_SIZE] like here. 
    strncpy_s(applicationInfo.applicationName, m_ApplicationName, XR_MAX_APPLICATION_NAME_SIZE);
    applicationInfo.applicationVersion = m_ApplicationVersion;
    strncpy_s(applicationInfo.engineName, AxrEngineName, XR_MAX_ENGINE_NAME_SIZE);
    applicationInfo.engineVersion = AXR_ENGINE_VERSION;
    // If we update to a newer version, make sure we update axrToString(XrResult xrResult) to include any new results
    applicationInfo.apiVersion = XR_MAKE_VERSION(1, 0, 34);

    const XrInstanceCreateInfo instanceCreateInfo{
        .type = XR_TYPE_INSTANCE_CREATE_INFO,
        .next = instanceStructureChain,
        .createFlags = {},
        .applicationInfo = applicationInfo,
        .enabledApiLayerCount = static_cast<uint32_t>(layers.size()),
        .enabledApiLayerNames = layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .enabledExtensionNames = extensions.data(),
    };

    const XrResult xrResult = xrCreateInstance(&instanceCreateInfo, &m_Instance);
    axrLogXrResult(xrResult, "xrCreateInstance");

    destroyChain(instanceStructureChain);

    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrXrSystem::destroyInstance() {
    if (m_Instance == XR_NULL_HANDLE) return;

    xrDestroyInstance(m_Instance);
    m_Instance = XR_NULL_HANDLE;
}

XrBaseOutStructure* AxrXrSystem::createInstanceChain() const {
    XrBaseOutStructure* chain = nullptr;

    if (m_Extensions.exists(AXR_XR_EXTENSION_TYPE_DEBUG_UTILS)) {
        const XrDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo = createDebugUtilsCreateInfo();

        chain = appendNextChain(
            chain,
            reinterpret_cast<const XrBaseInStructure*>(&debugUtilsCreateInfo),
            sizeof(debugUtilsCreateInfo)
        );
    }

    return chain;
}

void AxrXrSystem::destroyChain(XrBaseOutStructure* chain) {
    XrBaseOutStructure* currentStructure = chain;

    while (currentStructure != nullptr) {
        XrBaseOutStructure* nextStructure = currentStructure->next;
        free(currentStructure);
        currentStructure = nextStructure;
    }
}

XrBaseOutStructure* AxrXrSystem::appendNextChain(
    XrBaseOutStructure* chain,
    const XrBaseInStructure* structure,
    const size_t structureSize
) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (structure == nullptr) {
        axrLogErrorLocation("Structure is null.");
        return chain;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    // Copy the extension structure, so we can safely modify it.
    // Any structure that isn't on the end will need it's `next` variable modified to point to
    // the next extension structure.
    const auto structureCopy = static_cast<XrBaseOutStructure*>(malloc(structureSize));
    memcpy_s(structureCopy, structureSize, structure, structureSize);

    if (chain == nullptr) {
        // If it's the first structure in the chain, return it as the head of the chain
        return structureCopy;
    }

    // Get the end of the next chain
    XrBaseOutStructure* chainTail = chain;
    while (chainTail->next != nullptr) {
        chainTail = chainTail->next;
    }

    // Add the current structure to the end of the structure chain
    chainTail->next = structureCopy;

    // Return the structure chain
    return chain;
}

std::vector<const char*> AxrXrSystem::getAllApiLayerNames() const {
    std::vector<const char*> apiLayerNames;

    for (const AxrXrApiLayerConst_T apiLayer : m_ApiLayers) {
        if (apiLayer == nullptr) continue;

        apiLayerNames.push_back(axrGetXrApiLayerName(apiLayer->Type));
    }

    return apiLayerNames;
}

std::vector<const char*> AxrXrSystem::getAllExtensionNames() const {
    std::vector<const char*> extensionNames;

    for (const AxrXrExtensionConst_T extension : m_Extensions) {
        if (extension == nullptr) continue;

        extensionNames.push_back(axrGetXrExtensionName(extension->Type));
    }

    return extensionNames;
}

void AxrXrSystem::addRequiredExtensions() {
    if (m_GraphicsApi == AXR_GRAPHICS_API_VULKAN) {
        auto vulkanExtension = AxrXrExtensionVulkanEnable{};
        m_Extensions.add(reinterpret_cast<AxrXrExtension_T>(&vulkanExtension));
    }
}

std::vector<std::string> AxrXrSystem::getSupportedApiLayers() const {
    // Get the number of supported api layers
    uint32_t supportedApiLayersCount;
    XrResult xrResult = xrEnumerateApiLayerProperties(
        0,
        &supportedApiLayersCount,
        nullptr
    );
    axrLogXrResult(xrResult, "xrEnumerateApiLayerProperties");
    if (XR_FAILED(xrResult)) return {};

    // Get the supported api layers
    std::vector<XrApiLayerProperties> supportedApiLayers(
        supportedApiLayersCount,
        {.type = XR_TYPE_API_LAYER_PROPERTIES}
    );
    xrResult = xrEnumerateApiLayerProperties(
        supportedApiLayersCount,
        &supportedApiLayersCount,
        supportedApiLayers.data()
    );
    axrLogXrResult(xrResult, "xrEnumerateApiLayerProperties");
    if (XR_FAILED(xrResult)) return {};

    std::vector<std::string> supportedApiLayerNames{};
    supportedApiLayerNames.reserve(supportedApiLayers.size());

    for (const XrApiLayerProperties apiLayer : supportedApiLayers) {
        supportedApiLayerNames.emplace_back(apiLayer.layerName);
    }

    return supportedApiLayerNames;
}

std::vector<std::string> AxrXrSystem::getSupportedExtensions() const {
    // Get the number of supported extensions
    uint32_t supportedExtensionsCount;
    XrResult xrResult = xrEnumerateInstanceExtensionProperties(
        nullptr,
        0,
        &supportedExtensionsCount,
        nullptr
    );
    axrLogXrResult(xrResult, "xrEnumerateInstanceExtensionProperties");
    if (XR_FAILED(xrResult)) return {};

    // Get the supported extensions
    std::vector<XrExtensionProperties> supportedExtensions(
        supportedExtensionsCount,
        {.type = XR_TYPE_EXTENSION_PROPERTIES}
    );
    xrResult = xrEnumerateInstanceExtensionProperties(
        nullptr,
        supportedExtensionsCount,
        &supportedExtensionsCount,
        supportedExtensions.data()
    );
    axrLogXrResult(xrResult, "xrEnumerateInstanceExtensionProperties");
    if (XR_FAILED(xrResult)) return {};

    std::vector<std::string> supportedExtensionNames{};
    supportedExtensionNames.reserve(supportedExtensions.size());

    for (XrExtensionProperties extension : supportedExtensions) {
        supportedExtensionNames.emplace_back(extension.extensionName);
    }

    return supportedExtensionNames;
}

void AxrXrSystem::removeUnsupportedApiLayers() {
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

    const std::vector<std::string> supportedApiLayers = getSupportedApiLayers();

    for (auto it = m_ApiLayers.begin(); it != m_ApiLayers.end();) {
        if (*it == nullptr) {
            ++it;
            continue;
        }

        if (!axrContainsString(axrGetXrApiLayerName((*it)->Type), supportedApiLayers)) {
            axrLogWarning("Unsupported api layer: {0}", axrGetXrApiLayerName((*it)->Type));

            delete *it;
            it = m_ApiLayers.erase(it);
        } else {
            ++it;
        }
    }
}

void AxrXrSystem::removeUnsupportedExtensions() {
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

    const std::vector<std::string> supportedExtensions = getSupportedExtensions();

    // TODO: Some extensions may be required and this function should fail if it doesn't have it.
    for (auto it = m_Extensions.begin(); it != m_Extensions.end();) {
        if (*it == nullptr) {
            ++it;
            continue;
        }

        if (!axrContainsString(axrGetXrExtensionName((*it)->Type), supportedExtensions)) {
            axrLogWarning("Unsupported instance extension: {0}", axrGetXrExtensionName((*it)->Type));

            delete *it;
            it = m_Extensions.erase(it);
        } else {
            ++it;
        }
    }
}

XrDebugUtilsMessengerCreateInfoEXT AxrXrSystem::createDebugUtilsCreateInfo() const {
    const auto debugUtilsExtension = reinterpret_cast<AxrXrExtensionDebugUtils*>(
        m_Extensions.get(AXR_XR_EXTENSION_TYPE_DEBUG_UTILS)
    );

    if (debugUtilsExtension == nullptr) {
        return {};
    }

    return XrDebugUtilsMessengerCreateInfoEXT{
        .type = XR_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .next = nullptr,
        .messageSeverities = debugUtilsExtension->SeverityFlags,
        .messageTypes = debugUtilsExtension->TypeFlags,
        .userCallback = debugUtilsCallback,
    };
}

AxrResult AxrXrSystem::createDebugUtils() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_Extensions.exists(AXR_XR_EXTENSION_TYPE_DEBUG_UTILS)) {
        return AXR_SUCCESS;
    }

    if (m_DebugUtilsMessenger != XR_NULL_HANDLE) {
        axrLogErrorLocation("Debug Utils already exist.");
        return AXR_ERROR;
    }

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const XrDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo = createDebugUtilsCreateInfo();

    // Create debug utils messenger
    const XrResult xrResult = xrCreateDebugUtilsMessengerEXT(
        m_Instance,
        &debugUtilsCreateInfo,
        &m_DebugUtilsMessenger
    );

    axrLogXrResult(xrResult, "xrCreateDebugUtilsMessengerEXT");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrXrSystem::destroyDebugUtils() {
    if (m_DebugUtilsMessenger == XR_NULL_HANDLE) return;

    const XrResult xrResult = xrDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugUtilsMessenger);
    axrLogXrResult(xrResult, "xrDestroyDebugUtilsMessengerEXT");

    if (XR_SUCCEEDED(xrResult)) {
        m_DebugUtilsMessenger = XR_NULL_HANDLE;
    }
}

AxrResult AxrXrSystem::setSystemId() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId != XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("System ID already exists.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    constexpr XrSystemGetInfo systemGetInfo{
        .type = XR_TYPE_SYSTEM_GET_INFO,
        .next = nullptr,
        .formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY,
    };

    const XrResult xrResult = xrGetSystem(m_Instance, &systemGetInfo, &m_SystemId);
    axrLogXrResult(xrResult, "xrGetSystem");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrXrSystem::resetSystemId() {
    m_SystemId = XR_NULL_SYSTEM_ID;
}

void AxrXrSystem::logSystemDetails() const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("System ID is null.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    XrInstanceProperties instanceProperties{
        .type = XR_TYPE_INSTANCE_PROPERTIES,
    };

    XrResult xrResult = xrGetInstanceProperties(m_Instance, &instanceProperties);
    axrLogXrResult(xrResult, "xrGetInstanceProperties");
    if (XR_FAILED(xrResult)) {
        return;
    }

    XrSystemProperties systemProperties{
        .type = XR_TYPE_SYSTEM_PROPERTIES,
    };

    xrResult = xrGetSystemProperties(m_Instance, m_SystemId, &systemProperties);
    axrLogXrResult(xrResult, "xrGetSystemProperties");
    if (XR_FAILED(xrResult)) {
        return;
    }

    axrLogInfo(
        "OpenXR Runtime: {0} - {1}.{2}.{3} | System name: {4} | Resolution: {5}:{6}",
        instanceProperties.runtimeName,
        XR_VERSION_MAJOR(instanceProperties.runtimeVersion),
        XR_VERSION_MINOR(instanceProperties.runtimeVersion),
        XR_VERSION_PATCH(instanceProperties.runtimeVersion),
        systemProperties.systemName,
        systemProperties.graphicsProperties.maxSwapchainImageWidth,
        systemProperties.graphicsProperties.maxSwapchainImageHeight
    );
}

AxrResult AxrXrSystem::setViewConfiguration() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!m_Views.empty()) {
        axrLogErrorLocation("Views already exists.");
        return AXR_ERROR;
    }

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("System ID is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    axrResult = getViewConfigurationType(m_ViewConfigurationType);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    uint32_t viewConfigurationViewCount = 0;
    XrResult xrResult = xrEnumerateViewConfigurationViews(
        m_Instance,
        m_SystemId,
        m_ViewConfigurationType,
        0,
        &viewConfigurationViewCount,
        nullptr
    );
    axrLogXrResult(xrResult, "xrEnumerateViewConfigurationViews");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    std::vector<XrViewConfigurationView> viewConfigurationViews(
        viewConfigurationViewCount,
        {
            .type = XR_TYPE_VIEW_CONFIGURATION_VIEW
        }
    );

    xrResult = xrEnumerateViewConfigurationViews(
        m_Instance,
        m_SystemId,
        m_ViewConfigurationType,
        viewConfigurationViewCount,
        &viewConfigurationViewCount,
        viewConfigurationViews.data()
    );
    axrLogXrResult(xrResult, "xrEnumerateViewConfigurationViews");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    m_Views = std::vector<View>(viewConfigurationViews.size());
    for (size_t i = 0; i < viewConfigurationViews.size(); ++i) {
        m_Views[i].ViewConfigurationView = viewConfigurationViews[i];
    }

    return AXR_SUCCESS;
}

void AxrXrSystem::resetViewConfiguration() {
    m_Views.clear();
    m_ViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM;
}

AxrResult AxrXrSystem::getViewConfigurationType(XrViewConfigurationType& viewConfigurationType) const {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("SystemId is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t viewConfigurationCount = 0;
    XrResult xrResult = xrEnumerateViewConfigurations(
        m_Instance,
        m_SystemId,
        0,
        &viewConfigurationCount,
        nullptr
    );
    axrLogXrResult(xrResult, "xrEnumerateViewConfigurations");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    std::vector<XrViewConfigurationType> viewConfigurations(viewConfigurationCount);
    xrResult = xrEnumerateViewConfigurations(
        m_Instance,
        m_SystemId,
        viewConfigurationCount,
        &viewConfigurationCount,
        viewConfigurations.data()
    );
    axrLogXrResult(xrResult, "xrEnumerateViewConfigurations");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    auto foundViewConfiguration = std::find_first_of(
        viewConfigurations.begin(),
        viewConfigurations.end(),
        m_SupportedViewConfigurationTypes.begin(),
        m_SupportedViewConfigurationTypes.end()
    );

    if (foundViewConfiguration != viewConfigurations.end()) {
        viewConfigurationType = *foundViewConfiguration;
        return AXR_SUCCESS;
    }

    axrLogErrorLocation("Failed to find a supported view configuration type.");
    return AXR_ERROR;
}

AxrResult AxrXrSystem::setEnvironmentBlendMode() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_EnvironmentBlendMode != XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM) {
        axrLogErrorLocation("Environment blend mode already exists.");
        return AXR_ERROR;
    }

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("SystemId is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    uint32_t environmentBlendModesCount;
    XrResult xrResult = xrEnumerateEnvironmentBlendModes(
        m_Instance,
        m_SystemId,
        m_ViewConfigurationType,
        0,
        &environmentBlendModesCount,
        nullptr
    );
    axrLogXrResult(xrResult, "xrEnumerateEnvironmentBlendModes");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    std::vector<XrEnvironmentBlendMode> environmentBlendModes(environmentBlendModesCount);
    xrResult = xrEnumerateEnvironmentBlendModes(
        m_Instance,
        m_SystemId,
        m_ViewConfigurationType,
        environmentBlendModesCount,
        &environmentBlendModesCount,
        environmentBlendModes.data()
    );
    axrLogXrResult(xrResult, "xrEnumerateEnvironmentBlendModes");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    const auto foundEnvironmentBlendMode = std::find_first_of(
        environmentBlendModes.begin(),
        environmentBlendModes.end(),
        m_SupportedEnvironmentBlendModes.begin(),
        m_SupportedEnvironmentBlendModes.end()
    );

    if (foundEnvironmentBlendMode != environmentBlendModes.end()) {
        m_EnvironmentBlendMode = *foundEnvironmentBlendMode;
        return AXR_SUCCESS;
    }

    axrLogErrorLocation("Failed to find a supported environment blend mode.");
    return AXR_ERROR;
}

void AxrXrSystem::resetEnvironmentBlendMode() {
    m_EnvironmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM;
}

void AxrXrSystem::destroyGraphicsBinding() {
    if (m_GraphicsBinding == nullptr) return;

    delete m_GraphicsBinding;
    m_GraphicsBinding = nullptr;
}

void AxrXrSystem::destroySessionData() {
    destroySession();
    m_IsSessionRunning = false;
    OnXrSessionStateChangedCallbackGraphics(false);
}

AxrResult AxrXrSystem::createSession() {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (m_Session != XR_NULL_HANDLE) {
        axrLogErrorLocation("Session already exists.");
        return AXR_ERROR;
    }

    if (m_Instance == XR_NULL_HANDLE) {
        axrLogErrorLocation("Instance is null.");
        return AXR_ERROR;
    }

    if (m_SystemId == XR_NULL_SYSTEM_ID) {
        axrLogErrorLocation("System ID is null.");
        return AXR_ERROR;
    }

    if (m_GraphicsBinding == nullptr) {
        axrLogErrorLocation("Graphics binding is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const XrSessionCreateInfo sessionCreateInfo{
        .type = XR_TYPE_SESSION_CREATE_INFO,
        .next = m_GraphicsBinding,
        .createFlags = {},
        .systemId = m_SystemId,
    };

    const XrResult xrResult = xrCreateSession(m_Instance, &sessionCreateInfo, &m_Session);
    axrLogXrResult(xrResult, "xrCreateSession");
    if (XR_FAILED(xrResult)) {
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

void AxrXrSystem::destroySession() {
    if (m_Session == XR_NULL_HANDLE) return;

    const XrResult xrResult = xrDestroySession(m_Session);
    axrLogXrResult(xrResult, "xrDestroySession");

    if (XR_SUCCEEDED(xrResult)) {
        m_Session = XR_NULL_HANDLE;
    }
}

void AxrXrSystem::xrEvent_EventsLost(const XrEventDataEventsLost& eventData) {
    axrLogWarningLocation("OpenXR - Events Lost: {0}", eventData.lostEventCount);
}

void AxrXrSystem::xrEvent_InstanceLossPending(const XrEventDataInstanceLossPending& eventData) {
    axrLogWarningLocation("OpenXR - Instance Loss Pending at: {0}", eventData.lossTime);
    destroySessionData();
    resetSetup();
}

void AxrXrSystem::xrEvent_InteractionProfileChanged(const XrEventDataInteractionProfileChanged& eventData) {
    axrLogInfo("OpenXR - Interaction Profile changed.");

    if (eventData.session != m_Session) {
        axrLogWarningLocation("XrEventDataInteractionProfileChanged for unknown Session.");
        return;
    }
}

void AxrXrSystem::xrEvent_ReferenceSpaceChangePending(const XrEventDataReferenceSpaceChangePending& eventData) {
    axrLogInfo("OpenXR - Reference Space Change pending.");

    if (eventData.session != m_Session) {
        axrLogWarningLocation("XrEventDataReferenceSpaceChangePending for unknown Session.");
        return;
    }
}

void AxrXrSystem::xrEvent_SessionStateChanged(const XrEventDataSessionStateChanged& eventData) {
    if (eventData.session != m_Session) {
        axrLogWarningLocation("XrEventDataSessionStateChanged for unknown Session.");
        return;
    }

    m_SessionState = eventData.state;

    // NOLINTNEXTLINE(clang-diagnostic-switch-enum)
    switch (m_SessionState) {
        case XR_SESSION_STATE_READY: {
            XrSessionBeginInfo sessionBeginInfo{.type = XR_TYPE_SESSION_BEGIN_INFO};
            sessionBeginInfo.primaryViewConfigurationType = m_ViewConfigurationType;
            const XrResult xrResult = xrBeginSession(m_Session, &sessionBeginInfo);
            axrLogXrResult(xrResult, "xrBeginSession");
            if (XR_SUCCEEDED(xrResult)) {
                m_IsSessionRunning = true;
                OnXrSessionStateChangedCallbackGraphics(true);
            }
            break;
        }
        case XR_SESSION_STATE_STOPPING: {
            const XrResult xrResult = xrEndSession(m_Session);
            axrLogXrResult(xrResult, "xrEndSession");
            destroySessionData();
            break;
        }
        case XR_SESSION_STATE_LOSS_PENDING:
        case XR_SESSION_STATE_EXITING: {
            destroySessionData();
            break;
        }
        default: {
            break;
        }
    }
}

XrBool32 AxrXrSystem::debugUtilsCallback(
    const XrDebugUtilsMessageSeverityFlagsEXT messageSeverities,
    const XrDebugUtilsMessageTypeFlagsEXT messageTypes,
    const XrDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* userData
) {
    AxrLogLevelEnum logLevel = AXR_LOG_LEVEL_ERROR;
    const char* messageSeverityString;
    const char* messageTypeString;

    switch (messageTypes) {
        case XR_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: {
            messageTypeString = "General";
            break;
        }
        case XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: {
            messageTypeString = "Validation";
            break;
        }
        case XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: {
            messageTypeString = "Performance";
            break;
        }
        case XR_DEBUG_UTILS_MESSAGE_TYPE_CONFORMANCE_BIT_EXT: {
            messageTypeString = "Conformance";
            break;
        }
        default: {
            messageTypeString = "Unknown Type";
            break;
        }
    }

    switch (messageSeverities) {
        case XR_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            messageSeverityString = "Verbose";
            logLevel = AXR_LOG_LEVEL_INFO;
            break;
        }
        case XR_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            messageSeverityString = "Info";
            logLevel = AXR_LOG_LEVEL_INFO;
            break;
        }
        case XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            messageSeverityString = "Warning";
            logLevel = AXR_LOG_LEVEL_WARNING;
            break;
        }
        case XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            messageSeverityString = "Error";
            logLevel = AXR_LOG_LEVEL_ERROR;
            break;
        }
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            messageSeverityString = "Unknown Severity";
            logLevel = AXR_LOG_LEVEL_ERROR;
            break;
        }
    }

    axrLog(
        logLevel,
        "[OpenXR | {0} | {1}] : {2}",
        messageTypeString,
        messageSeverityString,
        pCallbackData->message
    );

    return VK_FALSE;
}
