#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "stackAllocator.h"

/// Axr Main Allocator singleton
class AxrAllocator {
public:
    // ----------------------------------------- //
    // Public Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Copy Constructor
    /// @param src Source AxrAllocator to copy from
    AxrAllocator(const AxrAllocator& src) = delete;
    /// Move Constructor
    /// @param src Source AxrAllocator to move from
    AxrAllocator(AxrAllocator&& src) noexcept = delete;

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrAllocator to copy from
    AxrAllocator& operator=(const AxrAllocator& src) = delete;
    /// Move Assignment Operator
    /// @param src Source AxrAllocator to move from
    AxrAllocator& operator=(AxrAllocator&& src) noexcept = delete;

private:
    // ----------------------------------------- //
    // Private Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrAllocator();

    // ---- Destructor ----

    /// Destructor
    ~AxrAllocator();

public:
    // ----------------------------------------- //
    // Public Structs
    // ----------------------------------------- //

    /// AxrAllocator Config
    struct Config {
        /// Size in bytes
        size_t FrameAllocatorSize;
    };

    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    AxrStackAllocator FrameAllocator{};

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the AxrAllocator singleton
    /// @return A reference to the AxrAllocator singleton
    static AxrAllocator& get();

    /// Set up the allocator
    /// @param config Allocator config
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult setup(const Config& config);
    /// Shut down the allocator
    void shutDown();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    uint8_t* m_Memory{};
    size_t m_MemorySize{};
    bool m_IsSetup = false;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Callback function for when the frame allocator gets deallocated
    /// @param memory Frame allocator memory block to deallocate
    void deallocateFrameAllocatorCallback(void*& memory);
};
