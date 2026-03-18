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
    const size_t handlesAllocatorSize =
        AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node>::getAllocatorSize(config.MaxHandleCount);
    const size_t engineDataAllocatorSize = config.EngineDataAllocatorMainMemorySize;

    m_MemorySize = frameAllocatorSize + handlesAllocatorSize + engineDataAllocatorSize;
    m_Memory = malloc(m_MemorySize);

    // ---- Frame Allocator ----
    AxrDeallocateBlock frameAllocatorDeallocateCallback;
    frameAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateFrameAllocatorCallback>();
    void* frameAllocatorMemory = m_Memory;
    FrameAllocator = AxrStackAllocator(AxrMemoryBlock{
        .Memory = frameAllocatorMemory,
        .Size = frameAllocatorSize,
        .Deallocator = frameAllocatorDeallocateCallback,
    });

    // ---- Handles Allocator ----
    AxrDeallocateBlock handlesAllocatorDeallocateCallback;
    handlesAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateHandlesAllocatorCallback>();
    const auto handlesAllocatorMemory =
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(frameAllocatorMemory) + frameAllocatorSize);
    HandlesAllocator = AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node>(AxrMemoryBlock{
        .Memory = handlesAllocatorMemory,
        .Size = handlesAllocatorSize,
        .Deallocator = handlesAllocatorDeallocateCallback,
    });

    // ---- Engine Data Allocator ----
    AxrDeallocateBlock engineDataAllocatorDeallocateCallback;
    engineDataAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateEngineDataAllocatorCallback>();
    const auto engineDataAllocatorMemory =
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(handlesAllocatorMemory) + handlesAllocatorSize);
    EngineDataAllocator = AxrDynamicAllocator(
        AxrMemoryBlock{
            .Memory = engineDataAllocatorMemory,
            .Size = engineDataAllocatorSize,
            .Deallocator = engineDataAllocatorDeallocateCallback,
        },
        &HandlesAllocator);

    m_IsSetup = true;
    return AXR_SUCCESS;
}

void AxrAllocator::shutDown() {
    FrameAllocator = {};
    HandlesAllocator = {};
    EngineDataAllocator = {};

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
               " Peak usage reached {:.2f}%."
#endif
               ,
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

void AxrAllocator::logHandlesAllocatorUsage(const char* message) const {
    const size_t size = HandlesAllocator.size();
    const size_t capacity = HandlesAllocator.chunkCapacity();
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    const size_t peakSize = HandlesAllocator.peakChunkCount();
#endif

    axrLogInfo("{}: Handles Allocator memory usage."
               " {:.2f}% Handles used currently. {} Handles used out of {}."
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
               " Peak handles usage reached {:.2f}%."
#endif
               ,
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
    const size_t size = EngineDataAllocator.size();
    const size_t capacity = EngineDataAllocator.capacity();
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    const size_t peakSize = EngineDataAllocator.peakSize();
#endif

    axrLogInfo("{}: Engine Data Allocator memory usage."
               " {:.2f}% Memory used currently. {} Bytes used out of {}."
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
               " Peak memory usage reached {:.2f}%."
#endif
               ,
               message,
               static_cast<float>(size) / static_cast<float>(capacity) * 100,
               size,
               capacity,
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
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

void AxrAllocator::deallocateHandlesAllocatorCallback(void*& memory) {
    // NEVER EVER modify anything about the HandlesAllocator within this function. We don't want to risk retriggering
    // this callback and entering an infinite loop. We don't really need to do anything here since we don't intend to
    // reuse its memory block and the entire block gets freed when the allocator is cleaned up.
    memory = nullptr;
}

void AxrAllocator::deallocateEngineDataAllocatorCallback(void*& memory) {
    // NEVER EVER modify anything about the EngineDataAllocator within this function. We don't want to risk retriggering
    // this callback and entering an infinite loop. We don't really need to do anything here since we don't intend to
    // reuse its memory block and the entire block gets freed when the allocator is cleaned up.
    memory = nullptr;
}
