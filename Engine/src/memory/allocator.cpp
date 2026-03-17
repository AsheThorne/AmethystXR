// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "allocator.h"
#include "axr/logging.h"

#include <cassert>
#include <cstdlib>

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrAllocator::AxrAllocator() = default;

AxrAllocator::~AxrAllocator() {
    shutDown();
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

AxrAllocator& AxrAllocator::get() {
    static AxrAllocator singleton;
    return singleton;
}

AxrResult AxrAllocator::setup(const Config& config) {
    assert(!m_IsSetup);

    m_MemorySize = config.FrameAllocatorSize;
    m_Memory = static_cast<uint8_t*>(malloc(m_MemorySize));

    // ---- Frame Allocator ----
    AxrDeallocateBlock frameAllocatorDeallocateCallback;
    frameAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateFrameAllocatorCallback>(this);
    FrameAllocator = AxrStackAllocator(m_Memory, config.FrameAllocatorSize, frameAllocatorDeallocateCallback);

    m_IsSetup = true;
    return AXR_SUCCESS;
}

void AxrAllocator::shutDown() {
    FrameAllocator = {};

    if (m_Memory != nullptr) {
        free(m_Memory);
        m_Memory = nullptr;
    }
    m_MemorySize = {};

    m_IsSetup = false;
}

void AxrAllocator::logFrameAllocatorUsage(const char* message) const {
    const size_t size = FrameAllocator.size();
    const size_t capacity = FrameAllocator.capacity();
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    const size_t peakSize = FrameAllocator.peakSize();
#endif

    axrLogInfo("{}: Frame Allocator memory usage. {:.2f}% Used currently. {} Bytes used out of {}."
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
               " Peak usage reached {:.2f}%.",
#endif
               message,
               static_cast<float>(size) / static_cast<float>(capacity) * 100,
               size,
               capacity
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
               ,
               static_cast<float>(peakSize) / static_cast<float>(capacity) * 100
#endif
    );
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrAllocator::deallocateFrameAllocatorCallback(void*& memory) {
    // NEVER EVER modify anything about the FrameAllocator within this function. We don't want to risk retriggering this
    // callback and entering an infinite loop.
    // We don't really need to do anything here since we don't intend to reuse
    // its memory block and the entire block gets freed when the allocator is cleaned up.
    memory = nullptr;
}
