// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "subAllocator.h"
#include "axr/logging.h"

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrSubAllocator::AxrSubAllocator() = default;

AxrSubAllocator::AxrSubAllocator(void* memory, const size_t size, const AxrDeallocateBlock& deallocator) {
    m_MainMemoryDeallocator = deallocator;
    m_Memory = static_cast<uint8_t*>(memory);
    m_Capacity = size;
}

AxrSubAllocator::AxrSubAllocator(AxrSubAllocator&& src) noexcept {
    m_MainMemoryDeallocator = src.m_MainMemoryDeallocator;
    m_Memory = src.m_Memory;
    m_Capacity = src.m_Capacity;

    src.m_MainMemoryDeallocator = {};
    src.m_Memory = {};
    src.m_Capacity = {};
}

AxrSubAllocator::~AxrSubAllocator() {
    cleanup();
}

AxrSubAllocator& AxrSubAllocator::operator=(AxrSubAllocator&& src) noexcept {
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

size_t AxrSubAllocator::capacity() const {
    return m_Capacity;
}

// ----------------------------------------- //
// Protected Functions
// ----------------------------------------- //

void AxrSubAllocator::cleanup() {
    if (m_Memory != nullptr) {
        if (m_MainMemoryDeallocator) {
            m_MainMemoryDeallocator(reinterpret_cast<void*&>(m_Memory));
        } else {
            axrLogWarning("Memory leak detected inside AxrSubAllocator. Failed to deallocate a block of memory. No "
                          "deallocator available.");
        }
    }
    m_Capacity = {};
    m_MainMemoryDeallocator.reset();
}
