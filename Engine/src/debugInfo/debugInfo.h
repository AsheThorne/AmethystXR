#pragma once
#ifdef AXR_DEBUG_INFO_ENABLED

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../common/containers/unorderedMap_dynamic.h"
#include "../common/id.h"
#include "../common/string.h"
#include "axr/common/enums.h"

/// Axr Debug Info singleton
class AxrDebugInfo {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrDebugInfo to copy from
    AxrDebugInfo(const AxrDebugInfo& src) = delete;
    /// Move Constructor
    /// @param src Source AxrDebugInfo to move from
    AxrDebugInfo(AxrDebugInfo&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrDebugInfo to copy from
    AxrDebugInfo& operator=(const AxrDebugInfo& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrDebugInfo to move from
    AxrDebugInfo& operator=(AxrDebugInfo&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrDebugInfo();

    // ---- Destructor ----

    /// Destructor
    ~AxrDebugInfo();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrDebugInfo Config
    struct Config {
        /// Must be a power of 2
        uint32_t MaxIDCount;
    };

    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    AxrUnorderedMap_Dynamic<AxrID, AxrString> IDNames{};

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrDebugInfo singleton
    /// @return A reference to the AxrDebugInfo singleton
    static AxrDebugInfo& get();

    /// Set up the debug info
    /// @param config Debug info config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the debug info
    void shutDown();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    bool m_IsSetup = false;
};

#endif
