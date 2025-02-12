#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "vulkanSurfaceDetails.hpp"
#include "axr/logger.h"
#include "vulkanUtils.hpp"

// ---- Special Functions ----

AxrVulkanSurfaceDetails::AxrVulkanSurfaceDetails() = default;

AxrVulkanSurfaceDetails::AxrVulkanSurfaceDetails(
    const vk::PhysicalDevice& physicalDevice,
    const vk::SurfaceKHR& surface,
    const vk::DispatchLoaderDynamic& dispatch
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (physicalDevice == VK_NULL_HANDLE) {
        axrLogErrorLocation("Physical device is null.");
        return;
    }

    if (surface == VK_NULL_HANDLE) {
        axrLogErrorLocation("Surface is null.");
        return;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    const vk::Result vkResult = physicalDevice.getSurfaceCapabilitiesKHR(surface, &Capabilities, dispatch);
    axrLogVkResult(vkResult, "physicalDevice.getSurfaceCapabilitiesKHR");
    if (axrVkFailed(vkResult)) {
        cleanup();
        return;
    }

    const auto surfaceFormatsResult = physicalDevice.getSurfaceFormatsKHR(surface, dispatch);
    axrLogVkResult(surfaceFormatsResult.result, "physicalDevice.getSurfaceFormatsKHR");
    if (axrVkFailed(surfaceFormatsResult.result)) {
        cleanup();
        return;
    }

    Formats = surfaceFormatsResult.value;

    const auto surfacePresentationModsResult = physicalDevice.getSurfacePresentModesKHR(surface, dispatch);
    axrLogVkResult(surfacePresentationModsResult.result, "physicalDevice.getSurfacePresentModesKHR");
    if (axrVkFailed(surfacePresentationModsResult.result)) {
        cleanup();
        return;
    }

    PresentationModes = surfacePresentationModsResult.value;
}

AxrVulkanSurfaceDetails::AxrVulkanSurfaceDetails(const AxrVulkanSurfaceDetails& src) {
    Capabilities = src.Capabilities;
    Formats = src.Formats;
    PresentationModes = src.PresentationModes;
}

AxrVulkanSurfaceDetails::AxrVulkanSurfaceDetails(AxrVulkanSurfaceDetails&& src) noexcept {
    Capabilities = std::move(src.Capabilities);
    Formats = std::move(src.Formats);
    PresentationModes = std::move(src.PresentationModes);
}

AxrVulkanSurfaceDetails::~AxrVulkanSurfaceDetails() {
    cleanup();
}

AxrVulkanSurfaceDetails& AxrVulkanSurfaceDetails::operator=(const AxrVulkanSurfaceDetails& src) {
    if (this != &src) {
        cleanup();

        Capabilities = src.Capabilities;
        Formats = src.Formats;
        PresentationModes = src.PresentationModes;
    }

    return *this;
}

AxrVulkanSurfaceDetails& AxrVulkanSurfaceDetails::operator=(AxrVulkanSurfaceDetails&& src) noexcept {
    if (this != &src) {
        cleanup();

        Capabilities = std::move(src.Capabilities);
        Formats = std::move(src.Formats);
        PresentationModes = std::move(src.PresentationModes);
    }

    return *this;
}

// ---- Public Functions ----

bool AxrVulkanSurfaceDetails::isValid() const {
    return !Formats.empty() && !PresentationModes.empty();
}

// ---- Private Functions ----

void AxrVulkanSurfaceDetails::cleanup() {
    Capabilities = vk::SurfaceCapabilitiesKHR();
    Formats.clear();
    PresentationModes.clear();
}

#endif
