// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "xrSystem.hpp"
#include "axr/logger.h"
#include "xrUtils.hpp"
#include "../common.hpp"
#include "../utils.hpp"
#include "xrExtensionFunctions.hpp"

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
    m_SystemId(XR_NULL_SYSTEM_ID) {
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
    return false;
}

AxrResult AxrXrSystem::startXrSession() {
    return AXR_ERROR;
}

void AxrXrSystem::stopXrSession() {
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

    return AXR_SUCCESS;
}

void AxrXrSystem::resetSetup() {
    resetSystemId();
    destroyDebugUtils();
    destroyInstance();
}

void AxrXrSystem::processEvents() {
}

// ---- Private Functions ----

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
    // If we update to a newer version, make sure we update axrToString(XrResult result) to include any new results
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

    const XrResult result = xrDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugUtilsMessenger);
    axrLogXrResult(result, "xrDestroyDebugUtilsMessengerEXT");

    m_DebugUtilsMessenger = XR_NULL_HANDLE;
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
