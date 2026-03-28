#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "vulkan/types.h"

// ---------------------------------------------------------------------------------- //
//                                  Render Commands                                   //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Axr render command type enum
enum AxrRenderCommandTypeEnum {
    AXR_RENDER_COMMAND_TYPE_UNDEFINED = 0,
    AXR_RENDER_COMMAND_TYPE_WAIT_FOR_PREVIOUS_FRAME,
    AXR_RENDER_COMMAND_TYPE_UPDATE_UNIFORM_BUFFERS,
    AXR_RENDER_COMMAND_TYPE_BEGIN_RENDERING,
    AXR_RENDER_COMMAND_TYPE_END_RENDERING,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// AXR_RENDER_COMMAND_TYPE_WAIT_FOR_PREVIOUS_FRAME data
struct AxrWaitForPreviousFrameCommandData {};

/// AXR_RENDER_COMMAND_TYPE_UPDATE_UNIFORM_BUFFERS data
struct AxrUpdateUniformBuffersCommandData {};

/// AXR_RENDER_COMMAND_TYPE_BEGIN_RENDERING data
struct AxrBeginRenderingCommandData {};

/// AXR_RENDER_COMMAND_TYPE_END_RENDERING data
struct AxrEndRenderingCommandData {};

/// Axr render command data
union AxrRenderCommandData {
    AxrWaitForPreviousFrameCommandData WaitForPreviousFrame;
    AxrUpdateUniformBuffersCommandData UpdateUniformBuffers;
    AxrBeginRenderingCommandData BeginRendering;
    AxrEndRenderingCommandData EndRendering;
};

/// Axr render command
struct AxrRenderCommand {
    AxrRenderCommandData RenderData;
    AxrRenderCommandTypeEnum Type;
};

// ---------------------------------------------------------------------------------- //
//                                  Render Context                                    //
// ---------------------------------------------------------------------------------- //

/// AxrRenderer Context
class AxrRendererContext {
public:
    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //

    union {
        // Don't wrap in an AXR_VULKAN_SUPPORTED preprocessor.
        // An empty AxrVulkanRendererContext struct is declared if AXR_VULKAN_SUPPORTED isn't defined
        AxrVulkanRendererContext Vulkan;
    };
    AxrRendererApiTypeEnum ApiType = AXR_RENDERER_API_TYPE_UNDEFINED;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrRendererContext() {
        // NOTE (Ashe): If a new api is added, like OpenGL, then modify the line below to be this instead
        //  memset(reinterpret_cast<void*>(&Vulkan), 0, std::max(sizeof(Vulkan), sizeof(OpenGL)));
        memset(reinterpret_cast<void*>(&Vulkan), 0, sizeof(Vulkan));
    };

    /// Constructor
    /// @param apiType Api type to use
    explicit AxrRendererContext(const AxrRendererApiTypeEnum apiType) :
        ApiType(apiType) {
        switch (ApiType) {
            case AXR_RENDERER_API_TYPE_VULKAN: {
                // We have to zero out Vulkan first. Since we trigger the copy assignment operator instead of the
                // copy constructor. So it will try cleaning up garbage data which can cause undefined behavior.
                memset(reinterpret_cast<void*>(&Vulkan), 0, sizeof(Vulkan));
                Vulkan = AxrVulkanRendererContext{};
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
    /// @param src Source AxrRendererContext to copy from
    AxrRendererContext(const AxrRendererContext& src) = delete;

    /// Move Constructor
    /// @param src Source AxrRendererContext to move from
    AxrRendererContext(AxrRendererContext&& src) noexcept {
        move_internal(std::move(src));
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrRendererContext() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrRendererContext to copy from
    AxrRendererContext& operator=(const AxrRendererContext& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrRendererContext to move from
    AxrRendererContext& operator=(AxrRendererContext&& src) noexcept {
        if (this != &src) {
            cleanup();

            move_internal(std::move(src));
        }
        return *this;
    }

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        switch (ApiType) {
            case AXR_RENDERER_API_TYPE_VULKAN: {
                using ContextApi_T = AxrVulkanRendererContext;
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

    /// Move the given AxrRendererContext to this class
    /// @param src AxrRendererContext to move
    void move_internal(AxrRendererContext&& src) {
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
};

// ---------------------------------------------------------------------------------- //
//                                  Render Surface                                    //
// ---------------------------------------------------------------------------------- //

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

class AxrRenderSurface {
public:
    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //

    union {
        // Don't wrap in an AXR_VULKAN_SUPPORTED preprocessor.
        // An empty AxrVulkanRenderSurface struct is declared if AXR_VULKAN_SUPPORTED isn't defined
        AxrVulkanRenderSurface Vulkan;
    };
    AxrRendererApiTypeEnum ApiType = AXR_RENDERER_API_TYPE_UNDEFINED;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrRenderSurface() {
        // NOTE (Ashe): If a new api is added, like OpenGL, then modify the line below to be this instead
        //  memset(reinterpret_cast<void*>(&Vulkan), 0, std::max(sizeof(Vulkan), sizeof(OpenGL)));
        memset(reinterpret_cast<void*>(&Vulkan), 0, sizeof(Vulkan));
    };

    /// Constructor
    /// @param apiType Api type to use
    explicit AxrRenderSurface(const AxrRendererApiTypeEnum apiType) :
        ApiType(apiType) {
        switch (ApiType) {
            case AXR_RENDERER_API_TYPE_VULKAN: {
                // We have to zero out Vulkan first. Since we trigger the copy assignment operator instead of the
                // copy constructor. So it will try cleaning up garbage data which can cause undefined behavior.
                memset(reinterpret_cast<void*>(&Vulkan), 0, sizeof(Vulkan));
                Vulkan = AxrVulkanRenderSurface{};
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
    /// @param src Source AxrRenderSurface to copy from
    AxrRenderSurface(const AxrRenderSurface& src) = delete;

    /// Move Constructor
    /// @param src Source AxrRenderSurface to move from
    AxrRenderSurface(AxrRenderSurface&& src) noexcept {
        move_internal(std::move(src));
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrRenderSurface() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrRenderSurface to copy from
    AxrRenderSurface& operator=(const AxrRenderSurface& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrRenderSurface to move from
    AxrRenderSurface& operator=(AxrRenderSurface&& src) noexcept {
        if (this != &src) {
            cleanup();

            move_internal(std::move(src));
        }
        return *this;
    }

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        switch (ApiType) {
            case AXR_RENDERER_API_TYPE_VULKAN: {
                using ContextApi_T = AxrVulkanRenderSurface;
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

    /// Move the given AxrRenderSurface to this class
    /// @param src AxrRenderSurface to move
    void move_internal(AxrRenderSurface&& src) {
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
};
