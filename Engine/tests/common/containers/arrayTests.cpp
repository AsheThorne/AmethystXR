// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/containers/array.h"

TEST(AxrArray, Initialization) {
    struct TestData {
        uint32_t value;
    };

    constexpr uint32_t capacity = 16;
    constexpr AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.capacity() == capacity);
    ASSERT_TRUE(array.size() == 0);
    ASSERT_TRUE(array.empty());
}

TEST(AxrArray, PushBackOne) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData{.value = 43};

    constexpr uint32_t capacity = 16;
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.empty());

    array.pushBack(testData);

    ASSERT_TRUE(array.size() == 1);
    ASSERT_TRUE(array[0].value == testData.value);
}

TEST(AxrArray, PushBackCharArray) {
    constexpr uint32_t capacity = 16;
    AxrArray<char[8], capacity> array;
    ASSERT_TRUE(array.empty());

    constexpr char data[8] = "Test";
    array.pushBack(data);

    ASSERT_TRUE(array.size() == 1);
    ASSERT_TRUE(strncmp(array[0], data, 8) == 0);
}

TEST(AxrArray, PushBackConstCharPtr) {
    constexpr uint32_t capacity = 16;
    AxrArray<const char*, capacity> array;
    ASSERT_TRUE(array.empty());

    array.pushBack("Test");

    ASSERT_TRUE(array.size() == 1);
    ASSERT_TRUE(strcmp(array[0], "Test") == 0);
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
    ASSERT_TRUE(array.empty());

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    ASSERT_TRUE(array.size() == capacity);

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
    ASSERT_TRUE(array.empty());

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    const size_t size = array.size();
    ASSERT_TRUE(size == capacity);

    array.pushBack(TestData{.value = 99});

    ASSERT_TRUE(array.size() == size);
    ASSERT_TRUE(array[capacity - 1].value == testData[capacity - 1].value);
}

TEST(AxrArray, PopBack_1) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData{.value = 43};

    constexpr uint32_t capacity = 16;
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.empty());

    array.pushBack(testData);

    ASSERT_TRUE(array.size() == 1);

    array.popBack();

    ASSERT_TRUE(array.empty());
}

TEST(AxrArray, PopBack_2) {
    struct TestData {
        uint32_t value;
    };

    constexpr TestData testData{.value = 43};

    constexpr uint32_t capacity = 16;
    AxrArray<TestData, capacity> array;
    ASSERT_TRUE(array.empty());

    array.pushBack(testData);
    array.pushBack(testData);
    array.pushBack(testData);

    ASSERT_TRUE(array.size() == 3);

    array.popBack();

    ASSERT_TRUE(array.size() == 2);
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
    ASSERT_TRUE(array.empty());

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    ASSERT_TRUE(array.size() == capacity);

    array.clear();

    ASSERT_TRUE(array.empty());
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
    ASSERT_TRUE(array.empty());

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    ASSERT_TRUE(array.size() == capacity);

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
    ASSERT_TRUE(array.empty());

    for (const TestData& i : testData) {
        array.pushBack(i);
    }

    ASSERT_TRUE(array.size() == capacity);

    ASSERT_TRUE(array.at(16) == nullptr);
}

TEST(AxrArray, FindFirst_Exists) {
    struct TestData {
        uint32_t value;

        bool operator==(const TestData& other) const {
            return value == other.value;
        }
    };

    constexpr uint32_t capacity = 16;
    AxrArray<TestData, capacity> array;

    constexpr TestData testData{
        .value = 10,
    };
    array.pushBack(TestData{
        .value = 1,
    });
    array.pushBack(testData);
    array.pushBack(TestData{
        .value = 2,
    });

    const auto iterator = array.findFirst(testData);
    ASSERT_TRUE(iterator != array.end());
    if (iterator != array.end()) {
        ASSERT_TRUE(iterator->value == testData.value);
    }
}

TEST(AxrArray, FindFirst_DoesntExist) {
    struct TestData {
        uint32_t value;

        bool operator==(const TestData& other) const {
            return value == other.value;
        }
    };

    constexpr uint32_t capacity = 16;
    AxrArray<TestData, capacity> array;

    array.pushBack(TestData{
        .value = 1,
    });
    array.pushBack(TestData{
        .value = 15,
    });
    array.pushBack(TestData{
        .value = 2,
    });

    const auto iterator = array.findFirst(TestData{.value = 10});
    ASSERT_TRUE(iterator == array.end());
}
