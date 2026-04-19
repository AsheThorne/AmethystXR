// ReSharper disable CppDFAMemoryLeak

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/string/path.h"

// ----------------------------------------- //
// Macros
// ----------------------------------------- //

#ifdef AXR_PLATFORM_WIN32
#define AXR_PATH_SEPARATOR u8"\\"
#else
#define AXR_PATH_SEPARATOR u8"/"
#endif

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

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(AxrPath, Initialization) {
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

    constexpr char testArray[]{"シ Hello"};
    const auto testCharPtr = "シ Hello";

    const auto testData = "シ Hello";
    const AxrPath path(testData, &allocator);

    ASSERT_TRUE(!path.empty());
    ASSERT_TRUE(path.size() == std::char_traits<char>::length(testData));
    ASSERT_TRUE(path == u8"シ Hello");
    ASSERT_TRUE(path == testArray);
    ASSERT_TRUE(path == testCharPtr);
    ASSERT_TRUE(path != "シ Hellooooo");
    ASSERT_TRUE(path != "シ He");
    ASSERT_TRUE(path != "Hello");
}

TEST(AxrPath, Append_AxrPath) {
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

    AxrPath path1(u8"Hello/", &allocator);
    const AxrPath path2(u8"World.txt", &allocator);

    path1.append(path2);
    ASSERT_TRUE(path1 == u8"Hello" AXR_PATH_SEPARATOR "World.txt");
}

TEST(AxrPath, AppendString_AxrString) {
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

    AxrPath path(u8"Hello/", &allocator);
    const AxrString string(u8"World", &allocator);

    path.appendString(string);
    ASSERT_TRUE(path == u8"Hello" AXR_PATH_SEPARATOR u8"World");
}

TEST(AxrPath, AppendString_Array) {
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

    AxrPath path(u8"Hello/", &allocator);
    constexpr char8_t string[]{u8"World"};

    path.appendString(string);
    ASSERT_TRUE(path == u8"Hello" AXR_PATH_SEPARATOR u8"World");
}

TEST(AxrPath, AppendString_Char8Ptr) {
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

    AxrPath path(u8"Hello/", &allocator);
    const auto string = u8"World";

    path.appendString(string);
    ASSERT_TRUE(path == u8"Hello" AXR_PATH_SEPARATOR u8"World");
}

TEST(AxrPath, AppendString_CharPtr) {
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

    AxrPath path(u8"Hello/", &allocator);
    const auto string = "World";

    path.appendString(string);
    ASSERT_TRUE(path == u8"Hello" AXR_PATH_SEPARATOR u8"World");
}

TEST(AxrPath, AppendPath_Array) {
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

    AxrPath path1(u8"Hello/", &allocator);
    constexpr char8_t path2[]{u8"World.txt"};

    path1.appendPath(path2);
    ASSERT_TRUE(path1 == u8"Hello" AXR_PATH_SEPARATOR u8"World.txt");
}

TEST(AxrPath, AppendPath_Char8Ptr) {
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

    AxrPath path1(u8"Hello/", &allocator);
    const auto path2 = u8"World.txt";

    path1.appendPath(path2);
    ASSERT_TRUE(path1 == u8"Hello" AXR_PATH_SEPARATOR u8"World.txt");
}

TEST(AxrPath, AppendPath_CharPtr) {
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

    AxrPath path1(u8"Hello/", &allocator);
    const auto path2 = "World.txt";

    path1.appendPath(path2);
    ASSERT_TRUE(path1 == u8"Hello" AXR_PATH_SEPARATOR u8"World.txt");
}