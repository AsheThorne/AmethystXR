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

        AxrVectorBase<Type>::append(list);
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
        m_AllocatorMarkerID = src.m_AllocatorMarkerID;
        m_AutoDeallocateMemory = src.m_AutoDeallocateMemory;

        src.m_StackAllocator = {};
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
            m_AllocatorMarkerID = src.m_AllocatorMarkerID;
            m_AutoDeallocateMemory = src.m_AutoDeallocateMemory;

            src.m_StackAllocator = {};
            src.m_AllocatorMarkerID = {};
            src.m_AutoDeallocateMemory = {};
        }
        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    AxrStackAllocator* m_StackAllocator{};
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

        if (AxrVectorBase<Type>::m_Data != nullptr) [[unlikely]] {
            axrLogWarning("Data has already been allocated.");
            return AXR_SUCCESS;
        }

        const AxrResult axrResult = m_StackAllocator->allocate(AxrVectorBase<Type>::m_Capacity,
                                                               AxrVectorBase<Type>::m_Data,
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

        if (AxrVectorBase<Type>::m_Data == nullptr) {
            return;
        }

        if (m_StackAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Memory leak detected. StackAllocator is null.");
            return;
        }

        if (m_StackAllocator->deallocateIfLast(m_AllocatorMarkerID)) {
            AxrVectorBase<Type>::m_Data = nullptr;
        }
    }
#undef AXR_FUNCTION_FAILED_STRING
};
