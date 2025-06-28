#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/xrSystem.h"
#include "../extensionCollection.hpp"
#include "axr/graphicsSystem.h"

// Vulkan headers are required for <openxr/openxr_platform.h>
#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
// ----------------------------------------- //
// Vulkan Headers
// ----------------------------------------- //
#include <vulkan/vulkan_core.h>
#endif

// ----------------------------------------- //
// OpenXR Headers
// ----------------------------------------- //
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>


/// Axr Xr System
class AxrXrSystem {
public:
    // ----------------------------------------- //
    // Structs
    // ----------------------------------------- //

    /// Xr System Config
    struct Config {
        const char* ApplicationName;
        uint32_t ApplicationVersion;
        AxrGraphicsApiEnum GraphicsApi;
        AxrXrReferenceSpaceEnum StageReferenceSpace;
        uint32_t ApiLayerCount;
        AxrXrApiLayer_T* ApiLayers;
        uint32_t ExtensionCount;
        AxrXrExtension_T* Extensions;
    };

    // Xr view data
    struct View {
        XrViewConfigurationView ViewConfigurationView;
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param config Xr system config
    AxrXrSystem(const Config& config);
    /// Copy Constructor
    /// @param src Source AxrXrSystem to copy from
    AxrXrSystem(const AxrXrSystem& src) = delete;
    /// Move Constructor
    /// @param src Source AxrXrSystem to move from
    AxrXrSystem(AxrXrSystem&& src) noexcept = delete;

    // ---- Destructor ----

    /// Destructor
    ~AxrXrSystem();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrXrSystem to copy from
    AxrXrSystem& operator=(const AxrXrSystem& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrXrSystem to move from
    AxrXrSystem& operator=(AxrXrSystem&& src) noexcept = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the xr session is running
    /// @returns True if the xr session is running
    [[nodiscard]] bool isXrSessionRunning() const;

    /// Start the xr session
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult startXrSession();
    /// Signal that we want to stop the xr session
    void stopXrSession();

    // ---- For Internal Use ----
    // These functions are only to be used internally in the AmethystXr engine.
    // They have not been given a publicly accessible function in the 'include headers' to be used by an application.

    /// Set up the xr system
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup();
    /// Reset setup()
    void resetSetup();

    /// Process the xr events
    void processEvents();

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    // ---- Vulkan Functions ----

    /// Create the vulkan instance to use
    /// @param pfnGetInstanceProcAddr A function pointer to vkGetInstanceProcAddr or a compatible entry point
    /// @param createInfo The VkInstanceCreateInfo
    /// @param vkInstance Output created vkInstance
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createVulkanInstance(
        PFN_vkGetInstanceProcAddr pfnGetInstanceProcAddr,
        const VkInstanceCreateInfo* createInfo,
        VkInstance* vkInstance
    ) const;
#endif

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //

    // ---- Config Variables ----
    const char* m_ApplicationName;
    uint32_t m_ApplicationVersion;
    AxrGraphicsApiEnum m_GraphicsApi;
    AxrXrReferenceSpaceEnum m_StageReferenceSpace;
    AxrExtensionCollection<AxrXrApiLayer_T, AxrXrApiLayerTypeEnum> m_ApiLayers;
    AxrExtensionCollection<AxrXrExtension_T, AxrXrExtensionTypeEnum> m_Extensions;

    // ---- Data ----
    XrInstance m_Instance;
    XrDebugUtilsMessengerEXT m_DebugUtilsMessenger;
    XrSystemId m_SystemId;
    std::vector<XrViewConfigurationType> m_SupportedViewConfigurationTypes;
    XrViewConfigurationType m_ViewConfigurationType;
    std::vector<View> m_Views;
    std::vector<XrEnvironmentBlendMode> m_SupportedEnvironmentBlendModes;
    XrEnvironmentBlendMode m_EnvironmentBlendMode;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

#ifdef AXR_SUPPORTED_GRAPHICS_VULKAN
    // ---- Vulkan Functions ----

    /// Choose the vulkan api version to use
    /// @param desiredApiVersion The desired vulkan api version to use
    /// @param apiVersion Output vulkan api version to use
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodscard]] AxrResult chooseVulkanApiVersion(uint32_t desiredApiVersion, uint32_t& apiVersion) const;
#endif

    // ---- Instance ----

    /// Create the instance
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createInstance();
    /// Destroy the instance
    void destroyInstance();

    /// Create the instance pNext chain
    /// @returns The instance structure chain
    [[nodiscard]] XrBaseOutStructure* createInstanceChain() const;

    // ---- .Next Chain ----

    /// Destroy the given chain
    /// @param chain The head of the chain
    void destroyChain(XrBaseOutStructure* chain);

    /// Append the given structure to the given chain
    /// @param chain The head of the chain
    /// @param structure The structure to add to the chain
    /// @param structureSize The size of the structure
    [[nodiscard]] XrBaseOutStructure* appendNextChain(
        XrBaseOutStructure* chain,
        const XrBaseInStructure* structure,
        size_t structureSize
    ) const;

    // ---- Api Layers / Extensions ----

    /// Get a collection of all api layer names to use
    /// @returns A collection of api layer names
    [[nodiscard]] std::vector<const char*> getAllApiLayerNames() const;
    /// Get a collection of all extension names to use
    /// @returns A collection of extension names
    [[nodiscard]] std::vector<const char*> getAllExtensionNames() const;

    /// Add the required extensions to m_Extensions
    void addRequiredExtensions();

    /// Find the supported api layers
    /// @returns The supported api layers
    [[nodiscard]] std::vector<std::string> getSupportedApiLayers() const;
    /// Find the supported extensions
    /// @returns The supported extensions
    [[nodiscard]] std::vector<std::string> getSupportedExtensions() const;

    /// Remove api layers from m_ApiLayers that aren't supported by the instance
    void removeUnsupportedApiLayers();
    /// Remove extensions from m_Extensions that aren't supported by the instance 
    void removeUnsupportedExtensions();

    // ---- Debug Utils ----

    /// Create the debug utils messenger create info
    /// @retrns The debug utils messenger create info
    [[nodiscard]] XrDebugUtilsMessengerCreateInfoEXT createDebugUtilsCreateInfo() const;

    /// Create the debug utils
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult createDebugUtils();
    /// Destroy the debug utils
    void destroyDebugUtils();

    // ---- System ----

    /// Set the system ID
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setSystemId();
    /// Reset setSystemId()
    void resetSystemId();

    /// Log the system details
    void logSystemDetails() const;

    // ---- View Configuration ----

    /// Set the view configuration
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setViewConfiguration();
    /// Reset setViewConfiguration()
    void resetViewConfiguration();

    /// Get view configuration type
    [[nodiscard]] AxrResult getViewConfigurationType(XrViewConfigurationType& viewConfigurationType) const;

    // ---- Environment ----

    /// Set the environment blend mode
    /// @returns AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setEnvironmentBlendMode();
    /// reset setEnvironmentBlendMode()
    void resetEnvironmentBlendMode();

    // ----------------------------------------- //
    // Private Static Functions
    // ----------------------------------------- //

    /// Debug messages callback function
    /// @param messageSeverities The severity of the message
    /// @param messageTypes The type of the message
    /// @param pCallbackData Callback data
    /// @param userData User data
    /// @returns Indicates to the calling layer the application's desire to abort the call.
    /// Applications should always return XR_FALSE
    static XrBool32 debugUtilsCallback(
        XrDebugUtilsMessageSeverityFlagsEXT messageSeverities,
        XrDebugUtilsMessageTypeFlagsEXT messageTypes,
        const XrDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* userData
    );
};
