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
    class Context {
    public:
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //

        union {
            // Don't wrap in an AXR_VULKAN_SUPPORTED preprocessor.
            // An empty AxrVulkanRenderer::Context struct is declared if AXR_VULKAN_SUPPORTED isn't defined
            AxrVulkanRenderer::Context Vulkan;
        };
        AxrRendererApiTypeEnum ApiType = AXR_RENDERER_API_TYPE_UNDEFINED;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        Context() {
            // NOTE (Ashe): If a new api is added, like OpenGL, then modify the line below to be this instead
            //  memset(reinterpret_cast<void*>(&Vulkan), 0, std::max(sizeof(Vulkan), sizeof(OpenGL)));
            memset(reinterpret_cast<void*>(&Vulkan), 0, sizeof(Vulkan));
        };

        /// Constructor
        explicit Context(const AxrRendererApiTypeEnum apiType) :
            ApiType(apiType) {
            switch (ApiType) {
                case AXR_RENDERER_API_TYPE_VULKAN: {
                    // We have to zero out Vulkan first. Since we trigger the copy assignment operator instead of the
                    // copy constructor. So it will try cleaning up garbage data which can cause undefined behavior.
                    memset(reinterpret_cast<void*>(&Vulkan), 0, sizeof(Vulkan));
                    Vulkan = AxrVulkanRenderer::Context();
                    break;
                }
                case AXR_RENDERER_API_TYPE_UNDEFINED: {
                    // Nothing to do. No api selected
                    break;
                }
                default: {
                    axrLogWarning("Failed to properly construct renderer context. Unknown renderer api.");
                    break;
                }
            }
        }

        /// Copy Constructor
        /// @param src Source Context to copy from
        Context(const Context& src) = delete;

        /// Move Constructor
        /// @param src Source Context to move from
        Context(Context&& src) noexcept {
            switch (src.ApiType) {
                case AXR_RENDERER_API_TYPE_VULKAN: {
                    Vulkan = std::move(src.Vulkan);
                    break;
                }
                case AXR_RENDERER_API_TYPE_UNDEFINED: {
                    // Nothing to do. No api selected
                    break;
                }
                default: {
                    axrLogWarning("Failed to properly move renderer context. Unknown renderer api.");
                    break;
                }
            }

            ApiType = src.ApiType;

            src.ApiType = AXR_RENDERER_API_TYPE_UNDEFINED;
        }

        // ---- Destructor ----

        /// Destructor
        ~Context() {
            switch (ApiType) {
                case AXR_RENDERER_API_TYPE_VULKAN: {
                    using ContextApi_T = AxrVulkanRenderer::Context;
                    Vulkan.~ContextApi_T();
                    break;
                }
                case AXR_RENDERER_API_TYPE_UNDEFINED: {
                    // Nothing to do. No api selected
                    break;
                }
                default: {
                    axrLogWarning("Failed to properly destroy renderer context. Unknown renderer api.");
                    break;
                }
            }

            ApiType = AXR_RENDERER_API_TYPE_UNDEFINED;
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source Context to copy from
        Context& operator=(const Context& src) = delete;

        /// Move Assignment Operator
        /// @param src Source Context to move from
        Context& operator=(Context&& src) noexcept {
            if (this != &src) {
                switch (src.ApiType) {
                    case AXR_RENDERER_API_TYPE_VULKAN: {
                        Vulkan = std::move(src.Vulkan);
                        break;
                    }
                    case AXR_RENDERER_API_TYPE_UNDEFINED: {
                        // Nothing to do. No api selected
                        break;
                    }
                    default: {
                        axrLogWarning("Failed to properly move renderer context. Unknown renderer api.");
                        break;
                    }
                }

                ApiType = src.ApiType;

                src.ApiType = AXR_RENDERER_API_TYPE_UNDEFINED;
            }
            return *this;
        }
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

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    Context m_Context{};
    bool m_IsSetup = false;
};
