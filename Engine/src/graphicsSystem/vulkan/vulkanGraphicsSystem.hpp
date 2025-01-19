#pragma once
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <cstdint>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"
#include "axr/graphicsSystem.h"
#include "axr/windowSystem.h"

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

    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Vulkan Graphics System Config
    struct Config {
        const char* ApplicationName;
        uint32_t ApplicationVersion;
        AxrWindowPlatformEnum WindowPlatform;
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
    AxrWindowPlatformEnum m_WindowPlatform;
    std::vector<AxrVulkanApiLayer_T> m_ApiLayers;
    std::vector<AxrVulkanExtension_T> m_Extensions;

    vk::DispatchLoaderDynamic m_DynamicDispatchLoader;
    std::vector<std::string> m_SupportedInstanceApiLayers;
    std::vector<std::string> m_SupportedInstanceExtensions;

    vk::Instance m_Instance;
    vk::DebugUtilsMessengerEXT m_DebugUtilsMessenger;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

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
    /// Find the supported instance extensions
    /// @returns The supported instance extensions
    std::vector<std::string> getSupportedInstanceExtensions() const;

    /// Destroy the api layers
    void destroyApiLayers();
    /// Destroy the extensions
    void destroyExtensions();

    /// Get a collection of all instance api layer names to use
    /// @returns A collection of instance api layer names
    std::vector<const char*> getAllInstanceApiLayerNames();
    /// Get a collection of all instance extension names to use
    /// @returns A collection of instance extension names
    std::vector<const char*> getAllInstanceExtensionNames();

    /// Check if the given extension type exists in m_Extensions
    /// @param extensionType Extension type to check
    /// @returns True if the given extension type exists in m_Extensions 
    bool extensionExists(AxrVulkanExtensionTypeEnum extensionType) const;

    /// Add the required instance extensions to m_Extensions
    void addRequiredInstanceExtensions();
    /// Add the given extension to m_Extensions if it is supported
    /// @param extension The extension to add
    void addExtension(AxrVulkanExtension_T extension);

    /// Get the extension from the given extension type
    /// @param type Extension type
    /// @returns The extension of the given type or nullptr if it doesn't exist
    [[nodiscard]] AxrVulkanExtension_T getExtension(AxrVulkanExtensionTypeEnum type) const;

    // ---- Debug Utils ----

    /// Create the debug utils messenger create info
    /// @retrns The debug utils messenger create info
    [[nodiscard]] vk::DebugUtilsMessengerCreateInfoEXT createDebugUtilsCreateInto() const;

    /// Create the debug utils messenger
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createDebugUtils();
    /// Destroy the debug utils messenger
    void destroyDebugUtils();

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
