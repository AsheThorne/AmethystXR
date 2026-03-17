// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "doubleStackAllocator.h"
#include "axr/logging.h"
#include "utils.h"

#include <cstring>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrDoubleStackAllocator::AxrDoubleStackAllocator() = default;

AxrDoubleStackAllocator::AxrDoubleStackAllocator(const AxrMemoryBlock& memoryBlock) :
    AxrSubAllocatorBase(memoryBlock) {
}

AxrDoubleStackAllocator::AxrDoubleStackAllocator(AxrDoubleStackAllocator&& src) noexcept :
    AxrSubAllocatorBase(std::move(src)) {
    m_SizeLower = src.m_SizeLower;
    m_SizeUpper = src.m_SizeUpper;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    m_PeakSize = src.m_PeakSize;
#endif

    src.m_SizeLower = {};
    src.m_SizeUpper = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    src.m_PeakSize = {};
#endif
}

AxrDoubleStackAllocator::~AxrDoubleStackAllocator() {
    cleanup();
}

AxrDoubleStackAllocator& AxrDoubleStackAllocator::operator=(AxrDoubleStackAllocator&& src) noexcept {
    if (this != &src) {
        cleanup();

        AxrSubAllocatorBase::operator=(std::move(src));

        m_SizeLower = src.m_SizeLower;
        m_SizeUpper = src.m_SizeUpper;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        m_PeakSize = src.m_PeakSize;
#endif

        src.m_SizeLower = {};
        src.m_SizeUpper = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
        src.m_PeakSize = {};
#endif
    }
    return *this;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory block on the lower end for AxrDoubleStackAllocator. "
AxrResult AxrDoubleStackAllocator::allocateLowerBlock(const size_t size,
                                                      const uint8_t alignment,
                                                      void*& memory,
                                                      MarkerID& markerID,
                                                      const bool zeroOutMemory) {
    // Make sure there's enough space for both the requested memory size and for its marker.
    const size_t blockSize = size + alignment + sizeof(Marker);
    const size_t dataSize = size + alignment;
    if (blockSize > m_Capacity - AxrDoubleStackAllocator::size()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of memory for a block of size {} bytes and alignment of {}.",
                    size,
                    alignment);
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    MarkerID currentID = getCurrentMarkerLower().ID;

    if (zeroOutMemory) {
        std::memset(reinterpret_cast<void*>(endLower()), 0, blockSize);
    }

    memory = axrAlignMemory(reinterpret_cast<void*>(endLower()), alignment);
    // Yes, we will never get an ID of 0. This is so if we get a marker ID of 0 from `getCurrentMarkerLower()`,
    // then it means there is nothing allocated.
    markerID = ++currentID;

    m_SizeLower += blockSize;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    if (m_SizeLower + m_SizeUpper > m_PeakSize) {
        m_PeakSize = m_SizeLower + m_SizeUpper;
    }
#endif

    setCurrentMarkerLower(Marker{.Size = dataSize, .ID = markerID});

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory block on the upper end for AxrDoubleStackAllocator. "
AxrResult AxrDoubleStackAllocator::allocateUpperBlock(const size_t size,
                                                      const uint8_t alignment,
                                                      void*& memory,
                                                      MarkerID& markerID,
                                                      const bool zeroOutMemory) {
    // Make sure there's enough space for both the requested memory size and for its marker.
    const size_t blockSize = size + alignment + sizeof(Marker);
    const size_t dataSize = size + alignment;
    if (blockSize > m_Capacity - AxrDoubleStackAllocator::size()) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of memory for a block of size {} bytes and alignment of {}.",
                    size,
                    alignment);
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    MarkerID currentID = getCurrentMarkerUpper().ID;

    if (zeroOutMemory) {
        std::memset(reinterpret_cast<void*>(endUpper() - blockSize), 0, blockSize);
    }

    // minus size, not block size. if it was block size then the marker would be at the head of this address which we
    // obviously don't want
    memory = axrAlignMemory(reinterpret_cast<void*>(endUpper() - dataSize), alignment);
    // Yes, we will never get an ID of 0. This is so if we get a marker ID of 0 from `getCurrentMarkerUpper()`,
    // then it means there is nothing allocated.
    markerID = ++currentID;

    m_SizeUpper += blockSize;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    if (m_SizeLower + m_SizeUpper > m_PeakSize) {
        m_PeakSize = m_SizeLower + m_SizeUpper;
    }
#endif

    setCurrentMarkerUpper(Marker{.Size = dataSize, .ID = markerID});

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

bool AxrDoubleStackAllocator::deallocateIfLastLower(const MarkerID markerID) {
    if (getCurrentMarkerLower().ID == markerID) {
        deallocateLower(markerID);
        return true;
    }
    return false;
}

bool AxrDoubleStackAllocator::deallocateIfLastUpper(const MarkerID markerID) {
    if (getCurrentMarkerUpper().ID == markerID) {
        deallocateUpper(markerID);
        return true;
    }
    return false;
}

void AxrDoubleStackAllocator::clear() {
    clearLower();
    clearUpper();
}

void AxrDoubleStackAllocator::clearLower() {
    m_SizeLower = 0;
    // Don't zero out memory
}

void AxrDoubleStackAllocator::clearUpper() {
    m_SizeUpper = 0;
    // Don't zero out memory
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

#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
size_t AxrDoubleStackAllocator::peakSize() const {
    return m_PeakSize;
}
#endif

bool AxrDoubleStackAllocator::empty() const {
    return emptyLower() && emptyUpper();
}

bool AxrDoubleStackAllocator::emptyLower() const {
    return m_SizeLower == 0;
}

bool AxrDoubleStackAllocator::emptyUpper() const {
    return m_SizeUpper == 0;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrDoubleStackAllocator::cleanup() {
    m_SizeLower = {};
    m_SizeUpper = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    m_PeakSize = {};
#endif

    AxrSubAllocatorBase::cleanup();
}

inline uintptr_t AxrDoubleStackAllocator::beginLower() const {
    assert(m_Memory);

    return reinterpret_cast<uintptr_t>(m_Memory);
}

uintptr_t AxrDoubleStackAllocator::beginUpper() const {
    assert(m_Memory);

    return reinterpret_cast<uintptr_t>(m_Memory) + m_Capacity;
}

inline uintptr_t AxrDoubleStackAllocator::endLower() const {
    return beginLower() + m_SizeLower;
}

uintptr_t AxrDoubleStackAllocator::endUpper() const {
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
    if (currentMarker.ID == 0) [[unlikely]] {
        return;
    }

    const size_t blockSize = currentMarker.Size + sizeof(Marker);
    m_SizeLower -= blockSize;
    // Don't zero out memory
}

void AxrDoubleStackAllocator::popUpper() {
    const Marker currentMarker = getCurrentMarkerUpper();
    if (currentMarker.ID == 0) [[unlikely]] {
        return;
    }

    const size_t blockSize = currentMarker.Size + sizeof(Marker);
    m_SizeUpper -= blockSize;
    // Don't zero out memory
}
