#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <cassert>
#include <cstddef>
#include <cstdint>

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

/// Shift the given address upwards as necessary to ensure it's aligned to the given number of `alignment` bytes
/// @param address Address to align
/// @param alignment The number of bytes to align to
/// @return The shifted address, aligned
inline uintptr_t axrAlignAddress(const uintptr_t address, const size_t alignment) {
    const size_t mask = alignment - 1;
    // Make sure alignment is a power of two (or 1)
    assert((alignment & mask) == 0);
    return (address + mask) & ~mask;
}

/// Align the given memory with an address that's ideal for the given alignment.
/// The given memory MUST contain extra space for the alignment, equal to the size of the alignment.
/// E.g. Memory meant for 64 bytes and an alignment of 16 bytes, must have a size of 64 + 16 = 80 bytes.
/// We need the extra space because we shift the memory upwards until it's aligned optimally and return that address.
/// @param memory Memory to align
/// @param alignment The number of bytes to align to
/// @return The aligned memory
inline void* axrAlignMemory(void* memory, const size_t alignment) {
    assert(memory != nullptr);

    auto memoryAligned = reinterpret_cast<uint8_t*>(axrAlignAddress(reinterpret_cast<uintptr_t>(memory), alignment));
    // If no alignment occurred, shift the memory up so we have space to store the shift.
    // We shift it by the full alignment instead of just 1 byte to so that the data stays aligned properly.
    if (memoryAligned == memory) {
        memoryAligned += alignment;
    }

    // Store the shift
    const ptrdiff_t shift = memoryAligned - static_cast<uint8_t*>(memory);
    assert(shift > 0 && shift <= 255);
    memoryAligned[-1] = static_cast<uint8_t>(shift & 0xFF);

    return memoryAligned;
}

/// Align the given memory with an address that's ideal for the given alignment.
/// The given memory MUST contain extra space for the alignment, equal to the size of the alignment.
/// E.g. Memory meant for 64 bytes and an alignment of 16 bytes, must have a size of 64 + 16 = 80 bytes.
/// We need the extra space because we shift the memory upwards until it's aligned optimally and return that address.
///
/// The alignment is automatically obtained through alignof(Type).
/// @tparam Type The data type
/// @return The aligned memory
template<typename Type>
Type* axrAlignMemory(Type* memory) {
    return static_cast<Type*>(axrAlignMemory(reinterpret_cast<void*>(memory), alignof(Type)));
}

/// Unalign the given memory
/// @param memory Memory that has previously been aligned
/// @return The original memory, unaligned
inline void* axrUnalignMemory(void* memory) {
    assert(memory != nullptr);

    auto* memoryAligned = static_cast<uint8_t*>(memory);
    const ptrdiff_t shift = memoryAligned[-1];
    return memoryAligned - shift;
}

/// Unalign the given memory
/// @tparam Type The data type
/// @param memory Memory that has previously been aligned
/// @return The original memory, unaligned
template<typename Type>
Type* axrUnalignMemory(Type* memory) {
    return static_cast<Type*>(axrUnalignMemory(reinterpret_cast<void*>(memory)));
}
