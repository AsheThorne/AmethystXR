#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanQueueFamilies.hpp"
#include "axr/logger.h"

// ---- Special Functions ----

AxrVulkanQueueFamilies::AxrVulkanQueueFamilies():
    GraphicsQueue(VK_NULL_HANDLE),
    PresentationQueue(VK_NULL_HANDLE),
    TransferQueue(VK_NULL_HANDLE) {
}

AxrVulkanQueueFamilies::AxrVulkanQueueFamilies(const AxrVulkanQueueFamilies& src) {
    GraphicsQueueFamilyIndex = src.GraphicsQueueFamilyIndex;
    PresentationQueueFamilyIndex = src.PresentationQueueFamilyIndex;
    TransferQueueFamilyIndex = src.TransferQueueFamilyIndex;

    GraphicsQueue = src.GraphicsQueue;
    PresentationQueue = src.PresentationQueue;
    TransferQueue = src.TransferQueue;
}

AxrVulkanQueueFamilies::AxrVulkanQueueFamilies(AxrVulkanQueueFamilies&& src) noexcept {
    GraphicsQueueFamilyIndex = std::move(src.GraphicsQueueFamilyIndex);
    PresentationQueueFamilyIndex = std::move(src.PresentationQueueFamilyIndex);
    TransferQueueFamilyIndex = std::move(src.TransferQueueFamilyIndex);

    GraphicsQueue = std::move(src.GraphicsQueue);
    PresentationQueue = std::move(src.PresentationQueue);
    TransferQueue = std::move(src.TransferQueue);
}

AxrVulkanQueueFamilies::~AxrVulkanQueueFamilies() {
    cleanup();
}

AxrVulkanQueueFamilies& AxrVulkanQueueFamilies::operator=(const AxrVulkanQueueFamilies& src) {
    if (this != &src) {
        cleanup();

        GraphicsQueueFamilyIndex = src.GraphicsQueueFamilyIndex;
        PresentationQueueFamilyIndex = src.PresentationQueueFamilyIndex;
        TransferQueueFamilyIndex = src.TransferQueueFamilyIndex;

        GraphicsQueue = src.GraphicsQueue;
        PresentationQueue = src.PresentationQueue;
        TransferQueue = src.TransferQueue;
    }

    return *this;
}

AxrVulkanQueueFamilies& AxrVulkanQueueFamilies::operator=(AxrVulkanQueueFamilies&& src) noexcept {
    if (this != &src) {
        cleanup();

        GraphicsQueueFamilyIndex = std::move(src.GraphicsQueueFamilyIndex);
        PresentationQueueFamilyIndex = std::move(src.PresentationQueueFamilyIndex);
        TransferQueueFamilyIndex = std::move(src.TransferQueueFamilyIndex);

        GraphicsQueue = std::move(src.GraphicsQueue);
        PresentationQueue = std::move(src.PresentationQueue);
        TransferQueue = std::move(src.TransferQueue);
    }

    return *this;
}

// ---- Public Functions ----

AxrResult AxrVulkanQueueFamilies::setQueueFamilyIndices(
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    // Reset the queue families
    cleanup();

    const std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties(
        dispatch
    );

    for (size_t i = 0; i < queueFamilyProperties.size(); ++i) {
        // Look for the graphics queue family
        if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics &&
            !GraphicsQueueFamilyIndex.has_value()) {
            GraphicsQueueFamilyIndex = static_cast<uint32_t>(i);
        }

        // Look for a dedicated transfer queue family.
        // All queues with the graphics bit also have the transfer bit.
        // But because we want to look for a dedicated transfer queue,
        // we want to find one that has the transfer bit but not the graphics bit.
        else if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eTransfer &&
            !TransferQueueFamilyIndex.has_value()) {
            TransferQueueFamilyIndex = static_cast<uint32_t>(i);
        }

        // Look for presentation queue family
        if (doesQueueFamilyIndexSupportsPresentation(
            static_cast<uint32_t>(i),
            physicalDevice,
            dispatch
        ) && !PresentationQueueFamilyIndex.has_value()) {
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
    if (!areIndicesValid()) {
        // Reset the queue families
        cleanup();
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult AxrVulkanQueueFamilies::setQueueFamilyQueues(
    const vk::Device& device,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (!GraphicsQueueFamilyIndex.has_value() ||
        !TransferQueueFamilyIndex.has_value() ||
        !PresentationQueueFamilyIndex.has_value()) {
        axrLogErrorLocation("Queue family indices are not valid.");
        return AXR_ERROR;
    }

    if (GraphicsQueue != VK_NULL_HANDLE ||
        PresentationQueue != VK_NULL_HANDLE ||
        TransferQueue != VK_NULL_HANDLE) {
        axrLogErrorLocation("Queue family queues are already set.");
        return AXR_ERROR;
    }

    if (device == VK_NULL_HANDLE) {
        axrLogErrorLocation("Device is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    device.getQueue(GraphicsQueueFamilyIndex.value(), 0, &GraphicsQueue, dispatch);
    device.getQueue(PresentationQueueFamilyIndex.value(), 0, &PresentationQueue, dispatch);
    device.getQueue(TransferQueueFamilyIndex.value(), 0, &TransferQueue, dispatch);

    return AXR_SUCCESS;
}

bool AxrVulkanQueueFamilies::areIndicesValid() const {
    return GraphicsQueueFamilyIndex.has_value() &&
        TransferQueueFamilyIndex.has_value() &&
        PresentationQueueFamilyIndex.has_value();
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

std::vector<uint32_t> AxrVulkanQueueFamilies::getAllQueueFamilyIndices() const {
    if (!GraphicsQueueFamilyIndex.has_value() ||
        !TransferQueueFamilyIndex.has_value() ||
        !PresentationQueueFamilyIndex.has_value()) {
        axrLogErrorLocation("Queue family indices are not valid");
        return {};
    }

    return {
        GraphicsQueueFamilyIndex.value(),
        PresentationQueueFamilyIndex.value(),
        TransferQueueFamilyIndex.value()
    };
}

std::unordered_set<uint32_t> AxrVulkanQueueFamilies::getUniqueQueueFamilyIndices() const {
    if (!areIndicesValid()) {
        axrLogErrorLocation("Queue family indices are not valid");
        return {};
    }

    std::unordered_set<uint32_t> uniqueQueueFamilyIndices;

    for (uint32_t queueFamilyIndex : getAllQueueFamilyIndices()) {
        uniqueQueueFamilyIndices.insert(queueFamilyIndex);
    }

    return uniqueQueueFamilyIndices;
}

// ---- Private Functions ----

void AxrVulkanQueueFamilies::cleanup() {
    GraphicsQueueFamilyIndex.reset();
    PresentationQueueFamilyIndex.reset();
    TransferQueueFamilyIndex.reset();

    GraphicsQueue = VK_NULL_HANDLE;
    PresentationQueue = VK_NULL_HANDLE;
    TransferQueue = VK_NULL_HANDLE;
}

bool AxrVulkanQueueFamilies::doesQueueFamilyIndexSupportsPresentation(
    const uint32_t queueFamilyIndex,
    const vk::PhysicalDevice& physicalDevice,
    const vk::DispatchLoaderDynamic& dispatch
) const {
#ifdef AXR_USE_PLATFORM_WIN32
    const vk::Bool32 presentationSupportExists = physicalDevice.getWin32PresentationSupportKHR(
        queueFamilyIndex,
        dispatch
    );
    return static_cast<bool>(presentationSupportExists);
#else
    axrLogErrorLocation("Unknown platform.");
    return false;
#endif
}

#endif
