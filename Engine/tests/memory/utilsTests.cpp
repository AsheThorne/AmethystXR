// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "memory/utils.h"

// ----------------------------------------- //
// Align Address Tests
// ----------------------------------------- //

TEST(MemoryUtils_AlignAddress, Align1Byte) {
    ASSERT_TRUE(axrAlignAddress(0x00000000, 1) == 0x00000000);
    ASSERT_TRUE(axrAlignAddress(0x00000001, 1) == 0x00000001);
    ASSERT_TRUE(axrAlignAddress(0x00000002, 1) == 0x00000002);
    ASSERT_TRUE(axrAlignAddress(0x00000003, 1) == 0x00000003);
    ASSERT_TRUE(axrAlignAddress(0x00000004, 1) == 0x00000004);
    ASSERT_TRUE(axrAlignAddress(0x00000005, 1) == 0x00000005);
    ASSERT_TRUE(axrAlignAddress(0x00000006, 1) == 0x00000006);
    ASSERT_TRUE(axrAlignAddress(0x00000007, 1) == 0x00000007);
    ASSERT_TRUE(axrAlignAddress(0x00000008, 1) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000009, 1) == 0x00000009);
    ASSERT_TRUE(axrAlignAddress(0x0000000A, 1) == 0x0000000A);
    ASSERT_TRUE(axrAlignAddress(0x0000000B, 1) == 0x0000000B);
    ASSERT_TRUE(axrAlignAddress(0x0000000C, 1) == 0x0000000C);
    ASSERT_TRUE(axrAlignAddress(0x0000000D, 1) == 0x0000000D);
    ASSERT_TRUE(axrAlignAddress(0x0000000E, 1) == 0x0000000E);
    ASSERT_TRUE(axrAlignAddress(0x0000000F, 1) == 0x0000000F);
    ASSERT_TRUE(axrAlignAddress(0x00000010, 1) == 0x00000010);
}

TEST(MemoryUtils_AlignAddress, Align2Bytes) {
    ASSERT_TRUE(axrAlignAddress(0x00000000, 2) == 0x00000000);
    ASSERT_TRUE(axrAlignAddress(0x00000001, 2) == 0x00000002);
    ASSERT_TRUE(axrAlignAddress(0x00000002, 2) == 0x00000002);
    ASSERT_TRUE(axrAlignAddress(0x00000003, 2) == 0x00000004);
    ASSERT_TRUE(axrAlignAddress(0x00000004, 2) == 0x00000004);
    ASSERT_TRUE(axrAlignAddress(0x00000005, 2) == 0x00000006);
    ASSERT_TRUE(axrAlignAddress(0x00000006, 2) == 0x00000006);
    ASSERT_TRUE(axrAlignAddress(0x00000007, 2) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000008, 2) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000009, 2) == 0x0000000A);
    ASSERT_TRUE(axrAlignAddress(0x0000000A, 2) == 0x0000000A);
    ASSERT_TRUE(axrAlignAddress(0x0000000B, 2) == 0x0000000C);
    ASSERT_TRUE(axrAlignAddress(0x0000000C, 2) == 0x0000000C);
    ASSERT_TRUE(axrAlignAddress(0x0000000D, 2) == 0x0000000E);
    ASSERT_TRUE(axrAlignAddress(0x0000000E, 2) == 0x0000000E);
    ASSERT_TRUE(axrAlignAddress(0x0000000F, 2) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000010, 2) == 0x00000010);
}

TEST(MemoryUtils_AlignAddress, Align4Bytes) {
    ASSERT_TRUE(axrAlignAddress(0x00000000, 4) == 0x00000000);
    ASSERT_TRUE(axrAlignAddress(0x00000001, 4) == 0x00000004);
    ASSERT_TRUE(axrAlignAddress(0x00000002, 4) == 0x00000004);
    ASSERT_TRUE(axrAlignAddress(0x00000003, 4) == 0x00000004);
    ASSERT_TRUE(axrAlignAddress(0x00000004, 4) == 0x00000004);
    ASSERT_TRUE(axrAlignAddress(0x00000005, 4) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000006, 4) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000007, 4) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000008, 4) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000009, 4) == 0x0000000C);
    ASSERT_TRUE(axrAlignAddress(0x0000000A, 4) == 0x0000000C);
    ASSERT_TRUE(axrAlignAddress(0x0000000B, 4) == 0x0000000C);
    ASSERT_TRUE(axrAlignAddress(0x0000000C, 4) == 0x0000000C);
    ASSERT_TRUE(axrAlignAddress(0x0000000D, 4) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000E, 4) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000F, 4) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000010, 4) == 0x00000010);
}

TEST(MemoryUtils_AlignAddress, Align8Bytes) {
    ASSERT_TRUE(axrAlignAddress(0x00000000, 8) == 0x00000000);
    ASSERT_TRUE(axrAlignAddress(0x00000001, 8) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000002, 8) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000003, 8) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000004, 8) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000005, 8) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000006, 8) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000007, 8) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000008, 8) == 0x00000008);
    ASSERT_TRUE(axrAlignAddress(0x00000009, 8) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000A, 8) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000B, 8) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000C, 8) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000D, 8) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000E, 8) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000F, 8) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000010, 8) == 0x00000010);
}

TEST(MemoryUtils_AlignAddress, Align16Bytes) {
    ASSERT_TRUE(axrAlignAddress(0x00000000, 16) == 0x00000000);
    ASSERT_TRUE(axrAlignAddress(0x00000001, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000002, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000003, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000004, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000005, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000006, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000007, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000008, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000009, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000A, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000B, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000C, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000D, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000E, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x0000000F, 16) == 0x00000010);
    ASSERT_TRUE(axrAlignAddress(0x00000010, 16) == 0x00000010);
}

// ----------------------------------------- //
// Align Memory Tests
// ----------------------------------------- //

template<typename Type>
static void alignMemory_CheckShiftValue_Test() {
    Type* memory = static_cast<Type*>(malloc(sizeof(Type) + alignof(Type)));
    Type* memoryAligned = axrAlignMemory(memory);
    ASSERT_TRUE(memoryAligned > memory);

    const uint8_t shiftValue = reinterpret_cast<uint8_t*>(memoryAligned)[-1];

    // The shift can never be more than the alignment of the given type
    ASSERT_TRUE(shiftValue != 0 && shiftValue <= alignof(Type));
    free(memory);
}

template<typename Type>
static void alignMemory_CheckAddress_Test(const uint8_t addressMask) {
    Type* memory = static_cast<Type*>(malloc(sizeof(Type) + alignof(Type)));
    Type* memoryAligned = axrAlignMemory(memory);

    // Check if the address is a multiple of the given addressMask
    const auto memoryAddress = reinterpret_cast<uintptr_t>(memoryAligned);
    ASSERT_TRUE(memoryAddress % addressMask == 0);

    free(memory);
}

TEST(MemoryUtils_AlignMemory, CheckShiftValue1Byte) {
    struct alignas(1) TestData {
        uint8_t value[1];
    };
    alignMemory_CheckShiftValue_Test<TestData>();
}

TEST(MemoryUtils_AlignMemory, CheckShiftValue2Bytes) {
    struct alignas(2) TestData {
        uint8_t value[2];
    };
    alignMemory_CheckShiftValue_Test<TestData>();
}

TEST(MemoryUtils_AlignMemory, CheckShiftValue4Bytes) {
    struct alignas(4) TestData {
        uint8_t value[4];
    };
    alignMemory_CheckShiftValue_Test<TestData>();
}

TEST(MemoryUtils_AlignMemory, CheckShiftValue8Bytes) {
    struct alignas(8) TestData {
        uint8_t value[8];
    };
    alignMemory_CheckShiftValue_Test<TestData>();
}

TEST(MemoryUtils_AlignMemory, CheckShiftValue16Bytes) {
    struct alignas(16) TestData {
        uint8_t value[16];
    };
    alignMemory_CheckShiftValue_Test<TestData>();
}

TEST(MemoryUtils_AlignMemory, CheckAddress1Byte) {
    struct alignas(1) TestData {
        uint8_t value[1];
    };
    alignMemory_CheckAddress_Test<TestData>(0x1);
}

TEST(MemoryUtils_AlignMemory, CheckAddress2Bytes) {
    struct alignas(2) TestData {
        uint8_t value[2];
    };
    alignMemory_CheckAddress_Test<TestData>(0x2);
}

TEST(MemoryUtils_AlignMemory, CheckAddress4Bytes) {
    struct alignas(4) TestData {
        uint8_t value[4];
    };
    alignMemory_CheckAddress_Test<TestData>(0x4);
}

TEST(MemoryUtils_AlignMemory, CheckAddress8Bytes) {
    struct alignas(8) TestData {
        uint8_t value[8];
    };
    alignMemory_CheckAddress_Test<TestData>(0x8);
}

TEST(MemoryUtils_AlignMemory, CheckAddress16Bytes) {
    struct alignas(16) TestData {
        uint8_t value[16];
    };
    alignMemory_CheckAddress_Test<TestData>(0x10);
}

// ----------------------------------------- //
// Unalign Memory Tests
// ----------------------------------------- //

template<typename Type>
static void unalignMemory_Test() {
    Type* memory = static_cast<Type*>(malloc(sizeof(Type) + alignof(Type)));
    Type* memoryAligned = axrAlignMemory(memory);
    Type* memoryUnaligned = axrUnalignMemory(memoryAligned);
    ASSERT_TRUE(memory == memoryUnaligned);
    free(memory);
}

TEST(MemoryUtils_UnalignMemory, Unaligned1Byte) {
    struct alignas(1) TestData {
        uint8_t value[1];
    };
    unalignMemory_Test<TestData>();
}

TEST(MemoryUtils_UnalignMemory, Unaligned2Byte) {
    struct alignas(2) TestData {
        uint8_t value[2];
    };
    unalignMemory_Test<TestData>();
}

TEST(MemoryUtils_UnalignMemory, Unaligned4Byte) {
    struct alignas(4) TestData {
        uint8_t value[4];
    };
    unalignMemory_Test<TestData>();
}

TEST(MemoryUtils_UnalignMemory, Unaligned8Byte) {
    struct alignas(8) TestData {
        uint8_t value[8];
    };
    unalignMemory_Test<TestData>();
}

TEST(MemoryUtils_UnalignMemory, Unaligned16Byte) {
    struct alignas(16) TestData {
        uint8_t value[16];
    };
    unalignMemory_Test<TestData>();
}
