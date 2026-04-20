// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "string.h"

#include <utility>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrString::AxrString() :
    m_StackString() {
}

AxrString::AxrString(AxrDynamicAllocator* dynamicAllocator) :
    m_DynamicAllocator(dynamicAllocator),
    m_StackString() {
}

AxrString::AxrString(AxrString&& src) noexcept {
    move_internal(std::move(src), true);
}

AxrString::~AxrString() {
    cleanup();
}

AxrString& AxrString::operator=(const char8_t* src) {
    if (src == nullptr) {
        return *this;
    }

    buildString(src);
    return *this;
}

AxrString& AxrString::operator=(AxrString&& src) noexcept {
    if (this != &src) {
        cleanup();

        move_internal(std::move(src), false);
    }
    return *this;
}

bool AxrString::operator==(const char8_t* srcString) const {
    const size_t srcStringSize = std::char_traits<char8_t>::length(srcString);
    const size_t thisStringSize = m_IsHeapAllocated ? m_HeapString.Size : m_StackString.Size;
    const char8_t* thisStringData = m_IsHeapAllocated ? m_HeapString.Data.getDataPtr() : m_StackString.Data;

    if (thisStringSize != srcStringSize) {
        return false;
    }

    return std::char_traits<char8_t>::compare(thisStringData, srcString, thisStringSize) == 0;
}

bool AxrString::operator==(const AxrString& srcString) const {
    return operator==(srcString.data());
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

size_t AxrString::size() const {
    return m_IsHeapAllocated ? m_HeapString.Size : m_StackString.Size;
}

size_t AxrString::capacity() const {
    return m_IsHeapAllocated ? m_HeapString.Capacity : StackCapacity;
}

bool AxrString::empty() const {
    return m_IsHeapAllocated ? m_HeapString.Size == 0 : m_StackString.Size == 0;
}

AxrString::Iterator AxrString::begin() const {
    return m_IsHeapAllocated ? Iterator(&m_HeapString.Data[0]) : Iterator(&m_StackString.Data[0]);
}

AxrString::Iterator AxrString::end() const {
    return m_IsHeapAllocated ? Iterator(&m_HeapString.Data[m_HeapString.Size])
                             : Iterator(&m_StackString.Data[m_StackString.Size]);
}

const char8_t* AxrString::data() const {
    return m_IsHeapAllocated ? m_HeapString.Data.getDataPtr() : m_StackString.Data;
}

AxrResult AxrString::copy(const AxrString& string) {
    return buildString(string.data());
}

AxrResult AxrString::append(const AxrString& string) {
    return append(string.data());
}

#define AXR_FUNCTION_FAILED_STRING "Failed to append AxrString. "
AxrResult AxrString::append(const char8_t* string) {
    const size_t stringSize = std::char_traits<char8_t>::length(string);
    const size_t currentStringSize = size();

    const AxrResult axrResult = growAllocation(currentStringSize + stringSize);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to grow allocation.");
        return axrResult;
    }

    setStringData(string, stringSize, currentStringSize);

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to grow allocation for AxrString. "
AxrResult AxrString::growAllocation(size_t size) {
    assert(m_DynamicAllocator != nullptr);

    // Plus 1 for the null terminator character
    size += 1;

    // ---- Handle Stack Allocation ----

    if (!m_IsHeapAllocated) {
        if (size <= StackCapacity) {
            return AXR_SUCCESS;
        }

        const StackStorage oldStackString = m_StackString;

        const AxrResult axrResult = m_DynamicAllocator->allocate(size, m_HeapString.Data);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to allocate memory of size: {}.", size);
            return axrResult;
        }

        m_IsHeapAllocated = true;
        m_HeapString.Capacity = size;

        // Copy the old string to the new allocation
        std::char_traits<char8_t>::copy(m_HeapString.Data.getDataPtr(), oldStackString.Data, oldStackString.Size);
        m_HeapString.Size = oldStackString.Size;

        // Set the null terminator
        m_HeapString.Data[m_HeapString.Size] = '\0';

        return AXR_SUCCESS;
    }

    // ---- Handle Heap Allocation ----

    if (size <= m_HeapString.Capacity) {
        return AXR_SUCCESS;
    }

    const AxrResult axrResult = m_DynamicAllocator->reallocate(size, m_HeapString.Data);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to reallocate memory of size: {}.", size);
        return axrResult;
    }

    m_HeapString.Capacity = size;

    // We don't need to set the null terminator. It gets copied when the allocator reallocates.

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

bool AxrString::compareWithCharString(const char* string) const {
    const size_t srcStringSize = std::char_traits<char>::length(string);
    const size_t thisStringSize = m_IsHeapAllocated ? m_HeapString.Size : m_StackString.Size;
    const char8_t* thisStringData = m_IsHeapAllocated ? m_HeapString.Data.getDataPtr() : m_StackString.Data;

    if (thisStringSize != srcStringSize) {
        return false;
    }

    for (size_t i = 0; i < thisStringSize; ++i) {
        if (thisStringData[i] != static_cast<char8_t>(string[i])) {
            return false;
        }
    }

    return true;
}

AxrStringView AxrString::substring(const size_t characterIndex, const size_t count) const {
    return AxrStringView(data(), size()).substring(characterIndex, count);
}

AxrStringView AxrString::substring(const Iterator& startIterator, const size_t count) const {
    return AxrStringView(data(), size()).substring(startIterator, count);
}

AxrStringView AxrString::substring(const Iterator& startIterator, const Iterator& endIterator) const {
    return AxrStringView(data(), size()).substring(startIterator, endIterator);
}

void AxrString::pop(const size_t count) {
    Iterator popEndIterator = end();

    uint32_t currentCount = 0;
    for (Iterator beginIt = end(), endIt = begin(); beginIt != endIt;
         /* Don't decrement here. It's done inside the loop*/) {
        if (currentCount == count) {
            popEndIterator = beginIt;
        }

        --beginIt;

        ++currentCount;
    }

    pop(popEndIterator);
}

void AxrString::pop(const Iterator& iterator) {
    if (iterator == end()) {
        return;
    }

    if (m_IsHeapAllocated) {
        const size_t newSize = reinterpret_cast<uintptr_t>(iterator.getDataPtr()) -
                               reinterpret_cast<uintptr_t>(m_HeapString.Data.getDataPtr());
        m_HeapString.Size = newSize;
        m_HeapString.Data[m_HeapString.Size] = '\0';
    } else {
        const size_t newSize = reinterpret_cast<uintptr_t>(iterator.getDataPtr()) -
                               reinterpret_cast<uintptr_t>(&m_StackString.Data);
        m_StackString.Size = newSize;
        m_StackString.Data[m_StackString.Size] = '\0';
    }
}

void AxrString::clear() {
    if (m_IsHeapAllocated) {
        m_HeapString.Size = 0;
        m_HeapString.Data[0] = '\0';
    } else {
        m_StackString.Size = 0;
        m_StackString.Data[0] = '\0';
    }
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrString::cleanup() {
    deallocateData();

    m_DynamicAllocator = nullptr;
}

void AxrString::move_internal(AxrString&& src, const bool useConstructor) {
    if (useConstructor) {
        if (src.m_IsHeapAllocated) {
            new (&m_HeapString.Data) AxrHandle(std::move(src.m_HeapString.Data));
        }
    } else {
        if (src.m_IsHeapAllocated) {
            m_HeapString.Data = std::move(src.m_HeapString.Data);
        }
    }

    if (src.m_IsHeapAllocated) {
        m_HeapString.Capacity = src.m_HeapString.Capacity;
        m_HeapString.Size = src.m_HeapString.Size;
    } else {
        m_StackString.Size = src.m_StackString.Size;

        memccpy(m_StackString.Data, src.m_StackString.Data, '\0', sizeof(m_StackString.Data));
    }

    m_DynamicAllocator = src.m_DynamicAllocator;
    m_IsHeapAllocated = src.m_IsHeapAllocated;

    src.m_DynamicAllocator = nullptr;
}

#define AXR_FUNCTION_FAILED_STRING "Failed to deallcoate AxrString data. "
void AxrString::deallocateData() {
    if (!m_IsHeapAllocated) {
        return;
    }

    if (m_HeapString.Data == nullptr) {
        return;
    }

    if (m_DynamicAllocator == nullptr) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Memory leak detected. DynamicAllocator is null.");
        return;
    }

    m_DynamicAllocator->deallocate(m_HeapString.Data);
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to set AxrString string data. "
void AxrString::setStringData(const char8_t* string, const size_t stringSize, const size_t dataOffset) {
    char8_t* data = m_IsHeapAllocated ? m_HeapString.Data.getDataPtr() : m_StackString.Data;
    const size_t dataCapacity = m_IsHeapAllocated ? m_HeapString.Capacity : StackCapacity;

    // Plus one to account for the null terminator
    if (dataOffset + stringSize + 1 > dataCapacity) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "String data doesn't fit. Grow the capacity first.");
        return;
    }

    data += dataOffset;

    std::char_traits<char8_t>::copy(data, string, stringSize);

    if (m_IsHeapAllocated) {
        m_HeapString.Size += stringSize;
        m_HeapString.Data[m_HeapString.Size] = '\0';
    } else {
        m_StackString.Size += stringSize;
        m_StackString.Data[m_StackString.Size] = '\0';
    }
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to set AxrString string data. "
void AxrString::setStringData(const char* string, const size_t stringSize, const size_t dataOffset) {
    char8_t* data = m_IsHeapAllocated ? m_HeapString.Data.getDataPtr() : m_StackString.Data;
    const size_t dataCapacity = m_IsHeapAllocated ? m_HeapString.Capacity : StackCapacity;

    // Plus one to account for the null terminator
    if (dataOffset + stringSize + 1 > dataCapacity) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "String data doesn't fit. Grow the capacity first.");
        return;
    }

    data += dataOffset;

    for (size_t i = 0; i < stringSize; ++i) {
        data[i] = static_cast<char8_t>(string[i]);
    }

    if (m_IsHeapAllocated) {
        m_HeapString.Size += stringSize;
        m_HeapString.Data[m_HeapString.Size] = '\0';
    } else {
        m_StackString.Size += stringSize;
        m_StackString.Data[m_StackString.Size] = '\0';
    }
}
#undef AXR_FUNCTION_FAILED_STRING
