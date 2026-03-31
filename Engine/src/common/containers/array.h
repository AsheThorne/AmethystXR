#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../utils.h"
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
    // Special Functions
    // ----------------------------------------- //

    // ---- Operator Overloads ----

    /// [] Operator
    /// @param index Array index to access
    /// @return A reference to the item at the given index
    [[nodiscard]] Type& operator[](size_t index) {
        return m_Data[index];
    }

    /// [] Operator
    /// @param index Array index to access
    /// @return A const reference to the item at the given index
    [[nodiscard]] const Type& operator[](size_t index) const {
        return m_Data[index];
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// The beginning of the array
    /// @return An iterator to the beginning of the array
    Iterator begin() {
        return &m_Data[0];
    }

    /// The beginning of the array
    /// @return A const iterator to the beginning of the array
    ConstIterator begin() const {
        return &m_Data[0];
    }

    /// The end of the array
    /// @return An iterator to the end of the array
    Iterator end() {
        return &m_Data[m_Size];
    }

    /// The end of the array
    /// @return A const iterator to the end of the array
    ConstIterator end() const {
        return &m_Data[m_Size];
    }

    /// Get the array size
    /// @return The array size
    [[nodiscard]] size_t size() const {
        return m_Size;
    }

    /// Get the array capacity
    /// @return The array capacity
    [[nodiscard]] size_t capacity() const {
        return m_Capacity;
    }

    /// Check if the array is empty
    /// @return True if the array is empty
    [[nodiscard]] bool empty() const {
        return m_Size == 0;
    }

    /// Get a pointer to the data of this array
    /// @return A pointer to the data
    [[nodiscard]] Type* data() {
        return &m_Data[0];
    }

    /// Get a pointer to the data of this array
    /// @return A pointer to the data
    [[nodiscard]] const Type* data() const {
        return &m_Data[0];
    }

    /// Get the item at the given index with bounds checking
    /// @param index Array index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] Type* at(size_t index) {
        if (index >= m_Size) [[unlikely]] {
            return nullptr;
        }
        return &m_Data[index];
    }

    /// Get the item at the given index with bounds checking
    /// @param index Array index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] const Type* at(size_t index) const {
        if (index >= m_Size) [[unlikely]] {
            return nullptr;
        }
        return &m_Data[index];
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] Iterator findFirst(const Type& data) {
        for (Iterator it = begin(), e = end(); it != e; ++it) {
            if constexpr (axrIsTypeCharArray<Type>()) {
                if (strncmp(*it, data, axrGetArrayLength<Type>()) == 0) {
                    return it;
                }
            } else if constexpr (axrIsTypeConstCharPtr<Type>()) {
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
            if constexpr (axrIsTypeCharArray<Type>()) {
                if (strncmp(*it, data, axrGetArrayLength<Type>()) == 0) {
                    return it;
                }
            } else if constexpr (axrIsTypeConstCharPtr<Type>()) {
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

#define AXR_FUNCTION_FAILED_STRING "Failed to push back data in AxrArray. "
    /// Add a new item to the end of the array
    /// @param data New item to add
    void pushBack(const Type& data) {
        if (m_Size == m_Capacity) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Array is full.");
            return;
        }

        if constexpr (axrIsTypeCharArray<Type>()) {
            strncpy(m_Data[m_Size], data, axrGetArrayLength<Type>());
        } else {
            m_Data[m_Size] = data;
        }
        m_Size++;
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to append data in AxrArray. "
    /// Append the array with the given items
    /// @param list Items to append
    void append(const std::initializer_list<Type>& list) {
        if (m_Size + list.size() > m_Capacity) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Not enough space for the whole list.");
            return;
        }

        for (const Type& item : list) {
            pushBack(item);
        }
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Remove the last item in the array
    void popBack() {
        if (m_Size == 0) [[unlikely]] {
            return;
        }

        axrCallDestructor(m_Data[m_Size - 1]);
        m_Size--;
    }

    /// Remove all items in the array
    void clear() {
        if (empty()) {
            return;
        }

        for (Iterator begin = end(), end = AxrArray::begin(); begin != end;
             /* Don't decrement here. It's done inside the loop*/) {
            --begin;
            Type& data = *begin;
            axrCallDestructor(data);
        }

        m_Size = 0;
    }

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    Type m_Data[Cap]{};
    size_t m_Capacity = Cap;
    size_t m_Size{};
};
