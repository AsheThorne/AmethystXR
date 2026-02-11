#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../common/extensionArray.h"
#include "../../common/vector_stack.h"
#include "axr/common/defines.h"
#include "axr/common/enums.h"
#include "axr/vulkanApi.h"

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

    using ApiLayersArray_T = AxrExtensionArray<AxrVulkanApiLayer, AxrVulkanApiLayerMaxCount>;
    using ExtensionsArray_T = AxrExtensionArray<AxrVulkanExtension, AxrVulkanExtensionMaxCount>;

    using ApiLayerNamesArray_T = AxrArray<const char*, AxrVulkanApiLayerMaxCount>;
    using ExtensionNamesArray_T = AxrArray<const char*, AxrVulkanExtensionMaxCount>;

    using InstanceChainTuple_T = std::tuple<VkInstanceCreateInfo, VkDebugUtilsMessengerCreateInfoEXT>;

    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// Vulkan renderer context
    struct Context {
        ApiLayersArray_T ApiLayers{};
        ExtensionsArray_T Extensions{};
        VkInstance Instance = VK_NULL_HANDLE;
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

    /// Populate ApiLayers within `context` with the given api layers
    /// @param apiLayerCount Number of api layers in the given array
    /// @param apiLayers Api layers array
    /// @param dstApiLayers Output extension array to populate
    static void populateApiLayers(uint32_t apiLayerCount,
                                  const AxrVulkanApiLayer apiLayers[],
                                  ApiLayersArray_T& dstApiLayers);
    /// Populate Extensions within `context` with the given extensions
    /// @param extensionCount Number of extensions in the given array
    /// @param extensions Extensions array
    /// @param dstExtensions Output extension array to populate
    static void populateExtensions(uint32_t extensionCount,
                                   const AxrVulkanExtension extensions[],
                                   ExtensionsArray_T& dstExtensions);

    /// Get the supported api layer names
    /// @param apiLayerNames Output supported api layer names
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult getSupportedApiLayers(AxrVector_Stack<const char*>& apiLayerNames);
    /// Get the supported instance extension names
    /// @param extensionNames Output supported extension names
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult getSupportedInstanceExtensions(AxrVector_Stack<const char*>& extensionNames);

    /// Filter supported api layers from the given array
    /// @param srcApiLayers Api layers to filter
    /// @return An array containing only supported api layers
    [[nodiscard]] static ApiLayersArray_T filterSupportedApiLayers(const ApiLayersArray_T& srcApiLayers);
    /// Filter supported instance extensions from the given array.
    /// Device extensions are ignored and are always passed through to the output array.
    /// @param srcExtensions Extensions to filter
    /// @return An array containing only supported extensions
    [[nodiscard]] static ExtensionsArray_T filterSupportedInstanceExtensions(const ExtensionsArray_T& srcExtensions);

    /// Get all api layer names from the given api layers. Strings are null terminated
    /// @param apiLayers Api layers
    /// @return Api layer names
    [[nodiscard]] static ApiLayerNamesArray_T getApiLayerNames(const ApiLayersArray_T& apiLayers);
    /// Get all instance extension names from the given extensions. Strings are null terminated
    /// @param extensions Extensions
    /// @return Instance extension names
    [[nodiscard]] static ExtensionNamesArray_T getInstanceExtensionNames(const ExtensionsArray_T& extensions);

    /// Append the `Next` pointer chain for the given `source` with the given `nextObject`.
    /// @param source Source object to append the `next` chain of
    /// @param nextStruct Next structure to append to the `next` chain
    static void appendNextPtrChain(VkBaseOutStructure* source, VkBaseOutStructure* nextStruct);

    /// Debug utils messages callback function
    /// @param messageSeverity The severity of the message
    /// @param messageType The type of the message
    /// @param pCallbackData Callback data
    /// @param pUserData User data
    /// @returns The application should always return VK_FALSE. VK_TRUE is typically only used in layer development
    static VkBool32 VKAPI_CALL debugUtilsCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                  void* pUserData);

    /// Log al of the given api layer names and extension names
    /// @param message Message to prefix the log message with
    /// @param apiLayerNames Api layer names to log. Or nullptr to not log any
    /// @param extensionNames Extension names to log. Or nullptr to not log any
    static void logExtensionNames(const char* message,
                                  const ApiLayerNamesArray_T* apiLayerNames,
                                  const ExtensionNamesArray_T* extensionNames);

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
                                                  const ApiLayersArray_T& apiLayers,
                                                  const ExtensionsArray_T& extensions,
                                                  VkInstance& instance);
    /// Destroy the given VkInstance
    /// @param instance Instance to destroy
    static void destroyInstance(VkInstance& instance);

    /// Create the instance chain for the given extensions
    /// @param extensions Extensions to use
    /// @param instanceCreateInfo Input/Output instance create info to append the chain to
    [[nodiscard]] static AxrResult createInstanceChain(const ExtensionsArray_T& extensions,
                                                       VkInstanceCreateInfo& instanceCreateInfo);
};
#endif

