#ifdef AXR_VULKAN_SUPPORTED
// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanQueueFamilies.h"
#include "../../common/vector_stack.h"
#include "../../memory/allocator.h"
#include "../../platform/platform.h"
#include "axr/logging.h"
#include "vulkanUtils.h"

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

void AxrVulkanQueueFamilies::reset() {
    resetQueueFamilyIndices();
    resetQueueFamilyQueues();
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set queue family indices. "
AxrResult AxrVulkanQueueFamilies::setQueueFamilyIndices(const VkInstance& instance,
                                                        const VkPhysicalDevice& physicalDevice) {
    if (physicalDevice == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Physical device is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    // Reset the queue families
    cleanup();

    uint32_t queueFamilyPropertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyPropertiesCount, nullptr);

    AxrVector_Stack<VkQueueFamilyProperties2> queueFamilyProperties(queueFamilyPropertiesCount,
                                                                    &AxrAllocator::get().FrameAllocator);
    queueFamilyProperties.prefillData(VkQueueFamilyProperties2{
        .sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2,
    });
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice,
                                              &queueFamilyPropertiesCount,
                                              queueFamilyProperties.data());

    for (size_t i = 0; i < queueFamilyProperties.size(); ++i) {
        // Look for the graphics queue family
        if (queueFamilyProperties[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT &&
            !GraphicsQueueFamilyIndex.has_value()) {
            GraphicsQueueFamilyIndex = static_cast<uint32_t>(i);
        }

        // Look for a dedicated transfer queue family.
        // All queues with the graphics bit also have the transfer bit.
        // But because we want to look for a dedicated transfer queue,
        // we want to find one that has the transfer bit but not the graphics bit.
        else if (queueFamilyProperties[i].queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT &&
                 !TransferQueueFamilyIndex.has_value()) {
            TransferQueueFamilyIndex = static_cast<uint32_t>(i);
        }

        // Look for presentation queue family
        if (queueFamilyIndexSupportsPresentation(instance, physicalDevice, static_cast<uint32_t>(i)) &&
            !PresentationQueueFamilyIndex.has_value()) {
            PresentationQueueFamilyIndex = static_cast<uint32_t>(i);
        }

        // Break the loop if we found all the queue families
        if (areIndicesValid()) {
            break;
        }
    }

    // If no dedicated transfer queue was found, use the graphics queue
    if (!TransferQueueFamilyIndex.has_value()) {
        TransferQueueFamilyIndex = GraphicsQueueFamilyIndex;
    }

    // Failed to find all queue families
    if (!areIndicesValid()) [[unlikely]] {
        // Reset the queue families
        cleanup();
        return AXR_ERROR_NOT_FOUND;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanQueueFamilies::resetQueueFamilyIndices() {
    resetQueueFamilyQueues();

    GraphicsQueueFamilyIndex.reset();
    PresentationQueueFamilyIndex.reset();
    TransferQueueFamilyIndex.reset();
}

#define AXR_FUNCTION_FAILED_STRING "Failed to set queue family queues. "
AxrResult AxrVulkanQueueFamilies::setQueueFamilyQueues(const VkDevice& device) {
    if (!GraphicsQueueFamilyIndex.has_value() || !TransferQueueFamilyIndex.has_value() ||
        !PresentationQueueFamilyIndex.has_value()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Queue family indices are not valid.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (GraphicsQueue != VK_NULL_HANDLE || PresentationQueue != VK_NULL_HANDLE || TransferQueue != VK_NULL_HANDLE)
        [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Queue family queues already exist.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    if (device == VK_NULL_HANDLE) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Device is null.");
        return AXR_ERROR_VALIDATION_FAILED;
    }

    vkGetDeviceQueue(device, GraphicsQueueFamilyIndex.value(), 0, &GraphicsQueue);
    vkGetDeviceQueue(device, PresentationQueueFamilyIndex.value(), 0, &PresentationQueue);
    vkGetDeviceQueue(device, TransferQueueFamilyIndex.value(), 0, &TransferQueue);

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrVulkanQueueFamilies::resetQueueFamilyQueues() {
    GraphicsQueue = VK_NULL_HANDLE;
    PresentationQueue = VK_NULL_HANDLE;
    TransferQueue = VK_NULL_HANDLE;
}

bool AxrVulkanQueueFamilies::isValid() const {
    return areIndicesValid() && areQueuesValid();
}

bool AxrVulkanQueueFamilies::areIndicesValid() const {
    return GraphicsQueueFamilyIndex.has_value() && TransferQueueFamilyIndex.has_value() &&
           PresentationQueueFamilyIndex.has_value();
}

bool AxrVulkanQueueFamilies::areQueuesValid() const {
    return GraphicsQueue != VK_NULL_HANDLE && PresentationQueue != VK_NULL_HANDLE && TransferQueue != VK_NULL_HANDLE;
}

bool AxrVulkanQueueFamilies::hasDedicatedTransferQueue() const {
    // If the queue families haven't been set yet, then of course there isn't a dedicated transfer queue
    if (!GraphicsQueueFamilyIndex.has_value() || !TransferQueueFamilyIndex.has_value()) {
        return false;
    }

    // We use the graphics queue if we fail to find a dedicated transfer queue.
    // So if the indices are different, we must have found a dedicated transfer queue.
    return GraphicsQueueFamilyIndex != TransferQueueFamilyIndex;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to get all queue family indices. "
AxrVector_Stack<uint32_t> AxrVulkanQueueFamilies::getAllQueueFamilyIndices() const {
    if (!GraphicsQueueFamilyIndex.has_value() || !TransferQueueFamilyIndex.has_value() ||
        !PresentationQueueFamilyIndex.has_value()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Queue family indices are not valid.");
        return {};
    }

    AxrVector_Stack<uint32_t> queueFamilyIndices(NumberOfQueueFamilies, &AxrAllocator::get().FrameAllocator);
    queueFamilyIndices.append({
        GraphicsQueueFamilyIndex.value(),
        PresentationQueueFamilyIndex.value(),
        TransferQueueFamilyIndex.value(),
    });

    return queueFamilyIndices;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to get unique queue family indices. "
AxrVector_Stack<uint32_t> AxrVulkanQueueFamilies::getUniqueQueueFamilyIndices() const {
    if (!areIndicesValid()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Queue family indices are not valid.");
        return {};
    }

    AxrVector_Stack<uint32_t> uniqueQueueFamilyIndices(NumberOfQueueFamilies, &AxrAllocator::get().FrameAllocator);

    for (uint32_t queueFamilyIndex : getAllQueueFamilyIndices()) {
        if (uniqueQueueFamilyIndices.findFirst(queueFamilyIndex) != uniqueQueueFamilyIndices.end()) {
            // If queue family index already exists...
            continue;
        }

        uniqueQueueFamilyIndices.pushBack(queueFamilyIndex);
    }

    return uniqueQueueFamilyIndices;
}
#undef AXR_FUNCTION_FAILED_STRING

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrVulkanQueueFamilies::cleanup() {
    reset();
}

bool AxrVulkanQueueFamilies::queueFamilyIndexSupportsPresentation(const VkInstance& instance,
                                                                  const VkPhysicalDevice& physicalDevice,
                                                                  const uint32_t queueFamilyIndex) {
    return AxrPlatform::getVulkanPresentationSupport(instance, physicalDevice, queueFamilyIndex);
}

#endif
