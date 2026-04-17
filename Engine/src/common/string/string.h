#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../memory/dynamicAllocator.h"
#include "../../utils.h"
#include "stringIterator.h"
#include "stringView.h"

#include <cstdint>
#include <type_traits>

// ----------------------------------------- //
// Forward Declarations
// ----------------------------------------- //
class AxrString;

#ifdef AXR_TESTING_ENABLED
static bool testStringIsInHeapMemory(const AxrString& string);
#endif

// ----------------------------------------- //
// String
// ----------------------------------------- //

/// Null terminated UTF-8 string
class AxrString {
public:
#ifdef AXR_TESTING_ENABLED
    // ----------------------------------------- //
    // Friends
    // ----------------------------------------- //

    friend bool testStringIsInHeapMemory(const AxrString& string);
#endif

    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    using Iterator = AxrStringIterator;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // NOTE (Ashe): Even though we have functions for basic chars, do NOT make special functions (constructors, operator
    //  overloads, etc...) for a basic char, only char8_t. This is because chars are less efficient to use, since we
    //  need to convert them to char8_ts. So we want it to be very intentional and not use it if we can avoid it.

    // ---- Constructors ----

    /// Default Constructor
    AxrString();
    /// Constructor
    /// @param dynamicAllocator Dynamic allocator to use
    explicit AxrString(AxrDynamicAllocator* dynamicAllocator);
    /// Constructor
    /// @param string String to initialize with
    /// @param dynamicAllocator Dynamic allocator to use
    template<AxrIsChar8StringLike Char_T>
    AxrString(Char_T string, AxrDynamicAllocator* dynamicAllocator) :
        m_DynamicAllocator(dynamicAllocator),
        m_StackString() {
        buildString(string);
    }

    // NOTE (Ashe): Even though we have a function to copy from one AxrString to another, we do NOT want a copy
    //  constructor or copy assignment operator for an AxrString. Copying from one AxrString to another should be very
    //  intentional.

    /// Copy Constructor
    /// @param src Source AxrString to copy from
    AxrString(const AxrString& src) = delete;
    /// Move Constructor
    /// @param src Source AxrString to move from
    AxrString(AxrString&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrString();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrString to copy from
    AxrString& operator=(const AxrString& src) = delete;
    /// Copy Assignment Operator
    /// @param src Source char8_t string to copy from
    AxrString& operator=(const char8_t* src);
    /// Move Assignment Operator
    /// @param src Source AxrString to move from
    AxrString& operator=(AxrString&& src) noexcept;

    /// == Operator overload
    /// @param srcString String to check equality with
    /// @returns True if this AxrString and the given string is equal
    [[nodiscard]] bool operator==(const char8_t* srcString) const;
    /// == Operator overload
    /// @param srcString String to check equality with
    /// @returns True if this AxrString and the given AxrString is equal
    [[nodiscard]] bool operator==(const AxrString& srcString) const;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the number of char8_ts of this string. It's not necessarily one char8_t per character
    /// @return This string's number of char8_ts
    [[nodiscard]] size_t size() const;
    /// Get the max number of char8_ts this string can hold before growing its allocation
    /// @return The string capacity
    [[nodiscard]] size_t capacity() const;
    /// Check if this string is empty
    /// @return True if this string is empty
    [[nodiscard]] bool empty() const;

    // NOTE (Ashe): We don't have a non-const data(), begin() or end() function because if a character is modified to a
    //  different character of a different size (different number of char8_ts representing it), it can cause undefined
    //  behaviour such as overwriting existing characters or changing the size of the string unexpectingly.

    /// The beginning of the string
    /// @return An iterator to the beginning of the string
    [[nodiscard]] Iterator begin() const;
    /// The end of the string
    /// @return An iterator to the end of the string
    [[nodiscard]] Iterator end() const;

    /// Get the string data
    /// @return The string data
    [[nodiscard]] const char8_t* data() const;

    /// Copy the given string to this string
    /// @param string Source string to copy from
    /// @return AXR_SUCCESS if the function succeeded
    AxrResult copy(const AxrString& string);
    /// Append the given string to the end of this string
    /// @param string Source string to append
    /// @return AXR_SUCCESS if the function succeeded
    AxrResult append(const AxrString& string);
    /// Append the given string to the end of this string
    /// @param string Source string to append
    /// @return AXR_SUCCESS if the function succeeded
    AxrResult append(const char8_t* string);

#define AXR_FUNCTION_FAILED_STRING "Failed to build AxrString. "
    /// Set this string to all the given strings concatenated together
    /// @param strings All strings to combine
    /// @return AXR_SUCCESS if the function succeeded
    template<AxrIsChar8StringLike... Args>
    AxrResult buildString(Args&&... strings) {
        auto countStringSize = []<typename T>(T&& string, size_t& size) {
            using Type = std::remove_cv_t<std::remove_reference_t<T>>;

            if constexpr (std::is_same_v<Type, const char8_t*>) {
                size += std::char_traits<char8_t>::length(string);
            } else if (std::is_array_v<Type>) {
                // Minus 1 so we don't count the null terminator
                constexpr std::size_t arraySize = std::extent_v<Type> - 1;
                size += arraySize;
            }
        };

        auto concatenateString = [countStringSize]<typename T>(T&& srcString, char8_t*& dstString) {
            size_t stringSize = 0;
            countStringSize(srcString, stringSize);

            std::char_traits<char8_t>::copy(dstString, srcString, stringSize);

            // Increment the dstString to the end of the srcString we just copied over, so that the next string
            // starts from there
            dstString = dstString + stringSize;
        };

        // Count the number of char8_ts for every string
        size_t stringSize = 0;
        (countStringSize(std::forward<Args>(strings), stringSize), ...);

        const AxrResult axrResult = growAllocation(stringSize);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to grow allocation.");
            return axrResult;
        }

        if (m_IsHeapAllocated) {
            char8_t* data = m_HeapString.Data.getDataPtr();
            (concatenateString(std::forward<Args>(strings), data), ...);
            m_HeapString.Size = stringSize;
            m_HeapString.Data[m_HeapString.Size] = '\0';
        } else {
            char8_t* data = m_StackString.Data;
            (concatenateString(std::forward<Args>(strings), data), ...);
            m_StackString.Size = stringSize;
            m_StackString.Data[m_StackString.Size] = '\0';
        }

        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to build AxrString from char string. "
    /// Set this string to the given char string, converted to a char8_t string.
    ///
    /// We don't have a constructor/assignment operator for basic chars because we want it to be very intentional and
    /// only use this if you really must. Since it's less efficient.
    /// @param strings All strings to combine
    /// @return AXR_SUCCESS if the function succeeded
    template<AxrIsCharStringLike... Args>
    AxrResult buildFromCharString(Args&&... strings) {
        auto countStringSize = []<typename T>(T&& string, size_t& size) {
            using Type = std::remove_cv_t<std::remove_reference_t<T>>;

            if constexpr (std::is_same_v<Type, const char*>) {
                size += std::char_traits<char>::length(string);
            } else if (std::is_array_v<Type>) {
                // Minus 1 so we don't count the null terminator
                constexpr std::size_t arraySize = std::extent_v<Type> - 1;
                size += arraySize;
            }
        };

        auto concatenateString = [countStringSize]<typename T>(T&& srcString, char8_t*& dstString) {
            size_t stringSize = 0;
            countStringSize(srcString, stringSize);

            for (size_t i = 0; i < stringSize; ++i) {
                dstString[i] = static_cast<char8_t>(srcString[i]);
            }

            // Increment the dstString to the end of the srcString we just copied over, so that the next string
            // starts from there
            dstString = dstString + stringSize;
        };

        size_t stringSize = 0;
        (countStringSize(std::forward<Args>(strings), stringSize), ...);

        const AxrResult axrResult = growAllocation(stringSize);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to grow allocation.");
            return axrResult;
        }

        if (m_IsHeapAllocated) {
            char8_t* data = m_HeapString.Data.getDataPtr();
            (concatenateString(std::forward<Args>(strings), data), ...);
            m_HeapString.Size = stringSize;
            m_HeapString.Data[m_HeapString.Size] = '\0';
        } else {
            char8_t* data = m_StackString.Data;
            (concatenateString(std::forward<Args>(strings), data), ...);
            m_StackString.Size = stringSize;
            m_StackString.Data[m_StackString.Size] = '\0';
        }

        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Grow the allocation to fit the given size plus a null terminator. If the given size is smaller than the current
    /// capacity, then nothing happens.
    /// @param size The number of char8_ts to allocate for.
    /// @return AXR_SUCCESS if the function succeeds
    [[nodiscard]] AxrResult growAllocation(size_t size);

    /// Compare this AxrString with the given basic char string.
    ///
    /// We don't have an equality operator for basic chars because we want it to be very intentional and only use this
    /// if you really must. Since it's less efficient.
    /// @param string Char string
    /// @return True if both strings are equal
    [[nodiscard]] bool compareWithCharString(const char* string) const;

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

    /// Pop the given number of characters off of this string
    /// @param count Number of characters to pop off
    void pop(size_t count);
    /// Pop characters off of this string until we reach the given iterator
    /// @param iterator Iterator to stop at. The character at this iterator IS popped off.
    void pop(const Iterator& iterator);

    /// Clear this string
    void clear();

protected:
    // ----------------------------------------- //
    // Protected Structs
    // ----------------------------------------- //

    struct HeapStorage {
        /// Capacity in terms of the number of char8_ts. Includes the null terminator.
        size_t Capacity{};
        /// The number of char8_ts currently in use. Doesn't include the null terminator.
        size_t Size{};
        AxrHandle<char8_t> Data{};
    };

    /// Capacity in terms of the number of char8_ts. Includes the null terminator.
    static constexpr size_t StackCapacity = (sizeof(HeapStorage) - sizeof(uint8_t)) / sizeof(char8_t);

    struct StackStorage {
        /// The number of char8_ts currently in use. Doesn't include the null terminator.
        uint8_t Size{};
        char8_t Data[StackCapacity]{};
    };

    // Make sure the stack capacity isn't any larger than the largest value we can store for the size + 1 for the null
    // terminator.
    static_assert(StackCapacity <= UINT8_MAX + 1);

    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    AxrDynamicAllocator* m_DynamicAllocator{};
    bool m_IsHeapAllocated{};
    union {
        HeapStorage m_HeapString;
        StackStorage m_StackString;
    };

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();
    /// Move the given AxrString to this class
    /// @param src AxrString to move
    void move_internal(AxrString&& src);

    /// Deallocate the string data
    void deallocateData();

    /// Set the string data
    /// @param string Source string to copy
    /// @param stringSize String size. Not including null terminator.
    /// @param dataOffset Data offset for where to insert the given string
    void setStringData(const char8_t* string, size_t stringSize, size_t dataOffset = 0);
    /// Set the string data
    /// @param string Source string to copy
    /// @param stringSize String size. Not including null terminator.
    /// @param dataOffset Data offset for where to insert the given string
    void setStringData(const char* string, size_t stringSize, size_t dataOffset = 0);
};
