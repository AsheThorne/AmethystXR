// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "axr/common/defines.h"
#include "memory/stackAllocator.h"

TEST(StackAllocator, DeallocatorCallback) {
    bool wasDeallocated = false;
    {
        auto deallocateCallback = [](bool* wasDeallocated, void*& memory) -> void {
            free(memory);
            memory = nullptr;
            *wasDeallocated = true;
        };

        AxrDeallocate callback;
        callback.connect<deallocateCallback>(&wasDeallocated);

        constexpr size_t allocatorSize = 128;
        void* memory = malloc(allocatorSize);
        AxrStackAllocator allocator(allocatorSize, memory, callback);
    }
    ASSERT_TRUE(wasDeallocated);
}

TEST(StackAllocator, AllocateOne) {
    auto deallocateCallback = [](void*& memory) -> void {
        free(memory);
        memory = nullptr;
    };

    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData {
        uint32_t ID{};
        uint32_t Data[7]{};

        bool operator==(const TestData& src) const {
            return ID == src.ID && std::equal(std::begin(Data), std::end(Data), std::begin(src.Data));
        }
    };

    const size_t allocatorSize = sizeof(TestData) + AxrStackAllocator::getMarkerSize();
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(allocatorSize, memory, callback);

    void* outTestData = nullptr;
    AxrStackAllocator::MarkerID markerID{};
    const AxrResult axrResult = allocator.allocate(sizeof(TestData), outTestData, markerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestData != nullptr);
    const auto testData = static_cast<TestData*>(outTestData);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*testData == TestData{});
}

TEST(StackAllocator, AllocateTwo) {
    auto deallocateCallback = [](void*& memory) -> void {
        free(memory);
        memory = nullptr;
    };

    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData1 {
        uint32_t ID{};
        uint32_t Data[7]{};

        bool operator==(const TestData1& src) const {
            return ID == src.ID && std::equal(std::begin(Data), std::end(Data), std::begin(src.Data));
        }
    };

    struct TestData2 {
        uint32_t ID{};
        uint32_t Data[15]{};

        bool operator==(const TestData2& src) const {
            return ID == src.ID && std::equal(std::begin(Data), std::end(Data), std::begin(src.Data));
        }
    };

    const size_t testData1MemSize = sizeof(TestData1) + AxrStackAllocator::getMarkerSize();
    const size_t testData2MemSize = sizeof(TestData2) + AxrStackAllocator::getMarkerSize();
    const size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(allocatorSize, memory, callback);

    void* outTestData1 = nullptr;
    void* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocate(sizeof(TestData1), outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    const auto testData1 = static_cast<TestData1*>(outTestData1);

    axrResult = allocator.allocate(sizeof(TestData2), outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    const auto testData2 = static_cast<TestData2*>(outTestData2);

    ASSERT_TRUE(outTestData1 != outTestData2);
    ASSERT_TRUE(testData1MarkerID != testData2MarkerID);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*testData1 == TestData1{});
    ASSERT_TRUE(*testData2 == TestData2{});

    constexpr auto exampleTestData1 = TestData1{.ID = 5, .Data = {1, 2, 3, 4, 5, 6, 7}};
    constexpr auto exampleTestData2 =
        TestData2{.ID = 9, .Data = {23, 32, 61, 12, 89, 14, 63, 323, 65, 344, 13, 87, 12, 34, 91}};
    *testData1 = exampleTestData1;
    *testData2 = exampleTestData2;

    // Check that there are no overlaps in memory by assigning data and checking it
    ASSERT_TRUE(*testData1 == exampleTestData1);
    ASSERT_TRUE(*testData2 == exampleTestData2);
}

TEST(StackAllocator, AllocateTwoDeallocateOne) {
    auto deallocateCallback = [](void*& memory) -> void {
        free(memory);
        memory = nullptr;
    };

    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData1 {
        uint32_t ID;
        uint32_t Data[7];
    };

    struct TestData2 {
        uint32_t ID;
        uint32_t Data[15];
    };

    const size_t testData1MemSize = sizeof(TestData1) + AxrStackAllocator::getMarkerSize();
    const size_t testData2MemSize = sizeof(TestData2) + AxrStackAllocator::getMarkerSize();
    const size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(allocatorSize, memory, callback);

    void* outTestData1 = nullptr;
    void* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocate(sizeof(TestData1), outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocate(sizeof(TestData2), outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full first.
    ASSERT_TRUE(allocator.size() == allocatorSize);
    allocator.deallocate(testData2MarkerID);
    // Check that the allocator now only holds data item 1
    ASSERT_TRUE(allocator.size() == testData1MemSize);
}

TEST(StackAllocator, AllocateTwoDeallocateMarker1) {
    auto deallocateCallback = [](void*& memory) -> void {
        free(memory);
        memory = nullptr;
    };

    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData1 {
        uint32_t ID;
        uint32_t Data[7];
    };

    struct TestData2 {
        uint32_t ID;
        uint32_t Data[15];
    };

    const size_t testData1MemSize = sizeof(TestData1) + AxrStackAllocator::getMarkerSize();
    const size_t testData2MemSize = sizeof(TestData2) + AxrStackAllocator::getMarkerSize();
    const size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator allocator(allocatorSize, memory, callback);

    void* outTestData1 = nullptr;
    void* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = allocator.allocate(sizeof(TestData1), outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = allocator.allocate(sizeof(TestData2), outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full.
    ASSERT_TRUE(allocator.size() == allocatorSize);
    allocator.deallocate(testData1MarkerID);
    // Check that deallocating data item 1 marker, also deallocates data item 2
    ASSERT_TRUE(allocator.empty());
}
