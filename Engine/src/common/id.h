#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/types.h"

#include <cstdlib>
#include <functional>

/// Axr ID
class AxrID {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrID();
    /// Constructor
    /// @param id ID to use
    // ReSharper disable once CppNonExplicitConvertingConstructor
    AxrID(AxrID_T id); // NOLINT(*-explicit-constructor)

    // ---- Operator Overloads ----

    /// Equality operator overload
    /// @param other AxrID to compare against
    /// @return True if both AxrIDs are the same
    bool operator==(const AxrID& other) const;
    /// Equality operator overload
    /// @param other Value to compare against
    /// @return True if this AxrID is the same as the given value
    bool operator==(AxrID_T other) const;

    /// Inequality operator overload
    /// @param other AxrID to compare against
    /// @return True if both AxrIDs are different
    bool operator!=(const AxrID& other) const;
    /// Inequality operator overload
    /// @param other Value to compare against
    /// @return True if this AxrID and the given value are different
    bool operator!=(AxrID_T other) const;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the ID value
    /// @return The ID
    [[nodiscard]] AxrID_T get() const;

    /// Generate an Axr ID for the given name
    /// @param name Name to use
    /// @param length Length of the given name
    /// @return The generated ID
    [[nodiscard]] static AxrID_T generateID(const char8_t* name, size_t length);
    /// Warning: This is a debug only function. This function will return an empty string if AXR_DEBUG_INFO_ENABLED is
    /// not defined.
    ///
    /// Get the name used during the creation of the given ID
    /// @param id ID to get the name of
    /// @return The name of the given ID. Or an empty string if no name for the ID was found.
    [[nodiscard]] static const char8_t* getIDName(AxrID_T id);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrID_T m_ID{};
};

// ----------------------------------------- //
// AxrID std::hash Specialization
// ----------------------------------------- //

template<>
struct std::hash<AxrID> {
    size_t operator()(const AxrID& p) const noexcept {
        // The ID is already a hash value. So no need to actually hash anything.
        return static_cast<size_t>(p.get());
    }
};
