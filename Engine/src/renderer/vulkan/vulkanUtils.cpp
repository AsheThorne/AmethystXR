#ifdef AXR_VULKAN_SUPPORTED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkanUtils.h"

AxrResult axrFindMemoryTypeIndex(const VkPhysicalDevice& physicalDevice,
                                 const uint32_t typeFilter,
                                 const VkMemoryPropertyFlags properties,
                                 uint32_t& memoryTypeIndex) {
    assert(physicalDevice != VK_NULL_HANDLE);

    VkPhysicalDeviceMemoryProperties2 physicalDeviceMemoryProperties{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
        .pNext = nullptr,
    };
    vkGetPhysicalDeviceMemoryProperties2(physicalDevice, &physicalDeviceMemoryProperties);

    for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryProperties.memoryTypeCount; ++i) {
        if (typeFilter & (1 << i) &&
            (physicalDeviceMemoryProperties.memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            memoryTypeIndex = i;
            return AXR_SUCCESS;
        }
    }

    return AXR_ERROR_NOT_FOUND;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to begin single time command. "
AxrResult axrBeginSingleTimeCommand(const VkDevice& device,
                                    const VkCommandPool& commandPool,
                                    VkCommandBuffer& commandBuffer) {
    assert(device != VK_NULL_HANDLE);
    assert(commandPool != VK_NULL_HANDLE);

    if (commandBuffer == VK_NULL_HANDLE) [[unlikely]] {
        axrLogWarning(AXR_FUNCTION_FAILED_STRING "Command buffer already exists.");
        return AXR_SUCCESS;
    }

    const VkCommandBufferAllocateInfo commandBufferAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkResult vkResult = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer);
    axrLogVkResult(vkResult, "vkAllocateCommandBuffers");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        return AXR_ERROR_VULKAN_ERROR;
    }

    constexpr VkCommandBufferBeginInfo commandBufferBeginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr,
    };

    vkResult = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    axrLogVkResult(vkResult, "vkBeginCommandBuffer");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
        commandBuffer = VK_NULL_HANDLE;
        return AXR_ERROR_VULKAN_ERROR;
    }

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

AxrResult axrEndSingleTimeCommand(const VkDevice& device,
                                  const VkCommandPool& commandPool,
                                  const VkQueue& queue,
                                  VkCommandBuffer& commandBuffer) {
    assert(device != VK_NULL_HANDLE);
    assert(commandPool != VK_NULL_HANDLE);
    assert(queue != VK_NULL_HANDLE);

    VkResult vkResult = vkEndCommandBuffer(commandBuffer);
    axrLogVkResult(vkResult, "vkEndCommandBuffer");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        return AXR_ERROR_VULKAN_ERROR;
    }

    const VkSubmitInfo submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = {},
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = nullptr,
    };

    vkResult = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    axrLogVkResult(vkResult, "vkQueueSubmit");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        return AXR_ERROR_VULKAN_ERROR;
    }

    vkResult = vkQueueWaitIdle(queue);
    axrLogVkResult(vkResult, "vkQueueWaitIdle");
    if (VK_FAILED(vkResult)) [[unlikely]] {
        return AXR_ERROR_VULKAN_ERROR;
    }

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    commandBuffer = VK_NULL_HANDLE;

    return AXR_SUCCESS;
}

#endif
