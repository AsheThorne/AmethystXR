#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../containers/array.h"

/// Axr String Iterator
class AxrStringIterator {
public:
    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrStringIterator() = default;

    /// Constructor
    /// @param character The character for this iterator to point to
    explicit AxrStringIterator(const char8_t* character);

    // ---- Operator Overloads ----

    /// Prefix increment operator overload
    /// @return The iterator, incremented by 1
    AxrStringIterator& operator++();
    /// Postfix increment operator overload
    /// @return the current iterator, before incrementing it by 1
    AxrStringIterator operator++(int);

    /// Prefix decrement operator overload
    /// @return The iterator, decremented by 1
    AxrStringIterator& operator--();
    /// Postfix decrement operator overload
    /// @return the current iterator, before decrementing it by 1
    AxrStringIterator operator--(int);

    /// Equality operator overload
    /// @other Iterator to compare against
    /// @return True if both iterators point to the same character
    bool operator==(const AxrStringIterator& other) const;
    /// Inequality operator overload
    /// @other Iterator to compare against
    /// @return True if both iterators point to different characters
    bool operator!=(const AxrStringIterator& other) const;

    /// Get the character data for this iterator
    /// @return The character data for this iterator
    AxrArray<char8_t, 4> operator*() const;

    /// Get the pointer to the character data
    /// @return The pointer to the character data
    [[nodiscard]] const char8_t* getDataPtr() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    const char8_t* m_Character{};
};
