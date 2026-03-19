#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/common/enums.h"
#include "dynamicAllocator.h"
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
        /// Size in bytes
        size_t EngineDataAllocatorMainMemorySize;
        /// The max number of dynamic allocator handles to allow for
        uint32_t MaxHandleCount;
    };

    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //

    /// Clears all data at the beginning of each frame
    AxrStackAllocator FrameAllocator{};
    /// Allocator for all dynamic allocator handles
    AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node> HandlesAllocator{};
    /// Allocator for any and all persistent engine related data
    // TODO (Ashe): Defragment this allocator every now and then
    AxrDynamicAllocator EngineDataAllocator{};

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

    /// Log the frame allocator's usage
    /// @param message Message to prefix log message with
    void logFrameAllocatorUsage(const char* message) const;
    /// Log the handles allocator's usage
    /// @param message Message to prefix log message with
    void logHandlesAllocatorUsage(const char* message) const;
    /// Log the engine data allocator's usage
    /// @param message Message to prefix log message with
    void logEngineDataAllocatorUsage(const char* message) const;

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    void* m_Memory{};
    size_t m_MemorySize{};
    bool m_IsSetup = false;

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Callback function for when the frame allocator gets deallocated
    /// @param memory Frame allocator memory block to deallocate
    static void deallocateFrameAllocatorCallback(void*& memory);
    /// Callback function for when the handles allocator gets deallocated
    /// @param memory Handles allocator memory block to deallocate
    static void deallocateHandlesAllocatorCallback(void*& memory);
    /// Callback function for when the engine data allocator gets deallocated
    /// @param memory Engine data allocator memory block to deallocate
    static void deallocateEngineDataAllocatorCallback(void*& memory);
};
