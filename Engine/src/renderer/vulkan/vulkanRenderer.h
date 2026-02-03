#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../common/extensionArray.h"
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
    // Public Structs
    // ----------------------------------------- //

    /// Vulkan renderer context
    struct Context {
        AxrExtensionArray<AxrVulkanApiLayer, AxrVulkanApiLayerMaxCount> ApiLayers{};
        AxrExtensionArray<AxrVulkanExtension, AxrVulkanExtensionMaxCount> Extensions{};
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
    /// @param context Vulkan context
    /// @param apiLayerCount Number of api layers in the given array
    /// @param apiLayers Api layers array
    static void populateApiLayers(Context& context, uint32_t apiLayerCount, const AxrVulkanApiLayer apiLayers[]);
    /// Populate Extensions within `context` with the given extensions
    /// @param context Vulkan context
    /// @param extensionCount Number of extensions in the given array
    /// @param extensions Extensions array
    static void populateExtensions(Context& context, uint32_t extensionCount, const AxrVulkanExtension extensions[]);
};
#endif

