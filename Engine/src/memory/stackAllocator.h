#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/defines.h"
#include "axr/common/enums.h"
#include "subAllocatorBase.h"
#include "types.h"
#include "utils.h"

#include <cstdint>

/// Stack allocator
class AxrStackAllocator : public AxrSubAllocatorBase {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// Marker ID type
    using MarkerID = uint32_t;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    AxrStackAllocator();
    /// Constructor
    /// @param memory A pointer to the block of memory this allocator has access to
    /// @param size The number of bytes the given block of memory has
    /// @param deallocator A function pointer to use when we're done with the given memory block and wish to deallocate
    /// it
    AxrStackAllocator(void* memory, size_t size, const AxrDeallocateBlock& deallocator);
    /// Copy Constructor
    /// @param src Source AxrStackAllocator to copy from
    AxrStackAllocator(const AxrStackAllocator& src) = delete;
    /// Move Constructor
    /// @param src Source AxrStackAllocator to move from
    AxrStackAllocator(AxrStackAllocator&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrStackAllocator();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrStackAllocator to copy from
    AxrStackAllocator& operator=(const AxrStackAllocator& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrStackAllocator to move from
    AxrStackAllocator& operator=(AxrStackAllocator&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Allocate new memory to the stack
    /// @param size Size in bytes for how much memory to allocate
    /// @param memory Output allocated memory
    /// @param markerID Output marker ID for this memory
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space on the stack for the requested memory.
    [[nodiscard]] AxrResult allocate(size_t size, void*& memory, MarkerID& markerID);

    /// Allocate new memory to the stack
    /// @tparam Type The memory data type
    /// @param memory Output allocated memory
    /// @param markerID Output marker ID for this memory
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space on the stack for the requested memory.
    template<typename Type>
    [[nodiscard]] AxrResult allocate(Type*& memory, MarkerID& markerID) {
        return allocate(sizeof(Type), reinterpret_cast<void*&>(memory), markerID);
    }

    /// Allocate new memory to the stack with optimal alignment
    /// @tparam Type The memory data type
    /// @param memory Output allocated memory
    /// @param markerID Output marker ID for this memory
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space on the stack for the requested memory.
    template<typename Type>
    [[nodiscard]] AxrResult allocateAligned(Type*& memory, MarkerID& markerID) {
        const AxrResult axrResult = allocate(sizeof(Type) + alignof(Type), reinterpret_cast<void*&>(memory), markerID);
        if (AXR_FAILED(axrResult)) {
            if (axrResult == AXR_ERROR_OUT_OF_MEMORY) {
                return AXR_ERROR_OUT_OF_MEMORY;
            }
            return AXR_ERROR_FALLTHROUGH;
        }

        memory = axrAlignMemory(memory);
        return axrResult;
    }

    /// Deallocate the memory for the given marker ID. Including all memory allocated after the given marker.
    /// @param markerID Memory marker ID
    void deallocate(MarkerID markerID);
    /// Clear the stack
    void clear();

    /// Get the size of the allocated memory
    /// @return The size of the allocated memory
    [[nodiscard]] size_t size() const;
    /// Get the empty state of the allocator
    /// @return True if the allocator is empty
    [[nodiscard]] bool empty() const;
    /// Get the size of the marker
    /// @return The size of the marker
    [[nodiscard]] static uint32_t getMarkerSize();

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    size_t m_Size{};

private:
    // ----------------------------------------- //
    // Private Structs
    // ----------------------------------------- //

    /// Memory item marker. It gets inserted at the end of each memory block to identify how far back the next marker
    /// is. E.g. [Item1][Marker][Item2][Marker]...
    struct Marker {
        /// Size of the item behind this marker.
        /// Does not include the size of this marker.
        size_t Size{};
        MarkerID ID{};
    };

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    /// Get the beginning of the allocated memory
    /// @return A handle to the beginning of the allocated memory
    [[nodiscard]] uint8_t* begin() const;
    /// Get the end of the allocated memory
    /// @return A handle to the end of the allocated memory
    [[nodiscard]] uint8_t* end() const;

    /// Get the current marker
    /// @return The current marker
    [[nodiscard]] Marker getCurrentMarker() const;
    /// Set the data for the current marker
    /// @param marker Marker to set as the current marker
    void setCurrentMarker(const Marker& marker) const;

    /// Pop the top of the stack
    void pop();
};
