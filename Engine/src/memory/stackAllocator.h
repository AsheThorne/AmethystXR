#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "types.h"

#include <cstdint>

/// Stack allocator
class AxrStackAllocator {
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

    /// Constructor
    /// @param size The number of bytes the given block of memory has
    /// @param memory A pointer to the block of memory this allocator has access to
    /// @param deallocate A function pointer to use when we're done with the given memory block and wish to deallocate it
    explicit AxrStackAllocator(std::size_t size, void* memory, const AxrDeallocate& deallocate);
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
    [[nodiscard]] AxrResult allocate(std::size_t size, void*& memory, MarkerID& markerID);
    /// Deallocate the memory for the given marker ID. Including all memory allocated after the given marker.
    /// @param markerID Memory marker ID
    void deallocate(MarkerID markerID);
    /// Clear the stack
    void clear();

    /// Get the size of the marker
    /// @return The size of the marker
    [[nodiscard]] static uint32_t getMarkerSize();

    /// Get the allocator's capacity
    /// @return The allocator's capacity
    [[nodiscard]] std::size_t capacity() const;
    /// Get the size of the allocated memory
    /// @return The size of the allocated memory
    [[nodiscard]] std::size_t size() const;
    /// Get the empty state of the allocator
    /// @return True if the allocator is empty
    [[nodiscard]] bool empty() const;

private:
    // ----------------------------------------- //
    // Private Structs
    // ----------------------------------------- //

    /// Memory item marker. It gets inserted at the end of each memory block to identify how far back the next marker
    /// is. E.g. [Item1][Marker][Item2][Marker]...
    struct Marker {
        /// Size of the item behind this marker.
        /// Does not include the size of this marker.
        std::size_t Size = 0;
        MarkerID ID = 0;
    };

    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrDeallocate m_MainMemoryDeallocator;
    uint8_t* m_Memory = nullptr;
    std::size_t m_Capacity = 0;
    std::size_t m_Size = 0;

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
