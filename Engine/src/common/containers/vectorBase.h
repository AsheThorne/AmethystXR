#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/logging.h"

#include <cstring>

/// Base generic vector to inherit from
/// @tparam Type Data type this vector contains
template<typename Type>
class AxrVectorBase {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    typedef Type* Iterator;
    typedef const Type* ConstIterator;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrVectorBase() = default;

    /// Constructor
    /// @param capacity The max number of objects that this vector can hold
    explicit AxrVectorBase(const size_t capacity) {
        m_Capacity = capacity;
    }

    /// Constructor
    /// @param list The list of items to set for this vector
    AxrVectorBase(const std::initializer_list<Type>& list) {
        m_Capacity = list.size();
    }

    /// Copy Constructor
    /// @param src Source AxrVectorBase to copy from
    AxrVectorBase(const AxrVectorBase& src) = delete;

    /// Move Constructor
    /// @param src Source AxrVectorBase to move from
    AxrVectorBase(AxrVectorBase&& src) noexcept {
        m_Capacity = src.m_Capacity;
        m_Size = src.m_Size;

        src.m_Capacity = {};
        src.m_Size = {};
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrVectorBase() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVectorBase to copy from
    AxrVectorBase& operator=(const AxrVectorBase& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrVectorBase to move from
    AxrVectorBase& operator=(AxrVectorBase&& src) noexcept {
        if (this != &src) {
            cleanup();

            m_Capacity = src.m_Capacity;
            m_Size = src.m_Size;

            src.m_Capacity = {};
            src.m_Size = {};
        }
        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the vector size
    /// @return The vector size
    [[nodiscard]] size_t size() const {
        return m_Size;
    }

    /// Get the vector capacity
    /// @return The vector capacity
    [[nodiscard]] size_t capacity() const {
        return m_Capacity;
    }

    /// Check if the vector is empty
    /// @return True if the vector is empty
    [[nodiscard]] bool empty() const {
        return m_Size == 0;
    }

    /// Check if the vector has been allocated
    /// @return True if the vector has been allocated
    [[nodiscard]] bool allocated() const {
        return m_Capacity != 0;
    }

    /// Remove the last item in the vector
    void popBack() {
        if (m_Size == 0) [[unlikely]] {
            return;
        }

        // Don't clear the data, just overwrite it when new data gets added
        m_Size--;
    }

    /// Remove all items in the vector
    void clear() {
        // Don't clear the data, just overwrite it when new data gets added
        m_Size = 0;
    }

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    size_t m_Capacity{};
    size_t m_Size{};
    static constexpr bool const& m_IsTypeCharArray = std::is_array_v<Type> &&
                                                     std::is_same_v<std::remove_extent_t<Type>, char>;
    static constexpr bool const& m_IsTypeConstCharPtr = std::is_same_v<std::remove_extent_t<Type>, const char*>;

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        m_Capacity = {};
        m_Size = {};
    }

    /// The beginning of the vector
    /// @param arrayData The data array to use
    /// @return An iterator to the beginning of the vector
    Iterator begin(Type* arrayData) {
        return &arrayData[0];
    }

    /// The beginning of the vector
    /// @param arrayData The data array to use
    /// @return A const iterator to the beginning of the vector
    ConstIterator begin(const Type* arrayData) const {
        return &arrayData[0];
    }

    /// The end of the vector
    /// @param arrayData The data array to use
    /// @return An iterator to the end of the vector
    Iterator end(Type* arrayData) {
        return &arrayData[m_Size];
    }

    /// The end of the vector
    /// @param arrayData The data array to use
    /// @return A const iterator to the end of the vector
    ConstIterator end(const Type* arrayData) const {
        return &arrayData[m_Size];
    }

    /// Get the item at the given index with bounds checking
    /// @param index Vector index to access
    /// @param arrayData The data array to use
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] Type* at(const size_t index, Type* arrayData) {
        if (index >= m_Size) [[unlikely]] {
            return nullptr;
        }
        return &arrayData[index];
    }

    /// Get the item at the given index with bounds checking
    /// @param index Vector index to access
    /// @param arrayData The data array to use
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] const Type* at(const size_t index, const Type* arrayData) const {
        if (index >= m_Size) [[unlikely]] {
            return nullptr;
        }
        return &arrayData[index];
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @param arrayData The data array to use
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] Iterator findFirst(const Type& data, Type* arrayData) {
        for (Iterator it = begin(arrayData), e = end(arrayData); it != e; ++it) {
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

        return end(arrayData);
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @param arrayData The data array to use
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] ConstIterator findFirst(const Type& data, const Type* arrayData) const {
        for (ConstIterator it = begin(arrayData), e = end(arrayData); it != e; ++it) {
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

        return end(arrayData);
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to push back data in AxrVectorBase. "
    /// Push a single item to the end of the vector
    /// @param dataItem Data item to push back
    /// @param arrayData The data array to use
    /// @return AXR_SUCCESS if the function succeeded. AXR_ERROR_OUT_OF_MEMORY if there isn't enough space.
    void pushBack(const Type& dataItem, Type* arrayData) {
        if (arrayData == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Array data is null.");
            return;
        }

        if (m_Size + 1 > m_Capacity) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Vector is full.");
            return;
        }

        if constexpr (m_IsTypeCharArray) {
            strncpy(arrayData[m_Size], dataItem, getArrayLength());
        } else {
            arrayData[m_Size] = dataItem;
        }
        m_Size++;
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to emplace back data in AxrVectorBase. "
    /// Emplace a single item to the end of the vector
    /// @param arrayData The data array to use
    /// @param args Data item emplace arguments
    /// @return AXR_SUCCESS if the function succeeded. AXR_ERROR_OUT_OF_MEMORY if there isn't enough space.
    template<typename... Args>
    void emplaceBack(Type* arrayData, Args&&... args) {
        if (arrayData == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Array data is null.");
            return;
        }

        if (m_Size + 1 > m_Capacity) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Vector is full.");
            return;
        }

        new (arrayData + m_Size++) Type(std::forward<Args>(args)...);
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Prefill the entire vector with the default value
    /// @param arrayData The data array to use
    void prefillData(Type* arrayData) {
        prefillData({}, arrayData);
    }

    /// Prefill the entire vector with the given value
    /// @param data Data to prefill vector with
    /// @param arrayData The data array to use
    void prefillData(const Type& data, Type* arrayData) {
        for (size_t i = 0; i < m_Capacity; ++i) {
            pushBack(data, arrayData);
        }
    }

    /// Prefill the entire vector with the given value using emplace
    /// @param arrayData The data array to use
    /// @param args Data item emplace arguments to prefill the vector with
    template<typename... Args>
    void prefillEmplaceData(Type* arrayData, Args&&... args) {
        for (size_t i = 0; i < m_Capacity; ++i) {
            emplaceBack(arrayData, std::forward<Args>(args)...);
        }
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to append data in AxrVectorBase. "
    /// Append the vector with the given items
    /// @param list Items to append
    /// @param arrayData The data array to use
    void append(const std::initializer_list<Type>& list, Type* arrayData) {
        if (m_Size + list.size() > m_Capacity) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Not enough space for the whole list.");
            return;
        }

        for (const Type& item : list) {
            pushBack(item, arrayData);
        }
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// If this type is an array, get the number of elements it holds
    /// @return The number of elements the `Type` array holds. Or 0 if it's not an array.
    [[nodiscard]] constexpr size_t getArrayLength() const {
        if constexpr (std::is_array_v<Type>) {
            return sizeof(Type) / sizeof(std::remove_extent_t<Type>);
        }
        return 0;
    }
};
