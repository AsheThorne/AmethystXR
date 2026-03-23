#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../memory/stackAllocator.h"
#include "vectorBase.h"

/// A vector that uses a stack allocator
/// @tparam Type Data type this vector contains
template<typename Type>
class AxrVector_Stack : public AxrVectorBase<Type> {
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
    AxrVector_Stack(const size_t capacity, AxrStackAllocator* stackAllocator, const bool autoDeallocate = true) :
        AxrVectorBase<Type>(capacity) {
        m_StackAllocator = stackAllocator;
        m_AutoDeallocateMemory = autoDeallocate;

        if (AXR_FAILED(allocateData())) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "`allocateData()` failed.");
            return;
        }
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to initialize AxrVector_Stack. "
    /// Constructor
    /// @param list The list of items to set for this vector
    /// @param stackAllocator The stack allocator to use
    /// @param autoDeallocate Set to false if this vector should not deallocate any memory and just trust that something
    /// else will clean it up later.
    /// If true, this vector will deallocate automatically if it can do so safely. If this wasn't the last thing
    /// allocated to the stack allocator, then it won't deallocate to prevent accidentally deallocating something it
    /// shouldn't that was added to the stack allocator after.
    AxrVector_Stack(const std::initializer_list<Type>& list,
                    AxrStackAllocator* stackAllocator,
                    const bool autoDeallocate = true) :
        AxrVectorBase<Type>(list) {
        m_StackAllocator = stackAllocator;
        m_AutoDeallocateMemory = autoDeallocate;

        if (AXR_FAILED(allocateData())) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "`allocateData()` failed.");
            return;
        }

        append(list);
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Copy Constructor
    /// @param src Source AxrVector_Stack to copy from
    AxrVector_Stack(const AxrVector_Stack& src) = delete;

    /// Move Constructor
    /// @param src Source AxrVector_Stack to move from
    AxrVector_Stack(AxrVector_Stack&& src) noexcept :
        AxrVectorBase<Type>(std::move(src)) {
        m_StackAllocator = src.m_StackAllocator;
        m_Data = src.m_Data;
        m_AllocatorMarkerID = src.m_AllocatorMarkerID;
        m_AutoDeallocateMemory = src.m_AutoDeallocateMemory;

        src.m_StackAllocator = {};
        src.m_Data = {};
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

            AxrVectorBase<Type>::operator=(std::move(src));

            m_StackAllocator = src.m_StackAllocator;
            m_Data = src.m_Data;
            m_AllocatorMarkerID = src.m_AllocatorMarkerID;
            m_AutoDeallocateMemory = src.m_AutoDeallocateMemory;

            src.m_StackAllocator = {};
            src.m_Data = {};
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

    /// The beginning of the vector
    /// @return An iterator to the beginning of the vector
    Iterator begin() {
        return AxrVectorBase<Type>::begin(m_Data);
    }

    /// The beginning of the vector
    /// @return A const iterator to the beginning of the vector
    ConstIterator begin() const {
        return AxrVectorBase<Type>::begin(m_Data);
    }

    /// The end of the vector
    /// @return An iterator to the end of the vector
    Iterator end() {
        return AxrVectorBase<Type>::end(m_Data);
    }

    /// The end of the vector
    /// @return A const iterator to the end of the vector
    ConstIterator end() const {
        return AxrVectorBase<Type>::end(m_Data);
    }

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

    /// Push a single item to the end of the vector
    /// @param dataItem Data item to push back
    /// @return AXR_SUCCESS if the function succeeded. AXR_ERROR_OUT_OF_MEMORY if there isn't enough space.
    void pushBack(const Type& dataItem) {
        AxrVectorBase<Type>::pushBack(dataItem, m_Data);
    }

    /// Emplace a single item to the end of the vector
    /// @param args Data item emplace arguments
    /// @return AXR_SUCCESS if the function succeeded. AXR_ERROR_OUT_OF_MEMORY if there isn't enough space.
    template<typename... Args>
    void emplaceBack(Args&&... args) {
        AxrVectorBase<Type>::emplaceBack(m_Data, std::forward<Args>(args)...);
    }

    /// Prefill the entire vector with the default value
    void prefillData() {
        AxrVectorBase<Type>::prefillData(m_Data);
    }

    /// Prefill the entire vector with the given value
    /// @param data Data to prefill vector with
    void prefillData(const Type& data) {
        AxrVectorBase<Type>::prefillData(data, m_Data);
    }

    /// Prefill the entire vector with the given value using emplace
    /// @param args Data item emplace arguments to prefill the vector with
    template<typename... Args>
    void prefillEmplaceData(Args&&... args) {
        AxrVectorBase<Type>::prefillEmplaceData(m_Data, std::forward<Args>(args)...);
    }

    /// Append the vector with the given items
    /// @param list Items to append
    void append(const std::initializer_list<Type>& list) {
        AxrVectorBase<Type>::append(list, m_Data);
    }

    /// Get the item at the given index with bounds checking
    /// @param index Vector index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] Type* at(const size_t index) {
        return AxrVectorBase<Type>::at(index, m_Data);
    }

    /// Get the item at the given index with bounds checking
    /// @param index Vector index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] const Type* at(const size_t index) const {
        return AxrVectorBase<Type>::at(index, m_Data);
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] Iterator findFirst(const Type& data) {
        return AxrVectorBase<Type>::findFirst(data, m_Data);
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] ConstIterator findFirst(const Type& data) const {
        return AxrVectorBase<Type>::findFirst(data, m_Data);
    }

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    AxrStackAllocator* m_StackAllocator{};
    Type* m_Data{};
    AxrStackAllocator::MarkerID m_AllocatorMarkerID{};
    bool m_AutoDeallocateMemory{};

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        deallocateData();

        m_StackAllocator = {};
        m_AllocatorMarkerID = {};
        m_AutoDeallocateMemory = {};

        AxrVectorBase<Type>::cleanup();
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate AxrVector_Stack data. "
    /// Allocate the data we need
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult allocateData() {
        assert(m_StackAllocator != nullptr);

        if (m_Data != nullptr) [[unlikely]] {
            axrLogWarning("Data has already been allocated.");
            return AXR_SUCCESS;
        }

        const AxrResult axrResult = m_StackAllocator->allocate(AxrVectorBase<Type>::m_Capacity,
                                                               m_Data,
                                                               m_AllocatorMarkerID);
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
};
