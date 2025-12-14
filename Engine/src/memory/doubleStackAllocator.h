#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "types.h"

#include <cstdint>

/// Double-Ended Stack allocator
class AxrDoubleStackAllocator {
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
    /// @param memory A pointer to the block of memory this allocator has access to
    /// @param size The number of bytes the given block of memory has
    /// @param deallocate A function pointer to use when we're done with the given memory block and wish to deallocate
    /// it
    AxrDoubleStackAllocator(void* memory, size_t size, const AxrDeallocate& deallocate);
    /// Copy Constructor
    /// @param src Source AxrDoubleStackAllocator to copy from
    AxrDoubleStackAllocator(const AxrDoubleStackAllocator& src) = delete;
    /// Move Constructor
    /// @param src Source AxrDoubleStackAllocator to move from
    AxrDoubleStackAllocator(AxrDoubleStackAllocator&& src) noexcept;

    // ---- Destructor ----

    /// Destructor
    ~AxrDoubleStackAllocator();

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrDoubleStackAllocator to copy from
    AxrDoubleStackAllocator& operator=(const AxrDoubleStackAllocator& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrDoubleStackAllocator to move from
    AxrDoubleStackAllocator& operator=(AxrDoubleStackAllocator&& src) noexcept;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Allocate new memory to the stack on the lower end.
    /// MarkerIDs are NOT UNIQUE between upper and lower bounds so don't mix lower and upper markers.
    /// @param size Size in bytes for how much memory to allocate
    /// @param memory Output allocated memory
    /// @param markerID Output marker ID for this memory
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space on the stack for the requested memory.
    [[nodiscard]] AxrResult allocateLower(size_t size, void*& memory, MarkerID& markerID);
    /// Allocate new memory to the stack on the upper end.
    /// MarkerIDs are NOT UNIQUE between upper and lower bounds so don't mix lower and upper markers.
    /// @param size Size in bytes for how much memory to allocate
    /// @param memory Output allocated memory
    /// @param markerID Output marker ID for this memory
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space on the stack for the requested memory.
    [[nodiscard]] AxrResult allocateUpper(size_t size, void*& memory, MarkerID& markerID);
    /// Deallocate the memory for the given marker ID. Including all memory allocated after the given marker. On the
    /// lower end.
    /// MarkerIDs are NOT UNIQUE between upper and lower bounds so don't mix lower and upper markers.
    /// @param markerID Memory marker ID
    void deallocateLower(MarkerID markerID);
    /// Deallocate the memory for the given marker ID. Including all memory allocated after the given marker. On the
    /// upper end.
    /// MarkerIDs are NOT UNIQUE between upper and lower bounds so don't mix lower and upper markers.
    /// @param markerID Memory marker ID
    void deallocateUpper(MarkerID markerID);
    /// Clear the stack
    void clear();
    /// Clear the stack on the lower end
    void clearLower();
    /// Clear the stack on the upper end
    void clearUpper();

    /// Get the size of the marker
    /// @return The size of the marker
    [[nodiscard]] static uint32_t getMarkerSize();

    /// Get the allocator's capacity
    /// @return The allocator's capacity
    [[nodiscard]] size_t capacity() const;
    /// Get the size of the allocated memory
    /// @return The size of the allocated memory
    [[nodiscard]] size_t size() const;
    /// Get the size of the allocated memory on the lower end
    /// @return The size of the allocated memory on the lower end
    [[nodiscard]] size_t sizeLower() const;
    /// Get the size of the allocated memory on the upper end
    /// @return The size of the allocated memory on the upper end
    [[nodiscard]] size_t sizeUpper() const;
    /// Get the empty state of the allocator
    /// @return True if the allocator is empty
    [[nodiscard]] bool empty() const;
    /// Get the empty state of the allocator on the lower end
    /// @return True if the allocator on the lower end is empty
    [[nodiscard]] bool emptyLower() const;
    /// Get the empty state of the allocator on the upper end
    /// @return True if the allocator on the upper end is empty
    [[nodiscard]] bool emptyUpper() const;

private:
    // ----------------------------------------- //
    // Private Structs
    // ----------------------------------------- //

    /// Memory item marker. It gets inserted at the end of each memory block to identify how far back the next marker
    /// is. E.g.
    /// [lowerItem1][lowerMarker][lowerItem2][lowerMarker].....[upperMarker][upperItem2][upperMarker][upperItem1]
    struct Marker {
        /// Size of the item behind this marker.
        /// Does not include the size of this marker.
        size_t Size = 0;
        MarkerID ID = 0;
    };

    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrDeallocate m_MainMemoryDeallocator{};
    uint8_t* m_Memory{};
    size_t m_Capacity{};
    size_t m_SizeLower{};
    size_t m_SizeUpper{};

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup();

    /// Get the beginning of the allocated memory on the lower end
    /// @return A handle to the beginning of the allocated memory on the lower end
    [[nodiscard]] uint8_t* beginLower() const;
    /// Get the beginning of the allocated memory on the upper end
    /// @return A handle to the beginning of the allocated memory on the upper end
    [[nodiscard]] uint8_t* beginUpper() const;
    /// Get the end of the allocated memory on the lower end
    /// @return A handle to the end of the allocated memory on the lower end
    [[nodiscard]] uint8_t* endLower() const;
    /// Get the end of the allocated memory on the upper end
    /// @return A handle to the end of the allocated memory on the upper end
    [[nodiscard]] uint8_t* endUpper() const;

    /// Get the current marker on the lower end
    /// @return The current marker on the lower end
    [[nodiscard]] Marker getCurrentMarkerLower() const;
    /// Get the current marker on the upper end
    /// @return The current marker on the upper end
    [[nodiscard]] Marker getCurrentMarkerUpper() const;
    /// Set the data for the current marker on the lower end
    /// @param marker Marker to set as the current marker on the lower end
    void setCurrentMarkerLower(const Marker& marker) const;
    /// Set the data for the current marker on the upper end
    /// @param marker Marker to set as the current marker on the upper end
    void setCurrentMarkerUpper(const Marker& marker) const;

    /// Pop the top of the stack on the lower end
    void popLower();
    /// Pop the top of the stack on the upper end
    void popUpper();
};
