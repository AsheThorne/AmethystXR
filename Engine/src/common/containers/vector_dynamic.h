#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../memory/dynamicAllocator.h"
#include "vectorBase.h"

/// A vector that uses a dynamic allocator
/// @tparam Type Data type this vector contains
template<typename Type>
class AxrVector_Dynamic : public AxrVectorBase<Type> {
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
    AxrVector_Dynamic() = default;

#define AXR_FUNCTION_FAILED_STRING "Failed to initialize AxrVector_Dynamic. "
    /// Constructor
    /// @param capacity The max number of objects that this vector can hold
    /// @param dynamicAllocator The dynamic allocator to use
    AxrVector_Dynamic(const size_t capacity, AxrDynamicAllocator* dynamicAllocator) :
        AxrVectorBase<Type>(capacity) {
        m_DynamicAllocator = dynamicAllocator;

        if (AXR_FAILED(allocateData())) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "`allocateData()` failed.");
            return;
        }
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to initialize AxrVector_Dynamic. "
    /// Constructor
    /// @param list The list of items to set for this vector
    /// @param dynamicAllocator The dynamic allocator to use
    AxrVector_Dynamic(const std::initializer_list<Type>& list, AxrDynamicAllocator* dynamicAllocator) :
        AxrVectorBase<Type>(list) {
        m_DynamicAllocator = dynamicAllocator;

        if (AXR_FAILED(allocateData())) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "`allocateData()` failed.");
            return;
        }

        append(list);
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Copy Constructor
    /// @param src Source AxrVector_Dynamic to copy from
    AxrVector_Dynamic(const AxrVector_Dynamic& src) = delete;

    /// Move Constructor
    /// @param src Source AxrVector_Dynamic to move from
    AxrVector_Dynamic(AxrVector_Dynamic&& src) noexcept :
        AxrVectorBase<Type>(std::move(src)) {
        move_internal(std::move(src));
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrVector_Dynamic() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrVector_Dynamic to copy from
    AxrVector_Dynamic& operator=(const AxrVector_Dynamic& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrVector_Dynamic to move from
    AxrVector_Dynamic& operator=(AxrVector_Dynamic&& src) noexcept {
        if (this != &src) {
            cleanup();

            AxrVectorBase<Type>::operator=(std::move(src));

            move_internal(std::move(src));
        }
        return *this;
    }

    /// [] Operator
    /// @param index Vector index to access
    /// @return A reference to the item at the given index
    [[nodiscard]] Type& operator[](size_t index) {
        return m_DataHandle.getDataPtr()[index];
    }

    /// [] Operator
    /// @param index Vector index to access
    /// @return A const reference to the item at the given index
    [[nodiscard]] const Type& operator[](size_t index) const {
        return m_DataHandle.getDataPtr()[index];
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the vector has been allocated
    /// @return True if the vector has been allocated
    [[nodiscard]] bool allocated() const {
        return m_DataHandle != nullptr;
    }

    /// The beginning of the vector
    /// @return An iterator to the beginning of the vector
    Iterator begin() {
        return AxrVectorBase<Type>::begin(m_DataHandle.getDataPtr());
    }

    /// The beginning of the vector
    /// @return A const iterator to the beginning of the vector
    ConstIterator begin() const {
        return AxrVectorBase<Type>::begin(m_DataHandle.getDataPtr());
    }

    /// The end of the vector
    /// @return An iterator to the end of the vector
    Iterator end() {
        return AxrVectorBase<Type>::end(m_DataHandle.getDataPtr());
    }

    /// The end of the vector
    /// @return A const iterator to the end of the vector
    ConstIterator end() const {
        return AxrVectorBase<Type>::end(m_DataHandle.getDataPtr());
    }

    /// Get the vector data
    /// @return The vector data
    Type* data() {
        return m_DataHandle.getDataPtr();
    }

    /// Get the vector data
    /// @return The vector data
    const Type* data() const {
        return m_DataHandle.getDataPtr();
    }

    /// Push a single item to the end of the vector
    /// @param dataItem Data item to push back
    /// @return AXR_SUCCESS if the function succeeded. AXR_ERROR_OUT_OF_MEMORY if there isn't enough space.
    void pushBack(const Type& dataItem) {
        AxrVectorBase<Type>::pushBack(dataItem, m_DataHandle.getDataPtr());
    }

    /// Emplace a single item to the end of the vector
    /// @param args Data item emplace arguments
    /// @return AXR_SUCCESS if the function succeeded. AXR_ERROR_OUT_OF_MEMORY if there isn't enough space.
    template<typename... Args>
    void emplaceBack(Args&&... args) {
        AxrVectorBase<Type>::emplaceBack(m_DataHandle.getDataPtr(), std::forward<Args>(args)...);
    }

    /// Prefill the entire vector with the default value
    void prefillData() {
        AxrVectorBase<Type>::prefillData(m_DataHandle.getDataPtr());
    }

    /// Prefill the entire vector with the given value
    /// @param data Data to prefill vector with
    void prefillData(const Type& data) {
        AxrVectorBase<Type>::prefillData(data, m_DataHandle.getDataPtr());
    }

    /// Prefill the entire vector with the given value using emplace
    /// @param args Data item emplace arguments to prefill the vector with
    template<typename... Args>
    void prefillEmplaceData(Args&&... args) {
        AxrVectorBase<Type>::prefillEmplaceData(m_DataHandle.getDataPtr(), std::forward<Args>(args)...);
    }

    /// Append the vector with the given items
    /// @param list Items to append
    void append(const std::initializer_list<Type>& list) {
        AxrVectorBase<Type>::append(list, m_DataHandle.getDataPtr());
    }

    /// Get the item at the given index with bounds checking
    /// @param index Vector index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] Type* at(const size_t index) {
        return AxrVectorBase<Type>::at(index, m_DataHandle.getDataPtr());
    }

    /// Get the item at the given index with bounds checking
    /// @param index Vector index to access
    /// @return A pointer to the item at the given index or nullptr if index is out of range
    [[nodiscard]] const Type* at(const size_t index) const {
        return AxrVectorBase<Type>::at(index, m_DataHandle.getDataPtr());
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] Iterator findFirst(const Type& data) {
        return AxrVectorBase<Type>::findFirst(data, m_DataHandle.getDataPtr());
    }

    /// Find an iterator to the first instance of the given value
    /// @param data Item to search for
    /// @return An iterator to the first instance of the given value. Or end() if it wasn't found
    [[nodiscard]] ConstIterator findFirst(const Type& data) const {
        return AxrVectorBase<Type>::findFirst(data, m_DataHandle.getDataPtr());
    }

    /// Remove the last item in the vector
    void popBack() {
        if (AxrVectorBase<Type>::empty()) [[unlikely]] {
            return;
        }

        axrCallDestructor(m_DataHandle[AxrVectorBase<Type>::m_Size - 1]);
        AxrVectorBase<Type>::popBack();
    }

    /// Remove all items in the vector
    void clear() {
        if (AxrVectorBase<Type>::empty()) {
            return;
        }

        for (Iterator begin = end(), end = AxrVector_Dynamic::begin(); begin != end;
             /* Don't decrement here. It's done inside the loop*/) {
            --begin;
            Type& data = *begin;
            axrCallDestructor(data);
        }

        AxrVectorBase<Type>::clear();
    }

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    AxrDynamicAllocator* m_DynamicAllocator{};
    AxrHandle<Type> m_DataHandle{};

    // ----------------------------------------- //
    // Protected Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        clear();
        deallocateData();

        m_DynamicAllocator = nullptr;

        AxrVectorBase<Type>::cleanup();
    }

    /// Move the given AxrVector_Dynamic to this class
    /// @param src AxrVector_Dynamic to move
    void move_internal(AxrVector_Dynamic&& src) {
        // Please note that we aren't moving the base class. That should be done before calling this function because
        // depending on how it's done, it changes if we call the base move constructor or move assignment operator.

        m_DataHandle = std::move(src.m_DataHandle);

        m_DynamicAllocator = src.m_DynamicAllocator;

        src.m_DynamicAllocator = nullptr;
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate AxrVector_Dynamic data. "
    /// Allocate the data we need
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult allocateData() {
        assert(m_DynamicAllocator != nullptr);

        if (m_DataHandle != nullptr) [[unlikely]] {
            axrLogWarning("Data has already been allocated.");
            return AXR_SUCCESS;
        }

        const AxrResult axrResult = m_DynamicAllocator->allocate(AxrVectorBase<Type>::m_Capacity, m_DataHandle);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to allocate memory.");
            return axrResult;
        }

        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to deallcoate AxrVector_Dynamic data. "
    /// Deallocate the data block.
    void deallocateData() {
        if (m_DataHandle == nullptr) {
            return;
        }

        if (m_DynamicAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Memory leak detected. DynamicAllocator is null.");
            return;
        }

        m_DynamicAllocator->deallocate(m_DataHandle);
    }
#undef AXR_FUNCTION_FAILED_STRING
};
