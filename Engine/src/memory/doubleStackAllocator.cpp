// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "doubleStackAllocator.h"
#include "axr/logging.h"

#include <cstring>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrDoubleStackAllocator::AxrDoubleStackAllocator(const size_t size,
                                                 void* memory,
                                                 const AxrDeallocate& deallocate) {
    m_Memory = static_cast<uint8_t*>(memory);
    m_Capacity = size;
    m_MainMemoryDeallocator = deallocate;
}

AxrDoubleStackAllocator::AxrDoubleStackAllocator(AxrDoubleStackAllocator&& src) noexcept {
    m_Memory = src.m_Memory;
    m_Capacity = src.m_Capacity;
    m_SizeLower = src.m_SizeLower;
    m_SizeUpper = src.m_SizeUpper;
    m_MainMemoryDeallocator = src.m_MainMemoryDeallocator;
}

AxrDoubleStackAllocator::~AxrDoubleStackAllocator() {
    cleanup();
}

AxrDoubleStackAllocator& AxrDoubleStackAllocator::operator=(AxrDoubleStackAllocator&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Memory = src.m_Memory;
        m_Capacity = src.m_Capacity;
        m_SizeLower = src.m_SizeLower;
        m_SizeUpper = src.m_SizeUpper;
        m_MainMemoryDeallocator = src.m_MainMemoryDeallocator;
    }
    return *this;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory on the lower end for AxrDoubleStackAllocator. "
AxrResult AxrDoubleStackAllocator::allocateLower(const size_t size, void*& memory, MarkerID& markerID) {
    // Make sure there's enough space for both the requested memory size and for its marker.
    const size_t blockSize = size + sizeof(Marker);
    if (blockSize > m_Capacity - AxrDoubleStackAllocator::size()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of memory for a block of size {} bytes.", size);
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    MarkerID currentID = getCurrentMarkerLower().ID;
    std::memset(endLower(), 0, blockSize);

    memory = endLower();
    // Yes, we will never get an ID of 0. This is so if we get a marker ID of 0 from `getCurrentMarkerLower()`,
    // then it means there is nothing allocated.
    markerID = ++currentID;

    m_SizeLower += blockSize;

    setCurrentMarkerLower(Marker{.Size = size, .ID = markerID});

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory on the lower end for AxrDoubleStackAllocator. "
AxrResult AxrDoubleStackAllocator::allocateUpper(const size_t size, void*& memory, MarkerID& markerID) {
    // Make sure there's enough space for both the requested memory size and for its marker.
    const size_t blockSize = size + sizeof(Marker);
    if (blockSize > m_Capacity - AxrDoubleStackAllocator::size()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of memory for a block of size {} bytes.", size);
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    MarkerID currentID = getCurrentMarkerUpper().ID;
    std::memset(endUpper() - blockSize, 0, blockSize);

    // minus size, not block size. if it was block size then the marker would be at the head of this address which we
    // obviously don't want
    memory = endUpper() - size;
    // Yes, we will never get an ID of 0. This is so if we get a marker ID of 0 from `getCurrentMarkerUpper()`,
    // then it means there is nothing allocated.
    markerID = ++currentID;

    m_SizeUpper += blockSize;

    setCurrentMarkerUpper(Marker{.Size = size, .ID = markerID});

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrDoubleStackAllocator::deallocateLower(const MarkerID markerID) {
    const Marker currentMarker = getCurrentMarkerLower();
    assert(markerID <= currentMarker.ID);

    // Continue to pop all the way until we pop the marker with the same ID
    for (MarkerID i = 0; i <= currentMarker.ID - markerID; ++i) {
        popLower();
    }
}

void AxrDoubleStackAllocator::deallocateUpper(const MarkerID markerID) {
    const Marker currentMarker = getCurrentMarkerUpper();
    assert(markerID <= currentMarker.ID);

    // Continue to pop all the way until we pop the marker with the same ID
    for (MarkerID i = 0; i <= currentMarker.ID - markerID; ++i) {
        popUpper();
    }
}

void AxrDoubleStackAllocator::clear() {
    clearLower();
    clearUpper();
}

void AxrDoubleStackAllocator::clearLower() {
    // Don't zero out memory. We 0 it out when allocating
    m_SizeLower = 0;
}

void AxrDoubleStackAllocator::clearUpper() {
    // Don't zero out memory. We 0 it out when allocating
    m_SizeUpper = 0;
}

size_t AxrDoubleStackAllocator::capacity() const {
    return m_Capacity;
}

size_t AxrDoubleStackAllocator::size() const {
    return sizeLower() + sizeUpper();
}

size_t AxrDoubleStackAllocator::sizeLower() const {
    return m_SizeLower;
}

size_t AxrDoubleStackAllocator::sizeUpper() const {
    return m_SizeUpper;
}

bool AxrDoubleStackAllocator::empty() const {
    return emptyLower() && emptyUpper();
}

bool AxrDoubleStackAllocator::emptyLower() const {
    return m_SizeLower == 0;
}

bool AxrDoubleStackAllocator::emptyUpper() const {
    return m_SizeUpper == 0;
}

uint32_t AxrDoubleStackAllocator::getMarkerSize() {
    return sizeof(Marker);
}

void AxrDoubleStackAllocator::cleanup() {
    if (m_Memory != nullptr) {
        if (m_MainMemoryDeallocator) {
            m_MainMemoryDeallocator(reinterpret_cast<void*&>(m_Memory));
        } else {
            axrLogWarning(
                "Memory leak detected inside AxrDoubleStackAllocator. Failed to deallocate a block of memory. No "
                "deallocator available.");
        }
    }
    m_Capacity = 0;
    m_SizeLower = 0;
    m_SizeUpper = 0;
    m_MainMemoryDeallocator.reset();
}

inline uint8_t* AxrDoubleStackAllocator::beginLower() const {
    assert(m_Memory);

    return m_Memory;
}

uint8_t* AxrDoubleStackAllocator::beginUpper() const {
    assert(m_Memory);

    return m_Memory + m_Capacity;
}

inline uint8_t* AxrDoubleStackAllocator::endLower() const {
    return beginLower() + m_SizeLower;
}

uint8_t* AxrDoubleStackAllocator::endUpper() const {
    return beginUpper() - m_SizeUpper;
}

inline AxrDoubleStackAllocator::Marker AxrDoubleStackAllocator::getCurrentMarkerLower() const {
    if (beginLower() == endLower()) [[unlikely]] {
        return {};
    }

    return *reinterpret_cast<Marker*>(endLower() - sizeof(Marker));
}

AxrDoubleStackAllocator::Marker AxrDoubleStackAllocator::getCurrentMarkerUpper() const {
    if (beginUpper() == endUpper()) [[unlikely]] {
        return {};
    }

    return *reinterpret_cast<Marker*>(endUpper());
}

inline void AxrDoubleStackAllocator::setCurrentMarkerLower(const Marker& marker) const {
    if (beginLower() == endLower()) [[unlikely]] {
        return;
    }

    Marker& currentMarker = *reinterpret_cast<Marker*>(endLower() - sizeof(Marker));
    currentMarker = marker;
}

void AxrDoubleStackAllocator::setCurrentMarkerUpper(const Marker& marker) const {
    if (beginUpper() == endUpper()) [[unlikely]] {
        return;
    }

    Marker& currentMarker = *reinterpret_cast<Marker*>(endUpper());
    currentMarker = marker;
}

inline void AxrDoubleStackAllocator::popLower() {
    const Marker currentMarker = getCurrentMarkerLower();
    if (currentMarker.ID == 0) {
        return;
    }

    const size_t blockSize = currentMarker.Size + sizeof(Marker);
    // Don't zero out memory. We 0 it out when allocating
    m_SizeLower -= blockSize;
}

void AxrDoubleStackAllocator::popUpper() {
    const Marker currentMarker = getCurrentMarkerUpper();
    if (currentMarker.ID == 0) {
        return;
    }

    const size_t blockSize = currentMarker.Size + sizeof(Marker);
    // Don't zero out memory. We 0 it out when allocating
    m_SizeUpper -= blockSize;
}
