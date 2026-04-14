// ReSharper disable CppDFAMemoryLeak

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/string/string.h"

// ----------------------------------------- //
// Shared Data
// ----------------------------------------- //

static AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node> HandlesAllocator;

// ----------------------------------------- //
// Shared Functions
// ----------------------------------------- //

static void deallocateCallback(void*& memory) {
    free(memory);
    memory = nullptr;
};

static void initializeHandlesAllocator(const uint32_t maxHandleCount) {
    AxrDeallocateBlock handlesDeallocator;
    handlesDeallocator.connect<deallocateCallback>();

    const size_t allocatorSize = AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node>::getAllocatorSize(
        maxHandleCount);

    const AxrMemoryBlock memoryBlock{
        .Memory = malloc(allocatorSize),
        .Size = allocatorSize,
        .Deallocator = handlesDeallocator,
    };

    HandlesAllocator = AxrPoolAllocator<AxrDynamicAllocator::HandlesTree_T::Node>(memoryBlock);
}

static bool testStringIsInHeapMemory(const AxrString& string) {
    return string.m_IsHeapAllocated;
}

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(AxrString, Initialization) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    constexpr char8_t testArray[]{u8"Test"};
    const auto testChar8Ptr = u8"Test";
    const AxrString string(u8"Test", &allocator);
    ASSERT_TRUE(!string.empty());
    ASSERT_TRUE(string.size() == 4);
    ASSERT_TRUE(string == u8"Test");
    ASSERT_TRUE(string == testArray);
    ASSERT_TRUE(string == testChar8Ptr);
}

TEST(AxrString, CopyAxrString) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string1(&allocator);
    const AxrString string2(u8"Hello World! シ", &allocator);

    string1.copy(string2);
    ASSERT_TRUE(!string1.empty());
    ASSERT_TRUE(string1 == string2);
}

TEST(AxrString, Assignment_Array) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    constexpr char8_t testData[] = u8"Hello World! シ";

    string = testData;
    ASSERT_TRUE(!string.empty());
    ASSERT_TRUE(string == testData);
}

TEST(AxrString, Assignment_Char8Ptr) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    const auto testData = u8"Hello World! シ";

    string = testData;
    ASSERT_TRUE(!string.empty());
    ASSERT_TRUE(string == testData);
}

TEST(AxrString, Assignment_StackMemory) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    const auto testData = u8"シ";

    string = testData;
    ASSERT_TRUE(!string.empty());
    ASSERT_TRUE(string == testData);
    ASSERT_FALSE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Assignment_HeapMemory) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    const auto testData = u8"Hello World! Testing large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";

    string = testData;
    ASSERT_TRUE(!string.empty());
    ASSERT_TRUE(string == testData);
    ASSERT_TRUE(testStringIsInHeapMemory(string));
}

TEST(AxrString, BuildString) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    auto char8PtrString = u8"Hello ";
    constexpr char8_t arrayString[]{u8"World!"};
    string.buildString(char8PtrString, arrayString, u8" シ");

    ASSERT_TRUE(!string.empty());
    ASSERT_TRUE(string == u8"Hello World! シ");
}

TEST(AxrString, Reallocate_Assignment_StackToStack) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const auto testData1 = u8"Hello";
    AxrString string(testData1, &allocator);
    ASSERT_TRUE(string == testData1);
    ASSERT_FALSE(testStringIsInHeapMemory(string));

    const auto testData2 = u8"World";
    string = testData2;
    ASSERT_TRUE(string == testData2);
    ASSERT_FALSE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Reallocate_Assignment_StackToHeap) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const auto testData1 = u8"Hello";
    AxrString string(testData1, &allocator);
    ASSERT_TRUE(string == testData1);
    ASSERT_FALSE(testStringIsInHeapMemory(string));

    const auto testData2 = u8"Hello World! Testing large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";
    string = testData2;
    ASSERT_TRUE(string == testData2);
    ASSERT_TRUE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Reallocate_Assignment_HeapToStack) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const auto testData1 = u8"Hello World! Testing large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";
    AxrString string(testData1, &allocator);
    ASSERT_TRUE(string == testData1);
    ASSERT_TRUE(testStringIsInHeapMemory(string));

    const auto testData2 = u8"Hello";
    string = testData2;
    ASSERT_TRUE(string == testData2);
    // Even though we assigned a string which should fit on the stack, we won't actually use stack memory. We will just
    // use the existing heap memory.
    ASSERT_TRUE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Reallocate_Assignment_HeapToHeap) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const auto testData1 = u8"Hello World! Testing large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";
    AxrString string(testData1, &allocator);
    ASSERT_TRUE(string == testData1);
    ASSERT_TRUE(testStringIsInHeapMemory(string));

    const auto testData2 = u8"Hello World! Testing extra large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";
    string = testData2;
    ASSERT_TRUE(string == testData2);
    ASSERT_TRUE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Reallocate_BuildString_StackToStack) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    const auto testData1 = u8"Hello";
    string.buildString(testData1);
    ASSERT_TRUE(string == testData1);
    ASSERT_FALSE(testStringIsInHeapMemory(string));

    const auto testData2 = u8"World";
    string.buildString(testData2);
    ASSERT_TRUE(string == testData2);
    ASSERT_FALSE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Reallocate_BuildString_StackToHeap) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    const auto testData1 = u8"Hello";
    string.buildString(testData1);
    ASSERT_TRUE(string == testData1);
    ASSERT_FALSE(testStringIsInHeapMemory(string));

    const auto testData2 = u8"Hello World! Testing large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";
    string.buildString(testData2);
    ASSERT_TRUE(string == testData2);
    ASSERT_TRUE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Reallocate_BuildString_HeapToStack) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    const auto testData1 = u8"Hello World! Testing large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";
    string.buildString(testData1);
    ASSERT_TRUE(string == testData1);
    ASSERT_TRUE(testStringIsInHeapMemory(string));

    const auto testData2 = u8"Hello";
    string.buildString(testData2);
    ASSERT_TRUE(string == testData2);
    // Even though we assigned a string which should fit on the stack, we won't actually use stack memory. We will just
    // use the existing heap memory.
    ASSERT_TRUE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Reallocate_BuildString_HeapToHeap) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(&allocator);
    const auto testData1 = u8"Hello World! Testing large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";
    string.buildString(testData1);
    ASSERT_TRUE(string == testData1);
    ASSERT_TRUE(testStringIsInHeapMemory(string));

    const auto testData2 = u8"Hello World! Testing extra large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆";
    string.buildString(testData2);
    ASSERT_TRUE(string == testData2);
    ASSERT_TRUE(testStringIsInHeapMemory(string));
}

TEST(AxrString, Append_AxrString) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string1(u8"Hello", &allocator);
    const AxrString string2(u8" World!", &allocator);

    string1.append(string2);
    ASSERT_TRUE(string1 == u8"Hello World!");
}

TEST(AxrString, Append_Array) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string1(u8"Hello", &allocator);
    constexpr char8_t string2[]{u8" World!"};

    string1.append(string2);
    ASSERT_TRUE(string1 == u8"Hello World!");
}

TEST(AxrString, Append_Char8Ptr) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string1(u8"Hello", &allocator);
    const auto string2 = u8" World!";

    string1.append(string2);
    ASSERT_TRUE(string1 == u8"Hello World!");
}

TEST(AxrString, ForLoop_Increment) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const auto testData = u8"シ Hello 🗸 😃";
    AxrString string(testData, &allocator);

    uint32_t index = 0;
    for (AxrArray character : string) {
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

TEST(AxrString, ForLoop_Decrement) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const auto testData = u8"シ Hello 🗸 😃";
    AxrString string(testData, &allocator);

    uint32_t index = 0;
    for (AxrString::Iterator begin = string.end(), end = string.begin(); begin != end;
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

TEST(AxrString, Substring_Index) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const AxrString string(u8"Hello World!", &allocator);
    const AxrString substring = string.substring(6, 7);

    ASSERT_TRUE(substring == u8"World!");
}

TEST(AxrString, Substring_Iterator) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const AxrString string(u8"Hello World!", &allocator);

    AxrString::Iterator substringStart;
    AxrString::Iterator substringEnd;
    for (AxrString::Iterator begin = string.begin(), end = string.end(); begin != end; ++begin) {
        const AxrArray<char8_t, 4> character = *begin;
        if (character == 'W') {
            substringStart = begin;
        } else if (character == '!') {
            substringEnd = begin;
        }
    }

    const AxrString substring = string.substring(substringStart, substringEnd);

    ASSERT_TRUE(substring == u8"World");
}

TEST(AxrString, AutoDeallocation) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);
    ASSERT_TRUE(allocator.empty());

    {
        AxrString string(u8"Hello World! Testing large string シベㄯㆨ㉄ 😃🙉🗸🕪😺🙏🚓🛑🠵🢆", &allocator);
        ASSERT_TRUE(!allocator.empty());
    }

    ASSERT_TRUE(allocator.empty());
}

TEST(AxrString, Pop_CharacterCount) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(u8"Hello World!", &allocator);
    string.pop(7);
    ASSERT_TRUE(string == u8"Hello");
}

TEST(AxrString, Pop_Iterator) {
    initializeHandlesAllocator(2);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(u8"Hello World!", &allocator);

    AxrString::Iterator iterator;
    for (AxrString::Iterator begin = string.begin(), end = string.end(); begin != end; ++begin) {
        const AxrArray<char8_t, 4> character = *begin;
        if (character == 'o') {
            // Increase the iterator before setting it because we want to chop off everything AFTER this character.
            iterator = ++begin;
            break;
        }
    }

    string.pop(iterator);
    ASSERT_TRUE(string == u8"Hello");
}

TEST(AxrString, Clear) {
    initializeHandlesAllocator(1);

    constexpr uint32_t capacity = 256;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(char8_t) * capacity) + alignof(char8_t) +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    AxrString string(u8"Hello World! シ", &allocator);
    ASSERT_TRUE(!string.empty());

    string.clear();

    ASSERT_TRUE(string.empty());
}
