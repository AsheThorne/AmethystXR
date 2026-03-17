// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "subAllocatorBase.h"
#include "axr/logging.h"

// ----------------------------------------- //
// Special Functions
// ----------------------------------------- //

AxrSubAllocatorBase::AxrSubAllocatorBase() = default;

AxrSubAllocatorBase::AxrSubAllocatorBase(const AxrMemoryBlock& memoryBlock) {
    m_Deallocator = memoryBlock.Deallocator;
    m_Memory = memoryBlock.Memory;
    m_Capacity = memoryBlock.Size;
}

AxrSubAllocatorBase::AxrSubAllocatorBase(AxrSubAllocatorBase&& src) noexcept {
    m_Deallocator = src.m_Deallocator;
    m_Memory = src.m_Memory;
    m_Capacity = src.m_Capacity;

    src.m_Deallocator = {};
    src.m_Memory = {};
    src.m_Capacity = {};
}

AxrSubAllocatorBase::~AxrSubAllocatorBase() {
    cleanup();
}

AxrSubAllocatorBase& AxrSubAllocatorBase::operator=(AxrSubAllocatorBase&& src) noexcept {
    if (this != &src) {
        cleanup();

        m_Deallocator = src.m_Deallocator;
        m_Memory = src.m_Memory;
        m_Capacity = src.m_Capacity;

        src.m_Deallocator = {};
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
        if (m_Deallocator) {
            m_Deallocator(reinterpret_cast<void*&>(m_Memory));
        } else {
            axrLogWarning("Memory leak detected inside AxrSubAllocatorBase. Failed to deallocate a block of memory. No "
                          "deallocator available.");
        }
    }
    m_Capacity = {};
    m_Deallocator.reset();
}
