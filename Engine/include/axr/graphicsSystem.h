#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common.h"

// ----------------------------------------- //
// Forward Declared Handles
// ----------------------------------------- //

/// AxrGraphicsSystem Handle
typedef class AxrGraphicsSystem* AxrGraphicsSystem_T;

// ----------------------------------------- //
// Enums
// ----------------------------------------- //

/// Graphics api enum
enum AxrGraphicsApiEnum {
    AXR_GRAPHICS_API_UNDEFINED = 0,
    AXR_GRAPHICS_API_VULKAN = 1,
};

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Config for the graphics system
struct AxrGraphicsSystemConfig {
    AxrGraphicsApiEnum GraphicsApi;
};

// ----------------------------------------- //
// External Function Definitions
// ----------------------------------------- //
extern "C" {
}
