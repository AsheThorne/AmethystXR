#pragma once
#ifdef AXR_VULKAN_SUPPORTED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../common/extensionArray.h"
#include "../../common/vector_stack.h"
#include "axr/vulkanApi.h"

/// Vulkan Api Layers/Extensions related functions
class AxrVulkanExtensions {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    using ApiLayersArray_T = AxrExtensionArray<AxrVulkanApiLayer, AxrVulkanApiLayerMaxCount>;
    using ExtensionsArray_T = AxrExtensionArray<AxrVulkanExtension, AxrVulkanExtensionMaxCount>;
    using ApiLayerNamesArray_T = AxrArray<const char*, AxrVulkanApiLayerMaxCount>;
    using ExtensionNamesArray_T = AxrArray<const char*, AxrVulkanExtensionMaxCount>;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrVulkanExtensions() = delete;
    /// Copy Constructor
    /// @param src Source AxrVulkanExtensions to copy from
    AxrVulkanExtensions(const AxrVulkanExtensions& src) = delete;
    /// Move Constructor
    /// @param src Source AxrVulkanExtensions to move from
    AxrVulkanExtensions(AxrVulkanExtensions&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrVulkanExtensions() = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVulkanExtensions to copy from
    AxrVulkanExtensions& operator=(const AxrVulkanExtensions& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrVulkanExtensions to move from
    AxrVulkanExtensions& operator=(AxrVulkanExtensions&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
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
    /// Get the supported device extension names
    /// @param physicalDevice Physical device to get extensions for
    /// @param extensionNames Output supported extension names
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult getSupportedDeviceExtensions(const VkPhysicalDevice& physicalDevice,
                                                                AxrVector_Stack<const char*>& extensionNames);

    /// Filter supported api layers from the given array
    /// @param srcApiLayers Api layers to filter
    /// @return An array containing only supported api layers. Or the original array if there was an error
    [[nodiscard]] static ApiLayersArray_T filterSupportedApiLayers(const ApiLayersArray_T& srcApiLayers);
    /// Filter supported instance extensions from the given array.
    /// Device extensions are ignored and are always passed through to the output array.
    /// @param srcExtensions Extensions to filter
    /// @return An array containing only supported extensions. Or the original array if there was an error
    [[nodiscard]] static ExtensionsArray_T filterSupportedInstanceExtensions(const ExtensionsArray_T& srcExtensions);
    /// Filter supported device extensions from the given array.
    /// Device extensions are ignored and are always passed through to the output array.
    /// @param physicalDevice Physical device to use
    /// @param srcExtensions Extensions to filter
    /// @return An array containing only supported extensions. Or the original array if there was an error
    [[nodiscard]] static ExtensionsArray_T filterSupportedDeviceExtensions(const VkPhysicalDevice& physicalDevice,
                                                                           const ExtensionsArray_T& srcExtensions);

    /// Get all api layer names from the given api layers. Strings are null terminated
    /// @param apiLayers Api layers
    /// @return Api layer names
    [[nodiscard]] static ApiLayerNamesArray_T getApiLayerNames(const ApiLayersArray_T& apiLayers);
    /// Get all instance extension names from the given extensions. Strings are null terminated
    /// @param extensions Extensions
    /// @return Instance extension names
    [[nodiscard]] static ExtensionNamesArray_T getInstanceExtensionNames(const ExtensionsArray_T& extensions);
    /// Get all device extension names from the given extensions. Strings are null terminated
    /// @param extensions Extensions
    /// @return Device extension names
    [[nodiscard]] static ExtensionNamesArray_T getDeviceExtensionNames(const ExtensionsArray_T& extensions);

    /// Log al of the given api layer names and extension names
    /// @param message Message to prefix the log message with
    /// @param apiLayerNames Api layer names to log. Or nullptr to not log any
    /// @param extensionNames Extension names to log. Or nullptr to not log any
    static void logExtensionNames(const char* message,
                                  const ApiLayerNamesArray_T* apiLayerNames,
                                  const ExtensionNamesArray_T* extensionNames);

    // ---- Debug Utils ----

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

    /// Create the VkDebugUtilsMessengerCreateInfoEXT
    /// @param extensions Extensions to use
    /// @return The VkDebugUtilsMessengerCreateInfoEXT
    static VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerCreateInfo(const ExtensionsArray_T& extensions);
};
#endif
