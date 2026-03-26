#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/enums.h"
#include "axr/lifecycle.h"
#include "types.h"
#include "vulkan/vulkanRenderer.h"

#include <cstdint>

/// Axr Renderer singleton
class AxrRenderer {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrRenderer to copy from
    AxrRenderer(const AxrRenderer& src) = delete;
    /// Move Constructor
    /// @param src Source AxrRenderer to move from
    AxrRenderer(AxrRenderer&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrRenderer to copy from
    AxrRenderer& operator=(const AxrRenderer& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrRenderer to move from
    AxrRenderer& operator=(AxrRenderer&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrRenderer();

    // ---- Destructor ----

    /// Destructor
    ~AxrRenderer();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrRenderer Config
    struct Config {
        const AxrRendererConfig* RendererConfig{};
        uint32_t ApplicationVersion{};
        char ApplicationName[AXR_MAX_APPLICATION_NAME_SIZE]{};
    };

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrRenderer singleton
    /// @return A reference to the AxrRenderer singleton
    static AxrRenderer& get();

    /// Set up the renderer
    /// @param config Renderer config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the renderer
    void shutDown();

    /// Destroy all desktop related resources
    void destroyDesktopResources();
    
    /// Render a scene
    /// @return ACR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult renderScene();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrRendererContext m_Context{};
    static constexpr size_t const& m_MaxRenderCommandCount = 100;
    bool m_IsSetup = false;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Construct the scene render commands
    /// @return The render commands
    [[nodiscard]] static AxrVector_Stack<AxrRenderCommand> constructRenderCommands();

    // ---- Renderer Context Functions ----

    /// Set up the vulkan renderer
    /// @param context Vulkan context to set up
    /// @param config Renderer config to use
    [[nodiscard]] static AxrResult setup_vulkan(AxrVulkanRendererContext& context, const Config& config);

    /// Shut down the vulkan renderer
    /// @param context Vulkan context to shut down
    static void shutDown_vulkan(AxrVulkanRendererContext& context);

    /// Destroy the vulkan desktop resources
    /// @param context Vulkan context
    static void destroyDesktopResources_vulkan(AxrVulkanRendererContext& context);

    /// Render a scene in vulkan
    /// @param context Vulkan context
    /// @param renderCommands Render commands
    /// @param renderSurface Output surface with the rendered scene
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult renderScene_vulkan(const AxrVulkanRendererContext& context,
                                                      const AxrVector_Stack<AxrRenderCommand>& renderCommands,
                                                      AxrRenderSurface& renderSurface);

    /// Present the given render surface to the desktop environment in vulkan
    /// @param context Vulkan context
    /// @param renderSurface Render surface to present
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] static AxrResult presentToDesktop_vulkan(const AxrVulkanRendererContext& context,
                                                           AxrRenderSurface& renderSurface);
};
