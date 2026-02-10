// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "axr/common/defines.h"
#include "memory/doubleStackAllocator.h"

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

template<typename Type>
static AxrResult allocate(const bool upperEnd,
                          const bool isAligned,
                          AxrDoubleStackAllocator& allocator,
                          Type*& memory,
                          AxrDoubleStackAllocator::MarkerID& markerID) {
    if (upperEnd) {
        if (isAligned) {
            return allocator.allocateUpperAligned(1, memory, markerID);
        }

        return allocator.allocateUpper(1, memory, markerID);
    }

    if (isAligned) {
        return allocator.allocateLowerAligned(1, memory, markerID);
    }

    return allocator.allocateLower(1, memory, markerID);
}

static void deallocate(const bool upperEnd,
                       AxrDoubleStackAllocator& allocator,
                       const AxrDoubleStackAllocator::MarkerID markerID) {
    if (upperEnd) {
        allocator.deallocateUpper(markerID);
    } else {
        allocator.deallocateLower(markerID);
    }
}

static void allocateOne_Test(const bool upperEnd, const bool isAligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    size_t allocatorSize = sizeof(TestData_Small) + AxrDoubleStackAllocator::getMarkerSize();
    if (isAligned) {
        allocatorSize += alignof(TestData_Small);
    }
    void* memory = malloc(allocatorSize);
    AxrDoubleStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData = nullptr;
    AxrDoubleStackAllocator::MarkerID markerID{};
    const AxrResult axrResult = allocate(upperEnd, isAligned, allocator, outTestData, markerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestData != nullptr);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestData == TestData_Small{});
}

static void allocateTwo_Test(const bool upperEnd, const bool isAligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    size_t testData1MemSize = sizeof(TestData_Small) + AxrDoubleStackAllocator::getMarkerSize();
    size_t testData2MemSize = sizeof(TestData_Large) + AxrDoubleStackAllocator::getMarkerSize();
    if (isAligned) {
        testData1MemSize += alignof(TestData_Small);
        testData2MemSize += alignof(TestData_Large);
    }
    const size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrDoubleStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrDoubleStackAllocator::MarkerID testData1MarkerID{};
    AxrDoubleStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocate(upperEnd, isAligned, allocator, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocate(upperEnd, isAligned, allocator, outTestData2, testData2MarkerID);
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

static void allocateTooMuch_Test(const bool upperEnd, const bool isAligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    size_t allocatorSize = sizeof(TestData_Small) + AxrDoubleStackAllocator::getMarkerSize();
    if (isAligned) {
        allocatorSize += alignof(TestData_Small);
    }
    void* memory = malloc(allocatorSize);
    AxrDoubleStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    AxrDoubleStackAllocator::MarkerID testData1MarkerID{};
    AxrResult axrResult = allocate(upperEnd, isAligned, allocator, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    TestData_Small* outTestData2 = nullptr;
    AxrDoubleStackAllocator::MarkerID testData2MarkerID{};
    axrResult = allocate(upperEnd, isAligned, allocator, outTestData2, testData2MarkerID);
    ASSERT_TRUE(axrResult == AXR_ERROR_OUT_OF_MEMORY);

    TestData_Small* outTestData3 = nullptr;
    AxrDoubleStackAllocator::MarkerID testData3MarkerID{};
    axrResult = allocate(!upperEnd, isAligned, allocator, outTestData3, testData3MarkerID);
    ASSERT_TRUE(axrResult == AXR_ERROR_OUT_OF_MEMORY);
}

static void allocateTwoDeallocateOne_Test(const bool upperEnd, const bool isAligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    size_t testData1MemSize = sizeof(TestData_Small) + AxrDoubleStackAllocator::getMarkerSize();
    size_t testData2MemSize = sizeof(TestData_Large) + AxrDoubleStackAllocator::getMarkerSize();
    if (isAligned) {
        testData1MemSize += alignof(TestData_Small);
        testData2MemSize += alignof(TestData_Large);
    }
    const size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrDoubleStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrDoubleStackAllocator::MarkerID testData1MarkerID{};
    AxrDoubleStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocate(upperEnd, isAligned, allocator, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocate(upperEnd, isAligned, allocator, outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full first.
    ASSERT_TRUE(allocator.size() == allocatorSize);
    deallocate(upperEnd, allocator, testData2MarkerID);
    // Check that the allocator now only holds data item 1
    ASSERT_TRUE(allocator.size() == testData1MemSize);
}

static void allocateTwoDeallocateMarker1_Test(const bool upperEnd, const bool isAligned) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    size_t testData1MemSize = sizeof(TestData_Small) + AxrDoubleStackAllocator::getMarkerSize();
    size_t testData2MemSize = sizeof(TestData_Large) + AxrDoubleStackAllocator::getMarkerSize();
    if (isAligned) {
        testData1MemSize += alignof(TestData_Small);
        testData2MemSize += alignof(TestData_Large);
    }
    const size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrDoubleStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestData1 = nullptr;
    TestData_Large* outTestData2 = nullptr;
    AxrDoubleStackAllocator::MarkerID testData1MarkerID{};
    AxrDoubleStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocate(upperEnd, isAligned, allocator, outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocate(upperEnd, isAligned, allocator, outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full.
    ASSERT_TRUE(allocator.size() == allocatorSize);
    deallocate(upperEnd, allocator, testData1MarkerID);
    // Check that deallocating data item 1 marker, also deallocates data item 2
    ASSERT_TRUE(allocator.empty());
}

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(DoubleStackAllocator, DeallocatorCallback) {
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
        AxrDoubleStackAllocator allocator(memory, allocatorSize, callback);
    }
    ASSERT_TRUE(wasDeallocated);
}

TEST(DoubleStackAllocator, AllocateOneLower_Unaligned) {
    allocateOne_Test(false, false);
}

TEST(DoubleStackAllocator, AllocateOneLower_Aligned) {
    allocateOne_Test(false, true);
}

TEST(DoubleStackAllocator, AllocateOneUpper_Unaligned) {
    allocateOne_Test(true, false);
}

TEST(DoubleStackAllocator, AllocateOneUpper_Aligned) {
    allocateOne_Test(true, true);
}

TEST(DoubleStackAllocator, AllocateOneLowerOneUpper) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    const size_t allocatorSize = (sizeof(TestData_Small) + AxrDoubleStackAllocator::getMarkerSize()) * 2;
    void* memory = malloc(allocatorSize);
    AxrDoubleStackAllocator allocator(memory, allocatorSize, callback);

    TestData_Small* outTestDataLower = nullptr;
    TestData_Small* outTestDataUpper = nullptr;
    AxrDoubleStackAllocator::MarkerID markerIDLower{};
    AxrDoubleStackAllocator::MarkerID markerIDUpper{};
    AxrResult axrResult = allocator.allocateLower(1, outTestDataLower, markerIDLower);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestDataLower != nullptr);

    axrResult = allocator.allocateUpper(1, outTestDataUpper, markerIDUpper);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestDataUpper != nullptr);

    ASSERT_TRUE(outTestDataLower != outTestDataUpper);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestDataLower == TestData_Small{});
    ASSERT_TRUE(*outTestDataUpper == TestData_Small{});

    constexpr auto exampleTestDataLower = TestData_Small{.ID = 5, .Data = {1, 2, 3, 4, 5, 6, 7}};
    constexpr auto exampleTestDataUpper = TestData_Small{.ID = 9, .Data = {23, 32, 61, 12, 89, 14, 63}};
    *outTestDataLower = exampleTestDataLower;
    *outTestDataUpper = exampleTestDataUpper;

    // Check that there are no overlaps in memory by assigning data and checking it
    ASSERT_TRUE(*outTestDataLower == exampleTestDataLower);
    ASSERT_TRUE(*outTestDataUpper == exampleTestDataUpper);
}

TEST(DoubleStackAllocator, AllocateTwoLower_Unaligned) {
    allocateTwo_Test(false, false);
}

TEST(DoubleStackAllocator, AllocateTwoLower_Aligned) {
    allocateTwo_Test(false, true);
}

TEST(DoubleStackAllocator, AllocateTwoUpper_Unaligned) {
    allocateTwo_Test(true, false);
}

TEST(DoubleStackAllocator, AllocateTwoUpper_Aligned) {
    allocateTwo_Test(true, true);
}

TEST(DoubleStackAllocator, AllocateTooMuchLower_Unaligned) {
    allocateTooMuch_Test(false, false);
}

TEST(DoubleStackAllocator, AllocateTooMuchLower_Aligned) {
    allocateTooMuch_Test(false, true);
}

TEST(DoubleStackAllocator, AllocateTooMuchUpper_Unaligned) {
    allocateTooMuch_Test(true, false);
}

TEST(DoubleStackAllocator, AllocateTooMuchUpper_Aligned) {
    allocateTooMuch_Test(true, true);
}

TEST(DoubleStackAllocator, AllocateTwoDeallocateOneLower_Unaligned) {
    allocateTwoDeallocateOne_Test(false, false);
}

TEST(DoubleStackAllocator, AllocateTwoDeallocateOneLower_Aligned) {
    allocateTwoDeallocateOne_Test(false, true);
}

TEST(DoubleStackAllocator, AllocateTwoDeallocateOneUpper_Unaligned) {
    allocateTwoDeallocateOne_Test(true, false);
}

TEST(DoubleStackAllocator, AllocateTwoDeallocateOneUpper_Aligned) {
    allocateTwoDeallocateOne_Test(true, true);
}

TEST(DoubleStackAllocator, AllocateTwoDeallocateMarker1Lower_Unaligned) {
    allocateTwoDeallocateMarker1_Test(false, false);
}

TEST(DoubleStackAllocator, AllocateTwoDeallocateMarker1Lower_Aligned) {
    allocateTwoDeallocateMarker1_Test(false, true);
}

TEST(DoubleStackAllocator, AllocateTwoDeallocateMarker1Upper_Unaligned) {
    allocateTwoDeallocateMarker1_Test(true, false);
}

TEST(DoubleStackAllocator, AllocateTwoDeallocateMarker1Upper_Aligned) {
    allocateTwoDeallocateMarker1_Test(true, true);
}
