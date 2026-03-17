// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "axr/common/defines.h"
#include "memory/dynamicAllocator.h"

// ----------------------------------------- //
// Shared Structs
// ----------------------------------------- //

namespace {
    struct TestData_Small {
        uint32_t ID{};
        uint32_t Data[7]{};

        bool operator==(const TestData_Small& src) const {
            return ID == src.ID && std::equal(std::begin(Data), std::end(Data), std::begin(src.Data));
        }
    };
} // namespace

namespace {
    struct TestData_Large {
        uint32_t ID{};
        uint32_t Data[15]{};

        bool operator==(const TestData_Large& src) const {
            return ID == src.ID && std::equal(std::begin(Data), std::end(Data), std::begin(src.Data));
        }
    };
} // namespace

// ----------------------------------------- //
// Shared Functions
// ----------------------------------------- //

static void deallocateCallback(void*& memory) {
    free(memory);
    memory = nullptr;
};

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(DynamicAllocator, DeallocatorCallback) {
    bool wasDeallocated = false;
    {
        auto deallocateCallback = [](bool* wasDeallocated, void*& memory) -> void {
            free(memory);
            memory = nullptr;
            *wasDeallocated = true;
        };

        AxrDeallocateBlock callback;
        callback.connect<deallocateCallback>(&wasDeallocated);

        constexpr uint32_t maxHandleCount = 32;
        const size_t allocatorSize = 128 + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
        void* memory = malloc(allocatorSize);
        AxrDynamicAllocator allocator(
            AxrMemoryBlock{
                .Memory = memory,
                .Size = allocatorSize,
                .Deallocator = callback,
            },
            maxHandleCount);
    }
    ASSERT_TRUE(wasDeallocated);
}

TEST(DynamicAllocator, Allocate_One) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = sizeof(TestData_Small) + alignof(TestData_Small) +
                                 sizeof(AxrDynamicAllocator::DataHeader) +
                                 AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestDataHandle{};
    const AxrResult axrResult = allocator.allocate(1, outTestDataHandle, true);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestDataHandle != nullptr);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestDataHandle == TestData_Small{});

    constexpr TestData_Small testData{
        .ID = 5,
        .Data = {1, 2, 3, 4, 5, 6, 7},
    };
    *outTestDataHandle = testData;

    ASSERT_TRUE(*outTestDataHandle == testData);
}

TEST(DynamicAllocator, Allocate_Two) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();
    constexpr uint32_t maxHandleCount = 2;
    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData2MemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + sizeof(AxrDynamicAllocator::DataHeader);
    const size_t allocatorSize =
        testData1MemSize + testData2MemSize + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestData1Handle{};
    AxrHandle<TestData_Large> outTestData2Handle{};
    AxrResult axrResult = allocator.allocate(1, outTestData1Handle, true);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocate(1, outTestData2Handle, true);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestData1Handle == TestData_Small{});
    ASSERT_TRUE(*outTestData2Handle == TestData_Large{});

    constexpr auto exampleTestData1 = TestData_Small{.ID = 5, .Data = {1, 2, 3, 4, 5, 6, 7}};
    constexpr auto exampleTestData2 =
        TestData_Large{.ID = 9, .Data = {23, 32, 61, 12, 89, 14, 63, 323, 65, 344, 13, 87, 12, 34, 91}};
    *outTestData1Handle = exampleTestData1;
    *outTestData2Handle = exampleTestData2;

    // Check that there are no overlaps in memory by assigning data and checking it
    ASSERT_TRUE(*outTestData1Handle == exampleTestData1);
    ASSERT_TRUE(*outTestData2Handle == exampleTestData2);
}

TEST(DynamicAllocator, Allocate_TooMuch) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    // Make sure there's enough space for 2 handles, just not enough for 2 blocks of memory
    constexpr uint32_t maxHandleCount = 2;
    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    const size_t allocatorSize = testData1MemSize + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestData1Handle{};
    AxrResult axrResult = allocator.allocate(1, outTestData1Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    AxrHandle<TestData_Small> outTestData2Handle{};
    axrResult = allocator.allocate(1, outTestData2Handle);
    ASSERT_TRUE(axrResult == AXR_ERROR_OUT_OF_MEMORY);

    ASSERT_TRUE(allocator.mainMemorySize() == testData1MemSize);
}

TEST(DynamicAllocator, Allocate_FailedToCreateHandle) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    // * 2 to make sure there is enough space for another block to be allocated, there's just not enough handles.
    const size_t allocatorSize =
        (testData1MemSize * 2) + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestData1Handle{};
    AxrResult axrResult = allocator.allocate(1, outTestData1Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    AxrHandle<TestData_Small> outTestData2Handle{};
    axrResult = allocator.allocate(1, outTestData2Handle);
    ASSERT_TRUE(axrResult == AXR_ERROR_OUT_OF_MEMORY);

    ASSERT_TRUE(allocator.mainMemorySize() == testData1MemSize);
}

TEST(DynamicAllocator, AllocateTwo_DeallocateOne) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 2;
    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData2MemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t allocatorMainMemSize = testData1MemSize + testData2MemSize;
    const size_t allocatorSize =
        allocatorMainMemSize + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestData1Handle{};
    AxrHandle<TestData_Large> outTestData2Handle{};
    AxrResult axrResult = allocator.allocate(1, outTestData1Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocate(1, outTestData2Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full first.
    ASSERT_TRUE(allocator.mainMemorySize() == allocatorMainMemSize);
    allocator.deallocate(outTestData2Handle);
    // Check that the allocator now only holds data item 1
    ASSERT_TRUE(allocator.mainMemorySize() == testData1MemSize);
}

TEST(DynamicAllocator, AutoDeallocate) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 2;
    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData2MemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t allocatorMainMemSize = testData1MemSize + testData2MemSize;
    const size_t allocatorSize =
        allocatorMainMemSize + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestData1Handle{};
    AxrResult axrResult = allocator.allocate(1, outTestData1Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    {
        AxrHandle<TestData_Large> outTestData2Handle{};
        axrResult = allocator.allocate(1, outTestData2Handle);
        ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

        // Check allocator is full first.
        ASSERT_TRUE(allocator.mainMemorySize() == allocatorMainMemSize);
    }

    // Check that the allocator now only holds data item 1
    ASSERT_TRUE(allocator.mainMemorySize() == testData1MemSize);
}

TEST(DynamicAllocator, Deallocate_CausingFragmentation) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 3;
    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData2MemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData3MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t allocatorMainMemSize = testData1MemSize + testData2MemSize + testData3MemSize;
    const size_t allocatorSize =
        allocatorMainMemSize + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestData1Handle{};
    AxrResult axrResult = allocator.allocate(1, outTestData1Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData1{
        .ID = 1,
        .Data = {9, 1, 5, 6, 123, 867, 2},
    };
    *outTestData1Handle = exampleTestData1;

    AxrHandle<TestData_Large> outTestData2Handle{};
    axrResult = allocator.allocate(1, outTestData2Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Large exampleTestData2{
        .ID = 76,
        .Data = {7, 12, 90, 51, 23, 65, 1, 1, 1, 1, 6, 6, 6, 4, 8},
    };
    *outTestData2Handle = exampleTestData2;

    AxrHandle<TestData_Small> outTestData3Handle{};
    axrResult = allocator.allocate(1, outTestData3Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData3{
        .ID = 74,
        .Data = {7, 54, 15, 97, 12, 867, 35},
    };
    *outTestData3Handle = exampleTestData3;

    ASSERT_TRUE(*outTestData1Handle == exampleTestData1);
    ASSERT_TRUE(*outTestData2Handle == exampleTestData2);
    ASSERT_TRUE(*outTestData3Handle == exampleTestData3);

    allocator.deallocate(outTestData2Handle);

    ASSERT_TRUE(*outTestData1Handle == exampleTestData1);
    ASSERT_TRUE(outTestData2Handle == nullptr);
    ASSERT_TRUE(*outTestData3Handle == exampleTestData3);

    ASSERT_TRUE(allocator.mainMemorySize() == allocatorMainMemSize - testData2MemSize);
}

// Test allocating several blocks [ 1-2-3-4-5 ], then deallocating blocks [ 2-3-4 ], so there's a gap of a few
// blocks. Then allocate a single large block that takes up most of the free space created be [ 2-3-4 ]. Also make
// sure there isn't room at the end (after 5) for the new block. We're testing that blocks [ 2-3-4 ] join up to make
// one big free space
TEST(DynamicAllocator, Deallocate_CausingFragmentationAndMerging_ThenAllocate) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 5;
    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData2MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData3MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData4MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData5MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t allocatorMainMemSize =
        testData1MemSize + testData2MemSize + testData3MemSize + testData4MemSize + testData5MemSize;
    const size_t allocatorSize =
        allocatorMainMemSize + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestData1Handle{};
    AxrResult axrResult = allocator.allocate(1, outTestData1Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData1{
        .ID = 1,
        .Data = {9, 1, 5, 6, 123, 867, 2},
    };
    *outTestData1Handle = exampleTestData1;

    AxrHandle<TestData_Small> outTestData2Handle{};
    axrResult = allocator.allocate(1, outTestData2Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData2{
        .ID = 76,
        .Data = {7, 12, 90, 51, 23, 65, 1},
    };
    *outTestData2Handle = exampleTestData2;

    AxrHandle<TestData_Small> outTestData3Handle{};
    axrResult = allocator.allocate(1, outTestData3Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData3{
        .ID = 74,
        .Data = {7, 54, 15, 97, 12, 867, 35},
    };
    *outTestData3Handle = exampleTestData3;

    AxrHandle<TestData_Small> outTestData4Handle{};
    axrResult = allocator.allocate(1, outTestData4Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData4{
        .ID = 9,
        .Data = {1, 1, 6, 6, 6, 4, 8},
    };
    *outTestData4Handle = exampleTestData4;

    AxrHandle<TestData_Small> outTestData5Handle{};
    axrResult = allocator.allocate(1, outTestData5Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData5{
        .ID = 54,
        .Data = {8, 3, 3, 5, 187, 12, 6},
    };
    *outTestData5Handle = exampleTestData5;

    ASSERT_TRUE(*outTestData1Handle == exampleTestData1);
    ASSERT_TRUE(*outTestData2Handle == exampleTestData2);
    ASSERT_TRUE(*outTestData3Handle == exampleTestData3);
    ASSERT_TRUE(*outTestData4Handle == exampleTestData4);
    ASSERT_TRUE(*outTestData5Handle == exampleTestData5);

    allocator.deallocate(outTestData2Handle);
    allocator.deallocate(outTestData3Handle);
    allocator.deallocate(outTestData4Handle);

    ASSERT_TRUE(*outTestData1Handle == exampleTestData1);
    ASSERT_TRUE(outTestData2Handle == nullptr);
    ASSERT_TRUE(outTestData3Handle == nullptr);
    ASSERT_TRUE(outTestData4Handle == nullptr);
    ASSERT_TRUE(*outTestData5Handle == exampleTestData5);

    ASSERT_TRUE(allocator.mainMemorySize() ==
                allocatorMainMemSize - testData2MemSize - testData3MemSize - testData4MemSize);

    // To make sure slot 2, 3 and 4 combine to make one free block, we allocate a large block that wouldn't fit anywhere
    // unless they combined

    constexpr size_t testDataLargeMemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + sizeof(AxrDynamicAllocator::DataHeader);

    AxrHandle<TestData_Large> outTestDataLargeHandle{};
    axrResult = allocator.allocate(1, outTestDataLargeHandle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Large exampleTestDataLarge{
        .ID = 54,
        .Data = {8, 1, 6, 3, 1, 3, 6, 5'673, 1, 4, 23, 6, 567, 234, 67},
    };
    *outTestDataLargeHandle = exampleTestDataLarge;

    ASSERT_TRUE(*outTestData1Handle == exampleTestData1);
    ASSERT_TRUE(*outTestData5Handle == exampleTestData5);
    ASSERT_TRUE(*outTestDataLargeHandle == exampleTestDataLarge);

    ASSERT_TRUE(allocator.mainMemorySize() ==
                allocatorMainMemSize - testData2MemSize - testData3MemSize - testData4MemSize + testDataLargeMemSize);
}

TEST(DynamicAllocator, Defragmentation) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 5;
    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData2MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData3MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData4MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t testData5MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + sizeof(AxrDynamicAllocator::DataHeader);
    constexpr size_t allocatorMainMemSize =
        testData1MemSize + testData2MemSize + testData3MemSize + testData4MemSize + testData5MemSize;
    const size_t allocatorSize =
        allocatorMainMemSize + AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    AxrHandle<TestData_Small> outTestData1Handle{};
    AxrResult axrResult = allocator.allocate(1, outTestData1Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData1{
        .ID = 1,
        .Data = {9, 1, 5, 6, 123, 867, 2},
    };
    *outTestData1Handle = exampleTestData1;

    AxrHandle<TestData_Small> outTestData2Handle{};
    axrResult = allocator.allocate(1, outTestData2Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData2{
        .ID = 76,
        .Data = {7, 12, 90, 51, 23, 65, 1},
    };
    *outTestData2Handle = exampleTestData2;

    AxrHandle<TestData_Small> outTestData3Handle{};
    axrResult = allocator.allocate(1, outTestData3Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData3{
        .ID = 74,
        .Data = {7, 54, 15, 97, 12, 867, 35},
    };
    *outTestData3Handle = exampleTestData3;

    AxrHandle<TestData_Small> outTestData4Handle{};
    axrResult = allocator.allocate(1, outTestData4Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData4{
        .ID = 9,
        .Data = {1, 1, 6, 6, 6, 4, 8},
    };
    *outTestData4Handle = exampleTestData4;

    AxrHandle<TestData_Small> outTestData5Handle{};
    axrResult = allocator.allocate(1, outTestData5Handle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Small exampleTestData5{
        .ID = 54,
        .Data = {8, 3, 3, 5, 187, 12, 6},
    };
    *outTestData5Handle = exampleTestData5;

    ASSERT_TRUE(*outTestData1Handle == exampleTestData1);
    ASSERT_TRUE(*outTestData2Handle == exampleTestData2);
    ASSERT_TRUE(*outTestData3Handle == exampleTestData3);
    ASSERT_TRUE(*outTestData4Handle == exampleTestData4);
    ASSERT_TRUE(*outTestData5Handle == exampleTestData5);

    allocator.deallocate(outTestData1Handle);
    allocator.deallocate(outTestData3Handle);
    allocator.deallocate(outTestData5Handle);

    ASSERT_TRUE(outTestData1Handle == nullptr);
    ASSERT_TRUE(*outTestData2Handle == exampleTestData2);
    ASSERT_TRUE(outTestData3Handle == nullptr);
    ASSERT_TRUE(*outTestData4Handle == exampleTestData4);
    ASSERT_TRUE(outTestData5Handle == nullptr);

    ASSERT_TRUE(allocator.mainMemorySize() ==
                allocatorMainMemSize - testData1MemSize - testData3MemSize - testData5MemSize);

    // Even there are only 2 data blocks remaining, defrag more just to make sure nothing goes wrong when there's
    // nothing left to defrag
    allocator.defragment(5);

    // Make sure the handles got updated correctly and still point to the correct data
    ASSERT_TRUE(outTestData1Handle == nullptr);
    ASSERT_TRUE(*outTestData2Handle == exampleTestData2);
    ASSERT_TRUE(outTestData3Handle == nullptr);
    ASSERT_TRUE(*outTestData4Handle == exampleTestData4);
    ASSERT_TRUE(outTestData5Handle == nullptr);

    ASSERT_TRUE(allocator.mainMemorySize() ==
                allocatorMainMemSize - testData1MemSize - testData3MemSize - testData5MemSize);

    // To make sure slots 1, 3 and 5 combine to make one free block, we allocate a large block that wouldn't fit
    // anywhere unless they combined

    constexpr size_t testDataLargeMemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + sizeof(AxrDynamicAllocator::DataHeader);

    AxrHandle<TestData_Large> outTestDataLargeHandle{};
    axrResult = allocator.allocate(1, outTestDataLargeHandle);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    constexpr TestData_Large exampleTestDataLarge{
        .ID = 54,
        .Data = {8, 1, 6, 3, 1, 3, 6, 5'673, 1, 4, 23, 6, 567, 234, 67},
    };
    *outTestDataLargeHandle = exampleTestDataLarge;

    ASSERT_TRUE(*outTestData2Handle == exampleTestData2);
    ASSERT_TRUE(*outTestData4Handle == exampleTestData4);
    ASSERT_TRUE(*outTestDataLargeHandle == exampleTestDataLarge);

    ASSERT_TRUE(allocator.mainMemorySize() ==
                allocatorMainMemSize - testData1MemSize - testData3MemSize - testData5MemSize + testDataLargeMemSize);
}
