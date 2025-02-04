#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_set>
#include <optional>
#include <vector>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/result.h"
#include "axr/windowSystem.h"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

class AxrVulkanQueueFamilies {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrVulkanQueueFamilies();
    /// Copy Constructor
    /// @param src Source AxrVulkanQueueFamilies to copy from
    AxrVulkanQueueFamilies(const AxrVulkanQueueFamilies& src);
    /// Move Constructor
    /// @param src Source AxrVulkanQueueFamilies to move from
    AxrVulkanQueueFamilies(AxrVulkanQueueFamilies&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanQueueFamilies();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanQueueFamilies to copy from
    AxrVulkanQueueFamilies& operator=(const AxrVulkanQueueFamilies& src);
    /// Move Assignment Operator
    /// @param src Source AxrVulkanQueueFamilies to move from
    AxrVulkanQueueFamilies& operator=(AxrVulkanQueueFamilies&& src) noexcept;

    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    std::optional<uint32_t> GraphicsQueueFamilyIndex;
    std::optional<uint32_t> PresentationQueueFamilyIndex;
    std::optional<uint32_t> TransferQueueFamilyIndex;
    vk::Queue GraphicsQueue;
    vk::Queue PresentationQueue;
    vk::Queue TransferQueue;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set the queue family indices
    /// @param physicalDevice Physical device to use
    /// @param windowPlatform Window platform to use
    /// @param dispatch Dispatch to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setQueueFamilyIndices(
        const vk::PhysicalDevice& physicalDevice,
        AxrWindowPlatformEnum windowPlatform,
        const vk::DispatchLoaderDynamic& dispatch
    );
    /// Set the queue family queues
    /// @param device Logical device to use
    /// @param dispatch Dispatch to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setQueueFamilyQueues(
        const vk::Device& device,
        const vk::DispatchLoaderDynamic& dispatch
    );

    /// Check if the queue family indices are valid
    /// @returns True if the queue family indices are valid
    [[nodiscard]] bool areIndicesValid() const;
    /// Check if there is a dedicated transfer queue
    /// @returns True if there is a dedicated transfer queue
    [[nodiscard]] bool hasDedicatedTransferQueue() const;

    /// Get a collection of all queue family indices
    /// @returns A collection of all queue family indices
    [[nodiscard]] std::vector<uint32_t> getAllQueueFamilyIndices() const;
    /// Get a collection of all unique queue family indices
    /// @returns A collection of all unique queue family indices
    [[nodiscard]] std::unordered_set<uint32_t> getUniqueQueueFamilyIndices() const;

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Cleanup the AxrVulkanQueueFamilies
    void cleanup();

    /// Check if the given queue family index supports presentation
    /// @param queueFamilyIndex Queue family index
    /// @param physicalDevice Physical device to use
    /// @param windowPlatform Window platform to use
    /// @param dispatch Dispatch to use
    [[nodiscard]] bool doesQueueFamilyIndexSupportsPresentation(
        uint32_t queueFamilyIndex,
        const vk::PhysicalDevice& physicalDevice,
        AxrWindowPlatformEnum windowPlatform,
        const vk::DispatchLoaderDynamic& dispatch
    ) const;
};

#endif
