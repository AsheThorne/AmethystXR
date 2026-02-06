// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/logging.h"
#include "subAllocatorBase.h"

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrSubAllocatorBase::AxrSubAllocatorBase() = default;

AxrSubAllocatorBase::AxrSubAllocatorBase(void* memory, const size_t size, const AxrDeallocateBlock& deallocator) {
    m_MainMemoryDeallocator = deallocator;
    m_Memory = static_cast<uint8_t*>(memory);
    m_Capacity = size;
}

AxrSubAllocatorBase::AxrSubAllocatorBase(AxrSubAllocatorBase&& src) noexcept {
    m_MainMemoryDeallocator = src.m_MainMemoryDeallocator;
    m_Memory = src.m_Memory;
    m_Capacity = src.m_Capacity;

    src.m_MainMemoryDeallocator = {};
    src.m_Memory = {};
    src.m_Capacity = {};
}

AxrSubAllocatorBase::~AxrSubAllocatorBase() {
    cleanup();
}

AxrSubAllocatorBase& AxrSubAllocatorBase::operator=(AxrSubAllocatorBase&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_MainMemoryDeallocator = src.m_MainMemoryDeallocator;
        m_Memory = src.m_Memory;
        m_Capacity = src.m_Capacity;

        src.m_MainMemoryDeallocator = {};
        src.m_Memory = {};
        src.m_Capacity = {};
    }
    return *this;
}

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

size_t AxrSubAllocatorBase::capacity() const {
    return m_Capacity;
}

// ----------------------------------------- //
// Protected Functions
// ----------------------------------------- //

void AxrSubAllocatorBase::cleanup() {
    if (m_Memory != nullptr) {
        if (m_MainMemoryDeallocator) {
            m_MainMemoryDeallocator(reinterpret_cast<void*&>(m_Memory));
        } else {
            axrLogWarning("Memory leak detected inside AxrSubAllocatorBase. Failed to deallocate a block of memory. No "
                          "deallocator available.");
        }
    }
    m_Capacity = {};
    m_MainMemoryDeallocator.reset();
}
