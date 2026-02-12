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

#include "axr/vulkanApi.h"

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

    axrResult =
        setupPhysicalDevice(context.Instance, context.Extensions, context.QueueFamilies, context.PhysicalDevice);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        shutDown(context);
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to set up physical device.");
        return axrResult;
    }

    context.IsSetup = true;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanRenderer::shutDown(Context& context) {
    resetPhysicalDevice(context.QueueFamilies, context.PhysicalDevice);
    destroyDebugUtilsMessenger(context.Instance, context.DebugUtilsMessenger);
    destroyInstance(context.Instance);
    context.Extensions.clear();
    context.ApiLayers.clear();

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

    if (instance != VK_NULL_HANDLE) [[unlikely]] {
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
    if (VK_FAILED(vkResult)) [[unlikely]]
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

    if (debugUtilsMessenger != VK_NULL_HANDLE) [[unlikely]] {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Debug utils messenger already exists.");
        return AXR_SUCCESS;
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

#define AXR_FUNCTION_FAILED_STRING "Failed to set up physical device. "
AxrResult AxrVulkanRenderer::setupPhysicalDevice(const VkInstance& instance,
                                                 AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                 AxrVulkanQueueFamilies& queueFamilies,
                                                 VkPhysicalDevice& physicalDevice) {
    if (physicalDevice != VK_NULL_HANDLE) [[unlikely]] {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Physical device has already been set.");
        return AXR_SUCCESS;
    }

    AxrResult axrResult = AXR_SUCCESS;

    axrResult = pickPhysicalDevice(instance, extensions, physicalDevice);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to pick a physical device.");
        return axrResult;
    }

    axrResult = queueFamilies.setQueueFamilyIndices(instance, physicalDevice);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to set queue family indices.");
        return axrResult;
    }

    extensions = AxrVulkanExtensions::filterSupportedDeviceExtensions(physicalDevice, extensions);

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanRenderer::resetPhysicalDevice(AxrVulkanQueueFamilies& queueFamilies, VkPhysicalDevice& physicalDevice) {
    queueFamilies.reset();
    physicalDevice = VK_NULL_HANDLE;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to pick a physical device. "
AxrResult AxrVulkanRenderer::pickPhysicalDevice(const VkInstance& instance,
                                                const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                VkPhysicalDevice& physicalDevice) {
    if (physicalDevice != VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device already exists.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (instance == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Instance is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    // TODO: If OpenXR is set up, let it pick the physical device.

    uint32_t physicalDevicesCount = 0;
    VkResult vkResult = vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, nullptr);
    axrLogVkResult(vkResult, "vkEnumeratePhysicalDevices");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    // Disable Auto-Deallocation because there can be allocations within functions below here. And we don't want to
    // accidentally deallocate stuff automatically
    AxrVector_Stack<VkPhysicalDevice> physicalDevices(physicalDevicesCount, &AxrAllocator::get().FrameAllocator, false);
    physicalDevices.prefillData();
    vkResult = vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, physicalDevices.data());
    axrLogVkResult(vkResult, "vkEnumeratePhysicalDevices");
    if (VK_FAILED(vkResult)) [[unlikely]]
        return AXR_ERROR_VULKAN_ERROR;

    VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
    uint32_t selectedPhysicalDeviceScore = 0;

    for (VkPhysicalDevice& device : physicalDevices) {
        const uint32_t deviceScore = scorePhysicalDeviceSuitability(instance, device, extensions);

        if (deviceScore > selectedPhysicalDeviceScore) {
            selectedPhysicalDeviceScore = deviceScore;
            selectedPhysicalDevice = device;
        }
    }

    if (selectedPhysicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to find a suitable physical device.");
        return AXR_ERROR_NOT_FOUND;
    }

    physicalDevice = selectedPhysicalDevice;
    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

uint32_t AxrVulkanRenderer::scorePhysicalDeviceSuitability(const VkInstance& instance,
                                                           const VkPhysicalDevice& physicalDevice,
                                                           const AxrVulkanExtensions::ExtensionsArray_T& extensions) {
    const uint32_t queueFamiliesScore = scorePhysicalDeviceQueueFamilies(instance, physicalDevice);
    if (queueFamiliesScore == 0) {
        return 0;
    }

    const uint32_t extensionsScore = scorePhysicalDeviceExtensions(physicalDevice, extensions);
    if (extensionsScore == 0) {
        return 0;
    }

    const uint32_t featuresScore = scorePhysicalDeviceFeatures(physicalDevice);
    if (featuresScore == 0) {
        return 0;
    }

    const uint32_t propertiesScore = scorePhysicalDeviceProperties(physicalDevice);
    if (propertiesScore == 0) {
        return 0;
    }

    return queueFamiliesScore + extensionsScore + featuresScore + propertiesScore;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to score physical device queue families. "
uint32_t AxrVulkanRenderer::scorePhysicalDeviceQueueFamilies(const VkInstance& instance,
                                                             const VkPhysicalDevice& physicalDevice) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return 0;
    }

    AxrVulkanQueueFamilies queueFamilies;
    const AxrResult axrResult = queueFamilies.setQueueFamilyIndices(instance, physicalDevice);

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
    return 5;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to score physical device extensions. "
uint32_t AxrVulkanRenderer::scorePhysicalDeviceExtensions(const VkPhysicalDevice& physicalDevice,
                                                          const AxrVulkanExtensions::ExtensionsArray_T& extensions) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return 0;
    }

    // Get device extensions only
    AxrVulkanExtensions::ExtensionsArray_T deviceExtensions;
    for (const AxrVulkanExtension& extension : extensions) {
        const AxrVulkanExtensionProperties properties = AxrVulkanExtensionGetProperties(extension.Type);

        if (properties.Level == AXR_VULKAN_EXTENSION_LEVEL_DEVICE) {
            deviceExtensions.pushBack(extension);
        }
    }

    constexpr uint32_t maxScore = 50;

    if (deviceExtensions.empty()) {
        return maxScore;
    }

    AxrVector_Stack<const char*> supportedExtensions;
    const AxrResult axrResult = AxrVulkanExtensions::getSupportedDeviceExtensions(physicalDevice, supportedExtensions);
    if (AXR_FAILED(axrResult)) {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to get supported device extensions.");
        return 0;
    }

    // The closer the score is to `maxScore`, the more desired extensions are supported. A score of `maxScore` means all
    // desired extensions were found.
    const float extensionWeightedScore = static_cast<float>(maxScore) / static_cast<float>(deviceExtensions.size());
    float score = 0.0f;

    for (const AxrVulkanExtension& extension : deviceExtensions) {
        const AxrVulkanExtensionProperties properties = AxrVulkanExtensionGetProperties(extension.Type);

        if (supportedExtensions.findFirst(properties.Name) != supportedExtensions.end()) {
            score += extensionWeightedScore;
        } else if (extension.IsRequired) {
            // This device ia missing a required extension so it is invalid
            return 0;
        }
    }

    // Even if no extensions were found, we didn't encounter a required extension if we got here.
    // So we need to return at least 1 to signal that we meet the minimum requirements at least.
    return std::max(static_cast<uint32_t>(1), static_cast<uint32_t>(score));
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to score physical device features. "
uint32_t AxrVulkanRenderer::scorePhysicalDeviceFeatures(const VkPhysicalDevice& physicalDevice) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return 0;
    }

    uint32_t score = 0;
    VkPhysicalDeviceFeatures2 features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
    };
    vkGetPhysicalDeviceFeatures2(physicalDevice, &features);

    if (features.features.samplerAnisotropy) {
        score += 5;
    }
    if (features.features.sampleRateShading) {
        score += 5;
    }

    return score;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to score physical device properties. "
uint32_t AxrVulkanRenderer::scorePhysicalDeviceProperties(const VkPhysicalDevice& physicalDevice) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return 0;
    }

    uint32_t score = 0;
    VkPhysicalDeviceProperties2 properties{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
    };
    vkGetPhysicalDeviceProperties2(physicalDevice, &properties);

    if (properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1'000;
    } else {
        score += 1;
    }

    return score;
}
#undef AXR_FUNCTION_FAILED_STRING

#endif
