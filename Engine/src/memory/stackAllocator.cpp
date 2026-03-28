// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "stackAllocator.h"
#include "axr/logging.h"
#include "utils.h"

#include <cstring>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrStackAllocator::AxrStackAllocator() = default;

AxrStackAllocator::AxrStackAllocator(const AxrMemoryBlock& memoryBlock) :
    AxrSubAllocatorBase(memoryBlock) {
}

AxrStackAllocator::AxrStackAllocator(AxrStackAllocator&& src) noexcept :
    AxrSubAllocatorBase(std::move(src)) {
    move_internal(std::move(src));
}

AxrStackAllocator::~AxrStackAllocator() {
    cleanup();
}

AxrStackAllocator& AxrStackAllocator::operator=(AxrStackAllocator&& src) noexcept {
    if (this != &src) {
        cleanup();

        AxrSubAllocatorBase::operator=(std::move(src));

        move_internal(std::move(src));
    }
    return *this;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory block for AxrStackAllocator. "
AxrResult AxrStackAllocator::allocateBlock(const size_t size,
                                           const uint8_t alignment,
                                           void*& memory,
                                           MarkerID& markerID,
                                           const bool zeroOutMemory) {
    // Make sure there's enough space for both the requested memory size and for its marker.
    const size_t blockSize = size + alignment + sizeof(Marker);
    const size_t dataSize = size + alignment;
    if (blockSize > m_Capacity - m_Size) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of memory for a block of size {} bytes and alignment of {}.",
                    size,
                    alignment);
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    MarkerID currentID = getCurrentMarker().ID;

    if (zeroOutMemory) {
        std::memset(reinterpret_cast<void*>(end()), 0, blockSize);
    }

    memory = axrAlignMemory(reinterpret_cast<void*>(end()), alignment);
    // Yes, we will never get an ID of 0. This is so if we get a marker ID of 0 from `getCurrentMarker()`,
    // then it means there is nothing allocated.
    markerID = ++currentID;

    m_Size += blockSize;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    if (m_Size > m_PeakSize) {
        m_PeakSize = m_Size;
    }
#endif

    setCurrentMarker(Marker{.Size = dataSize, .ID = markerID});

    return AXR_SUCCESS;
}
#undef AXR_FUNCTION_FAILED_STRING

void AxrStackAllocator::deallocate(const MarkerID markerID) {
    const Marker currentMarker = getCurrentMarker();
    assert(markerID <= currentMarker.ID);

    // Continue to pop all the way until we pop the marker with the same ID
    for (MarkerID i = 0; i <= currentMarker.ID - markerID; ++i) {
        pop();
    }
}

bool AxrStackAllocator::deallocateIfLast(const MarkerID markerID) {
    if (getCurrentMarker().ID == markerID) {
        deallocate(markerID);
        return true;
    }
    return false;
}

void AxrStackAllocator::clear() {
    m_Size = 0;
    // Don't zero out memory
}

size_t AxrStackAllocator::size() const {
    return m_Size;
}

#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
size_t AxrStackAllocator::peakSize() const {
    return m_PeakSize;
}
#endif

bool AxrStackAllocator::empty() const {
    return m_Size == 0;
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrStackAllocator::cleanup() {
    m_Size = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    m_PeakSize = {};
#endif

    AxrSubAllocatorBase::cleanup();
}

void AxrStackAllocator::move_internal(AxrStackAllocator&& src) {
    // Please note that we aren't moving the base class. That should be done before calling this function because
    // depending on how it's done, it changes if we call the base move constructor or move assignment operator.

    m_Size = src.m_Size;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    m_PeakSize = src.m_PeakSize;
#endif

    src.m_Size = {};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    src.m_PeakSize = {};
#endif
}

inline uintptr_t AxrStackAllocator::begin() const {
    assert(m_Memory);

    return reinterpret_cast<uintptr_t>(m_Memory);
}

inline uintptr_t AxrStackAllocator::end() const {
    return begin() + m_Size;
}

inline AxrStackAllocator::Marker AxrStackAllocator::getCurrentMarker() const {
    if (begin() == end()) [[unlikely]] {
        return {};
    }

    return *reinterpret_cast<Marker*>(end() - sizeof(Marker));
}

inline void AxrStackAllocator::setCurrentMarker(const Marker& marker) const {
    if (begin() == end()) [[unlikely]] {
        return;
    }

    Marker& currentMarker = *reinterpret_cast<Marker*>(end() - sizeof(Marker));
    currentMarker = marker;
}

inline void AxrStackAllocator::pop() {
    const Marker currentMarker = getCurrentMarker();
    if (currentMarker.ID == 0) [[unlikely]] {
        return;
    }

    const size_t blockSize = currentMarker.Size + sizeof(Marker);
    m_Size -= blockSize;
    // Don't zero out memory.
}
