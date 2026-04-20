#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "subAllocatorBase.h"
#include "types.h"

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

    /// Default Constructor
    AxrStackAllocator();
    /// Constructor
    /// @param memoryBlock Memory block to use
    explicit AxrStackAllocator(const AxrMemoryBlock& memoryBlock);
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

    /// Allocate new memory block to the stack
    /// @param size Size in bytes for how much memory to allocate
    /// @param alignment Memory alignment
    /// @param memory Output allocated memory
    /// @param markerID Output marker ID for this memory
    /// @param zeroOutMemory If true, the allocated memory will be zeroed out
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space on the stack for the requested memory.
    [[nodiscard]] AxrResult allocateBlock(size_t size,
                                          uint8_t alignment,
                                          void*& memory,
                                          MarkerID& markerID,
                                          bool zeroOutMemory = false);

    /// Allocate new memory to the stack
    /// @tparam Type The memory data type
    /// @param size The number of data items of type `Type` to store in memory
    /// @param memory Output allocated memory
    /// @param markerID Output marker ID for this memory
    /// @param zeroOutMemory If true, the allocated memory will be zeroed out
    /// @return AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_OUT_OF_MEMORY if there isn't enough space on the stack for the requested memory.
    template<typename Type>
    [[nodiscard]] AxrResult allocate(const size_t size,
                                     Type*& memory,
                                     MarkerID& markerID,
                                     const bool zeroOutMemory = false) {
        return allocateBlock(sizeof(Type) * size,
                             alignof(Type),
                             reinterpret_cast<void*&>(memory),
                             markerID,
                             zeroOutMemory);
    }

    /// Deallocate the memory for the given marker ID. Including all memory allocated after the given marker.
    /// @param markerID Memory marker ID
    void deallocate(MarkerID markerID);
    /// Deallocate the memory for the given marker ID. ONLY if it's the most recent marker on the stack. Preventing
    /// extra data from being deleted.
    /// @param markerID Memory marker ID
    /// @return True if the data was deallocated successfully
    [[nodiscard]] bool deallocateIfLast(MarkerID markerID);
    /// Clear the stack
    void clear();

    /// Get the size of the allocated memory
    /// @return The size of the allocated memory
    [[nodiscard]] size_t size() const;
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    /// Get the peak usage size of the allocated memory
    /// @return The peak usage size of the allocated memory
    [[nodiscard]] size_t peakSize() const;
#endif
    /// Get the empty state of the allocator
    /// @return True if the allocator is empty
    [[nodiscard]] bool empty() const;
    /// Get the size of the marker
    /// @return The size of the marker
    [[nodiscard]] static constexpr uint32_t getMarkerSize() {
        return sizeof(Marker);
    }

protected:
    // ----------------------------------------- //
    // Protected Variables
    // ----------------------------------------- //
    size_t m_Size{};
#ifdef AXR_TRACK_ALLOCATOR_PEAK_USAGE
    size_t m_PeakSize{};
#endif

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
    
    /// Move the given AxrStackAllocator to this class
    /// @param src AxrStackAllocator to move
    /// @param useConstructor If true, this function will use the move constructor for non-primitive objects instead of
    /// the move assignment operator when moving variables
    void move_internal(AxrStackAllocator&& src, bool useConstructor);

    /// Get the beginning of the allocated memory
    /// @return The address to the beginning of the allocated memory
    [[nodiscard]] uintptr_t begin() const;
    /// Get the end of the allocated memory
    /// @return The address to the end of the allocated memory
    [[nodiscard]] uintptr_t end() const;

    /// Get the current marker
    /// @return The current marker
    [[nodiscard]] Marker getCurrentMarker() const;
    /// Set the data for the current marker
    /// @param marker Marker to set as the current marker
    void setCurrentMarker(const Marker& marker) const;

    /// Pop the top of the stack
    void pop();
};
