#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/ioActionSystem.h"

#ifdef AXR_USE_PLATFORM_WIN32
// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>
#endif


#ifdef AXR_USE_PLATFORM_WIN32
/// Convert the given wParam to an AxrBoolInputActionEnum
/// @param wParam wParam to convert
/// @returns the converted wParam
[[nodiscard]] AxrBoolInputActionEnum axrWParamToBoolInputActionEnum(WPARAM wParam);
#endif
