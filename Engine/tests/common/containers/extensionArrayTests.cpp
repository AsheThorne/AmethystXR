// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/containers/extensionArray.h"

// ----------------------------------------- //
// Types
// ----------------------------------------- //

enum TestDataTypeEnum {
    TEST_DATA_TYPE_UNDEFINED,
    TEST_DATA_TYPE_EXTENSION1,
    TEST_DATA_TYPE_EXTENSION2,
};

struct TestDataExtension1 {
    uint32_t value;
};

struct TestDataExtension2 {
    uint32_t value;
};

struct TestData {
    union {
        TestDataExtension1 Extension1;
        TestDataExtension2 Extension2;
    };
    TestDataTypeEnum Type;
};

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(AxrExtensionArray, PushBackOne) {
    constexpr uint32_t capacity = 2;
    AxrExtensionArray<TestData, capacity> array;

    ASSERT_TRUE(array.empty());

    array.pushBack(TestData{
        .Extension1 =
            TestDataExtension1{
                .value = 10,
            },
        .Type = TEST_DATA_TYPE_EXTENSION1,
    });

    ASSERT_TRUE(array.size() == 1);
    ASSERT_TRUE(array[0].Extension1.value == 10);
}

TEST(AxrExtensionArray, PushBackDuplicate) {
    constexpr uint32_t capacity = 2;
    AxrExtensionArray<TestData, capacity> array;

    ASSERT_TRUE(array.empty());

    array.pushBack(TestData{
        .Extension1 =
            TestDataExtension1{
                .value = 10,
            },
        .Type = TEST_DATA_TYPE_EXTENSION1,
    });

    array.pushBack(TestData{
        .Extension1 =
            TestDataExtension1{
                .value = 34,
            },
        .Type = TEST_DATA_TYPE_EXTENSION1,
    });

    ASSERT_TRUE(array.size() == 1);
    ASSERT_TRUE(array[0].Extension1.value == 34);
}

TEST(AxrExtensionArray, Find_Exists) {
    constexpr uint32_t capacity = 2;
    AxrExtensionArray<TestData, capacity> array;

    array.pushBack(TestData{
        .Extension1 =
            TestDataExtension1{
                .value = 10,
            },
        .Type = TEST_DATA_TYPE_EXTENSION1,
    });

    array.pushBack(TestData{
        .Extension2 =
            TestDataExtension2{
                .value = 34,
            },
        .Type = TEST_DATA_TYPE_EXTENSION2,
    });

    const auto iterator = array.find(TEST_DATA_TYPE_EXTENSION1);
    ASSERT_TRUE(iterator != array.end());
    if (iterator != array.end()) {
        ASSERT_TRUE(iterator->Extension1.value == 10);
    }
}

TEST(AxrExtensionArray, Find_DoesntExist) {
    constexpr uint32_t capacity = 2;
    AxrExtensionArray<TestData, capacity> array;

    array.pushBack(TestData{
        .Extension2 =
            TestDataExtension2{
                .value = 34,
            },
        .Type = TEST_DATA_TYPE_EXTENSION2,
    });

    const auto iterator = array.find(TEST_DATA_TYPE_EXTENSION1);
    ASSERT_TRUE(iterator == array.end());
}

TEST(AxrExtensionArray, Exists_True) {
    constexpr uint32_t capacity = 2;
    AxrExtensionArray<TestData, capacity> array;

    array.pushBack(TestData{
        .Extension1 =
            TestDataExtension1{
                .value = 10,
            },
        .Type = TEST_DATA_TYPE_EXTENSION1,
    });

    ASSERT_TRUE(array.exists(TEST_DATA_TYPE_EXTENSION1));
}

TEST(AxrExtensionArray, Exists_False) {
    constexpr uint32_t capacity = 2;
    AxrExtensionArray<TestData, capacity> array;

    array.pushBack(TestData{
        .Extension1 =
            TestDataExtension1{
                .value = 10,
            },
        .Type = TEST_DATA_TYPE_EXTENSION1,
    });

    ASSERT_FALSE(array.exists(TEST_DATA_TYPE_EXTENSION2));
}
