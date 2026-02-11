#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/enums.h"
#include "axr/vulkanApi.h"
#include "vulkanExtensions.h"

#ifndef AXR_VULKAN_SUPPORTED
/// Empty Axr Vulkan renderer static class. This class only holds static functions.
class AxrVulkanRenderer {
public:
    /// Empty Vulkan renderer context
    struct Context {};

    /// Empty Vulkan renderer setup Config
    struct Config {};
};
#else
/// Axr Vulkan renderer static class. This class only holds static functions.
class AxrVulkanRenderer {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    using InstanceChainTuple_T = std::tuple<VkInstanceCreateInfo, VkDebugUtilsMessengerCreateInfoEXT>;

    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// Vulkan renderer context
    struct Context {
        AxrVulkanExtensions::ApiLayersArray_T ApiLayers{};
        AxrVulkanExtensions::ExtensionsArray_T Extensions{};
        VkInstance Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT DebugUtilsMessenger = VK_NULL_HANDLE;
        bool IsSetup = false;
    };

    /// Vulkan renderer setup Config
    struct Config {
        const AxrVulkanRendererConfig* VulkanConfig{};
        uint32_t ApplicationVersion{};
        char ApplicationName[AXR_MAX_APPLICATION_NAME_SIZE]{};
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrVulkanRenderer() = delete;
    /// Copy Constructor
    /// @param src Source AxrVulkanRenderer to copy from
    AxrVulkanRenderer(const AxrVulkanRenderer& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanRenderer to move from
    AxrVulkanRenderer(AxrVulkanRenderer&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanRenderer() = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanRenderer to copy from
    AxrVulkanRenderer& operator=(const AxrVulkanRenderer& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanRenderer to move from
    AxrVulkanRenderer& operator=(AxrVulkanRenderer&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Set up the vulkan renderer
    /// @param context Vulkan renderer context
    /// @param config Vulkan renderer config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setup(Context& context, const Config& config);
    /// Shut down the vulkan renderer
    /// @param context Vulkan renderer context
    static void shutDown(Context& context);

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    // ---- Instance ----

    /// Create the VkInstance
    /// @param applicationName Application name
    /// @param applicationVersion Application version
    /// @param apiLayers Api layers to use
    /// @param extensions Extensions to use
    /// @param instance Output instance
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createInstance(const char applicationName[AXR_MAX_APPLICATION_NAME_SIZE],
                                                  uint32_t applicationVersion,
                                                  const AxrVulkanExtensions::ApiLayersArray_T& apiLayers,
                                                  const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                  VkInstance& instance);
    /// Destroy the given VkInstance
    /// @param instance Instance to destroy
    static void destroyInstance(VkInstance& instance);

    /// Create the instance chain for the given extensions
    /// @param extensions Extensions to use
    /// @param instanceCreateInfo Input/Output instance create info to append the chain to
    [[nodiscard]] static AxrResult createInstanceChain(const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                       VkInstanceCreateInfo& instanceCreateInfo);

    // ---- Debug Utils ----

    /// Create the debug utils messenger
    /// @param instance VkInstance to ise
    /// @param extensions Extensions to use
    /// @param debugUtilsMessenger Output created debug utils messenger
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createDebugUtilsMessenger(const VkInstance& instance,
                                                             const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                             VkDebugUtilsMessengerEXT& debugUtilsMessenger);
    /// Destroy the debug utils messenger
    /// @param instance VkInstance to use
    /// @param debugUtilsMessenger Debug utils messenger to destroy
    static void destroyDebugUtilsMessenger(const VkInstance& instance, VkDebugUtilsMessengerEXT& debugUtilsMessenger);
};
#endif

