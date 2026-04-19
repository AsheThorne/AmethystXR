// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "platform.h"

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

bool AxrPlatform::isPathAbsolute(const AxrPath& path) {
    return !path.empty() && *path.begin() == u8"/";
}
