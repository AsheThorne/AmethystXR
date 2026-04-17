#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "string.h"

#include <filesystem>

/// Axr Path
class AxrPath : public AxrString {
public:
    // NOTE (Ashe): Unfortunately, we need to accept basic chars a lot here since that's what our platform layer uses
    //  for paths. Even though a pth can be in UTF-8 format (the same as the base AxrString class). So that's why we use
    //  a UTF-8 string as the base class, we just have to do a lot of conversions from basic chars to char8_ts.

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrPath();
    /// Constructor
    /// @param dynamicAllocator Dynamic allocator to use
    explicit AxrPath(AxrDynamicAllocator* dynamicAllocator);
    /// Constructor
    /// @param path Path to initialize with
    /// @param dynamicAllocator Dynamic allocator to use
    AxrPath(const char* path, AxrDynamicAllocator* dynamicAllocator);
    /// Copy Constructor
    /// @param src Source AxrPath to copy from
    AxrPath(const AxrPath& src) = delete;
    /// Move Constructor
    /// @param src Source AxrPath to move from
    AxrPath(AxrPath&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrPath();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrPath to copy from
    AxrPath& operator=(const AxrPath& src) = delete;
    /// Copy Assignment Operator
    /// @param src Source string to copy from
    AxrPath& operator=(const char* src);
    /// Move Assignment Operator
    /// @param src Source AxrPath to move from
    AxrPath& operator=(AxrPath&& src) noexcept;

    /// == Operator overload
    /// @param srcString String to check equality with
    /// @returns True if this AxrPath and the given string is equal
    [[nodiscard]] bool operator==(const char8_t* srcString) const;
    /// == Operator overload
    /// @param srcString String to check equality with
    /// @returns True if this AxrPath and the given string is equal
    [[nodiscard]] bool operator==(const char* srcString) const;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Append the given path to the end of this path
    /// @param path Source path to append
    /// @return AXR_SUCCESS if the function succeeded
    AxrResult append(const AxrPath& path);
    /// Append the given string to the end of this path
    /// @param string Source string to append
    /// @return AXR_SUCCESS if the function succeeded
    AxrResult append(const char* string);
    /// Append the given path to the end of this path. This function also checks for separator characters and converts
    /// them to the correct separator for the current platform.
    /// @param path Source path to append
    /// @return AXR_SUCCESS if the function succeeded
    AxrResult appendPath(const char8_t* path);
    /// Append the given path to the end of this path. This function also checks for separator characters and converts
    /// them to the correct separator for the current platform.
    /// @param path Source path to append
    /// @return AXR_SUCCESS if the function succeeded
    AxrResult appendPath(const char* path);

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //
    static constexpr char8_t m_Separator = static_cast<char8_t>(std::filesystem::path::preferred_separator);

    /// Clean up this class
    void cleanup();
    /// Move the given AxrPath to this class
    /// @param src AxrPath to move
    void move_internal(AxrPath&& src);

    /// Find every path separator character within the given range and convert it to the correct separator for the
    /// current platform
    /// @param characterIndex Character index to start at
    /// @param count Number of characters to check
    void correctPathSeparators(size_t characterIndex, size_t count);
    /// Find every path separator character within the given range and convert it to the correct separator for the
    /// current platform
    /// @param startIterator Character iterator to start at
    /// @param count Number of characters to check
    void correctPathSeparators(const Iterator& startIterator, size_t count);
    /// Find every path separator character within the given range and convert it to the correct separator for the
    /// current platform
    /// @param startIterator Character iterator to start at
    /// @param endIterator Character iterator to end at. This character is NOT checked.
    void correctPathSeparators(const Iterator& startIterator, const Iterator& endIterator);
};
