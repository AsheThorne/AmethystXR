// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "axr/common/defines.h"
#include "memory/stackAllocator.h"

void deallocateCallback(void*& memory) {
    free(memory);
    memory = nullptr;
}

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

        constexpr std::size_t allocatorSize = 128;
        void* memory = malloc(allocatorSize);
        AxrStackAllocator stackAllocator(allocatorSize, memory, callback);
    }
    ASSERT_TRUE(wasDeallocated);
}

TEST(StackAllocator, AllocateOne) {
    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData {
        uint32_t ID;
        uint32_t Data[7];
    };

    const std::size_t allocatorSize = sizeof(TestData) + AxrStackAllocator::getMarkerSize();
    void* memory = malloc(allocatorSize);
    AxrStackAllocator stackAllocator(allocatorSize, memory, callback);

    void* outTestData = nullptr;
    AxrStackAllocator::MarkerID markerID{};
    const AxrResult axrResult = stackAllocator.allocate(sizeof(TestData), outTestData, markerID);
    ASSERT_TRUE(outTestData != nullptr);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
}

TEST(StackAllocator, AllocateTwoDifferentSizes) {
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

    const std::size_t testData1MemSize = sizeof(TestData1) + AxrStackAllocator::getMarkerSize();
    const std::size_t testData2MemSize = sizeof(TestData2) + AxrStackAllocator::getMarkerSize();
    const std::size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator stackAllocator(allocatorSize, memory, callback);

    void* outTestData1 = nullptr;
    void* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = stackAllocator.allocate(sizeof(TestData1), outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = stackAllocator.allocate(sizeof(TestData2), outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    ASSERT_TRUE(outTestData1 != outTestData2);
    ASSERT_TRUE(testData1MarkerID != testData2MarkerID);
}

TEST(StackAllocator, AllocateTwoDeallocateOne) {
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

    const std::size_t testData1MemSize = sizeof(TestData1) + AxrStackAllocator::getMarkerSize();
    const std::size_t testData2MemSize = sizeof(TestData2) + AxrStackAllocator::getMarkerSize();
    const std::size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator stackAllocator(allocatorSize, memory, callback);

    void* outTestData1 = nullptr;
    void* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = stackAllocator.allocate(sizeof(TestData1), outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = stackAllocator.allocate(sizeof(TestData2), outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full first.
    ASSERT_TRUE(stackAllocator.size() == allocatorSize);
    stackAllocator.deallocate(testData2MarkerID);
    // Check that the allocator now only holds data item 1
    ASSERT_TRUE(stackAllocator.size() == testData1MemSize);
}

TEST(StackAllocator, AllocateTwoDeallocateFirstItem) {
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

    const std::size_t testData1MemSize = sizeof(TestData1) + AxrStackAllocator::getMarkerSize();
    const std::size_t testData2MemSize = sizeof(TestData2) + AxrStackAllocator::getMarkerSize();
    const std::size_t allocatorSize = testData1MemSize + testData2MemSize;
    void* memory = malloc(allocatorSize);
    AxrStackAllocator stackAllocator(allocatorSize, memory, callback);

    void* outTestData1 = nullptr;
    void* outTestData2 = nullptr;
    AxrStackAllocator::MarkerID testData1MarkerID{};
    AxrStackAllocator::MarkerID testData2MarkerID{};
    AxrResult axrResult = stackAllocator.allocate(sizeof(TestData1), outTestData1, testData1MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    axrResult = stackAllocator.allocate(sizeof(TestData2), outTestData2, testData2MarkerID);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));

    // Check allocator is full.
    ASSERT_TRUE(stackAllocator.size() == allocatorSize);
    stackAllocator.deallocate(testData1MarkerID);
    // Check that deallocating data item 1 marker, also deallocates data item 2
    ASSERT_TRUE(stackAllocator.empty());
}
