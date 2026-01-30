#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/enums.h"
#include "axr/lifecycle.h"
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

    /// AxrRenderer Context
    struct Context {
        union {
            // Don't wrap in an AXR_VULKAN_SUPPORTED preprocessor.
            // An empty AxrVulkanRenderer::Context struct is declared if AXR_VULKAN_SUPPORTED isn't defined
            AxrVulkanRenderer::Context Vulkan;
        };
        AxrRendererApiTypeEnum ApiType{};
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

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    Context m_Context{};
    bool m_IsSetup = false;
};
