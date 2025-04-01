#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdint>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/result.h"
#include "axr/windowSystem.h"
#include "axr/assets.h"
#include "vulkanQueueFamilies.hpp"
#include "vulkanWindowGraphics.hpp"
#include "vulkanExtensionCollection.hpp"
#include "sceneAssets/vulkanSceneAssets.hpp"

// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan.hpp>

/// Vulkan Graphics System
class AxrVulkanGraphicsSystem {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// The instance pNext structure chain type
    using InstanceChain_T = vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>;
    /// The device pNext structure chain type
    using DeviceChain_T = vk::StructureChain<vk::DeviceCreateInfo>;

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan Graphics System Config
    struct Config {
        const char* ApplicationName;
        uint32_t ApplicationVersion;
        AxrWindowSystem_T WindowSystem;
        AxrAssetCollection_T GlobalAssetCollection;
        AxrVulkanApiConfig_T VulkanConfig;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Vulkan graphics system config
    AxrVulkanGraphicsSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrVulkanGraphicsSystem to copy from
    AxrVulkanGraphicsSystem(const AxrVulkanGraphicsSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanGraphicsSystem to move from
    AxrVulkanGraphicsSystem(AxrVulkanGraphicsSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanGraphicsSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanGraphicsSystem to copy from
    AxrVulkanGraphicsSystem& operator=(const AxrVulkanGraphicsSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanGraphicsSystem to move from
    AxrVulkanGraphicsSystem& operator=(AxrVulkanGraphicsSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the vulkan graphics system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_ApplicationName;
    uint32_t m_ApplicationVersion;
    AxrVulkanExtensionCollection<AxrVulkanApiLayer_T, AxrVulkanApiLayerTypeEnum> m_ApiLayers;
    AxrVulkanExtensionCollection<AxrVulkanExtension_T, AxrVulkanExtensionTypeEnum> m_Extensions;

    /// Ordered from most desired to the least desired
    std::vector<vk::SurfaceFormatKHR> m_SwapchainColorFormatOptions;
    /// Ordered from most desired to the least desired
    std::vector<vk::Format> m_SwapchainDepthFormatOptions;

    vk::DispatchLoaderDynamic m_DynamicDispatchLoader;
    vk::Instance m_Instance;
    vk::DebugUtilsMessengerEXT m_DebugUtilsMessenger;
    vk::PhysicalDevice m_PhysicalDevice;
    AxrVulkanQueueFamilies m_QueueFamilies;
    vk::Device m_Device;

    AxrVulkanSceneAssets m_GlobalSceneAssets;
    AxrVulkanWindowGraphics* m_WindowGraphics;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Reset setup()
    void resetSetup();

    // ---- Instance Functions ----

    /// Create the vulkan instance
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createInstance();
    /// Destroy the vulkan instance
    void destroyInstance();

    /// Create the instance pNext chain using the given instance create info
    /// @param instanceCreateInfo Instance create info to use
    /// @returns The instance structure chain
    [[nodiscard]] InstanceChain_T createInstanceChain(const vk::InstanceCreateInfo& instanceCreateInfo) const;

    // ---- Api Layers / Extensions ----

    /// Find the supported instance api layers
    /// @returns The supported instance api layers
    std::vector<std::string> getSupportedInstanceApiLayers() const;
    /// Find the supported device api layers for the given physical device
    /// @returns The supported device api layers
    std::vector<std::string> getSupportedDeviceApiLayers(const vk::PhysicalDevice& physicalDevice) const;
    /// Find the supported instance extensions
    /// @returns The supported instance extensions
    std::vector<std::string> getSupportedInstanceExtensions() const;
    /// Find the supported device extensions for the given physical device
    /// @returns The supported device extensions
    std::vector<std::string> getSupportedDeviceExtensions(const vk::PhysicalDevice& physicalDevice) const;

    /// Remove api layers from m_ApiLayers that aren't supported by the m_Instance
    void removeUnsupportedApiLayers();
    /// Remove instance level extensions from m_Extensions that aren't supported by the m_Instance 
    void removeUnsupportedInstanceExtensions();
    /// Remove device level extensions from m_Extensions that aren't supported by the m_PhysicalDevice 
    void removeUnsupportedDeviceExtensions();

    /// Get a collection of all api layer names to use
    /// @returns A collection of api layer names
    std::vector<const char*> getAllApiLayerNames() const;
    /// Get a collection of all instance extension names to use
    /// @returns A collection of instance extension names
    std::vector<const char*> getAllInstanceExtensionNames() const;
    /// Get a collection of all device extension names to use
    /// @returns A collection of device extension names
    std::vector<const char*> getAllDeviceExtensionNames() const;

    /// Add the required instance extensions to m_Extensions
    void addRequiredInstanceExtensions();
    /// Add the required device extensions to m_Extensions
    void addRequiredDeviceExtensions();

    // ---- Debug Utils ----

    /// Create the debug utils messenger create info
    /// @retrns The debug utils messenger create info
    [[nodiscard]] vk::DebugUtilsMessengerCreateInfoEXT createDebugUtilsCreateInto() const;

    /// Create the debug utils messenger
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createDebugUtils();
    /// Destroy the debug utils messenger
    void destroyDebugUtils();

    // ---- Physical Device ----

    /// Set up the physical device
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setupPhysicalDevice();
    /// Reset setupPhysicalDevice()
    void resetPhysicalDevice();

    /// Decide on which physical device we'd like to use.
    /// If OpenXR is being used though, we need to use the one it selects for us
    /// @returns The physical device we should use
    [[nodiscard]] vk::PhysicalDevice pickPhysicalDevice() const;

    /// Score the given physical device. The higher the number, the better.
    /// 0 Means it is not suitable to use.
    /// @param physicalDevice Physical device to score
    /// @returns The physical device score
    uint32_t scorePhysicalDeviceSuitability(const vk::PhysicalDevice& physicalDevice) const;
    /// Score the given physical device on its available queue families.
    /// @param physicalDevice Physical device to score
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, has extra desired features.
    uint32_t scorePhysicalDeviceQueueFamilies(const vk::PhysicalDevice& physicalDevice) const;
    /// Score the given physical device on its available api layers.
    /// @details
    /// All api layers used by the instance should also be used for the device.
    /// This is for compatibility with older version of vulkan.
    /// "https://docs.vulkan.org/spec/latest/chapters/extensions.html#extendingvulkan-layers-devicelayerdeprecation"
    /// If any of the api layers aren't supported, then it's still ok for more recent versions of vulkan.
    /// So, a score of 1 indicates that not all api layers are supported, but it's not desirable.
    /// A score of more than 1 means all api layers are supported.
    /// @param physicalDevice Physical device to score
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, supports all api layers
    uint32_t scorePhysicalDeviceApiLayers(const vk::PhysicalDevice& physicalDevice) const;
    /// Score the given physical device on its available extensions.
    /// @param physicalDevice Physical device to score
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, supports more than just the required extensions.
    uint32_t scorePhysicalDeviceExtensions(const vk::PhysicalDevice& physicalDevice) const;
    /// Score the given physical device on its available features.
    /// @param physicalDevice Physical device to score
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, has extra desired features.
    uint32_t scorePhysicalDeviceFeatures(const vk::PhysicalDevice& physicalDevice) const;
    /// Score the given physical device on its available properties.
    /// @param physicalDevice Physical device to score
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, has extra desired features.
    uint32_t scorePhysicalDeviceProperties(const vk::PhysicalDevice& physicalDevice) const;

    /// Check if all the api layers are supported for the given physical device
    /// @param physicalDevice Physical device to check
    /// @returns True if all the api layers are supported for the given physical device
    bool areApiLayersSupportedForPhysicalDevice(const vk::PhysicalDevice& physicalDevice) const;

    // ---- Logical Device ----

    /// Create the vulkan device
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createLogicalDevice();
    /// Destroy the vulkan device
    void destroyLogicalDevice();

    /// Create the device pNext chain using the given device create info
    /// @param deviceCreateInfo Device create info to use
    /// @returns The device structure chain
    [[nodiscard]] DeviceChain_T createDeviceChain(const vk::DeviceCreateInfo& deviceCreateInfo) const;

    // ----------------------------------------- //
    // Private Static Functions
    // ----------------------------------------- //

    /// Debug utils messages callback function
    /// @param messageSeverity The severity of the message
    /// @param messageType The type of the message
    /// @param pCallbackData Callback data
    /// @param pUserData User data
    /// @returns The application should always return VK_FALSE. VK_TRUE is typically only used in layer development
    static VkBool32 VKAPI_CALL debugUtilsCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    );
};

#endif
