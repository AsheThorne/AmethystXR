// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "axr/common/defines.h"
#include "memory/stackAllocator.h"

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

TEST(StackAllocator, DeallocatorCallback) {
    bool wasDeallocated = false;
    {
        auto deallocateCallback = [](bool* wasDeallocated, void*& memory) -> void {
            free(memory);
            memory = nullptr;
            *wasDeallocated = true;
        };

        AxrDeallocateBlock callback;
        callback.connect<deallocateCallback>(&wasDeallocated);

        constexpr size_t allocatorSize = 128;
        void* memory = malloc(allocatorSize);
        AxrStackAllocator allocator(memory, allocatorSize, callback);
    }
    ASSERT_TRUE(wasDeallocated);
}

TEST(StackAllocator, AllocateOne_Unaligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = sizeof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData = nullptr;
    AxrStackAllocator::MarkerID markerID{};
    const AxrResult axrResult = allocator.allocate(1, outTestData, markerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestData != nullptr);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestData == TestData_Small{});
}

TEST(StackAllocator, AllocateOne_Aligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = sizeof(TestData_Small) + alignof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData = nullptr;
    AxrStackAllocator::MarkerID markerID{};
    const AxrResult axrResult = allocator.allocateAligned(1, outTestData, markerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestData != nullptr);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestData == TestData_Small{});
}

TEST(StackAllocator, AllocateTwo_Unaligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t testData1MemSize = sizeof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    constexpr size_t testData2MemSize = sizeof(TestData_Large) + AxrStackAllocator::getMarkerSize();
    constexpr size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocate(1, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocate(1, outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    ASSERT_TRUE(reinterpret_cast<uintptr_t>(outTestData1) != reinterpret_cast<uintptr_t>(outTestData2));
    ASSERT_TRUE(testData1MarkerID != testData2MarkerID);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestData1 == TestData_Small{});
    ASSERT_TRUE(*outTestData2 == TestData_Large{});

    constexpr auto exampleTestData1 = TestData_Small{.ID = 5, .Data = {1, 2, 3, 4, 5, 6, 7}};
    constexpr auto exampleTestData2 =
        TestData_Large{.ID = 9, .Data = {23, 32, 61, 12, 89, 14, 63, 323, 65, 344, 13, 87, 12, 34, 91}};
    *outTestData1 = exampleTestData1;
    *outTestData2 = exampleTestData2;

    // Check that there are no overlaps in memory by assigning data and checking it
    ASSERT_TRUE(*outTestData1 == exampleTestData1);
    ASSERT_TRUE(*outTestData2 == exampleTestData2);
}

TEST(StackAllocator, AllocateTwo_Aligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    constexpr size_t testData2MemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + AxrStackAllocator::getMarkerSize();
    constexpr size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocateAligned(1, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocateAligned(1, outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    ASSERT_TRUE(reinterpret_cast<uintptr_t>(outTestData1) != reinterpret_cast<uintptr_t>(outTestData2));
    ASSERT_TRUE(testData1MarkerID != testData2MarkerID);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestData1 == TestData_Small{});
    ASSERT_TRUE(*outTestData2 == TestData_Large{});

    constexpr auto exampleTestData1 = TestData_Small{.ID = 5, .Data = {1, 2, 3, 4, 5, 6, 7}};
    constexpr auto exampleTestData2 =
        TestData_Large{.ID = 9, .Data = {23, 32, 61, 12, 89, 14, 63, 323, 65, 344, 13, 87, 12, 34, 91}};
    *outTestData1 = exampleTestData1;
    *outTestData2 = exampleTestData2;

    // Check that there are no overlaps in memory by assigning data and checking it
    ASSERT_TRUE(*outTestData1 == exampleTestData1);
    ASSERT_TRUE(*outTestData2 == exampleTestData2);
}

TEST(StackAllocator, AllocateTooMuch_Unaligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = sizeof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrResult axrResult = allocator.allocate(1, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    TestData_Small* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData2MarkerID{};
    axrResult = allocator.allocate(1, outTestData2, testData2MarkerID);
    ASSERT_TRUE(axrResult == AXR_ERROR_OUT_OF_MEMORY);
}

TEST(StackAllocator, AllocateTooMuch_Aligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = sizeof(TestData_Small) + alignof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrResult axrResult = allocator.allocateAligned(1, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    TestData_Small* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData2MarkerID{};
    axrResult = allocator.allocateAligned(1, outTestData2, testData2MarkerID);
    ASSERT_TRUE(axrResult == AXR_ERROR_OUT_OF_MEMORY);
}

TEST(StackAllocator, AllocateTwoDeallocateOne_Unaligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t testData1MemSize = sizeof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    constexpr size_t testData2MemSize = sizeof(TestData_Large) + AxrStackAllocator::getMarkerSize();
    constexpr size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocate(1, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocate(1, outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full first.
    ASSERT_TRUE(allocator.size() == allocatorSize);
    allocator.deallocate(testData2MarkerID);
    // Check that the allocator now only holds data item 1
    ASSERT_TRUE(allocator.size() == testData1MemSize);
}

TEST(StackAllocator, AllocateTwoDeallocateOne_Aligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    constexpr size_t testData2MemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + AxrStackAllocator::getMarkerSize();
    constexpr size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocateAligned(1, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocateAligned(1, outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full first.
    ASSERT_TRUE(allocator.size() == allocatorSize);
    allocator.deallocate(testData2MarkerID);
    // Check that the allocator now only holds data item 1
    ASSERT_TRUE(allocator.size() == testData1MemSize);
}

TEST(StackAllocator, AllocateTwoDeallocateMarker1_Unaligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t testData1MemSize = sizeof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    constexpr size_t testData2MemSize = sizeof(TestData_Large) + AxrStackAllocator::getMarkerSize();
    constexpr size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocate(1, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocate(1, outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full.
    ASSERT_TRUE(allocator.size() == allocatorSize);
    allocator.deallocate(testData1MarkerID);
    // Check that deallocating data item 1 marker, also deallocates data item 2
    ASSERT_TRUE(allocator.empty());
}

TEST(StackAllocator, AllocateTwoDeallocateMarker1_Aligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t testData1MemSize =
        sizeof(TestData_Small) + alignof(TestData_Small) + AxrStackAllocator::getMarkerSize();
    constexpr size_t testData2MemSize =
        sizeof(TestData_Large) + alignof(TestData_Large) + AxrStackAllocator::getMarkerSize();
    constexpr size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocateAligned(1, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocateAligned(1, outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full.
    ASSERT_TRUE(allocator.size() == allocatorSize);
    allocator.deallocate(testData1MarkerID);
    // Check that deallocating data item 1 marker, also deallocates data item 2
    ASSERT_TRUE(allocator.empty());
}
