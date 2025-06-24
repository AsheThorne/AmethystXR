#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/common/callback.h"

namespace axr {
    /// Generic callback
    /// @tparam T Return type
    /// @tparam Args Callback arguments
    template <typename T, typename... Args>
    // ReSharper disable once CppInconsistentNaming
    using Callback = AxrCallback<T(Args...)>;
}
