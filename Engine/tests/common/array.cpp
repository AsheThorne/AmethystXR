// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/array.h"

TEST(AxrArray, Initialization) {
    struct TestData {
        uint32_t value;
    };

    constexpr uint32_t capacity = 16;
    constexpr AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Capacity == capacity);
    ASSERT_TRUE(array.Capacity == array.capacity());
    ASSERT_TRUE(array.Size == 0);
    ASSERT_TRUE(array.Size == array.size());
}

TEST(AxrArray, PushBackOne) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData{.value = 43};

    constexpr uint32_t capacity = 16;
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Size == 0);

    array.pushBack(testData);

    ASSERT_TRUE(array.Size == 1);
    ASSERT_TRUE(array[0].value == testData.value);
}

TEST(AxrArray, PushBackAll) {
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
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Size == 0);

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    ASSERT_TRUE(array.Size == capacity);

    for (uint32_t i = 0; i < capacity; ++i) {
        ASSERT_TRUE(array[i].value == testData[i].value);
    }
}

TEST(AxrArray, PushBackTooMany) {
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
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Size == 0);

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    const size_t size = array.Size;
    ASSERT_TRUE(size == capacity);

    array.pushBack(TestData{.value = 99});

    ASSERT_TRUE(array.Size == size);
    ASSERT_TRUE(array[capacity - 1].value == testData[capacity - 1].value);
}

TEST(AxrArray, PopBack_1) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData{.value = 43};

    constexpr uint32_t capacity = 16;
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Size == 0);

    array.pushBack(testData);

    ASSERT_TRUE(array.Size == 1);

    array.popBack();

    ASSERT_TRUE(array.Size == 0);
}

TEST(AxrArray, PopBack_2) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData{.value = 43};

    constexpr uint32_t capacity = 16;
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Size == 0);

    array.pushBack(testData);
    array.pushBack(testData);
    array.pushBack(testData);

    ASSERT_TRUE(array.Size == 3);

    array.popBack();

    ASSERT_TRUE(array.Size == 2);
}

TEST(AxrArray, Clear) {
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
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Size == 0);

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    ASSERT_TRUE(array.Size == capacity);

    array.clear();

    ASSERT_TRUE(array.Size == 0);
}

TEST(AxrArray, GetAt_InBounds) {
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
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Size == 0);

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    ASSERT_TRUE(array.Size == capacity);

    const TestData* dataAt = array.at(5);
    if (dataAt == nullptr) {
        ASSERT_TRUE(false);
        return;
    }

    ASSERT_TRUE(dataAt->value == testData[5].value);
}

TEST(AxrArray, GetAt_OutBounds) {
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
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.Size == 0);

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    ASSERT_TRUE(array.Size == capacity);

    ASSERT_TRUE(array.at(16) == nullptr);
}
