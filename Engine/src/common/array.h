#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/logging.h"

/// Axr Array
/// @tparam Type Item type
/// @tparam Cap Array capacity
template<typename Type, size_t Cap>
class AxrArray {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    typedef Type* Iterator;
    typedef const Type* ConstIterator;

    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    Type Data[Cap]{};
    size_t Capacity = Cap;
    size_t Size{};

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Operator Overloads ----

    /// [] Operator
    /// @param index Array index to access
    /// @return A reference to the item at the given index
    [[nodiscard]] Type& operator[](size_t index) {
        return Data[index];
    }

    /// [] Operator
    /// @param index Array index to access
    /// @return A const reference to the item at the given index
    [[nodiscard]] const Type& operator[](size_t index) const {
        return Data[index];
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// The beginning of the array
    /// @return An iterator to the beginning of the array
    Iterator begin() {
        return &Data[0];
    }

    /// The beginning of the array
    /// @return A const iterator to the beginning of the array
    ConstIterator begin() const {
        return &Data[0];
    }

    /// The end of the array
    /// @return An iterator to the end of the array
    Iterator end() {
        return &Data[Size];
    }

    /// The end of the array
    /// @return A const iterator to the end of the array
    ConstIterator end() const {
        return &Data[Size];
    }

    /// Get the array size
    /// @return The array size
    [[nodiscard]] size_t size() const {
        return Size;
    }

    /// Get the array capacity
    /// @return The array capacity
    [[nodiscard]] size_t capacity() const {
        return Capacity;
    }

    /// Check if the array is empty
    /// @return True if the array is empty
    [[nodiscard]] bool empty() const {
        return Size == 0;
    }

    /// Get a pointer to the data of this array
    /// @return A pointer to the data
    [[nodiscard]] Type* data() {
        return &Data[0];
    }

    /// Get a pointer to the data of this array
    /// @return A pointer to the data
    [[nodiscard]] const Type* data() const {
        return &Data[0];
    }

    /// Get the item at the given index with bounds checking
    /// @param index Array index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] Type* at(size_t index) {
        if (index >= Size) [[unlikely]] {
            return nullptr;
        }
        return &Data[index];
    }

    /// Get the item at the given index with bounds checking
    /// @param index Array index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] const Type* at(size_t index) const {
        if (index >= Size) [[unlikely]] {
            return nullptr;
        }
        return &Data[index];
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] Iterator findFirst(const Type& data) {
        for (Iterator it = begin(), e = end(); it != e; ++it) {
            if constexpr (m_IsTypeCharArray) {
                if (strncmp(*it, data, getArrayLength()) == 0) {
                    return it;
                }
            } else if constexpr (m_IsTypeConstCharPtr) {
                if (strcmp(*it, data) == 0) {
                    return it;
                }
            } else {
                if (*it == data) {
                    return it;
                }
            }
        }

        return end();
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] ConstIterator findFirst(const Type& data) const {
        for (ConstIterator it = begin(), e = end(); it != e; ++it) {
            if constexpr (m_IsTypeCharArray) {
                if (strncmp(*it, data, getArrayLength()) == 0) {
                    return it;
                }
            } else if constexpr (m_IsTypeConstCharPtr) {
                if (strcmp(*it, data) == 0) {
                    return it;
                }
            } else {
                if (*it == data) {
                    return it;
                }
            }
        }

        return end();
    }

    /// Add a new item to the end of the array
    /// @param data New item to add
    void pushBack(const Type& data) {
        if (Size == Capacity) [[unlikely]] {
            axrLogError("Failed to push back array. Array is full.");
            return;
        }

        if constexpr (m_IsTypeCharArray) {
            strncpy(Data[Size], data, getArrayLength());
        } else {
            Data[Size] = data;
        }
        Size++;
    }

    /// Remove the last item in the array
    void popBack() {
        if (Size == 0) [[unlikely]] {
            return;
        }

        // Don't clear the data, just overwrite it when new data gets added
        Size--;
    }

    /// Remove all items in the array
    void clear() {
        // Don't clear the data, just overwrite it when new data gets added
        Size = 0;
    }

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    static constexpr bool const& m_IsTypeCharArray =
        std::is_array_v<Type> && std::is_same_v<std::remove_extent_t<Type>, char>;
    static constexpr bool const& m_IsTypeConstCharPtr = std::is_same_v<std::remove_extent_t<Type>, const char*>;

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// If this type is an array, get the number of elements it holds
    /// @return The number of elements the `Type` array holds. Or 0 if it's not an array.
    [[nodiscard]] constexpr size_t getArrayLength() const {
        if constexpr (std::is_array_v<Type>) {
            return sizeof(Type) / sizeof(std::remove_extent_t<Type>);
        }
        return 0;
    }
};
