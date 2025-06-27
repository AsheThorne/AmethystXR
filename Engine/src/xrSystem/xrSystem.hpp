#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/xrSystem.h"
#include "../extensionCollection.hpp"
#include "axr/graphicsSystem.h"

// ----------------------------------------- //
// OpenXR Headers
// ----------------------------------------- //
#include <openxr/openxr.h>


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

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

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
    XrBaseOutStructure* appendNextChain(
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
