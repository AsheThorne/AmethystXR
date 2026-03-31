// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "utils.h"

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(Containers_Utils, CallDestructor_Basic) {
    struct TestData {
        uint32_t Value{};

        ~TestData() {
            Value = 1;
        }
    };

    TestData testData{
        .Value = 22,
    };
    ASSERT_TRUE(testData.Value == 22);

    axrCallDestructor(testData);

    ASSERT_TRUE(testData.Value == 1);
}

TEST(Containers_Utils, CallDestructor_Array) {
    struct TestData {
        uint32_t Value{};

        ~TestData() {
            Value = 1;
        }
    };

    TestData testData[5] = {
        TestData{
            .Value = 22,
        },
        TestData{
            .Value = 8,
        },
        TestData{
            .Value = 23,
        },
        TestData{
            .Value = 9,
        },
        TestData{
            .Value = 7,
        },
    };
    ASSERT_TRUE(testData[0].Value == 22);
    ASSERT_TRUE(testData[1].Value == 8);
    ASSERT_TRUE(testData[2].Value == 23);
    ASSERT_TRUE(testData[3].Value == 9);
    ASSERT_TRUE(testData[4].Value == 7);

    axrCallDestructor(testData);

    ASSERT_TRUE(testData[0].Value == 1);
    ASSERT_TRUE(testData[1].Value == 1);
    ASSERT_TRUE(testData[2].Value == 1);
    ASSERT_TRUE(testData[3].Value == 1);
    ASSERT_TRUE(testData[4].Value == 1);
}

TEST(Containers_Utils, CallDestructor_2DimensionArrays) {
    struct TestData {
        uint32_t Value{};

        ~TestData() {
            Value = 1;
        }
    };

    TestData testData[2][2] = {
        {
            TestData{
                .Value = 22,
            },
            TestData{
                .Value = 8,
            },
        },
        {
            TestData{
                .Value = 23,
            },
            TestData{
                .Value = 9,
            },
        },
    };
    ASSERT_TRUE(testData[0][0].Value == 22);
    ASSERT_TRUE(testData[0][1].Value == 8);
    ASSERT_TRUE(testData[1][0].Value == 23);
    ASSERT_TRUE(testData[1][1].Value == 9);

    axrCallDestructor(testData);

    ASSERT_TRUE(testData[0][0].Value == 1);
    ASSERT_TRUE(testData[0][1].Value == 1);
    ASSERT_TRUE(testData[1][0].Value == 1);
    ASSERT_TRUE(testData[1][1].Value == 1);
}
