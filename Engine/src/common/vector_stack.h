#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../memory/stackAllocator.h"
#include "axr/logging.h"

#include <cstring>

/// A vector that uses a stack allocator
/// @tparam Type Data type this vector contains
template<typename Type>
class AxrVector_Stack {
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
    AxrVector_Stack() = default;

#define AXR_FUNCTION_FAILED_STRING "Failed to initialize AxrVector_Stack. "
    /// Constructor
    /// @param capacity The max number of objects that this vector can hold
    /// @param stackAllocator The stack allocator to use
    /// @param autoDeallocate Set to false if this vector should not deallocate any memory and just trust that something
    /// else will clean it up later.
    /// If true, this vector will deallocate automatically if it can do so safely. If this wasn't the last thing
    /// allocated to the stack allocator, then it won't deallocate to prevent accidentally deallocating something it
    /// shouldn't that was added to the stack allocator after.
    AxrVector_Stack(const size_t capacity, AxrStackAllocator* stackAllocator, const bool autoDeallocate = true) {
        m_StackAllocator = stackAllocator;
        m_Capacity = capacity;
        m_AutoDeallocateMemory = autoDeallocate;

        if (AXR_FAILED(allocateData())) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "`allocateData()` failed.");
            return;
        }
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Copy Constructor
    /// @param src Source AxrVector_Stack to copy from
    AxrVector_Stack(const AxrVector_Stack& src) = delete;

    /// Move Constructor
    /// @param src Source AxrVector_Stack to move from
    AxrVector_Stack(AxrVector_Stack&& src) noexcept {
        m_StackAllocator = src.m_StackAllocator;
        m_Data = src.m_Data;
        m_Capacity = src.m_Capacity;
        m_Size = src.m_Size;
        m_AllocatorMarkerID = src.m_AllocatorMarkerID;
        m_AutoDeallocateMemory = src.m_AutoDeallocateMemory;

        src.m_StackAllocator = {};
        src.m_Data = {};
        src.m_Capacity = {};
        src.m_Size = {};
        src.m_AllocatorMarkerID = {};
        src.m_AutoDeallocateMemory = {};
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrVector_Stack() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVector_Stack to copy from
    AxrVector_Stack& operator=(const AxrVector_Stack& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrVector_Stack to move from
    AxrVector_Stack& operator=(AxrVector_Stack&& src) noexcept {
        if (this != &src) {
            cleanup();

            m_StackAllocator = src.m_StackAllocator;
            m_Data = src.m_Data;
            m_Capacity = src.m_Capacity;
            m_Size = src.m_Size;
            m_AllocatorMarkerID = src.m_AllocatorMarkerID;
            m_AutoDeallocateMemory = src.m_AutoDeallocateMemory;

            src.m_StackAllocator = {};
            src.m_Data = {};
            src.m_Capacity = {};
            src.m_Size = {};
            src.m_AllocatorMarkerID = {};
            src.m_AutoDeallocateMemory = {};
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

#define AXR_FUNCTION_FAILED_STRING "Failed to push back data in AxrVector_Stack. "
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
    AxrStackAllocator* m_StackAllocator{};
    Type* m_Data{};
    size_t m_Capacity{};
    size_t m_Size{};
    AxrStackAllocator::MarkerID m_AllocatorMarkerID{};
    bool m_AutoDeallocateMemory{};
    static constexpr bool const& m_IsTypeCharArray =
        std::is_array_v<Type> && std::is_same_v<std::remove_extent_t<Type>, char>;
    static constexpr bool const& m_IsTypeConstCharPtr = std::is_same_v<std::remove_extent_t<Type>, const char*>;

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        deallocateData();

        m_StackAllocator = {};
        m_Data = {};
        m_Capacity = {};
        m_Size = {};
        m_AllocatorMarkerID = {};
        m_AutoDeallocateMemory = {};
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to initialize AxrVector_Stack data. "
    /// Allocate the data we need
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult allocateData() {
        if (m_Data != nullptr) [[unlikely]] {
            axrLogWarning("Data has already been allocated.");
            return AXR_SUCCESS;
        }

        if (m_StackAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "`stackAllocator` is null.");
            return AXR_ERROR_VALIDATION_FAILED;
        }

        const AxrResult axrResult = m_StackAllocator->allocateAligned(m_Capacity, m_Data, m_AllocatorMarkerID);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to allocate memory.");
            return axrResult;
        }

        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to deallcoate AxrVector_Stack data. "
    /// Deallocate the data block.
    void deallocateData() {
        if (!m_AutoDeallocateMemory) {
            return;
        }

        if (m_Data == nullptr) {
            return;
        }

        if (m_StackAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Memory leak detected. StackAllocator is null.");
            return;
        }

        if (m_StackAllocator->deallocateIfLast(m_AllocatorMarkerID)) {
            m_Data = nullptr;
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
