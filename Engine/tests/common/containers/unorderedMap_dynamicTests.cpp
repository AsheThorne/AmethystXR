// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/containers/unorderedMap_dynamic.h"

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

TEST(AxrUnorderedMap_Dynamic, Initialization) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 64;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    const Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.capacity() == capacity);
    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty());
}

TEST(AxrUnorderedMap_Dynamic, Insert_One) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    constexpr TestData testData{.value = 43};

    Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.empty());

    const Map_T::Iterator it = map.insert("Key", testData);

    ASSERT_TRUE(strcmp((*it).first, "Key") == 0);
    ASSERT_TRUE((*it).second.value == testData.value);
    ASSERT_TRUE(map.size() == 1);
}

TEST(AxrUnorderedMap_Dynamic, Insert_ConstCharPtr) {
    initializeHandlesAllocator(1);

    using TestData = const char*;
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.empty());

    const Map_T::Iterator it = map.insert("Key", "Test");

    ASSERT_TRUE(strcmp((*it).first, "Key") == 0);
    ASSERT_TRUE(strcmp((*it).second, "Test") == 0);
    ASSERT_TRUE(map.size() == 1);
}

TEST(AxrUnorderedMap_Dynamic, Insert_Duplicate) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    constexpr TestData testData{.value = 43};

    Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.empty());

    Map_T::Iterator it = map.insert("Key", testData);

    ASSERT_TRUE(strcmp((*it).first, "Key") == 0);
    ASSERT_TRUE((*it).second.value == testData.value);

    it = map.insert("Key", testData);
    ASSERT_TRUE(it == map.end());
}

TEST(AxrUnorderedMap_Dynamic, Insert_All) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;

    constexpr std::pair<const char*, TestData> testData[16]{
        std::pair("Test1", TestData{.value = 1}),
        std::pair("Test2", TestData{.value = 63}),
        std::pair("Test3", TestData{.value = 80}),
        std::pair("Test4", TestData{.value = 51}),
        std::pair("Test5", TestData{.value = 287}),
        std::pair("Test6", TestData{.value = 6}),
        std::pair("Test7", TestData{.value = 5}),
        std::pair("Test8", TestData{.value = 987}),
        std::pair("Test9", TestData{.value = 12}),
        std::pair("Test10", TestData{.value = 43}),
        std::pair("Test11", TestData{.value = 16}),
        std::pair("Test12", TestData{.value = 27}),
        std::pair("Test13", TestData{.value = 49}),
        std::pair("Test14", TestData{.value = 91}),
        std::pair("Test15", TestData{.value = 37}),
        std::pair("Test16", TestData{.value = 88}),
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.empty());

    for (const auto& i : testData) {
        const Map_T::Iterator it = map.insert(i.first, i.second);
        ASSERT_TRUE(strcmp((*it).first, i.first) == 0);
        ASSERT_TRUE((*it).second.value == i.second.value);
    }

    ASSERT_TRUE(map.size() == capacity);
}

TEST(AxrUnorderedMap_Dynamic, Insert_TooMany) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;

    constexpr std::pair<const char*, TestData> testData[16]{
        std::pair("Test1", TestData{.value = 1}),
        std::pair("Test2", TestData{.value = 63}),
        std::pair("Test3", TestData{.value = 80}),
        std::pair("Test4", TestData{.value = 51}),
        std::pair("Test5", TestData{.value = 287}),
        std::pair("Test6", TestData{.value = 6}),
        std::pair("Test7", TestData{.value = 5}),
        std::pair("Test8", TestData{.value = 987}),
        std::pair("Test9", TestData{.value = 12}),
        std::pair("Test10", TestData{.value = 43}),
        std::pair("Test11", TestData{.value = 16}),
        std::pair("Test12", TestData{.value = 27}),
        std::pair("Test13", TestData{.value = 49}),
        std::pair("Test14", TestData{.value = 91}),
        std::pair("Test15", TestData{.value = 37}),
        std::pair("Test16", TestData{.value = 88}),
    };

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.empty());

    for (const auto& i : testData) {
        map.insert(i.first, i.second);
    }

    const size_t size = map.size();
    ASSERT_TRUE(size == capacity);

    const Map_T::Iterator it = map.insert("Test17", TestData{.value = 99});

    ASSERT_TRUE(it == map.end());
    ASSERT_TRUE(map.size() == size);
}

TEST(AxrUnorderedMap_Dynamic, AutoDeallocation) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
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
        Map_T map(capacity, &allocator);
        ASSERT_TRUE(!allocator.empty());
    }

    ASSERT_TRUE(allocator.empty());
}

TEST(AxrUnorderedMap_Dynamic, Remove1) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 16;

    constexpr TestData testData{.value = 43};

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.empty());

    map.insert("Test", testData);

    ASSERT_TRUE(map.size() == 1);

    map.remove("Test");

    ASSERT_TRUE(map.empty());
}

TEST(AxrUnorderedMap_Dynamic, Remove2) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 16;

    constexpr TestData testData{.value = 43};

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.empty());

    map.insert("Test1", testData);
    map.insert("Test2", testData);
    map.insert("Test3", testData);
    map.insert("Test4", testData);
    map.insert("Test5", testData);
    map.insert("Test6", testData);
    map.insert("Test7", testData);
    map.insert("Test8", testData);
    map.insert("Test9", testData);
    map.insert("Test10", testData);
    map.insert("Test11", testData);
    map.insert("Test12", testData);
    map.insert("Test13", testData);
    map.insert("Test14", testData);
    map.insert("Test15", testData);
    map.insert("Test16", testData);

    ASSERT_TRUE(map.size() == 16);

    map.remove("Test1");

    ASSERT_TRUE(map.size() == 15);
}

TEST(AxrUnorderedMap_Dynamic, Clear) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;

    constexpr std::pair<const char*, TestData> testData[16]{
        std::pair("Test1", TestData{.value = 1}),
        std::pair("Test2", TestData{.value = 63}),
        std::pair("Test3", TestData{.value = 80}),
        std::pair("Test4", TestData{.value = 51}),
        std::pair("Test5", TestData{.value = 287}),
        std::pair("Test6", TestData{.value = 6}),
        std::pair("Test7", TestData{.value = 5}),
        std::pair("Test8", TestData{.value = 987}),
        std::pair("Test9", TestData{.value = 12}),
        std::pair("Test10", TestData{.value = 43}),
        std::pair("Test11", TestData{.value = 16}),
        std::pair("Test12", TestData{.value = 27}),
        std::pair("Test13", TestData{.value = 49}),
        std::pair("Test14", TestData{.value = 91}),
        std::pair("Test15", TestData{.value = 37}),
        std::pair("Test16", TestData{.value = 88}),
    };

    constexpr uint32_t capacity = 32;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);
    ASSERT_TRUE(map.empty());

    for (const auto& i : testData) {
        map.insert(i.first, i.second);
    }

    ASSERT_TRUE(map.size() == 16);

    map.clear();

    ASSERT_TRUE(map.empty());
}

TEST(AxrUnorderedMap_Dynamic, Find_Exists) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;

        bool operator==(const TestData& other) const {
            return value == other.value;
        }
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);

    constexpr TestData testData{
        .value = 10,
    };
    map.insert("Test1", testData);
    map.insert("Test2", testData);
    map.insert("Test3", testData);

    const auto iterator = map.find("Test1");
    ASSERT_TRUE(iterator != map.end());
    if (iterator != map.end()) {
        ASSERT_TRUE((*iterator).second.value == testData.value);
    }
}

TEST(AxrUnorderedMap_Dynamic, Find_DoesntExist) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;

        bool operator==(const TestData& other) const {
            return value == other.value;
        }
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);

    constexpr TestData testData{
        .value = 10,
    };
    map.insert("Test1", testData);
    map.insert("Test2", testData);
    map.insert("Test3", testData);

    const auto iterator = map.find("Test4");
    ASSERT_TRUE(iterator == map.end());
}

TEST(AxrUnorderedMap_Dynamic, FindAfterDelete_Exists) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;

        bool operator==(const TestData& other) const {
            return value == other.value;
        }
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;

    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);

    constexpr TestData testData{
        .value = 10,
    };
    map.insert("Test1", testData);
    map.insert("Test2", testData);
    map.insert("Test3", testData);
    map.insert("Test4", testData);
    map.insert("Test5", testData);
    map.insert("Test6", testData);
    map.insert("Test7", testData);
    map.insert("Test8", testData);
    map.insert("Test9", testData);
    map.insert("Test10", testData);
    map.insert("Test11", testData);
    map.insert("Test12", testData);
    map.insert("Test13", testData);
    map.insert("Test14", testData);
    map.insert("Test15", testData);
    map.insert("Test16", testData);

    // Note (Ashe): Test1, Test9 and Test16 should all desire to be in the same index. So deleting Test1 should shift
    // back Test9 and Test16.
    map.remove("Test1");

    auto iterator = map.find("Test9");
    ASSERT_TRUE(iterator != map.end());
    if (iterator != map.end()) {
        ASSERT_TRUE(strcmp((*iterator).first, "Test9") == 0);
        ASSERT_TRUE((*iterator).second.value == testData.value);
    }
    iterator = map.find("Test16");
    ASSERT_TRUE(iterator != map.end());
    if (iterator != map.end()) {
        ASSERT_TRUE(strcmp((*iterator).first, "Test16") == 0);
        ASSERT_TRUE((*iterator).second.value == testData.value);
    }
}

TEST(AxrUnorderedMap_Dynamic, FindAfterDelete_DoesntExist) {
    initializeHandlesAllocator(1);

    struct TestData {
        uint32_t value;

        bool operator==(const TestData& other) const {
            return value == other.value;
        }
    };
    using Map_T = AxrUnorderedMap_Dynamic<const char*, TestData>;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Map_T::getItemSize() * capacity) + Map_T::getItemAlignment() +
                                     sizeof(AxrDynamicAllocator::DataHeader);
    void* memory = malloc(allocatorSize);
    AxrDynamicAllocator allocator(
        AxrMemoryBlock{
            .Memory = memory,
            .Size = allocatorSize,
            .Deallocator = callback,
        },
        &HandlesAllocator);

    Map_T map(capacity, &allocator);

    constexpr TestData testData{
        .value = 10,
    };
    map.insert("Test1", testData);
    map.insert("Test2", testData);
    map.insert("Test3", testData);

    map.remove("Test1");

    const auto iterator = map.find("Test1");
    ASSERT_TRUE(iterator == map.end());
}
