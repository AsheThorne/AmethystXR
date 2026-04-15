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

private:
    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //
    static constexpr char m_Separator = std::filesystem::path::preferred_separator;

    /// Clean up this class
    void cleanup();
    /// Move the given AxrPath to this class
    /// @param src AxrPath to move
    void move_internal(AxrPath&& src);
};
