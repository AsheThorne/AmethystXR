// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "axr/common/defines.h"
#include "memory/poolAllocator.h"

static void deallocateCallback(void*& memory) {
    free(memory);
    memory = nullptr;
};

TEST(PoolAllocator_TypeFitsPointer, DeallocatorCallback) {
    bool wasDeallocated = false;
    {
        auto deallocateCallback = [](bool* wasDeallocated, void*& memory) -> void {
            free(memory);
            memory = nullptr;
            *wasDeallocated = true;
        };

        struct TestData {
            uint32_t ID{};
            uint32_t Data[7]{};
        };

        AxrDeallocate callback;
        callback.connect<deallocateCallback>(&wasDeallocated);

        constexpr size_t chunkCount = 10;
        constexpr size_t allocatorSize = chunkCount * sizeof(TestData);
        AxrPoolAllocator<TestData> allocator(malloc(allocatorSize), allocatorSize, callback);
    }
    ASSERT_TRUE(wasDeallocated);
}

TEST(PoolAllocator_TypeFitsPointer, AllocateOne) {
    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData {
        uint32_t ID{};
        uint32_t Data[7]{};

        bool operator==(const TestData& src) const {
            return ID == src.ID && std::equal(std::begin(Data), std::end(Data), std::begin(src.Data));
        }
    };

    constexpr size_t chunkCount = 10;
    constexpr size_t allocatorSize = chunkCount * sizeof(TestData);
    AxrPoolAllocator<TestData> allocator(malloc(allocatorSize), allocatorSize, callback);

    TestData* outTestData = nullptr;
    const AxrResult axrResult = allocator.allocate(outTestData);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestData != nullptr);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestData == TestData{});
}

TEST(PoolAllocator_TypeFitsPointer, AllocateAll) {
    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData {
        uint32_t ID{};
        uint32_t Data[7]{};

        bool operator==(const TestData& src) const {
            return ID == src.ID && std::equal(std::begin(Data), std::end(Data), std::begin(src.Data));
        }
    };

    constexpr size_t chunkCount = 5;
    constexpr size_t allocatorSize = chunkCount * sizeof(TestData);
    AxrPoolAllocator<TestData> allocator(malloc(allocatorSize), allocatorSize, callback);

    constexpr TestData exampleTestDatas[chunkCount]{
        TestData{.ID = 1, .Data = {1, 2, 3, 4, 5, 6, 7}},
        TestData{.ID = 2, .Data = {23, 32, 61, 12, 89, 14, 63}},
        TestData{.ID = 3, .Data = {323, 65, 344, 13, 87, 12, 34}},
        TestData{.ID = 4, .Data = {671, 12, 843, 41, 1, 26, 845}},
        TestData{.ID = 5, .Data = {9, 12, 11, 22, 22, 22, 77}},
    };

    TestData* outTestDatas[chunkCount]{};
    for (int i = 0; i < chunkCount; i++) {
        const AxrResult axrResult = allocator.allocate(outTestDatas[i]);
        ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
        ASSERT_TRUE(outTestDatas[i] != nullptr);

        // Check that the data is empty and zeroed out
        ASSERT_TRUE(*outTestDatas[i] == TestData{});

        *outTestDatas[i] = exampleTestDatas[i];
    }

    // Check that there are no overlaps in memory by assigning data (happens in the loop) and checking it
    for (int i = 0; i < chunkCount; i++) {
        ASSERT_TRUE(*outTestDatas[i] == exampleTestDatas[i]);
    }

    ASSERT_TRUE(allocator.size() == allocator.chunkCapacity());
}

TEST(PoolAllocator_TypeFitsPointer, AllocateTooMuch) {
    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData {
        uint32_t ID{};
        uint32_t Data[7]{};
    };

    constexpr size_t chunkCount = 10;
    constexpr size_t allocatorSize = chunkCount * sizeof(TestData);
    AxrPoolAllocator<TestData> allocator(malloc(allocatorSize), allocatorSize, callback);

    TestData* outTestDatas[chunkCount]{};
    for (TestData*& outTestData : outTestDatas) {
        const AxrResult axrResult = allocator.allocate(outTestData);
        ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
        ASSERT_TRUE(outTestData != nullptr);
    }

    ASSERT_TRUE(allocator.size() == allocator.chunkCapacity());

    TestData* outTestData = nullptr;
    const AxrResult axrResult = allocator.allocate(outTestData);
    ASSERT_TRUE(axrResult == AXR_ERROR_OUT_OF_MEMORY);
    ASSERT_TRUE(outTestData == nullptr);
}

TEST(PoolAllocator_TypeFitsPointer, AllocateAllDeallocateTwoAllocateTwo) {
    AxrDeallocate callback;
    callback.connect<deallocateCallback>();

    struct TestData {
        uint32_t ID{};
        uint32_t Data[7]{};
    };

    constexpr size_t chunkCount = 10;
    constexpr size_t allocatorSize = chunkCount * sizeof(TestData);
    AxrPoolAllocator<TestData> allocator(malloc(allocatorSize), allocatorSize, callback);

    auto allocate = [&allocator](TestData*& outTestData) -> void {
        const AxrResult axrResult = allocator.allocate(outTestData);
        ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
        ASSERT_TRUE(outTestData != nullptr);
    };

    TestData* outTestDatas[chunkCount]{};
    for (TestData*& outTestData : outTestDatas) {
        allocate(outTestData);
    }

    ASSERT_TRUE(allocator.size() == allocator.chunkCapacity());

    allocator.deallocate(outTestDatas[0]);
    allocator.deallocate(outTestDatas[1]);

    ASSERT_TRUE(allocator.size() == allocator.chunkCapacity() - 2);

    allocate(outTestDatas[0]);
    allocate(outTestDatas[1]);

    ASSERT_TRUE(allocator.size() == allocator.chunkCapacity());
}
