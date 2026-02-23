#pragma once
#ifdef AXR_VULKAN_SUPPORTED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../common/containers/vector_stack.h"
#include "axr/common/enums.h"

#include <vulkan/vulkan_core.h>

#include <optional>

/// Vulkan queue families
class AxrVulkanQueueFamilies {
public:
    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    VkQueue GraphicsQueue = VK_NULL_HANDLE;
    VkQueue PresentationQueue = VK_NULL_HANDLE;
    VkQueue TransferQueue = VK_NULL_HANDLE;
    std::optional<uint32_t> GraphicsQueueFamilyIndex;
    std::optional<uint32_t> PresentationQueueFamilyIndex;
    std::optional<uint32_t> TransferQueueFamilyIndex;
    static constexpr uint32_t const& NumberOfQueueFamilies = 3;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Reset the queue family indices and queues
    void reset();

    /// Set the queue family indices
    /// @param instance Instance to use
    /// @param physicalDevice Physical device to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setQueueFamilyIndices(const VkInstance& instance, const VkPhysicalDevice& physicalDevice);
    /// Reset queue family indices
    void resetQueueFamilyIndices();

    /// Set the queue family queues
    /// @param device Logical device to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setQueueFamilyQueues(const VkDevice& device);
    /// Reset queue family queues
    void resetQueueFamilyQueues();

    /// Check if the queue family indices and queues are valid
    /// @returns True if the queue family indices and queues are valid
    [[nodiscard]] bool isValid() const;
    /// Check if the queue family indices are valid
    /// @returns True if the queue family indices are valid
    [[nodiscard]] bool areIndicesValid() const;
    /// Check if the queue family queues are valid
    /// @returns True if the queue family queues are valid
    [[nodiscard]] bool areQueuesValid() const;
    /// Check if there is a dedicated transfer queue
    /// @returns True if there is a dedicated transfer queue
    [[nodiscard]] bool hasDedicatedTransferQueue() const;

    /// Get a collection of all queue family indices
    /// @returns A collection of all queue family indices
    [[nodiscard]] AxrVector_Stack<uint32_t> getAllQueueFamilyIndices() const;
    /// Get a collection of all unique queue family indices
    /// @returns A collection of all unique queue family indices
    [[nodiscard]] AxrVector_Stack<uint32_t> getUniqueQueueFamilyIndices() const;

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Cleanup the AxrVulkanQueueFamilies
    void cleanup();

    /// Check if the given queue family index supports presentation
    /// @param instance Instance to use
    /// @param physicalDevice Physical device to use
    /// @param queueFamilyIndex Queue family index
    [[nodiscard]] static bool queueFamilyIndexSupportsPresentation(const VkInstance& instance,
                                                                   const VkPhysicalDevice& physicalDevice,
                                                                   uint32_t queueFamilyIndex);
};

#endif
