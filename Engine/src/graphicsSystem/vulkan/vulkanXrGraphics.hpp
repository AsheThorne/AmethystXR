#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "../../xrSystem/xrSystem.hpp"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan Xr Graphics
class AxrVulkanXrGraphics {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan xr graphics config
    struct Config {
        AxrXrSystem& XrSystem;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Xr graphics config
    AxrVulkanXrGraphics(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanXrGraphics to copy from
    AxrVulkanXrGraphics(const AxrVulkanXrGraphics& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanXrGraphics to move from
    AxrVulkanXrGraphics(AxrVulkanXrGraphics&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanXrGraphics();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanXrGraphics to copy from
    AxrVulkanXrGraphics& operator=(const AxrVulkanXrGraphics& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanXrGraphics to move from
    AxrVulkanXrGraphics& operator=(AxrVulkanXrGraphics&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Create the vulkan instance to use.
    /// @param createInfo The VkInstanceCreateInfo
    /// @param vkInstance Output created vkInstance
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createVulkanInstance(
        const vk::InstanceCreateInfo& createInfo,
        vk::Instance& vkInstance
    ) const;

    /// Get the vulkan physical device to use
    /// @param vkInstance The vkInstance to use
    /// @param vkPhysicalDevice The Output VkPhysicalDevice
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult getVulkanPhysicalDevice(
        vk::Instance vkInstance,
        vk::PhysicalDevice& vkPhysicalDevice
    ) const;

    /// Create the vulkan device to use
    /// @param vkPhysicalDevice VkPhysicalDevice to use
    /// @param createInfo The VkDeviceCreateInfo
    /// @param vkDevice Output created vkDevice
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createVulkanDevice(
        vk::PhysicalDevice vkPhysicalDevice,
        const vk::DeviceCreateInfo& createInfo,
        vk::Device& vkDevice
    ) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config ----
    AxrXrSystem& m_XrSystem;
};

#endif
