// ReSharper disable CppDFAMemoryLeak

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/string/stringView.h"

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(AxrStringView, Initialization) {
    constexpr char8_t testArray[]{u8"シ Hello"};
    const auto testChar8Ptr = u8"シ Hello";

    const auto testData = u8"シ Hello World!";
    const size_t stringSize = std::char_traits<char8_t>::length(testData) - 7;
    const AxrStringView stringView(testData, stringSize);

    ASSERT_TRUE(!stringView.empty());
    ASSERT_TRUE(stringView.size() == stringSize);
    ASSERT_TRUE(stringView == u8"シ Hello");
    ASSERT_TRUE(stringView == testArray);
    ASSERT_TRUE(stringView == testChar8Ptr);
}

TEST(AxrStringView, CopyAxrStringView) {
    const auto testData = u8"Hello World! シ";
    AxrStringView stringView1;
    const AxrStringView stringView2(testData, std::char_traits<char8_t>::length(testData));

    stringView1 = stringView2;
    ASSERT_TRUE(!stringView1.empty());
    ASSERT_TRUE(stringView1 == stringView2);
}

TEST(AxrStringView, ForLoop_Increment) {
    const auto testData = u8"シ Hello 🗸 😃";
    const AxrStringView stringView(testData, std::char_traits<char8_t>::length(testData));

    uint32_t index = 0;
    for (AxrArray character : stringView) {
        switch (index) {
            case 0: {
                ASSERT_TRUE(character == u8"シ");
                break;
            }
            case 1: {
                ASSERT_TRUE(character == ' ');
                break;
            }
            case 2: {
                ASSERT_TRUE(character == 'H');
                break;
            }
            case 3: {
                ASSERT_TRUE(character == 'e');
                break;
            }
            case 4: {
                ASSERT_TRUE(character == 'l');
                break;
            }
            case 5: {
                ASSERT_TRUE(character == 'l');
                break;
            }
            case 6: {
                ASSERT_TRUE(character == 'o');
                break;
            }
            case 7: {
                ASSERT_TRUE(character == ' ');
                break;
            }
            case 8: {
                ASSERT_TRUE(character == u8"🗸");
                break;
            }
            case 9: {
                ASSERT_TRUE(character == ' ');
                break;
            }
            case 10: {
                ASSERT_TRUE(character == u8"😃");
                break;
            }
            default: {
                // We shouldn't get here
                ASSERT_TRUE(false);
                break;
            }
        }
        ++index;
    }

    // Make sure we reached the end and checked each value
    ASSERT_TRUE(index == 11);
}

TEST(AxrStringView, ForLoop_Decrement) {
    const auto testData = u8"シ Hello 🗸 😃";
    const AxrStringView stringView(testData, std::char_traits<char8_t>::length(testData));

    uint32_t index = 0;
    for (AxrStringView::Iterator begin = stringView.end(), end = stringView.begin(); begin != end;
         /* Don't decrement here. It's done inside the loop*/) {
        --begin;
        const AxrArray<char8_t, 4> character = *begin;
        switch (index) {
            case 10: {
                ASSERT_TRUE(character == u8"シ");
                break;
            }
            case 9: {
                ASSERT_TRUE(character == ' ');
                break;
            }
            case 8: {
                ASSERT_TRUE(character == 'H');
                break;
            }
            case 7: {
                ASSERT_TRUE(character == 'e');
                break;
            }
            case 6: {
                ASSERT_TRUE(character == 'l');
                break;
            }
            case 5: {
                ASSERT_TRUE(character == 'l');
                break;
            }
            case 4: {
                ASSERT_TRUE(character == 'o');
                break;
            }
            case 3: {
                ASSERT_TRUE(character == ' ');
                break;
            }
            case 2: {
                ASSERT_TRUE(character == u8"🗸");
                break;
            }
            case 1: {
                ASSERT_TRUE(character == ' ');
                break;
            }
            case 0: {
                ASSERT_TRUE(character == u8"😃");
                break;
            }
            default: {
                // We shouldn't get here
                ASSERT_TRUE(false);
                break;
            }
        }
        ++index;
    }

    // Make sure we reached the end and checked each value
    ASSERT_TRUE(index == 11);
}

TEST(AxrStringView, Substring_Index) {
    const auto testData = u8"Hello World! シ";
    const AxrStringView stringView(testData, std::char_traits<char8_t>::length(testData));
    const AxrStringView substring = stringView.substring(0, 5);

    ASSERT_TRUE(substring == u8"Hello");
}

TEST(AxrStringView, Substring_Iterator) {
    const auto testData = u8"Hello World! シ";
    const AxrStringView stringView(testData, std::char_traits<char8_t>::length(testData));

    AxrStringView::Iterator substringStart;
    AxrStringView::Iterator substringEnd;
    for (AxrStringView::Iterator begin = stringView.begin(), end = stringView.end(); begin != end; ++begin) {
        const AxrArray<char8_t, 4> character = *begin;
        if (character == 'W') {
            substringStart = begin;
        } else if (character == '!') {
            substringEnd = begin;
        }
    }

    const AxrStringView substring = stringView.substring(substringStart, substringEnd);

    ASSERT_TRUE(substring == u8"World");
}
