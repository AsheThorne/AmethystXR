#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/enums.h"
#include "axr/vulkanApi.h"
#include "vulkanEnvironment.h"
#include "vulkanExtensions.h"
#include "vulkanQueueFamilies.h"

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
        AxrVulkanEnvironment::DesktopContext DesktopEnvironmentContext{};
        AxrVulkanExtensions::ApiLayersArray_T ApiLayers{};
        AxrVulkanExtensions::ExtensionsArray_T Extensions{};
        AxrVulkanQueueFamilies QueueFamilies{};
        VkInstance Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT DebugUtilsMessenger = VK_NULL_HANDLE;
        VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
        VkDevice Device = VK_NULL_HANDLE;
        VkCommandPool GraphicsCommandPool = VK_NULL_HANDLE;
        VkCommandPool TransferCommandPool = VK_NULL_HANDLE;
        VkPhysicalDeviceMultiviewFeatures EnabledDeviceMultiviewFeatures{};
        VkPhysicalDeviceFeatures EnabledDeviceFeatures{};
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

    /// Append the `Next` pointer chain for the given `source` with the given `nextObject`.
    /// @param source Source object to append the `next` chain of
    /// @param nextStruct Next structure to append to the `next` chain
    static void appendNextPtrChain(VkBaseOutStructure* source, VkBaseOutStructure* nextStruct);

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

    // ---- Physical Device ----

    /// Set up the physical device
    /// @param instance Instance to use
    /// @param extensions Input/Output Extensions to use. Filter out extensions that aren't supported by the physical
    /// device
    /// @param queueFamilies Output queue families
    /// @param physicalDevice Output physical device
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult setupPhysicalDevice(const VkInstance& instance,
                                                       AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                       AxrVulkanQueueFamilies& queueFamilies,
                                                       VkPhysicalDevice& physicalDevice);
    /// Reset the physical device
    /// @param queueFamilies Queue families to reset
    /// @param physicalDevice Physical device to reset
    static void resetPhysicalDevice(AxrVulkanQueueFamilies& queueFamilies, VkPhysicalDevice& physicalDevice);

    /// Decide on which physical device we'd like to use.
    /// If OpenXR is being used though, we need to use the one it selects for us
    /// @param instance Instance to use
    /// @param extensions Extensions to check
    /// @param physicalDevice Output physical device we should use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult pickPhysicalDevice(const VkInstance& instance,
                                                      const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                      VkPhysicalDevice& physicalDevice);
    /// Score the given physical device. The higher the number, the better.
    /// 0 Means it is not suitable to use.
    /// @param instance Instance to use
    /// @param physicalDevice Physical device to score
    /// @param extensions Extensions to check
    /// @returns The physical device score
    [[nodiscard]] static uint32_t scorePhysicalDeviceSuitability(
        const VkInstance& instance,
        const VkPhysicalDevice& physicalDevice,
        const AxrVulkanExtensions::ExtensionsArray_T& extensions);
    /// Score the given physical device on its available queue families.
    /// @param instance Instance to use
    /// @param physicalDevice Physical device to score
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, has extra desired features.
    [[nodiscard]] static uint32_t scorePhysicalDeviceQueueFamilies(const VkInstance& instance,
                                                                   const VkPhysicalDevice& physicalDevice);
    /// Score the given physical device on its available extensions.
    /// @param physicalDevice Physical device to score
    /// @param extensions Extensions to check
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, supports more than just the required extensions.
    [[nodiscard]] static uint32_t scorePhysicalDeviceExtensions(
        const VkPhysicalDevice& physicalDevice,
        const AxrVulkanExtensions::ExtensionsArray_T& extensions);
    /// Score the given physical device on its available features.
    /// @param physicalDevice Physical device to score
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, has extra desired features.
    [[nodiscard]] static uint32_t scorePhysicalDeviceFeatures(const VkPhysicalDevice& physicalDevice);
    /// Score the given physical device on its available properties.
    /// @param physicalDevice Physical device to score
    /// @returns 0 -> Not suitable at all.
    /// @returns 1 -> Meets the minimum requirements.
    /// @returns More than 1, has extra desired features.
    [[nodiscard]] static uint32_t scorePhysicalDeviceProperties(const VkPhysicalDevice& physicalDevice);

    // ---- Logical Device ----

    /// Create the logical device
    /// @param extensions Extensions to use
    /// @param physicalDevice Physical device to use
    /// @param queueFamilies Input/Output Queue families to use. Sets VkQueues
    /// @param device Output logical device
    /// @param enabledFeatures Output device features that have been enabled
    /// @param enabledMultiviewFeatures Output device multiview features that have been enabled
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createLogicalDevice(const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                       const VkPhysicalDevice& physicalDevice,
                                                       AxrVulkanQueueFamilies& queueFamilies,
                                                       VkDevice& device,
                                                       VkPhysicalDeviceFeatures& enabledFeatures,
                                                       VkPhysicalDeviceMultiviewFeatures& enabledMultiviewFeatures);
    /// Destroy the logical device
    /// @param queueFamilies Queue families to reset queues
    /// @param device Logical device to destroy
    static void destroyLogicalDevice(AxrVulkanQueueFamilies& queueFamilies, VkDevice& device);

    /// Create the device chain for the given extensions
    /// @param physicalDevice Physical device to use
    /// @param extensions Extensions to use
    /// @param deviceCreateInfo Input/Output device create info to append the chain to
    /// @param enabledFeatures Output device features that have been enabled
    /// @param enabledMultiviewFeatures Output device multiview features that have been enabled
    [[nodiscard]] static AxrResult createDeviceChain(const VkPhysicalDevice& physicalDevice,
                                                     const AxrVulkanExtensions::ExtensionsArray_T& extensions,
                                                     VkDeviceCreateInfo& deviceCreateInfo,
                                                     VkPhysicalDeviceFeatures& enabledFeatures,
                                                     VkPhysicalDeviceMultiviewFeatures& enabledMultiviewFeatures);

    /// Get device features to use
    /// @param physicalDevice Physical device to use
    /// @param enabledFeatures Output enabled features to use
    /// @param enabledMultiviewFeatures Output enabled multiview features to use
    [[nodiscard]] static AxrResult getDeviceFeaturesToUse(const VkPhysicalDevice& physicalDevice,
                                                          VkPhysicalDeviceFeatures& enabledFeatures,
                                                          VkPhysicalDeviceMultiviewFeatures& enabledMultiviewFeatures);

    // ---- Command Pools ----

    /// Create all command pools
    /// @param device Device to use
    /// @param queueFamilies Queue families to use
    /// @param graphicsCommandPool Output graphics command pool
    /// @param transferCommandPool Output transfer command pool
    [[nodiscard]] static AxrResult createCommandPools(const VkDevice& device,
                                                      const AxrVulkanQueueFamilies& queueFamilies,
                                                      VkCommandPool& graphicsCommandPool,
                                                      VkCommandPool& transferCommandPool);
    /// Destroy all command pools
    /// @param device Device to use
    /// @param graphicsCommandPool Output graphics command pool
    /// @param transferCommandPool Output transfer command pool
    static void destroyCommandPools(const VkDevice& device,
                                    VkCommandPool& graphicsCommandPool,
                                    VkCommandPool& transferCommandPool);

    /// Create a command pool
    /// @param device Device to use
    /// @param queueFamilyIndex Queue family index to use
    /// @param commandPoolFlags Command pool flags
    /// @param commandPool Output created command pool
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult createCommandPool(const VkDevice& device,
                                                     uint32_t queueFamilyIndex,
                                                     VkCommandPoolCreateFlags commandPoolFlags,
                                                     VkCommandPool& commandPool);
    /// Destroy a command pool
    /// @param device Device to use
    /// @param commandPool Command pool to destroy
    static void destroyCommandPool(const VkDevice& device, VkCommandPool& commandPool);
};
#endif

