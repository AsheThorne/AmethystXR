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

// TODO: Remove from here or safe guard it with preprocessor
#include <windows.h>
#include <vulkan/vulkan_win32.h>

/// Vulkan Graphics System
class AxrVulkanGraphicsSystem {
public:
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

    std::vector<std::string> m_SupportedInstanceApiLayers;
    std::vector<std::string> m_SupportedInstanceExtensions;

    vk::Instance m_Instance;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    // ---- Instance Functions ----

    /// Create the vulkan instance
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createInstance();
    /// Destroy the vulkan instance
    void destroyInstance();

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
};

#endif
