#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanQueueFamilies.hpp"
#include "axr/logger.h"

// ---- Special Functions ----

AxrVulkanQueueFamilies::AxrVulkanQueueFamilies() = default;

AxrVulkanQueueFamilies::AxrVulkanQueueFamilies(const AxrVulkanQueueFamilies& src) {
    GraphicsQueueFamilyIndex = src.GraphicsQueueFamilyIndex;
    PresentationQueueFamilyIndex = src.PresentationQueueFamilyIndex;
    TransferQueueFamilyIndex = src.TransferQueueFamilyIndex;
}

AxrVulkanQueueFamilies::AxrVulkanQueueFamilies(AxrVulkanQueueFamilies&& src) noexcept {
    GraphicsQueueFamilyIndex = std::move(src.GraphicsQueueFamilyIndex);
    PresentationQueueFamilyIndex = std::move(src.PresentationQueueFamilyIndex);
    TransferQueueFamilyIndex = std::move(src.TransferQueueFamilyIndex);
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
    }

    return *this;
}

AxrVulkanQueueFamilies& AxrVulkanQueueFamilies::operator=(AxrVulkanQueueFamilies&& src) noexcept {
    if (this != &src) {
        cleanup();

        GraphicsQueueFamilyIndex = std::move(src.GraphicsQueueFamilyIndex);
        PresentationQueueFamilyIndex = std::move(src.PresentationQueueFamilyIndex);
        TransferQueueFamilyIndex = std::move(src.TransferQueueFamilyIndex);
    }

    return *this;
}

// ---- Public Functions ----

AxrResult AxrVulkanQueueFamilies::setQueueFamilyIndices(
    const vk::PhysicalDevice& physicalDevice,
    const AxrWindowPlatformEnum windowPlatform,
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
            windowPlatform,
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
    if (!areIndicesValid()) {
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
}

bool AxrVulkanQueueFamilies::doesQueueFamilyIndexSupportsPresentation(
    const uint32_t queueFamilyIndex,
    const vk::PhysicalDevice& physicalDevice,
    const AxrWindowPlatformEnum windowPlatform,
    const vk::DispatchLoaderDynamic& dispatch
) const {
    switch (windowPlatform) {
        case AXR_WINDOW_PLATFORM_WIN32: {
#ifdef AXR_PLATFORM_WIN32
            const vk::Bool32 presentationSupportExists = physicalDevice.getWin32PresentationSupportKHR(
                queueFamilyIndex,
                dispatch
            );
            return static_cast<bool>(presentationSupportExists);
#else
            axrLogErrorLocation("Window platform is not supported.");
            return false;
#endif
        }
        case AXR_WINDOW_PLATFORM_UNDEFINED:
        default: { // NOLINT(clang-diagnostic-covered-switch-default)
            axrLogErrorLocation("Unknown window platform.");
            return false;
        }
    }
}

#endif
