#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <cstdlib>

/// Axr String View
///
/// This class doesn't own the string it references. It just points to a start position and a size. So the string isn't
/// guaranteed to be null terminated.
class AxrStringView {
public:
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

    /// Get the string data. Please note, this is NOT guaranteed to be null terminated. Do NOT traverse beyond this
    /// string's size.
    /// @return The string data
    [[nodiscard]] const char8_t* data() const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    const char8_t* m_Data{};
    size_t m_Size{};
};
