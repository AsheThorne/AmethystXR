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
        m_Data = src.m_Data;
        m_Capacity = src.m_Capacity;
        m_Size = src.m_Size;

        src.m_Data = {};
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

            m_Data = src.m_Data;
            m_Capacity = src.m_Capacity;
            m_Size = src.m_Size;

            src.m_Data = {};
            src.m_Capacity = {};
            src.m_Size = {};
        }
        return *this;
    }

    /// [] Operator
    /// @param index Vector index to access
    /// @return A reference to the item at the given index
    [[nodiscard]] Type& operator[](size_t index) {
        return m_Data[index];
    }

    /// [] Operator
    /// @param index Vector index to access
    /// @return A const reference to the item at the given index
    [[nodiscard]] const Type& operator[](size_t index) const {
        return m_Data[index];
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the vector data
    /// @return The vector data
    Type* data() {
        return m_Data;
    }

    /// Get the vector data
    /// @return The vector data
    const Type* data() const {
        return m_Data;
    }

    /// The beginning of the vector
    /// @return An iterator to the beginning of the vector
    Iterator begin() {
        return &m_Data[0];
    }

    /// The beginning of the vector
    /// @return A const iterator to the beginning of the vector
    ConstIterator begin() const {
        return &m_Data[0];
    }

    /// The end of the vector
    /// @return An iterator to the end of the vector
    Iterator end() {
        return &m_Data[m_Size];
    }

    /// The end of the vector
    /// @return A const iterator to the end of the vector
    ConstIterator end() const {
        return &m_Data[m_Size];
    }

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

    /// Get the item at the given index with bounds checking
    /// @param index Vector index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] Type* at(size_t index) {
        if (index >= m_Size) [[unlikely]] {
            return nullptr;
        }
        return &m_Data[index];
    }

    /// Get the item at the given index with bounds checking
    /// @param index Vector index to access
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

#define AXR_FUNCTION_FAILED_STRING "Failed to push back data in AxrVectorBase. "
    /// Push a single item to the end of the vector
    /// @param data Data to push back
    /// @return AXR_SUCCESS if the function succeeded. AXR_ERROR_OUT_OF_MEMORY if there isn't enough space.
    void pushBack(const Type& data) {
        if (m_Data == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Data is null.");
            return;
        }

        if (m_Size + 1 > m_Capacity) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Vector is full.");
            return;
        }

        if constexpr (m_IsTypeCharArray) {
            strncpy(m_Data[m_Size], data, getArrayLength());
        } else {
            m_Data[m_Size] = data;
        }
        m_Size++;
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Prefill the entire vector with the default value
    void prefillData() {
        prefillData({});
    }

    /// Prefill the entire vector with the given value
    /// @param data Data to prefill vector with
    void prefillData(const Type& data) {
        for (size_t i = 0; i < m_Capacity; ++i) {
            pushBack(data);
        }
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to append data in AxrVectorBase. "
    /// Append the vector with the given items
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
    Type* m_Data{};
    size_t m_Capacity{};
    size_t m_Size{};
    static constexpr bool const& m_IsTypeCharArray =
        std::is_array_v<Type> && std::is_same_v<std::remove_extent_t<Type>, char>;
    static constexpr bool const& m_IsTypeConstCharPtr = std::is_same_v<std::remove_extent_t<Type>, const char*>;

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        m_Data = {};
        m_Capacity = {};
        m_Size = {};
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to initialize AxrVectorBase data. "
    /// Allocate the data we need
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult allocateData() {
        axrLogError(AXR_FUNCTION_FAILED_STRING "allocateData() is not implemented.");
        return AXR_ERROR_NOT_IMPLEMENTED;
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to deallcoate AxrVectorBase data. "
    /// Deallocate the data block.
    void deallocateData() {
        axrLogError(AXR_FUNCTION_FAILED_STRING "deallocateData() is not implemented.");
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
