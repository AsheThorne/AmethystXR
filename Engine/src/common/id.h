#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <cstdint>
#include <cstdlib>
#include <functional>

/// Axr ID
class AxrID {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    /// @param id ID to use
    // ReSharper disable once CppNonExplicitConvertingConstructor
    AxrID(uint64_t id); // NOLINT(*-explicit-constructor)

    // ---- Operator Overloads ----

    /// Equality operator overload
    /// @param other AxrID to compare against
    /// @return True if both AxrIDs are the same
    bool operator==(const AxrID& other) const {
        return m_ID == other.m_ID;
    }
    /// Equality operator overload
    /// @param other Value to compare against
    /// @return True if this AxrID is the same as the given value
    bool operator==(const uint64_t other) const {
        return m_ID == other;
    }

    /// Inequality operator overload
    /// @param other AxrID to compare against
    /// @return True if both AxrIDs are different
    bool operator!=(const AxrID& other) const {
        return !(*this == other);
    }
    /// Inequality operator overload
    /// @param other Value to compare against
    /// @return True if this AxrID and the given value are different
    bool operator!=(const uint64_t other) const {
        return !(*this == other);
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the ID value
    /// @return The ID
    [[nodiscard]] uint64_t get() const;

    /// Generate an Axr ID for the given string
    /// @param string String to use
    /// @param length Length of the given string
    /// @return Axr ID
    [[nodiscard]] static uint64_t generateID(const char* string, size_t length);

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    uint64_t m_ID{};
};

// ----------------------------------------- //
// User defined literals
// ----------------------------------------- //

uint64_t operator""_id(const char* string, size_t length);

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
