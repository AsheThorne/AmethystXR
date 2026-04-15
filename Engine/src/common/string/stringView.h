#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "stringIterator.h"

/// Axr String View
///
/// This class doesn't own the string it references. It just points to a start position and a size. So the string isn't
/// guaranteed to be null terminated.
class AxrStringView {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    using Iterator = AxrStringIterator;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrStringView();
    /// Constructor
    /// @param string String
    /// @param size String size
    AxrStringView(const char8_t* string, size_t size);

    // ---- Operator Overloads ----

    /// == Operator overload
    /// @param srcString String to check equality with
    /// @returns True if this AxrStringView and the given string is equal
    [[nodiscard]] bool operator==(const char8_t* srcString) const;
    /// == Operator overload
    /// @param srcString String to check equality with
    /// @returns True if this AxrStringView and the given string is equal
    [[nodiscard]] bool operator==(const AxrStringView& srcString) const;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the number of char8_ts of this string view. It's not necessarily one char8_t per character
    /// @return This string view's number of char8_ts
    [[nodiscard]] size_t size() const;
    /// Check if this string view is empty
    /// @return True if this string view is empty
    [[nodiscard]] bool empty() const;

    /// The beginning of the string view
    /// @return An iterator to the beginning of the string
    [[nodiscard]] Iterator begin() const;
    /// The end of the string view
    /// @return An iterator to the end of the string
    [[nodiscard]] Iterator end() const;

    /// Get the string data. Please note, this is NOT guaranteed to be null terminated. Do NOT traverse beyond this
    /// string's size.
    /// @return The string data
    [[nodiscard]] const char8_t* data() const;

    /// Get the substring starting from the given character index, and ending after `count` number of characters.
    /// @param characterIndex Character index to start at
    /// @param count Number of characters to include in the substring
    /// @return A new AxrStringView, storing the substring
    [[nodiscard]] AxrStringView substring(size_t characterIndex, size_t count) const;
    /// Get the substring starting from the given `startIterator`, and ending after `count` number of characters.
    /// @param startIterator Character iterator to start at
    /// @param count Number of characters to include in the substring
    /// @return A new AxrStringView, storing the substring
    [[nodiscard]] AxrStringView substring(const Iterator& startIterator, size_t count) const;
    /// Get the substring starting from the given `startIterator`, and ending at the given `endIterator`.
    /// @param startIterator Character iterator to start at
    /// @param endIterator Character iterator to end at. This character is NOT included in the substring.
    /// @return A new AxrStringView, storing the substring
    [[nodiscard]] AxrStringView substring(const Iterator& startIterator, const Iterator& endIterator) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    const char8_t* m_Data{};
    size_t m_Size{};
};
