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
    const size_t handlesAllocatorSize = AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node>::getAllocatorSize(
        config.MaxHandleCount);
    const size_t engineDataAllocatorSize = config.EngineDataAllocatorMainMemorySize;
    const size_t debugHandlesAllocatorSize =
        AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node>::getAllocatorSize(config.MaxDebugHandleCount);
    [[maybe_unused]] const size_t debugInfoAllocatorSize = config.DebugInfoAllocatorMainMemorySize;

    m_MemorySize = frameAllocatorSize + handlesAllocatorSize + engineDataAllocatorSize;
#ifdef AXR_DEBUG_INFO_ENABLED
    m_MemorySize += debugHandlesAllocatorSize + debugInfoAllocatorSize;
#endif
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
    const auto handlesAllocatorMemory = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(frameAllocatorMemory) +
                                                                frameAllocatorSize);
    HandlesAllocator = AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node>(AxrMemoryBlock{
        .Memory = handlesAllocatorMemory,
        .Size = handlesAllocatorSize,
        .Deallocator = handlesAllocatorDeallocateCallback,
    });

    // ---- Engine Data Allocator ----
    AxrDeallocateBlock engineDataAllocatorDeallocateCallback;
    engineDataAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateEngineDataAllocatorCallback>();
    const auto engineDataAllocatorMemory = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(handlesAllocatorMemory) +
                                                                   handlesAllocatorSize);
    EngineDataAllocator = AxrDynamicAllocator(
        AxrMemoryBlock{
            .Memory = engineDataAllocatorMemory,
            .Size = engineDataAllocatorSize,
            .Deallocator = engineDataAllocatorDeallocateCallback,
        },
        &HandlesAllocator);

    // ---- Debug Handles Allocator ----
    const auto debugHandlesAllocatorMemory = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(engineDataAllocatorMemory) + engineDataAllocatorSize);
#ifdef AXR_DEBUG_INFO_ENABLED
    AxrDeallocateBlock debugHandlesAllocatorDeallocateCallback;
    debugHandlesAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateDebugHandlesAllocatorCallback>();
    DebugHandlesAllocator = AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node>(AxrMemoryBlock{
        .Memory = debugHandlesAllocatorMemory,
        .Size = debugHandlesAllocatorSize,
        .Deallocator = debugHandlesAllocatorDeallocateCallback,
    });
#endif

    // ---- Debug Info Allocator ----
    [[maybe_unused]] const auto debugInfoAllocatorMemory = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(debugHandlesAllocatorMemory) + debugHandlesAllocatorSize);
#ifdef AXR_DEBUG_INFO_ENABLED
    AxrDeallocateBlock debugInfoAllocatorDeallocateCallback;
    debugInfoAllocatorDeallocateCallback.connect<&AxrAllocator::deallocateDebugInfoAllocatorCallback>();
    DebugInfoAllocator = AxrDynamicAllocator(
        AxrMemoryBlock{
            .Memory = debugInfoAllocatorMemory,
            .Size = debugInfoAllocatorSize,
            .Deallocator = debugInfoAllocatorDeallocateCallback,
        },
        &DebugHandlesAllocator);
#endif

    m_IsSetup = true;
    return AXR_SUCCESS;
}

void AxrAllocator::shutDown() {
#ifdef AXR_DEBUG_INFO_ENABLED
    DebugInfoAllocator.~AxrDynamicAllocator();
    DebugHandlesAllocator.~AxrPoolAllocator();
#endif
    EngineDataAllocator.~AxrDynamicAllocator();
    HandlesAllocator.~AxrPoolAllocator();
    FrameAllocator.~AxrStackAllocator();

    if (m_Memory != nullptr) {
        free(m_Memory);
        m_Memory = nullptr;
    }
    m_MemorySize = {};

    m_IsSetup = false;
}
void AxrAllocator::logAllAllocatorUsage(const char* message) const {
    axrLogDebug("----------------------------------------------------------------");
    logFrameAllocatorUsage(message);
    logHandlesAllocatorUsage(message);
    logEngineDataAllocatorUsage(message);
    logDebugHandlesAllocatorUsage(message);
    logDebugInfoAllocatorUsage(message);
    axrLogDebug("----------------------------------------------------------------");
}

void AxrAllocator::logFrameAllocatorUsage(const char* message) const {
    const size_t size = FrameAllocator.size();
    const size_t capacity = FrameAllocator.capacity();
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    const size_t peakSize = FrameAllocator.peakSize();
#endif

    axrLogDebug("{}: Frame Allocator memory usage. {:.2f}% Used currently. {} Bytes used out of {}."
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

    axrLogDebug("{}: Handles Allocator memory usage."
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

    axrLogDebug("{}: Engine Data Allocator memory usage."
                " {:.2f}% Memory used currently. {} Bytes used out of {}."
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
                " Peak memory usage reached {:.2f}%."
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

void AxrAllocator::logDebugHandlesAllocatorUsage(const char* message) const {
#ifdef AXR_DEBUG_INFO_ENABLED
    const size_t size = DebugHandlesAllocator.size();
    const size_t capacity = DebugHandlesAllocator.chunkCapacity();
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    const size_t peakSize = DebugHandlesAllocator.peakChunkCount();
#endif

    axrLogDebug("{}: Debug Handles Allocator memory usage."
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
#endif
}

void AxrAllocator::logDebugInfoAllocatorUsage(const char* message) const {
#ifdef AXR_DEBUG_INFO_ENABLED
    const size_t size = DebugInfoAllocator.size();
    const size_t capacity = DebugInfoAllocator.capacity();
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    const size_t peakSize = DebugInfoAllocator.peakSize();
#endif

    axrLogDebug("{}: Debug info Allocator memory usage."
                " {:.2f}% Memory used currently. {} Bytes used out of {}."
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
                " Peak memory usage reached {:.2f}%."
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
#endif
}

// ----------------------------------------- //
// Private Functions
// ----------------------------------------- //

void AxrAllocator::deallocateFrameAllocatorCallback(void*& memory) {
    memory = nullptr;
}

void AxrAllocator::deallocateHandlesAllocatorCallback(void*& memory) {
    memory = nullptr;
}

void AxrAllocator::deallocateEngineDataAllocatorCallback(void*& memory) {
    memory = nullptr;
}

void AxrAllocator::deallocateDebugHandlesAllocatorCallback(void*& memory) {
    memory = nullptr;
}

void AxrAllocator::deallocateDebugInfoAllocatorCallback(void*& memory) {
    memory = nullptr;
}
