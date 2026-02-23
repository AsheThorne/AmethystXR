// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "axr/common/defines.h"
#include "memory/poolAllocator.h"

// ----------------------------------------- //
// Shared Types
// ----------------------------------------- //

namespace {
    struct TestData_Large {
        uint32_t ID{};
        uint32_t Data[7]{};

        bool operator==(const TestData_Large& src) const {
            return ID == src.ID && std::equal(std::begin(Data), std::end(Data), std::begin(src.Data));
        }
    };
} // namespace

namespace {
    using TestData_Small = uint8_t;
} // namespace

// ----------------------------------------- //
// Shared Functions
// ----------------------------------------- //

static void deallocateCallback(void*& memory) {
    free(memory);
    memory = nullptr;
};

template<typename DataType, bool IsAligned>
static void deallocatorCallback_Test() {
    bool wasDeallocated = false;
    {
        auto deallocateCallback = [](bool* wasDeallocated, void*& memory) -> void {
            free(memory);
            memory = nullptr;
            *wasDeallocated = true;
        };

        AxrDeallocateBlock callback;
        callback.connect<deallocateCallback>(&wasDeallocated);

        constexpr size_t chunkCount = 10;
        constexpr size_t allocatorSize = (chunkCount * sizeof(DataType)) + (IsAligned ? alignof(DataType) : 0);
        AxrPoolAllocator<DataType, IsAligned> allocator(malloc(allocatorSize), allocatorSize, callback);
    }
    ASSERT_TRUE(wasDeallocated);
}

template<typename DataType, bool IsAligned>
    requires std::equality_comparable<DataType>
static void allocateOne_Test() {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t chunkCount = 10;
    constexpr size_t allocatorSize = (chunkCount * sizeof(DataType)) + (IsAligned ? alignof(DataType) : 0);
    AxrPoolAllocator<DataType, IsAligned> allocator(malloc(allocatorSize), allocatorSize, callback);

    DataType* outTestData = nullptr;
    const AxrResult axrResult = allocator.allocate(outTestData);
    ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
    ASSERT_TRUE(outTestData != nullptr);

    // Check that the data is empty and zeroed out
    ASSERT_TRUE(*outTestData == DataType{});
}

template<typename DataType, bool IsAligned, size_t DataSize>
    requires std::equality_comparable<DataType>
static void allocateAll_Test(const DataType* exampleTestDatas) {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    const size_t allocatorSize = (DataSize * sizeof(DataType)) + (IsAligned ? alignof(DataType) : 0);
    AxrPoolAllocator<DataType, IsAligned> allocator(malloc(allocatorSize), allocatorSize, callback);

    DataType* outTestDatas[DataSize]{};
    for (int i = 0; i < DataSize; i++) {
        const AxrResult axrResult = allocator.allocate(outTestDatas[i]);
        ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
        ASSERT_TRUE(outTestDatas[i] != nullptr);

        // Check that the data is empty and zeroed out
        ASSERT_TRUE(*outTestDatas[i] == DataType{});

        *outTestDatas[i] = exampleTestDatas[i];
    }

    // Check that there are no overlaps in memory by assigning data (happens in the loop) and checking it
    for (int i = 0; i < DataSize; i++) {
        ASSERT_TRUE(*outTestDatas[i] == exampleTestDatas[i]);
    }

    ASSERT_TRUE(allocator.size() == allocator.chunkCapacity());
}

template<typename DataType, bool IsAligned>
static void allocateTooMuch_Test() {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t chunkCount = 10;
    constexpr size_t allocatorSize = (chunkCount * sizeof(DataType)) + (IsAligned ? alignof(DataType) : 0);
    AxrPoolAllocator<DataType, IsAligned> allocator(malloc(allocatorSize), allocatorSize, callback);

    DataType* outTestDatas[chunkCount]{};
    for (DataType*& outTestData : outTestDatas) {
        const AxrResult axrResult = allocator.allocate(outTestData);
        ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
        ASSERT_TRUE(outTestData != nullptr);
    }

    ASSERT_TRUE(allocator.size() == allocator.chunkCapacity());

    DataType* outTestData = nullptr;
    const AxrResult axrResult = allocator.allocate(outTestData);
    ASSERT_TRUE(axrResult == AXR_ERROR_OUT_OF_MEMORY);
    ASSERT_TRUE(outTestData == nullptr);
}

template<typename DataType, bool IsAligned>
static void allocateAllDeallocateTwoAllocateTwo_Test() {
    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t chunkCount = 10;
    constexpr size_t allocatorSize = (chunkCount * sizeof(DataType)) + (IsAligned ? alignof(DataType) : 0);
    AxrPoolAllocator<DataType, IsAligned> allocator(malloc(allocatorSize), allocatorSize, callback);

    auto allocate = [&allocator](DataType*& outTestData) -> void {
        const AxrResult axrResult = allocator.allocate(outTestData);
        ASSERT_TRUE(AXR_SUCCEEDED(axrResult));
        ASSERT_TRUE(outTestData != nullptr);
    };

    DataType* outTestDatas[chunkCount]{};
    for (DataType*& outTestData : outTestDatas) {
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

// ----------------------------------------- //
// PoolAllocator Tests
// ----------------------------------------- //

TEST(PoolAllocator_TypeFitsPointer_Unaligned, DeallocatorCallback) {
    deallocatorCallback_Test<TestData_Large, false>();
}

TEST(PoolAllocator_TypeSmallerThanPointer_Unaligned, DeallocatorCallback) {
    deallocatorCallback_Test<TestData_Small, false>();
}

TEST(PoolAllocator_TypeFitsPointer_Aligned, DeallocatorCallback) {
    deallocatorCallback_Test<TestData_Large, true>();
}

TEST(PoolAllocator_TypeSmallerThanPointer_Aligned, DeallocatorCallback) {
    deallocatorCallback_Test<TestData_Small, true>();
}

TEST(PoolAllocator_TypeFitsPointer_Unaligned, AllocateOne) {
    allocateOne_Test<TestData_Large, false>();
}

TEST(PoolAllocator_TypeSmallerThanPointer_Unaligned, AllocateOne) {
    allocateOne_Test<TestData_Small, false>();
}

TEST(PoolAllocator_TypeFitsPointer_Aligned, AllocateOne) {
    allocateOne_Test<TestData_Large, true>();
}

TEST(PoolAllocator_TypeSmallerThanPointer_Aligned, AllocateOne) {
    allocateOne_Test<TestData_Small, true>();
}

TEST(PoolAllocator_TypeFitsPointer_Unaligned, AllocateAll) {
    constexpr size_t exampleTestDataCount = 5;
    constexpr TestData_Large exampleTestDatas[exampleTestDataCount]{
        TestData_Large{.ID = 1, .Data = {1, 2, 3, 4, 5, 6, 7}},
        TestData_Large{.ID = 2, .Data = {23, 32, 61, 12, 89, 14, 63}},
        TestData_Large{.ID = 3, .Data = {323, 65, 344, 13, 87, 12, 34}},
        TestData_Large{.ID = 4, .Data = {671, 12, 843, 41, 1, 26, 845}},
        TestData_Large{.ID = 5, .Data = {9, 12, 11, 22, 22, 22, 77}},
    };

    allocateAll_Test<TestData_Large, false, exampleTestDataCount>(exampleTestDatas);
}

TEST(PoolAllocator_TypeSmallerThanPointer_Unaligned, AllocateAll) {
    constexpr size_t exampleTestDataCount = 5;
    constexpr TestData_Small exampleTestDatas[exampleTestDataCount]{
        TestData_Small{16},
        TestData_Small{17},
        TestData_Small{18},
        TestData_Small{19},
        TestData_Small{20},
    };

    allocateAll_Test<TestData_Small, false, exampleTestDataCount>(exampleTestDatas);
}

TEST(PoolAllocator_TypeFitsPointer_Aligned, AllocateAll) {
    constexpr size_t exampleTestDataCount = 5;
    constexpr TestData_Large exampleTestDatas[exampleTestDataCount]{
        TestData_Large{.ID = 1, .Data = {1, 2, 3, 4, 5, 6, 7}},
        TestData_Large{.ID = 2, .Data = {23, 32, 61, 12, 89, 14, 63}},
        TestData_Large{.ID = 3, .Data = {323, 65, 344, 13, 87, 12, 34}},
        TestData_Large{.ID = 4, .Data = {671, 12, 843, 41, 1, 26, 845}},
        TestData_Large{.ID = 5, .Data = {9, 12, 11, 22, 22, 22, 77}},
    };

    allocateAll_Test<TestData_Large, true, exampleTestDataCount>(exampleTestDatas);
}

TEST(PoolAllocator_TypeSmallerThanPointer_Aligned, AllocateAll) {
    constexpr size_t exampleTestDataCount = 5;
    constexpr TestData_Small exampleTestDatas[exampleTestDataCount]{
        TestData_Small{16},
        TestData_Small{17},
        TestData_Small{18},
        TestData_Small{19},
        TestData_Small{20},
    };

    allocateAll_Test<TestData_Small, true, exampleTestDataCount>(exampleTestDatas);
}

TEST(PoolAllocator_TypeFitsPointer_Unaligned, AllocateTooMuch) {
    allocateTooMuch_Test<TestData_Large, false>();
}

TEST(PoolAllocator_TypeSmallerThanPointer_Unaligned, AllocateTooMuch) {
    allocateTooMuch_Test<TestData_Small, false>();
}

TEST(PoolAllocator_TypeFitsPointer_Aligned, AllocateTooMuch) {
    allocateTooMuch_Test<TestData_Large, true>();
}

TEST(PoolAllocator_TypeSmallerThanPointer_Aligned, AllocateTooMuch) {
    allocateTooMuch_Test<TestData_Small, true>();
}

TEST(PoolAllocator_TypeFitsPointer_Unaligned, AllocateAllDeallocateTwoAllocateTwo) {
    allocateAllDeallocateTwoAllocateTwo_Test<TestData_Large, false>();
}

TEST(PoolAllocator_TypeSmallerThanPointer_Unaligned, AllocateAllDeallocateTwoAllocateTwo) {
    allocateAllDeallocateTwoAllocateTwo_Test<TestData_Small, false>();
}

TEST(PoolAllocator_TypeFitsPointer_Aligned, AllocateAllDeallocateTwoAllocateTwo) {
    allocateAllDeallocateTwoAllocateTwo_Test<TestData_Large, true>();
}

TEST(PoolAllocator_TypeSmallerThanPointer_Aligned, AllocateAllDeallocateTwoAllocateTwo) {
    allocateAllDeallocateTwoAllocateTwo_Test<TestData_Small, true>();
}
