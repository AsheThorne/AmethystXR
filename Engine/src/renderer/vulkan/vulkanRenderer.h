#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/enums.h"

#include <cstdint>

/// Axr Vulkan renderer static class. This class only holds static functions.
class AxrVulkanRenderer {
public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

#ifndef AXR_VULKAN_SUPPORTED
    /// Empty Vulkan renderer context
    struct Context {};
#else
    /// Vulkan renderer context
    struct Context {
        bool IsSetup;
    };
#endif

#ifndef AXR_VULKAN_SUPPORTED
    /// Empty Vulkan renderer setup Config
    struct Config {};
#else
    /// Vulkan renderer setup Config
    struct Config {
        uint32_t ApplicationVersion{};
        char ApplicationName[AXR_MAX_APPLICATION_NAME_SIZE]{};
        // TODO (Ashe): Api layers and extensions. Probably need to use the AxrAllocator.
    };
#endif

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
#ifdef AXR_VULKAN_SUPPORTED
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //
#endif
};

