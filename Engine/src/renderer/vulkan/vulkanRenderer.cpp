#ifdef AXR_VULKAN_SUPPORTED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanRenderer.h"
#include "../../memory/allocator.h"
#include "../../platform/platform.h"
#include "axr/logging.h"
#include "vulkanUtils.h"

#include <cassert>

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //
// Public functions need to check AXR_VULKAN_SUPPORTED but private ones don't.
// Private functions aren't expected to be called outside of public vulkan functions that have already been checked.

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr vulkan renderer. "
AxrResult AxrVulkanRenderer::setup(Context& context, const Config& config) {
    assert(!context.IsSetup);

    if (config.VulkanConfig == nullptr) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`config.VulkanConfig` is null.");
        return AXR_ERROR_NULLPTR;
    }

    AxrResult axrResult = AXR_SUCCESS;

    populateApiLayers(config.VulkanConfig->ApiLayerCount, config.VulkanConfig->ApiLayers, context.ApiLayers);
    populateExtensions(config.VulkanConfig->ExtensionCount, config.VulkanConfig->Extensions, context.Extensions);

    axrResult = createInstance(config.ApplicationName,
                               config.ApplicationVersion,
                               context.ApiLayers,
                               context.Extensions,
                               context.Instance);
    if (AXR_FAILED(axrResult)) {
        shutDown(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create instance");
        return AXR_ERROR_FALLTHROUGH;
    }

    context.IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanRenderer::shutDown(Context& context) {
    context.ApiLayers.clear();
    context.Extensions.clear();
    destroyInstance(context.Instance);
    context.IsSetup = false;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //
// Public functions need to check AXR_VULKAN_SUPPORTED but private ones don't.
// Private functions aren't expected to be called outside of public vulkan functions that have already been checked.

void AxrVulkanRenderer::populateApiLayers(const uint32_t apiLayerCount,
                                          const AxrVulkanApiLayer apiLayers[],
                                          ApiLayersArray_T& dstApiLayers) {
    ApiLayersArray_T requestedApiLayers;

    for (uint32_t i = 0; i < apiLayerCount; ++i) {
        requestedApiLayers.pushBack(apiLayers[i]);
    }

    dstApiLayers = filterSupportedApiLayers(requestedApiLayers);
}

void AxrVulkanRenderer::populateExtensions(const uint32_t extensionCount,
                                           const AxrVulkanExtension extensions[],
                                           ExtensionsArray_T& dstExtensions) {
    ExtensionsArray_T requestedExtensions;

    for (uint32_t i = 0; i < extensionCount; ++i) {
        requestedExtensions.pushBack(extensions[i]);
    }

    const ExtensionsArray_T requiredPlatformExtensions = AxrPlatform::getRequiredVulkanExtensions();

    for (const AxrVulkanExtension& extension : requiredPlatformExtensions) {
        if (!requestedExtensions.exists(extension.Type)) {
            requestedExtensions.pushBack(extension);
        }
    }

    dstExtensions = filterSupportedInstanceExtensions(requestedExtensions);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to get supported api layers. "
AxrResult AxrVulkanRenderer::getSupportedApiLayers(AxrVector_Stack<const char*>& apiLayerNames) {
    if (!apiLayerNames.empty()) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`apiLayerNames` is not empty.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    uint32_t apiLayerCount = 0;
    VkResult vkResult = vkEnumerateInstanceLayerProperties(&apiLayerCount, nullptr);
    axrLogVkResult(vkResult, "vkEnumerateInstanceLayerProperties");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    // Don't deallocate automatically because we're creating the `apiLayerNames` vector after this with the same stack
    // allocator
    AxrVector_Stack<VkLayerProperties> layerProperties(apiLayerCount, &AxrAllocator::get().FrameAllocator, false);
    vkResult = vkEnumerateInstanceLayerProperties(&apiLayerCount, layerProperties.data());
    axrLogVkResult(vkResult, "vkEnumerateInstanceLayerProperties");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    apiLayerNames = AxrVector_Stack<const char*>(apiLayerCount, &AxrAllocator::get().FrameAllocator, false);

    for (uint32_t i = 0; i < apiLayerCount; ++i) {
        apiLayerNames.pushBack(layerProperties[i].layerName);
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to get supported extensions. "
AxrResult AxrVulkanRenderer::getSupportedInstanceExtensions(AxrVector_Stack<const char*>& extensionNames) {
    if (!extensionNames.empty()) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`extensionNames` is not empty.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    uint32_t extensionCount = 0;
    VkResult vkResult = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    axrLogVkResult(vkResult, "vkEnumerateInstanceExtensionProperties");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    // Don't deallocate automatically because we're creating the `extensionNames` vector after this with the same stack
    // allocator
    AxrVector_Stack<VkExtensionProperties> extensionProperties(extensionCount,
                                                               &AxrAllocator::get().FrameAllocator,
                                                               false);
    vkResult = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());
    axrLogVkResult(vkResult, "vkEnumerateInstanceExtensionProperties");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    extensionNames = AxrVector_Stack<const char*>(extensionCount, &AxrAllocator::get().FrameAllocator, false);

    for (uint32_t i = 0; i < extensionCount; ++i) {
        extensionNames.pushBack(extensionProperties[i].extensionName);
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to filter supported api layers. "
AxrVulkanRenderer::ApiLayersArray_T AxrVulkanRenderer::filterSupportedApiLayers(const ApiLayersArray_T& srcApiLayers) {
    AxrVector_Stack<const char*> supportedApiLayerNames;
    const AxrResult axrResult = getSupportedApiLayers(supportedApiLayerNames);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported api layers.");
        return {};
    }

    ApiLayersArray_T supportedApiLayers;
    for (const AxrVulkanApiLayer& apiLayer : srcApiLayers) {
        AxrVulkanApiLayerProperties properties = AxrVulkanApiLayerGetProperties(apiLayer.Type);

        const auto iterator = supportedApiLayerNames.findFirst(properties.Name);
        if (iterator != supportedApiLayerNames.end()) {
            supportedApiLayers.pushBack(apiLayer);
        }
    }

    return supportedApiLayers;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to filter supported instance extensions. "
AxrVulkanRenderer::ExtensionsArray_T AxrVulkanRenderer::filterSupportedInstanceExtensions(
    const ExtensionsArray_T& srcExtensions) {
    AxrVector_Stack<const char*> supportedExtensionNames;
    const AxrResult axrResult = getSupportedInstanceExtensions(supportedExtensionNames);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported instance extensions.");
        return {};
    }

    ExtensionsArray_T supportedExtensions;
    for (const AxrVulkanExtension& extension : srcExtensions) {
        AxrVulkanExtensionProperties properties = AxrVulkanExtensionGetProperties(extension.Type);
        // Ignore filtering out anything that isn't an instance extensions
        if (properties.Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) {
            supportedExtensions.pushBack(extension);
            continue;
        }

        const auto iterator = supportedExtensionNames.findFirst(properties.Name);
        if (iterator != supportedExtensionNames.end()) {
            supportedExtensions.pushBack(extension);
        } else if (extension.IsRequired) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Extension type: {} is required but isn't supported.",
                        // TODO: Get enum string
                        static_cast<uint32_t>(extension.Type));
            return {};
        }
    }

    return supportedExtensions;
}
#undef AXR_FUNCTION_FAILED_STRING

AxrVulkanRenderer::ApiLayerNamesArray_T AxrVulkanRenderer::getApiLayerNames(const ApiLayersArray_T& apiLayers) {
    ApiLayerNamesArray_T apiLayerNames;

    for (const AxrVulkanApiLayer& apiLayer : apiLayers) {
        const AxrVulkanApiLayerProperties properties = AxrVulkanApiLayerGetProperties(apiLayer.Type);

        apiLayerNames.pushBack(properties.Name);
    }

    return apiLayerNames;
}

AxrVulkanRenderer::ExtensionNamesArray_T AxrVulkanRenderer::getInstanceExtensionNames(
    const ExtensionsArray_T& extensions) {
    ExtensionNamesArray_T extensionNames;

    for (const AxrVulkanExtension& extension : extensions) {
        const AxrVulkanExtensionProperties properties = AxrVulkanExtensionGetProperties(extension.Type);

        if (properties.Level != AXR_VULKAN_EXTENSION_LEVEL_INSTANCE) {
            continue;
        }

        extensionNames.pushBack(properties.Name);
    }

    return extensionNames;
}
void AxrVulkanRenderer::appendNextPtrChain(VkBaseOutStructure* source, VkBaseOutStructure* nextStruct) {
    VkBaseOutStructure*& currentNextStruct = source->pNext;
    while (currentNextStruct != nullptr) {
        currentNextStruct = currentNextStruct->pNext;
    }

    currentNextStruct = nextStruct;
}
VkBool32 AxrVulkanRenderer::debugUtilsCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                               VkDebugUtilsMessageTypeFlagsEXT messageType,
                                               const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                               void* pUserData) {
    AxrLogLevelEnum logLevel{};
    const char* messageSeverityString{};
    const char* messageTypeString{};

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

    axrLog(logLevel, "[Vulkan | {0} | {1}] : {2}", messageTypeString, messageSeverityString, pCallbackData->pMessage);

    return VK_FALSE;
}

void AxrVulkanRenderer::logExtensionNames(const char* message,
                                          const ApiLayerNamesArray_T* apiLayerNames,
                                          const ExtensionNamesArray_T* extensionNames) {
    const auto stringSeperator = ", ";
    // ReSharper disable once CppTooWideScope
    const auto lineSeperator = "\n";
    constexpr size_t stringSeperatorLength = 2;
    constexpr size_t lineSeperatorLength = 1;

    // ReSharper disable once CppTooWideScope
    const auto apiLayersMessage = "Api Layers: ";
    constexpr size_t apiLayersMessageLength = 12;

    constexpr size_t apiLayersStringMaxLength =
        apiLayersMessageLength + (VK_MAX_EXTENSION_NAME_SIZE + stringSeperatorLength) * AxrVulkanApiLayerMaxCount +
        lineSeperatorLength;
    char apiLayersString[apiLayersStringMaxLength];
    if (apiLayerNames != nullptr) {
        strcpy(apiLayersString, apiLayersMessage);

        size_t currentStringIndex = strlen(apiLayersMessage);
        for (size_t i = 0; i < apiLayerNames->size(); ++i) {
            strncpy(apiLayersString + currentStringIndex, (*apiLayerNames)[i], VK_MAX_EXTENSION_NAME_SIZE);
            currentStringIndex += strlen((*apiLayerNames)[i]);

            // If this isn't the last string to append
            if (i + 1 < apiLayerNames->size()) {
                strcpy(apiLayersString + currentStringIndex, stringSeperator);
                currentStringIndex += strlen(stringSeperator);
            } else {
                // If it is the last string, add the line separator
                strcpy(apiLayersString + currentStringIndex, lineSeperator);
                currentStringIndex += strlen(lineSeperator);
            }
        }
    }

    // ReSharper disable once CppTooWideScope
    const auto extensionsMessage = "Extensions: ";
    constexpr size_t extensionsMessageLength = 12;

    // + 2 for the comma and space seperator characters -> ", ".
    constexpr size_t extensionsStringMaxLength =
        extensionsMessageLength + (VK_MAX_EXTENSION_NAME_SIZE + stringSeperatorLength) * AxrVulkanExtensionMaxCount;
    char extensionsString[extensionsStringMaxLength];
    if (extensionNames != nullptr) {
        strcpy(extensionsString, extensionsMessage);

        size_t currentStringIndex = strlen(extensionsMessage);
        for (size_t i = 0; i < extensionNames->size(); ++i) {
            strncpy(extensionsString + currentStringIndex, (*extensionNames)[i], VK_MAX_EXTENSION_NAME_SIZE);
            currentStringIndex += strlen((*extensionNames)[i]);

            // If this isn't the last string to append
            if (i + 1 < extensionNames->size()) {
                strcpy(extensionsString + currentStringIndex, stringSeperator);
                currentStringIndex += strlen(stringSeperator);
            }
        }
    }

    axrLogInfo("{}\n{}{}", message, apiLayersString, extensionsString);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create instance. "
AxrResult AxrVulkanRenderer::createInstance(const char applicationName[AXR_MAX_APPLICATION_NAME_SIZE],
                                            const uint32_t applicationVersion,
                                            const ApiLayersArray_T& apiLayers,
                                            const ExtensionsArray_T& extensions,
                                            VkInstance& instance) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (instance != VK_NULL_HANDLE) {
        axrLogWarning("Instance already exists.");
        return AXR_SUCCESS;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //
    AxrResult axrResult = AXR_SUCCESS;

    const VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = applicationName,
        .applicationVersion = applicationVersion,
        .pEngineName = AxrEngineName,
        .engineVersion = AXR_ENGINE_VERSION,
        // OpenXR will choose the version if this isn't available for its runtime
        .apiVersion = VK_API_VERSION_1_3,
    };

    const ApiLayerNamesArray_T apiLayerNames = getApiLayerNames(apiLayers);
    const ExtensionNamesArray_T instanceExtensionNames = getInstanceExtensionNames(extensions);

    VkInstanceCreateInfo instanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(apiLayerNames.size()),
        .ppEnabledLayerNames = apiLayerNames.data(),
        .enabledExtensionCount = static_cast<uint32_t>(instanceExtensionNames.size()),
        .ppEnabledExtensionNames = instanceExtensionNames.data(),
    };

    axrResult = createInstanceChain(extensions, instanceCreateInfo);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create instance chain.");
        return AXR_ERROR_FALLTHROUGH;
    }

    // TODO: Create vulkan instance through OpenXR if that is set up
    const VkResult vkResult = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    axrLogVkResult(vkResult, "vkCreateInstance");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    logExtensionNames("Created vulkan instance with:", &apiLayerNames, &instanceExtensionNames);

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanRenderer::destroyInstance(VkInstance& instance) {
    if (instance == VK_NULL_HANDLE)
        return;

    vkDestroyInstance(instance, nullptr);
    instance = VK_NULL_HANDLE;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to create instance chain. "
AxrResult AxrVulkanRenderer::createInstanceChain(const ExtensionsArray_T& extensions,
                                                 VkInstanceCreateInfo& instanceCreateInfo) {
    AxrResult axrResult = AXR_SUCCESS;

    if (const ExtensionsArray_T::ConstIterator iterator = extensions.find(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS);
        iterator != extensions.end()) {
        VkDebugUtilsMessengerCreateInfoEXT* debugUtilsCreateInfo{};

        // We aren't deallocating this memory manually so we can ignore the markerID
        AxrStackAllocator::MarkerID markerID;
        axrResult = AxrAllocator::get().FrameAllocator.allocateAligned(1, debugUtilsCreateInfo, markerID);
        if (AXR_FAILED(axrResult)) {
            axrLogError("Failed to allocate memory for VkDebugUtilsMessengerCreateInfoEXT.");
            return AXR_ERROR_FALLTHROUGH;
        }

        *debugUtilsCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity = iterator->DebugUtils.SeverityFlags,
            .messageType = iterator->DebugUtils.TypeFlags,
            .pfnUserCallback = debugUtilsCallback,
            .pUserData = nullptr,
        };

        appendNextPtrChain(reinterpret_cast<VkBaseOutStructure*>(&instanceCreateInfo),
                           reinterpret_cast<VkBaseOutStructure*>(debugUtilsCreateInfo));
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#endif
