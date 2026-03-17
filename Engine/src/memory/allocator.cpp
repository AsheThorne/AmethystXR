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

    const size_t frameAllocatorSize = config.FrameAllocatorSize;
    const size_t engineDataAllocatorSize =
        config.EngineDataAllocatorMainMemorySize +
        AxrDynamicAllocator::getHandlesMemoryBlockCapacity(config.EngineDataAllocatorMaxHandleCount);
    m_MemorySize = frameAllocatorSize + engineDataAllocatorSize;
    m_Memory = static_cast<uint8_t*>(malloc(m_MemorySize));

    // ---- Frame Allocator ----
    AxrDeallocateBlock frameAllocatorDeallocateCallback;
    frameAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateFrameAllocatorCallback>();
    uint8_t* frameAllocatorMemory = m_Memory;
    FrameAllocator = AxrStackAllocator(AxrMemoryBlock{
        .Memory = frameAllocatorMemory,
        .Size = frameAllocatorSize,
        .Deallocator = frameAllocatorDeallocateCallback,
    });

    // ---- Engine Data Allocator ----
    AxrDeallocateBlock engineDataAllocatorDeallocateCallback;
    engineDataAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateEngineDataAllocatorCallback>();
    uint8_t* engineDataAllocatorMemory = frameAllocatorMemory + frameAllocatorSize;
    EngineDataAllocator = AxrDynamicAllocator(
        AxrMemoryBlock{
            .Memory = engineDataAllocatorMemory,
            .Size = config.EngineDataAllocatorMainMemorySize,
            .Deallocator = frameAllocatorDeallocateCallback,
        },
        config.EngineDataAllocatorMaxHandleCount);

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

void AxrAllocator::logEngineDataAllocatorUsage(const char* message) const {
    const size_t size = EngineDataAllocator.mainMemorySize();
    const size_t handlesMemorySize = EngineDataAllocator.handlesMemorySize();
    const size_t mainMemoryCapacity = EngineDataAllocator.mainMemoryCapacity();
    const size_t handlesMemoryCapacity = EngineDataAllocator.handlesMemoryCapacity();
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    const size_t peakMainMemorySize = EngineDataAllocator.peakMainMemorySize();
    const size_t peakHandlesMemorySize = EngineDataAllocator.peakHandlesMemorySize();
#endif

    axrLogInfo("{}: Engine Data Allocator memory usage."
               " {:.2f}% Main memory used currently. {} Bytes used out of {}."
               " {:.2f}% Handles memory used currently. {} Bytes used out of {}."
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
               " Peak main memory usage reached {:.2f}%."
               " Peak handles memory usage reached {:.2f}%.",
#endif
               message,
               static_cast<float>(size) / static_cast<float>(mainMemoryCapacity) * 100,
               size,
               mainMemoryCapacity,
               static_cast<float>(handlesMemorySize) / static_cast<float>(handlesMemoryCapacity) * 100,
               handlesMemorySize,
               handlesMemoryCapacity
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
               ,
               static_cast<float>(peakMainMemorySize) / static_cast<float>(mainMemoryCapacity) * 100,
               static_cast<float>(peakHandlesMemorySize) / static_cast<float>(handlesMemoryCapacity) * 100
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

void AxrAllocator::deallocateEngineDataAllocatorCallback(void*& memory) {
    // NEVER EVER modify anything about the FrameAllocator within this function. We don't want to risk retriggering this
    // callback and entering an infinite loop.
    // We don't really need to do anything here since we don't intend to reuse
    // its memory block and the entire block gets freed when the allocator is cleaned up.
    memory = nullptr;
}
