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

#define AXR_FUNCTION_FAILED_STRING "Failed to set up axr vulkan renderer. "
AxrResult AxrVulkanRenderer::setup(Context& context, const Config& config) {
    assert(!context.IsSetup);

    if (config.VulkanConfig == nullptr) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "`config.VulkanConfig` is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    AxrResult axrResult = AXR_SUCCESS;

    AxrVulkanExtensions::populateApiLayers(config.VulkanConfig->ApiLayerCount,
                                           config.VulkanConfig->ApiLayers,
                                           context.ApiLayers);
    AxrVulkanExtensions::populateExtensions(config.VulkanConfig->ExtensionCount,
                                            config.VulkanConfig->Extensions,
                                            context.Extensions);

    axrResult = createInstance(config.ApplicationName,
                               config.ApplicationVersion,
                               context.ApiLayers,
                               context.Extensions,
                               context.Instance);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        shutDown(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create instance");
        return axrResult;
    }

    axrResult = createDebugUtilsMessenger(context.Instance, context.Extensions, context.DebugUtilsMessenger);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        shutDown(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create debug utils.");
        return axrResult;
    }

    context.IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanRenderer::shutDown(Context& context) {
    context.ApiLayers.clear();
    context.Extensions.clear();
    destroyInstance(context.Instance);
    destroyDebugUtilsMessenger(context.Instance, context.DebugUtilsMessenger);
    context.IsSetup = false;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to create instance. "
AxrResult AxrVulkanRenderer::createInstance(const char applicationName[AXR_MAX_APPLICATION_NAME_SIZE],
                                            const uint32_t applicationVersion,
                                            const AxrVulkanExtensions::ApiLayersArray_T& apiLayers,
                                            const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                            VkInstance& instance) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (instance != VK_NULL_HANDLE) {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Instance already exists.");
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

    const AxrVulkanExtensions::ApiLayerNamesArray_T apiLayerNames = AxrVulkanExtensions::getApiLayerNames(apiLayers);
    const AxrVulkanExtensions::ExtensionNamesArray_T instanceExtensionNames =
        AxrVulkanExtensions::getInstanceExtensionNames(extensions);

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
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to create instance chain.");
        return axrResult;
    }

    // TODO: Create vulkan instance through OpenXR if that is set up
    const VkResult vkResult = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    axrLogVkResult(vkResult, "vkCreateInstance");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    AxrVulkanExtensions::logExtensionNames("Created vulkan instance with:", &apiLayerNames, &instanceExtensionNames);

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
AxrResult AxrVulkanRenderer::createInstanceChain(const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                 VkInstanceCreateInfo& instanceCreateInfo) {
    AxrResult axrResult = AXR_SUCCESS;

    if (const AxrVulkanExtensions::ExtensionsArray_T::ConstIterator iterator =
            extensions.find(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS);
        iterator != extensions.end()) {
        VkDebugUtilsMessengerCreateInfoEXT* debugUtilsCreateInfo{};

        // We aren't deallocating this memory manually so we can ignore the markerID
        AxrStackAllocator::MarkerID markerID;
        axrResult = AxrAllocator::get().FrameAllocator.allocateAligned(1, debugUtilsCreateInfo, markerID);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError("Failed to allocate memory for VkDebugUtilsMessengerCreateInfoEXT.");
            return axrResult;
        }

        *debugUtilsCreateInfo = AxrVulkanExtensions::createDebugUtilsMessengerCreateInfo(extensions);

        AxrVulkanExtensions::appendNextPtrChain(reinterpret_cast<VkBaseOutStructure*>(&instanceCreateInfo),
                                                reinterpret_cast<VkBaseOutStructure*>(debugUtilsCreateInfo));
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to create debug utils messenger. "
AxrResult AxrVulkanRenderer::createDebugUtilsMessenger(const VkInstance& instance,
                                                       const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                       VkDebugUtilsMessengerEXT& debugUtilsMessenger) {
    if (!extensions.exists(AXR_VULKAN_EXTENSION_TYPE_DEBUG_UTILS)) {
        // Don't create the debug utils messenger
        return AXR_SUCCESS;
    }

    if (debugUtilsMessenger != VK_NULL_HANDLE) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Debug utils messenger already exists");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    const VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo =
        AxrVulkanExtensions::createDebugUtilsMessengerCreateInfo(extensions);
    const VkResult vkResult =
        vkCreateDebugUtilsMessengerEXT(instance, &debugUtilsMessengerCreateInfo, nullptr, &debugUtilsMessenger);
    axrLogVkResult(vkResult, "vkCreateDebugUtilsMessengerEXT");
    if (VK_FAILED(vkResult))
        return AXR_ERROR_VULKAN_ERROR;

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanRenderer::destroyDebugUtilsMessenger(const VkInstance& instance,
                                                   VkDebugUtilsMessengerEXT& debugUtilsMessenger) {
    if (debugUtilsMessenger == VK_NULL_HANDLE)
        return;

    vkDestroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, nullptr);
    debugUtilsMessenger = VK_NULL_HANDLE;
}

#endif
