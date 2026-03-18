// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/containers/vector_dynamic.h"

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

TEST(AxrVector_Dynamic, Initialization) {
    struct TestData {
        uint32_t value;
    };
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
                                 sizeof(AxrDynamicAllocator::DataHeader) + sizeof(AxrDynamicAllocator::DataHeader) +
                                 AxrDynamicAllocator::getHandlesMemoryBlockCapacity(maxHandleCount);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        maxHandleCount);

    const AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.capacity() == capacity);
    ASSERT_TRUE(vector.size() == 0);
    ASSERT_TRUE(vector.empty());
}

TEST(AxrVector_Dynamic, PushBackOne) {
    struct TestData {
        uint32_t value;
    };
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    constexpr TestData testData{.value = 43};

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    vector.pushBack(testData);

    ASSERT_TRUE(vector.size() == 1);
    ASSERT_TRUE(vector[0].value == testData.value);
}

TEST(AxrVector_Dynamic, PushBackCharArray) {
    using TestData = char[8];
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    constexpr TestData data = "Test";
    vector.pushBack(data);

    ASSERT_TRUE(vector.size() == 1);
    ASSERT_TRUE(strncmp(vector[0], data, 8) == 0);
}

TEST(AxrVector_Dynamic, PushBackConstCharPtr) {
    using TestData = const char*;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    vector.pushBack("Test");

    ASSERT_TRUE(vector.size() == 1);
    ASSERT_TRUE(strcmp(vector[0], "Test") == 0);
}

TEST(AxrVector_Dynamic, PushBackAll) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData[16]{
        TestData{.value = 1},
        TestData{.value = 63},
        TestData{.value = 80},
        TestData{.value = 51},
        TestData{.value = 287},
        TestData{.value = 6},
        TestData{.value = 5},
        TestData{.value = 987},
        TestData{.value = 12},
        TestData{.value = 43},
        TestData{.value = 16},
        TestData{.value = 27},
        TestData{.value = 49},
        TestData{.value = 91},
        TestData{.value = 37},
        TestData{.value = 88},
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    for (const TestData& i : testData) {
        vector.pushBack(i);
    }

    ASSERT_TRUE(vector.size() == capacity);

    for (uint32_t i = 0; i < capacity; ++i) {
        ASSERT_TRUE(vector[i].value == testData[i].value);
    }
}

TEST(AxrVector_Dynamic, PushBackTooMany) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData[16]{
        TestData{.value = 1},
        TestData{.value = 63},
        TestData{.value = 80},
        TestData{.value = 51},
        TestData{.value = 287},
        TestData{.value = 6},
        TestData{.value = 5},
        TestData{.value = 987},
        TestData{.value = 12},
        TestData{.value = 43},
        TestData{.value = 16},
        TestData{.value = 27},
        TestData{.value = 49},
        TestData{.value = 91},
        TestData{.value = 37},
        TestData{.value = 88},
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    for (const TestData& i : testData) {
        vector.pushBack(i);
    }

    const size_t size = vector.size();
    ASSERT_TRUE(size == capacity);

    vector.pushBack(TestData{.value = 99});

    ASSERT_TRUE(vector.size() == size);
    ASSERT_TRUE(vector[capacity - 1].value == testData[capacity - 1].value);
}

TEST(AxrVector_Dynamic, AutoDeallocation) {
    struct TestData {
        uint32_t value;
    };
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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
    ASSERT_TRUE(allocator.empty());

    {
        AxrVector_Dynamic<TestData> vector(capacity, &allocator);
        ASSERT_TRUE(!allocator.empty());
    }

    ASSERT_TRUE(allocator.empty());
}

TEST(AxrVector_Dynamic, PopBack_1) {
    struct TestData {
        uint32_t value;
    };
    constexpr uint32_t capacity = 16;

    constexpr TestData testData{.value = 43};

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    vector.pushBack(testData);

    ASSERT_TRUE(vector.size() == 1);

    vector.popBack();

    ASSERT_TRUE(vector.empty());
}

TEST(AxrVector_Dynamic, PopBack_2) {
    struct TestData {
        uint32_t value;
    };
    constexpr uint32_t capacity = 16;

    constexpr TestData testData{.value = 43};

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    vector.pushBack(testData);
    vector.pushBack(testData);
    vector.pushBack(testData);

    ASSERT_TRUE(vector.size() == 3);

    vector.popBack();

    ASSERT_TRUE(vector.size() == 2);
}

TEST(AxrVector_Dynamic, Clear) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData[16]{
        TestData{.value = 1},
        TestData{.value = 63},
        TestData{.value = 80},
        TestData{.value = 51},
        TestData{.value = 287},
        TestData{.value = 6},
        TestData{.value = 5},
        TestData{.value = 987},
        TestData{.value = 12},
        TestData{.value = 43},
        TestData{.value = 16},
        TestData{.value = 27},
        TestData{.value = 49},
        TestData{.value = 91},
        TestData{.value = 37},
        TestData{.value = 88},
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    for (const TestData& i : testData) {
        vector.pushBack(i);
    }

    ASSERT_TRUE(vector.size() == capacity);

    vector.clear();

    ASSERT_TRUE(vector.empty());
}

TEST(AxrVector_Dynamic, GetAt_InBounds) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData[16]{
        TestData{.value = 1},
        TestData{.value = 63},
        TestData{.value = 80},
        TestData{.value = 51},
        TestData{.value = 287},
        TestData{.value = 6},
        TestData{.value = 5},
        TestData{.value = 987},
        TestData{.value = 12},
        TestData{.value = 43},
        TestData{.value = 16},
        TestData{.value = 27},
        TestData{.value = 49},
        TestData{.value = 91},
        TestData{.value = 37},
        TestData{.value = 88},
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    for (const TestData& i : testData) {
        vector.pushBack(i);
    }

    ASSERT_TRUE(vector.size() == capacity);

    const TestData* dataAt = vector.at(5);
    if (dataAt == nullptr) {
        ASSERT_TRUE(false);
        return;
    }

    ASSERT_TRUE(dataAt->value == testData[5].value);
}

TEST(AxrVector_Dynamic, GetAt_OutBounds) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData[16]{
        TestData{.value = 1},
        TestData{.value = 63},
        TestData{.value = 80},
        TestData{.value = 51},
        TestData{.value = 287},
        TestData{.value = 6},
        TestData{.value = 5},
        TestData{.value = 987},
        TestData{.value = 12},
        TestData{.value = 43},
        TestData{.value = 16},
        TestData{.value = 27},
        TestData{.value = 49},
        TestData{.value = 91},
        TestData{.value = 37},
        TestData{.value = 88},
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);
    ASSERT_TRUE(vector.empty());

    for (const TestData& i : testData) {
        vector.pushBack(i);
    }

    ASSERT_TRUE(vector.size() == capacity);

    ASSERT_TRUE(vector.at(16) == nullptr);
}

TEST(AxrVector_Dynamic, FindFirst_Exists) {
    struct TestData {
        uint32_t value;

        bool operator==(const TestData& other) const {
            return value == other.value;
        }
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);

    constexpr TestData testData{
        .value = 10,
    };
    vector.pushBack(TestData{
        .value = 1,
    });
    vector.pushBack(testData);
    vector.pushBack(TestData{
        .value = 2,
    });

    const auto iterator = vector.findFirst(testData);
    ASSERT_TRUE(iterator != vector.end());
    if (iterator != vector.end()) {
        ASSERT_TRUE(iterator->value == testData.value);
    }
}

TEST(AxrVector_Dynamic, FindFirst_DoesntExist) {
    struct TestData {
        uint32_t value;

        bool operator==(const TestData& other) const {
            return value == other.value;
        }
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr uint32_t maxHandleCount = 1;
    const size_t allocatorSize = (sizeof(TestData) * capacity) + alignof(TestData) +
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

    AxrVector_Dynamic<TestData> vector(capacity, &allocator);

    vector.pushBack(TestData{
        .value = 1,
    });
    vector.pushBack(TestData{
        .value = 15,
    });
    vector.pushBack(TestData{
        .value = 2,
    });

    const auto iterator = vector.findFirst(TestData{.value = 10});
    ASSERT_TRUE(iterator == vector.end());
}
