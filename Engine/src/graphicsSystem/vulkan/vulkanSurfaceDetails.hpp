#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <vector>

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

class AxrVulkanSurfaceDetails {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVulkanSurfaceDetails();
    /// Constructor
    /// @param physicalDevice Physical device to use
    /// @param surface Surface to use
    /// @param dispatch Dispatch to use
    AxrVulkanSurfaceDetails(
        const vk::PhysicalDevice& physicalDevice,
        const vk::SurfaceKHR& surface,
        const vk::DispatchLoaderDynamic& dispatch
    );
    /// Copy Constructor
    /// @param src Source AxrVulkanSurfaceDetails to copy from
    AxrVulkanSurfaceDetails(const AxrVulkanSurfaceDetails& src);
    /// Move Constructor
    /// @param src Source AxrVulkanSurfaceDetails to move from
    AxrVulkanSurfaceDetails(AxrVulkanSurfaceDetails&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanSurfaceDetails();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanSurfaceDetails to copy from
    AxrVulkanSurfaceDetails& operator=(const AxrVulkanSurfaceDetails& src);
    /// Move Assignment Operator
    /// @param src Source AxrVulkanSurfaceDetails to move from
    AxrVulkanSurfaceDetails& operator=(AxrVulkanSurfaceDetails&& src) noexcept;

    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    vk::SurfaceCapabilitiesKHR Capabilities;
    std::vector<vk::SurfaceFormatKHR> Formats;
    std::vector<vk::PresentModeKHR> PresentationModes;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the data is valid
    [[nodiscard]] bool isValid() const; 

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Cleanup this class
    void cleanup();
};

#endif
