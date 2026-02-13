#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanExtensions.h"
#include <axr/logging.h>
#include "../../memory/allocator.h"
#include "../../platform/platform.h"
#include "vulkanUtils.h"

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

void AxrVulkanExtensions::populateApiLayers(const uint32_t apiLayerCount,
                                            const AxrVulkanApiLayer apiLayers[],
                                            ApiLayersArray_T& dstApiLayers) {
    ApiLayersArray_T requestedApiLayers;

    for (uint32_t i = 0; i < apiLayerCount; ++i) {
        requestedApiLayers.pushBack(apiLayers[i]);
    }

    dstApiLayers = filterSupportedApiLayers(requestedApiLayers);
}

void AxrVulkanExtensions::populateExtensions(const uint32_t extensionCount,
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

    if (!requestedExtensions.exists(AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN)) {
        requestedExtensions.pushBack(AxrVulkanExtension{
            .Swapchain = {},
            .Type = AXR_VULKAN_EXTENSION_TYPE_SWAPCHAIN,
            .IsRequired = true,
        });
    }

    dstExtensions = filterSupportedInstanceExtensions(requestedExtensions);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to get supported api layers. "
AxrResult AxrVulkanExtensions::getSupportedApiLayers(AxrVector_Stack<const char*>& apiLayerNames) {
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
    layerProperties.prefillData();
    vkResult = vkEnumerateInstanceLayerProperties(&apiLayerCount, layerProperties.data());
    axrLogVkResult(vkResult, "vkEnumerateInstanceLayerProperties");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    apiLayerNames = AxrVector_Stack<const char*>(apiLayerCount, &AxrAllocator::get().FrameAllocator);

    for (uint32_t i = 0; i < apiLayerCount; ++i) {
        apiLayerNames.pushBack(layerProperties[i].layerName);
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to get supported instance extensions. "
AxrResult AxrVulkanExtensions::getSupportedInstanceExtensions(AxrVector_Stack<const char*>& extensionNames) {
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
    extensionProperties.prefillData();
    vkResult = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());
    axrLogVkResult(vkResult, "vkEnumerateInstanceExtensionProperties");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    extensionNames = AxrVector_Stack<const char*>(extensionCount, &AxrAllocator::get().FrameAllocator);

    for (uint32_t i = 0; i < extensionCount; ++i) {
        extensionNames.pushBack(extensionProperties[i].extensionName);
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to get supported device extensions. "
AxrResult AxrVulkanExtensions::getSupportedDeviceExtensions(const VkPhysicalDevice& physicalDevice,
                                                            AxrVector_Stack<const char*>& extensionNames) {
    if (!extensionNames.empty()) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`extensionNames` is not empty.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    uint32_t extensionCount = 0;
    VkResult vkResult = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    axrLogVkResult(vkResult, "vkEnumerateDeviceExtensionProperties");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    // Don't deallocate automatically because we're creating the `extensionNames` vector after this with the same stack
    // allocator
    AxrVector_Stack<VkExtensionProperties> extensionProperties(extensionCount,
                                                               &AxrAllocator::get().FrameAllocator,
                                                               false);
    extensionProperties.prefillData();
    vkResult =
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensionProperties.data());
    axrLogVkResult(vkResult, "vkEnumerateDeviceExtensionProperties");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    extensionNames = AxrVector_Stack<const char*>(extensionCount, &AxrAllocator::get().FrameAllocator);

    for (uint32_t i = 0; i < extensionCount; ++i) {
        extensionNames.pushBack(extensionProperties[i].extensionName);
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to filter supported api layers. "
AxrVulkanExtensions::ApiLayersArray_T AxrVulkanExtensions::filterSupportedApiLayers(
    const ApiLayersArray_T& srcApiLayers) {
    AxrVector_Stack<const char*> supportedApiLayerNames;
    const AxrResult axrResult = getSupportedApiLayers(supportedApiLayerNames);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported api layers.");
        return srcApiLayers;
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
AxrVulkanExtensions::ExtensionsArray_T AxrVulkanExtensions::filterSupportedInstanceExtensions(
    const ExtensionsArray_T& srcExtensions) {
    AxrVector_Stack<const char*> supportedExtensionNames;
    const AxrResult axrResult = getSupportedInstanceExtensions(supportedExtensionNames);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported instance extensions.");
        return srcExtensions;
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
                        axrVulkanExtensionTypeEnumToString(extension.Type));
            return srcExtensions;
        }
    }

    return supportedExtensions;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to filter supported device extensions. "
AxrVulkanExtensions::ExtensionsArray_T AxrVulkanExtensions::filterSupportedDeviceExtensions(
    const VkPhysicalDevice& physicalDevice,
    const ExtensionsArray_T& srcExtensions) {
    AxrVector_Stack<const char*> supportedExtensionNames;
    const AxrResult axrResult = getSupportedDeviceExtensions(physicalDevice, supportedExtensionNames);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported device extensions.");
        return srcExtensions;
    }

    ExtensionsArray_T supportedExtensions;
    for (const AxrVulkanExtension& extension : srcExtensions) {
        AxrVulkanExtensionProperties properties = AxrVulkanExtensionGetProperties(extension.Type);
        // Ignore filtering out anything that isn't a device extensions
        if (properties.Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE) {
            supportedExtensions.pushBack(extension);
            continue;
        }

        const auto iterator = supportedExtensionNames.findFirst(properties.Name);
        if (iterator != supportedExtensionNames.end()) {
            supportedExtensions.pushBack(extension);
        } else if (extension.IsRequired) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Extension type: {} is required but isn't supported.",
                        axrVulkanExtensionTypeEnumToString(extension.Type));
            return srcExtensions;
        }
    }

    return supportedExtensions;
}
#undef AXR_FUNCTION_FAILED_STRING

AxrVulkanExtensions::ApiLayerNamesArray_T AxrVulkanExtensions::getApiLayerNames(const ApiLayersArray_T& apiLayers) {
    ApiLayerNamesArray_T apiLayerNames;

    for (const AxrVulkanApiLayer& apiLayer : apiLayers) {
        const AxrVulkanApiLayerProperties properties = AxrVulkanApiLayerGetProperties(apiLayer.Type);

        apiLayerNames.pushBack(properties.Name);
    }

    return apiLayerNames;
}

AxrVulkanExtensions::ExtensionNamesArray_T AxrVulkanExtensions::getInstanceExtensionNames(
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

AxrVulkanExtensions::ExtensionNamesArray_T AxrVulkanExtensions::getDeviceExtensionNames(
    const ExtensionsArray_T& extensions) {
    ExtensionNamesArray_T extensionNames;

    for (const AxrVulkanExtension& extension : extensions) {
        const AxrVulkanExtensionProperties properties = AxrVulkanExtensionGetProperties(extension.Type);

        if (properties.Level != AXR_VULKAN_EXTENSION_LEVEL_DEVICE) {
            continue;
        }

        extensionNames.pushBack(properties.Name);
    }

    return extensionNames;
}

void AxrVulkanExtensions::appendNextPtrChain(VkBaseOutStructure* source, VkBaseOutStructure* nextStruct) {
    VkBaseOutStructure*& currentNextStruct = source->pNext;
    while (currentNextStruct != nullptr) {
        currentNextStruct = currentNextStruct->pNext;
    }

    currentNextStruct = nextStruct;
}

void AxrVulkanExtensions::logExtensionNames(const char* message,
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

VkBool32 AxrVulkanExtensions::debugUtilsCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                 const VkDebugUtilsMessageTypeFlagsEXT messageType,
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

VkDebugUtilsMessengerCreateInfoEXT AxrVulkanExtensions::createDebugUtilsMessengerCreateInfo(
    const ExtensionsArray_T& extensions) {
    if (const ExtensionsArray_T::ConstIterator iterator = extensions.find(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS);
        iterator != extensions.end()) {
        return VkDebugUtilsMessengerCreateInfoEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity = iterator->DebugUtils.SeverityFlags,
            .messageType = iterator->DebugUtils.TypeFlags,
            .pfnUserCallback = debugUtilsCallback,
            .pUserData = nullptr,
        };
    }

    return VkDebugUtilsMessengerCreateInfoEXT{};
}

#endif
