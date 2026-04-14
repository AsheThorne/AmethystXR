// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "string.h"

#include <algorithm>

// ---------------------------------------------------------------------------------- //
//                               AxrString - Iterator                                 //
// ---------------------------------------------------------------------------------- //

AxrString::Iterator::Iterator(const char8_t* character) :
    m_Character(character) {
}

AxrString::Iterator& AxrString::Iterator::operator++() {
    if ((*m_Character & 0b10000000) == 0) {
        // Current character is a 1 byte code point (ascii)
        ++m_Character;
        return *this;
    }

    if (*m_Character >> 5 == 0b00000110) {
        // Current character is a 2 byte code point
        m_Character += 2;
        return *this;
    }

    if (*m_Character >> 4 == 0b00001110) {
        // Current character is a 3 byte code point
        m_Character += 3;
        return *this;
    }

    if (*m_Character >> 3 == 0b00011110) {
        // Current character is a 4 byte code point
        m_Character += 4;
        return *this;
    }

    axrLogError("Failed to increment AxrString iterator. Current character is not a UTF-8 leading character.");
    return *this;
}

AxrString::Iterator AxrString::Iterator::operator++(int) {
    const Iterator returnValue = *this;
    operator++();

    return returnValue;
}

AxrString::Iterator& AxrString::Iterator::operator--() {
    --m_Character;

    if ((*m_Character & 0b10000000) == 0) {
        // previous character is a 1 byte code point (ascii)
        return *this;
    }

    // If we get here, the previous character must be a multibyte code point. So we keep stepping backwards
    // until we're no longer on a continuation byte
    while (*m_Character >> 6 == 0b00000010) {
        --m_Character;
    }

    return *this;
}

AxrString::Iterator AxrString::Iterator::operator--(int) {
    const Iterator returnValue = *this;
    operator--();

    return returnValue;
}

bool AxrString::Iterator::operator==(const Iterator& other) const {
    return m_Character == other.m_Character;
}

bool AxrString::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

AxrArray<char8_t, 4> AxrString::Iterator::operator*() const {
    AxrArray<char8_t, 4> array;
    const char8_t* currentByte = m_Character;
    // Add each byte making up the current character
    do {
        array.pushBack(*currentByte);
        ++currentByte;
        // Continue to loop if the current byte is a continuation byte
    } while (*currentByte >> 6 == 0b00000010);
    // We don't set a null terminator since it's an array of a fixed length. And this will only ever represent a single
    // UTF-8 character

    return array;
}

// ---------------------------------------------------------------------------------- //
//                                     AxrString                                      //
// ---------------------------------------------------------------------------------- //

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
    move_internal(std::move(src));
}

AxrString::~AxrString() {
    cleanup();
}

AxrString& AxrString::operator=(const char8_t* src) {
    buildString(src);
    return *this;
}

AxrString& AxrString::operator=(AxrString&& src) noexcept {
    if (this != &src) {
        cleanup();

        move_internal(std::move(src));
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
    const size_t currentStringSize = m_IsHeapAllocated ? m_HeapString.Size : m_StackString.Size;

    const AxrResult axrResult = growAllocation(currentStringSize + stringSize);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to grow allocation.");
        return axrResult;
    }

    setStringData(string, stringSize, currentStringSize);

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

AxrString AxrString::substring(const size_t characterIndex, const size_t count, AxrDynamicAllocator* allocator) const {
    Iterator startIterator = end();

    size_t currentCharacterIndex = 0;
    for (Iterator beginIt = begin(), endIt = end(); beginIt != endIt; ++beginIt) {
        if (currentCharacterIndex == characterIndex) {
            startIterator = beginIt;
            break;
        }

        ++currentCharacterIndex;
    }

    return substring(startIterator, count, allocator);
}

AxrString AxrString::substring(const Iterator& startIterator,
                               const size_t count,
                               AxrDynamicAllocator* allocator) const {
    Iterator endIterator = end();

    size_t currentCharacterIndex = 0;
    for (Iterator beginIt = startIterator, endIt = end(); beginIt != endIt; ++beginIt) {
        if (currentCharacterIndex == count) {
            endIterator = beginIt;
            break;
        }

        ++currentCharacterIndex;
    }

    return substring(startIterator, endIterator, allocator);
}

#define AXR_FUNCTION_FAILED_STRING "Failed to get AxrString substring. "
AxrString AxrString::substring(const Iterator& startIterator,
                               const Iterator& endIterator,
                               AxrDynamicAllocator* allocator) const {
    const size_t substringSize = reinterpret_cast<uintptr_t>(endIterator.getDataPtr()) -
                                 reinterpret_cast<uintptr_t>(startIterator.getDataPtr());

    AxrString substring(allocator != nullptr ? allocator : m_DynamicAllocator);

    const AxrResult axrResult = substring.growAllocation(substringSize);
    if (AXR_FAILED(axrResult)) [[unlikely]] {
        axrLogError("Failed to grow substring allocation.");
        return {};
    }

    substring.setStringData(startIterator.getDataPtr(), substringSize);

    return substring;
}
#undef AXR_FUNCTION_FAILED_STRING

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

    if (m_IsHeapAllocated) {
        m_HeapString.Capacity = {};
        m_HeapString.Size = {};
    } else {
        m_StackString.Size = {};
    }

    m_DynamicAllocator = {};
    m_IsHeapAllocated = {};
}

void AxrString::move_internal(AxrString&& src) {
    if (src.m_IsHeapAllocated) {
        m_HeapString.Data = std::move(src.m_HeapString.Data);

        m_HeapString.Capacity = src.m_HeapString.Capacity;
        m_HeapString.Size = src.m_HeapString.Size;

        src.m_HeapString.Capacity = {};
        src.m_HeapString.Size = {};
    } else {
        m_StackString.Size = src.m_StackString.Size;

        memccpy(m_StackString.Data, src.m_StackString.Data, '\0', sizeof(m_StackString.Data));

        src.m_StackString.Size = {};
        std::memset(src.m_StackString.Data, 0, sizeof(src.m_StackString.Data));
    }

    m_DynamicAllocator = src.m_DynamicAllocator;
    m_IsHeapAllocated = src.m_IsHeapAllocated;

    src.m_DynamicAllocator = {};
    src.m_IsHeapAllocated = {};
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

    if (size <= m_IsHeapAllocated) {
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
