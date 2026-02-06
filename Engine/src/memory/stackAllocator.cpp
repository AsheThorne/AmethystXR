// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "stackAllocator.h"
#include "axr/logging.h"

#include <cstring>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrStackAllocator::AxrStackAllocator(void* memory, const size_t size, const AxrDeallocateBlock& deallocator) :
    AxrSubAllocatorBase(memory, size, deallocator) {
}

AxrStackAllocator::AxrStackAllocator() = default;

AxrStackAllocator::AxrStackAllocator(AxrStackAllocator&& src) noexcept :
    AxrSubAllocatorBase(std::move(src)) {
    m_Size = src.m_Size;

    src.m_Size = {};
}

AxrStackAllocator::~AxrStackAllocator() {
    cleanup();
}

AxrStackAllocator& AxrStackAllocator::operator=(AxrStackAllocator&& src) noexcept {
    if (this != &src) {
        cleanup();

        AxrSubAllocatorBase::operator=(std::move(src));

        m_Size = src.m_Size;

        src.m_Size = {};
    }
    return *this;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate memory for AxrStackAllocator. "
AxrResult AxrStackAllocator::allocate(const size_t size, void*& memory, MarkerID& markerID) {
    // Make sure there's enough space for both the requested memory size and for its marker.
    const size_t blockSize = size + sizeof(Marker);
    if (blockSize > m_Capacity - m_Size) [[unlikely]] {
        axrLogError(AXR_FUNCTION_FAILED_STRING "Ran out of memory for a block of size {} bytes.", size);
        return AXR_ERROR_OUT_OF_MEMORY;
    }

    MarkerID currentID = getCurrentMarker().ID;
    // TODO (Ashe): Make zeroing out memory optional maybe. Possibly with a flag
    std::memset(end(), 0, blockSize);

    memory = end();
    // Yes, we will never get an ID of 0. This is so if we get a marker ID of 0 from `getCurrentMarker()`,
    // then it means there is nothing allocated.
    markerID = ++currentID;

    m_Size += blockSize;

    setCurrentMarker(Marker{.Size = size, .ID = markerID});

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

void AxrStackAllocator::clear() {
    m_Size = 0;
    // Don't zero out memory
}

size_t AxrStackAllocator::size() const {
    return m_Size;
}

bool AxrStackAllocator::empty() const {
    return m_Size == 0;
}

uint32_t AxrStackAllocator::getMarkerSize() {
    return sizeof(Marker);
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrStackAllocator::cleanup() {
    AxrSubAllocatorBase::cleanup();

    m_Size = {};
}

inline uint8_t* AxrStackAllocator::begin() const {
    assert(m_Memory);

    return m_Memory;
}

inline uint8_t* AxrStackAllocator::end() const {
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
    if (currentMarker.ID == 0) {
        return;
    }

    const size_t blockSize = currentMarker.Size + sizeof(Marker);
    m_Size -= blockSize;
    // Don't zero out memory.
}
