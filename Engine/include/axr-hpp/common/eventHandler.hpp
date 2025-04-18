#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/eventHandler.h"

namespace axr {
    /// Generic event handler
    /// @tparam Args Callback arguments
    template <typename... Args>
    // ReSharper disable once CppInconsistentNaming
    using EventHandler = AxrEventHandler<Args...>;
}
