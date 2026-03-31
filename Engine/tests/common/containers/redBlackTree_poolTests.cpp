// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <gtest/gtest.h>

#include "common/containers/redBlackTree_pool.h"

// ----------------------------------------- //
// Shared Functions
// ----------------------------------------- //

static void deallocateCallback(void*& memory) {
    free(memory);
    memory = nullptr;
};

// ----------------------------------------- //
// Shared Functions
// ----------------------------------------- //

/// Run tests for the given 'testNodes' against the given 'actualNodes'.
/// @param testNodes What the actual nodes should be equal to
/// @param actualNodes The actual nodes in the tree to check against
template<typename Type>
static void testRedBlackTreeNodes(const typename AxrRedBlackTree_Pool<Type>::Node* testNodes,
                                  const typename AxrRedBlackTree_Pool<Type>::Node::Iterator actualNodes) {
    if (testNodes == nullptr) {
        ASSERT_TRUE(actualNodes.m_Node == nullptr);
        return;
    }
    // If testNode isn't null, actualNode shouldn't be either
    if (actualNodes.m_Node == nullptr) {
        ASSERT_TRUE(false);
        return;
    }

    ASSERT_TRUE(actualNodes.m_Node->Data == testNodes->Data);
    ASSERT_TRUE(actualNodes.m_Node->IsRed == testNodes->IsRed);
    testRedBlackTreeNodes<Type>(
        testNodes->Left,
        typename AxrRedBlackTree_Pool<Type>::Node::Iterator(actualNodes.m_RootNode, actualNodes.m_Node->Left));
    testRedBlackTreeNodes<Type>(
        testNodes->Right,
        typename AxrRedBlackTree_Pool<Type>::Node::Iterator(actualNodes.m_RootNode, actualNodes.m_Node->Right));
}

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(AxrRedBlackTree_Pool, Initialization) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    const Tree_T tree(&allocator);
    ASSERT_TRUE(tree.size() == 0);
    ASSERT_TRUE(tree.empty());
}

TEST(AxrRedBlackTree_Pool, Insert_One) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //          25B
    //         /   \
    //       Null   Null
    Tree_T::Node node1{};
    node1.Left = nullptr;
    node1.Right = nullptr;
    node1.Parent = nullptr;
    node1.Data = 25;
    node1.IsRed = false;

    ASSERT_FALSE(tree.exists(node1.Data));
    tree.insert(node1.Data);
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.exists(node1.Data));
    testRedBlackTreeNodes<TestData_T>(&node1, tree.find(node1.Data));
}

TEST(AxrRedBlackTree_Pool, FindNode_Exists) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);

    constexpr TestData_T testData = 10;
    tree.insert(1);
    tree.insert(testData);
    tree.insert(2);

    const Tree_T::Node::Iterator foundNode = tree.find(testData);
    if (foundNode == tree.end()) {
        ASSERT_TRUE(false);
        return;
    }
    ASSERT_TRUE(*foundNode == testData);
}

TEST(AxrRedBlackTree_Pool, FindNode_DoesntExist) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);

    tree.insert(1);
    tree.insert(15);
    tree.insert(2);

    const Tree_T::Node::Iterator foundNode = tree.find(10);
    ASSERT_TRUE(foundNode == tree.end());
}

TEST(AxrRedBlackTree_Pool, FindNextLargest) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //                                     40B
    //                        /                           \
    //                      20B                           60B
    //              /                \              /              \
    //            10B                30B          50B              80R
    //          /      \           /    \       /     \        /         \
    //        Null    Null       Null   Null   Null  Null     70B        90B
    //                                                      /   \       /    \
    //                                                    Null  Null  Null   100R
    //                                                                      /    \
    //                                                                    Null  Null

    const TestData_T testDataInsertionOrder[capacity]{
        20,
        10,
        30,
        40,
        60,
        50,
        80,
        70,
        90,
        100,
    };

    for (const TestData_T data : testDataInsertionOrder) {
        tree.insert(data);
    }

    ASSERT_TRUE(*tree.findNextLargest(39) == 40);
    // Even though 40 exists, we're looking for the next largest, which would be 50
    ASSERT_TRUE(*tree.findNextLargest(40) == 50);
    ASSERT_TRUE(*tree.findNextLargest(41) == 50);

    ASSERT_TRUE(tree.findNextLargest(200) == tree.end());
}

TEST(AxrRedBlackTree_Pool, Insert_All) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 11;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Add 12
    // Tree should look like this:
    //          12B
    //         /   \
    //       Null   Null
    Tree_T::Node node12{};
    node12.Left = nullptr;
    node12.Right = nullptr;
    node12.Parent = nullptr;
    node12.Data = 12;
    node12.IsRed = false;

    tree.insert(node12.Data);
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.exists(node12.Data));
    testRedBlackTreeNodes<TestData_T>(&node12, tree.find(node12.Data));

    // Add 20
    // Tree should look like this:
    //          12B
    //         /    \
    //       Null   20R
    //             /    \
    //           Null  Null
    Tree_T::Node node20{};
    node20.Left = nullptr;
    node20.Right = nullptr;
    node20.Parent = &node12;
    node20.Data = 20;
    node20.IsRed = true;

    node12.Right = &node20;

    tree.insert(node20.Data);
    ASSERT_TRUE(tree.size() == 2);
    ASSERT_TRUE(tree.exists(node20.Data));
    testRedBlackTreeNodes<TestData_T>(&node12, tree.find(node12.Data));

    // Add 9
    // Tree should look like this:
    //            12B
    //         /      \
    //       9R        20R
    //     /    \     /    \
    //   Null  Null  Null  Null

    Tree_T::Node node9{};
    node9.Left = nullptr;
    node9.Right = nullptr;
    node9.Parent = &node12;
    node9.Data = 9;
    node9.IsRed = true;

    node12.Left = &node9;

    tree.insert(node9.Data);
    ASSERT_TRUE(tree.size() == 3);
    ASSERT_TRUE(tree.exists(node9.Data));
    testRedBlackTreeNodes<TestData_T>(&node12, tree.find(node12.Data));

    // Add 17
    // Tree should look like this:
    //            12B
    //         /      \
    //       9B        20B
    //     /    \     /    \
    //   Null  Null  17R  Null
    //              /   \
    //            Null  Null

    Tree_T::Node node17{};
    node17.Left = nullptr;
    node17.Right = nullptr;
    node17.Parent = &node20;
    node17.Data = 17;
    node17.IsRed = true;

    node20.IsRed = false;
    node9.IsRed = false;
    node20.Left = &node17;

    tree.insert(node17.Data);
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.exists(node17.Data));
    testRedBlackTreeNodes<TestData_T>(&node12, tree.find(node12.Data));

    // Add 18
    // Tree should look like this:
    //              12B
    //         /          \
    //       9B           18B
    //     /    \      /       \
    //   Null  Null  17R       20R
    //              /   \     /   \
    //           Null  Null Null  Null

    Tree_T::Node node18{};
    node18.Left = &node17;
    node18.Right = &node20;
    node18.Parent = &node12;
    node18.Data = 18;
    node18.IsRed = false;

    node17.Parent = &node18;
    node20.Parent = &node18;
    node20.IsRed = true;
    node20.Left = nullptr;
    node12.Right = &node18;

    tree.insert(node18.Data);
    ASSERT_TRUE(tree.size() == 5);
    ASSERT_TRUE(tree.exists(node18.Data));
    testRedBlackTreeNodes<TestData_T>(&node12, tree.find(node12.Data));

    // Add 32
    // Tree should look like this:
    //              12B
    //         /          \
    //       9B           18R
    //     /    \      /       \
    //   Null  Null  17B       20B
    //              /   \     /   \
    //           Null  Null Null  32R
    //                           /    \
    //                         Null  Null

    Tree_T::Node node32{};
    node32.Left = nullptr;
    node32.Right = nullptr;
    node32.Parent = &node20;
    node32.Data = 32;
    node32.IsRed = true;

    node18.IsRed = true;
    node17.IsRed = false;
    node20.IsRed = false;
    node20.Right = &node32;

    tree.insert(node32.Data);
    ASSERT_TRUE(tree.size() == 6);
    ASSERT_TRUE(tree.exists(node32.Data));
    testRedBlackTreeNodes<TestData_T>(&node12, tree.find(node12.Data));

    // Add 27
    // Tree should look like this:
    //              12B
    //         /           \
    //       9B            18R
    //     /    \      /          \
    //   Null  Null  17B           27B
    //              /   \       /       \
    //           Null  Null  20R        32R
    //                      /   \      /    \
    //                   Null  Null  Null  Null

    Tree_T::Node node27{};
    node27.Left = &node20;
    node27.Right = &node32;
    node27.Parent = &node18;
    node27.Data = 27;
    node27.IsRed = false;

    node18.Right = &node27;
    node32.Parent = &node27;
    node20.Parent = &node27;
    node20.Right = nullptr;
    node20.IsRed = true;

    tree.insert(node27.Data);
    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.exists(node27.Data));
    testRedBlackTreeNodes<TestData_T>(&node12, tree.find(node12.Data));

    // Add 42
    // Tree should look like this:
    //                      18B
    //           /                        \
    //         12R                        27R
    //      /        \                /          \
    //    9B         17B           20B           32B
    //   /   \      /    \        /   \       /       \
    // Null  Null  Null  Null   Null  Null  Null      42R
    //                                               /    \
    //                                             Null  Null

    Tree_T::Node node42{};
    node42.Left = nullptr;
    node42.Right = nullptr;
    node42.Parent = &node32;
    node42.Data = 42;
    node42.IsRed = true;

    node18.IsRed = false;
    node18.Parent = nullptr;
    node18.Left = &node12;
    node12.Parent = &node18;
    node12.Right = &node17;
    node12.IsRed = true;
    node17.Parent = &node12;
    node27.IsRed = true;
    node20.IsRed = false;
    node32.IsRed = false;
    node32.Right = &node42;

    tree.insert(node42.Data);
    ASSERT_TRUE(tree.size() == 8);
    ASSERT_TRUE(tree.exists(node42.Data));
    testRedBlackTreeNodes<TestData_T>(&node18, tree.find(node18.Data));

    // Add 80
    // Tree should look like this:
    //                      18B
    //           /                        \
    //         12R                        27R
    //      /        \                /           \
    //    9B         17B           20B            42B
    //   /   \      /    \        /   \       /         \
    // Null  Null  Null  Null   Null  Null  32R         80R
    //                                     /   \       /    \
    //                                   Null  Null  Null  Null

    Tree_T::Node node80{};
    node80.Left = nullptr;
    node80.Right = nullptr;
    node80.Parent = &node42;
    node80.Data = 80;
    node80.IsRed = true;

    node42.Parent = &node27;
    node42.IsRed = false;
    node42.Right = &node80;
    node42.Left = &node32;
    node27.Right = &node42;
    node32.Parent = &node42;
    node32.Right = nullptr;
    node32.IsRed = true;

    tree.insert(node80.Data);
    ASSERT_TRUE(tree.size() == 9);
    ASSERT_TRUE(tree.exists(node80.Data));
    testRedBlackTreeNodes<TestData_T>(&node18, tree.find(node18.Data));

    // Add 4
    // Tree should look like this:
    //                           18B
    //                /                        \
    //              12R                        27R
    //           /        \                /           \
    //         9B         17B           20B            42B
    //        /   \      /    \        /   \       /         \
    //       4R   Null  Null  Null   Null  Null  32R         80R
    //     /    \                              /   \       /    \
    //   Null  Null                          Null  Null  Null  Null

    Tree_T::Node node4{};
    node4.Left = nullptr;
    node4.Right = nullptr;
    node4.Parent = &node9;
    node4.Data = 4;
    node4.IsRed = true;

    node9.Left = &node4;

    tree.insert(node4.Data);
    ASSERT_TRUE(tree.size() == 10);
    ASSERT_TRUE(tree.exists(node4.Data));
    testRedBlackTreeNodes<TestData_T>(&node18, tree.find(node18.Data));

    // Add 2
    // Tree should look like this:
    //                                     18B
    //                        /                           \
    //                      12R                           27R
    //              /                \             /               \
    //            4B                 17B         20B               42B
    //        /          \         /    \       /   \          /         \
    //       2R          9R      Null  Null   Null  Null     32R         80R
    //     /    \      /    \                               /   \       /    \
    //   Null  Null  Null  Null                           Null  Null  Null  Null

    Tree_T::Node node2{};
    node2.Left = nullptr;
    node2.Right = nullptr;
    node2.Parent = &node9;
    node2.Data = 2;
    node2.IsRed = true;

    node9.IsRed = true;
    node9.Parent = &node4;
    node9.Left = nullptr;
    node4.Parent = &node12;
    node4.IsRed = false;
    node4.Left = &node2;
    node4.Right = &node9;
    node12.Left = &node4;

    tree.insert(node2.Data);
    ASSERT_TRUE(tree.size() == 11);
    ASSERT_TRUE(tree.exists(node2.Data));
    testRedBlackTreeNodes<TestData_T>(&node18, tree.find(node18.Data));
}

TEST(AxrRedBlackTree_Pool, ForLoop_Increment) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 100;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    for (uint32_t i = 0; i < capacity; ++i) {
        tree.insert(i + 1);
    }

    uint32_t currentValue = 0;
    for (AxrRedBlackTree_Pool<TestData_T>::Node::Iterator begin = tree.begin(), end = tree.end(); begin != end;
         ++begin) {
        ++currentValue;
        const TestData_T& data = *begin;
        ASSERT_TRUE(data == currentValue);
    }

    // Make sure we reached the end and checked each value
    ASSERT_TRUE(currentValue == 100);
}

TEST(AxrRedBlackTree_Pool, ForLoop_Decrement) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 100;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    for (uint32_t i = 0; i < capacity; ++i) {
        tree.insert(i + 1);
    }

    uint32_t currentValue = capacity + 1;
    for (AxrRedBlackTree_Pool<TestData_T>::Node::Iterator begin = tree.end(), end = tree.begin(); begin != end;
         /* Don't decrement here. It's done inside the loop*/) {
        --begin;
        --currentValue;
        const TestData_T& data = *begin;
        ASSERT_TRUE(data == currentValue);
    }

    // Make sure we reached the end and checked each value
    ASSERT_TRUE(currentValue == 1);
}

TEST(AxrRedBlackTree_Pool, Insert_TooMany) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //                                   63B
    //                   /                                  \
    //                  6R                                 287B
    //          /                   \                  /         \
    //         1B                   43B               80R        987R
    //       /    \            /          \         /    \      /    \
    //     Null    5R         12R         51R     Null  Null  Null  Null
    //           /   \       /   \       /   \
    //         Null  Null  Null  Null  Null  Null

    Tree_T::Node node1{};
    node1.Data = 1;
    Tree_T::Node node63{};
    node63.Data = 63;
    Tree_T::Node node80{};
    node80.Data = 80;
    Tree_T::Node node51{};
    node51.Data = 51;
    Tree_T::Node node287{};
    node287.Data = 287;
    Tree_T::Node node6{};
    node6.Data = 6;
    Tree_T::Node node5{};
    node5.Data = 5;
    Tree_T::Node node987{};
    node987.Data = 987;
    Tree_T::Node node12{};
    node12.Data = 12;
    Tree_T::Node node43{};
    node43.Data = 43;

    const Tree_T::Node testDataInsertionOrder[capacity]{
        node1,
        node63,
        node80,
        node51,
        node287,
        node6,
        node5,
        node987,
        node12,
        node43,
    };

    node63.Left = &node6;
    node63.Right = &node287;
    node63.Parent = nullptr;
    node63.IsRed = false;

    node6.Left = &node1;
    node6.Right = &node43;
    node6.Parent = &node63;
    node6.IsRed = true;

    node287.Left = &node80;
    node287.Right = &node987;
    node287.Parent = &node63;
    node287.IsRed = false;

    node1.Left = nullptr;
    node1.Right = &node5;
    node1.Parent = &node6;
    node1.IsRed = false;

    node43.Left = &node12;
    node43.Right = &node51;
    node43.Parent = &node6;
    node43.IsRed = false;

    node80.Left = nullptr;
    node80.Right = nullptr;
    node80.Parent = &node287;
    node80.IsRed = true;

    node987.Left = nullptr;
    node987.Right = nullptr;
    node987.Parent = &node287;
    node987.IsRed = true;

    node5.Left = nullptr;
    node5.Right = nullptr;
    node5.Parent = &node1;
    node5.IsRed = true;

    node12.Left = nullptr;
    node12.Right = nullptr;
    node12.Parent = &node43;
    node12.IsRed = true;

    node51.Left = nullptr;
    node51.Right = nullptr;
    node51.Parent = &node43;
    node51.IsRed = true;

    for (const Tree_T::Node& node : testDataInsertionOrder) {
        tree.insert(node.Data);
    }

    const size_t size = tree.size();
    ASSERT_TRUE(size == capacity);
    testRedBlackTreeNodes<TestData_T>(&node63, tree.find(node63.Data));

    tree.insert(99);

    // Make sure the tree didn't change
    ASSERT_TRUE(tree.size() == size);
    testRedBlackTreeNodes<TestData_T>(&node63, tree.find(node63.Data));
}

TEST(AxrRedBlackTree_Pool, Remove_1) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    constexpr TestData_T testData = 43;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    tree.insert(testData);

    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.find(testData) != tree.end());

    tree.remove(testData);

    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.find(testData) == tree.end());
}

TEST(AxrRedBlackTree_Pool, Remove_All) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //                                     40B
    //                        /                           \
    //                      20B                           60B
    //              /                \              /              \
    //            10B                30B          50B              80R
    //          /      \           /    \       /     \        /         \
    //        Null    Null       Null   Null   Null  Null     70B        90B
    //                                                      /   \       /    \
    //                                                    Null  Null  Null   100R
    //                                                                      /    \
    //                                                                    Null  Null

    Tree_T::Node node40{};
    node40.Data = 40;
    Tree_T::Node node20{};
    node20.Data = 20;
    Tree_T::Node node60{};
    node60.Data = 60;
    Tree_T::Node node10{};
    node10.Data = 10;
    Tree_T::Node node30{};
    node30.Data = 30;
    Tree_T::Node node50{};
    node50.Data = 50;
    Tree_T::Node node80{};
    node80.Data = 80;
    Tree_T::Node node70{};
    node70.Data = 70;
    Tree_T::Node node90{};
    node90.Data = 90;
    Tree_T::Node node100{};
    node100.Data = 100;

    const Tree_T::Node testDataInsertionOrder[capacity]{
        node20,
        node10,
        node30,
        node40,
        node60,
        node50,
        node80,
        node70,
        node90,
        node100,
    };

    node40.Left = &node20;
    node40.Right = &node60;
    node40.Parent = nullptr;
    node40.IsRed = false;

    node20.Left = &node10;
    node20.Right = &node30;
    node20.Parent = &node40;
    node20.IsRed = false;

    node60.Left = &node50;
    node60.Right = &node80;
    node60.Parent = &node40;
    node60.IsRed = false;

    node10.Left = nullptr;
    node10.Right = nullptr;
    node10.Parent = &node20;
    node10.IsRed = false;

    node30.Left = nullptr;
    node30.Right = nullptr;
    node30.Parent = &node20;
    node30.IsRed = false;

    node50.Left = nullptr;
    node50.Right = nullptr;
    node50.Parent = &node60;
    node50.IsRed = false;

    node80.Left = &node70;
    node80.Right = &node90;
    node80.Parent = &node60;
    node80.IsRed = true;

    node70.Left = nullptr;
    node70.Right = nullptr;
    node70.Parent = &node80;
    node70.IsRed = false;

    node90.Left = nullptr;
    node90.Right = &node100;
    node90.Parent = &node80;
    node90.IsRed = false;

    node100.Left = nullptr;
    node100.Right = nullptr;
    node100.Parent = &node90;
    node100.IsRed = true;

    for (const Tree_T::Node& node : testDataInsertionOrder) {
        tree.insert(node.Data);
    }

    ASSERT_TRUE(tree.size() == capacity);
    testRedBlackTreeNodes<TestData_T>(&node40, tree.find(node40.Data));

    // Delete 50
    // Tree should look like this:
    //                                     40B
    //                        /                           \
    //                      20B                           80B
    //              /                \              /              \
    //            10B                30B          60B              90B
    //          /      \           /    \       /     \           /    \
    //        Null    Null       Null   Null   Null   70R       Null   100R
    //                                               /   \            /    \
    //                                             Null  Null       Null   Null

    node50.Parent = nullptr;
    node40.Right = &node80;
    node80.Parent = &node40;
    node80.Left = &node60;
    node80.IsRed = false;
    node60.Left = nullptr;
    node60.Right = &node70;
    node60.Parent = &node80;
    node70.Parent = &node60;
    node70.IsRed = true;

    tree.remove(node50.Data);
    ASSERT_TRUE(tree.size() == capacity - 1);
    testRedBlackTreeNodes<TestData_T>(&node40, tree.find(node40.Data));

    // Delete 20
    // Tree should look like this:
    // 20 could be replaced with either its in order predecessor (10) or its in order successor (30).
    // We're going to say it should be replaced by the in order successor (30).
    //                                     40B
    //                        /                           \
    //                      30B                           80R
    //              /                \              /              \
    //            10R                Null          60B              90B
    //          /      \                        /     \           /    \
    //        Null    Null                    Null    70R       Null   100R
    //                                               /   \            /    \
    //                                             Null  Null       Null   Null

    node20.Parent = nullptr;
    node20.Right = nullptr;
    node20.Left = nullptr;
    node10.IsRed = true;
    node10.Parent = &node30;
    node30.Parent = &node40;
    node30.Left = &node10;
    node40.Left = &node30;
    node80.IsRed = true;

    tree.remove(node20.Data);
    ASSERT_TRUE(tree.size() == capacity - 2);
    testRedBlackTreeNodes<TestData_T>(&node40, tree.find(node40.Data));

    // Delete 100
    // Tree should look like this:
    //                                     40B
    //                        /                           \
    //                      30B                           80R
    //              /                \              /              \
    //            10R                Null          60B              90B
    //          /      \                        /     \           /    \
    //        Null    Null                    Null    70R       Null   Null
    //                                               /   \
    //                                             Null  Null

    node100.Parent = nullptr;
    node90.Right = nullptr;

    tree.remove(node100.Data);
    ASSERT_TRUE(tree.size() == capacity - 3);
    testRedBlackTreeNodes<TestData_T>(&node40, tree.find(node40.Data));

    // Delete 90
    // Tree should look like this:
    //                                     40B
    //                        /                           \
    //                      30B                           70R
    //              /                \              /              \
    //            10R                Null          60B             80B
    //          /      \                        /     \           /    \
    //        Null    Null                    Null   Null       Null   Null

    node90.Parent = nullptr;
    node40.Right = &node70;
    node70.Left = &node60;
    node70.Right = &node80;
    node70.Parent = &node40;
    node60.Right = nullptr;
    node60.Parent = &node70;
    node80.Right = nullptr;
    node80.Left = nullptr;
    node80.Parent = &node70;
    node80.IsRed = false;

    tree.remove(node90.Data);
    ASSERT_TRUE(tree.size() == capacity - 4);
    testRedBlackTreeNodes<TestData_T>(&node40, tree.find(node40.Data));

    // Delete 40
    // Tree should look like this:
    // 40 could be replaced with either its in order predecessor (30) or its in order successor (60).
    // We're going to say it should be replaced by the in order successor (60).
    //                                     60B
    //                        /                           \
    //                      30B                           70B
    //              /                \              /              \
    //            10R                Null         Null             80R
    //          /      \                                          /    \
    //        Null    Null                                      Null   Null

    node40.Left = nullptr;
    node40.Right = nullptr;
    node60.Parent = nullptr;
    node60.Left = &node30;
    node60.Right = &node70;
    node30.Parent = &node60;
    node70.Parent = &node60;
    node70.Left = nullptr;
    node70.IsRed = false;
    node80.IsRed = true;

    tree.remove(node40.Data);
    ASSERT_TRUE(tree.size() == capacity - 5);
    testRedBlackTreeNodes<TestData_T>(&node60, tree.find(node60.Data));

    // Delete 60
    // Tree should look like this:
    // 60 could be replaced with either its in order predecessor (30) or its in order successor (70).
    // We're going to say it should be replaced by the in order successor (70).
    //                                     70B
    //                        /                           \
    //                      30B                           80B
    //              /                \              /              \
    //            10R                Null         Null            Null
    //          /      \
    //        Null    Null

    node60.Left = nullptr;
    node60.Right = nullptr;
    node70.Parent = nullptr;
    node70.Left = &node30;
    node30.Parent = &node70;
    node80.IsRed = false;

    tree.remove(node60.Data);
    ASSERT_TRUE(tree.size() == capacity - 6);
    testRedBlackTreeNodes<TestData_T>(&node70, tree.find(node70.Data));

    // Delete 30
    // Tree should look like this:
    //                                     70B
    //                        /                           \
    //                      10B                           80B
    //              /                \              /              \
    //            Null               Null         Null            Null

    node30.Parent = nullptr;
    node30.Left = nullptr;
    node70.Left = &node10;
    node10.Parent = &node70;
    node10.IsRed = false;

    tree.remove(node30.Data);
    ASSERT_TRUE(tree.size() == capacity - 7);
    testRedBlackTreeNodes<TestData_T>(&node70, tree.find(node70.Data));

    // Delete 10
    // Tree should look like this:
    //         70B
    //      /      \
    //   Null      80R
    //            /   \
    //          Null  Null

    node10.Parent = nullptr;
    node70.Left = nullptr;
    node80.IsRed = true;

    tree.remove(node10.Data);
    ASSERT_TRUE(tree.size() == capacity - 8);
    testRedBlackTreeNodes<TestData_T>(&node70, tree.find(node70.Data));

    // Delete 70
    // Tree should look like this:
    //         80B
    //      /      \
    //   Null      Null

    node70.Right = nullptr;
    node80.Parent = nullptr;
    node80.IsRed = false;

    tree.remove(node70.Data);
    ASSERT_TRUE(tree.size() == capacity - 9);
    testRedBlackTreeNodes<TestData_T>(&node80, tree.find(node80.Data));

    // Delete 80
    // Tree should look like this:
    //         Null

    tree.remove(node80.Data);
    ASSERT_TRUE(tree.size() == capacity - 10);
    testRedBlackTreeNodes<TestData_T>(nullptr, tree.find(node80.Data));

    ASSERT_TRUE(tree.empty());
}

TEST(AxrRedBlackTree_Pool, Replace) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //                                     40B
    //                        /                           \
    //                      20B                           60B
    //              /                \              /              \
    //            10B                30B          50B              80R
    //          /      \           /    \       /     \        /         \
    //        Null    Null       Null   Null   Null  Null     70B        90B
    //                                                      /   \       /    \
    //                                                    Null  Null  Null   100R
    //                                                                      /    \
    //                                                                    Null  Null

    Tree_T::Node node40{};
    node40.Data = 40;
    Tree_T::Node node20{};
    node20.Data = 20;
    Tree_T::Node node60{};
    node60.Data = 60;
    Tree_T::Node node10{};
    node10.Data = 10;
    Tree_T::Node node30{};
    node30.Data = 30;
    Tree_T::Node node50{};
    node50.Data = 50;
    Tree_T::Node node80{};
    node80.Data = 80;
    Tree_T::Node node70{};
    node70.Data = 70;
    Tree_T::Node node90{};
    node90.Data = 90;
    Tree_T::Node node100{};
    node100.Data = 100;

    const Tree_T::Node testDataInsertionOrder[capacity]{
        node20,
        node10,
        node30,
        node40,
        node60,
        node50,
        node80,
        node70,
        node90,
        node100,
    };

    node40.Left = &node20;
    node40.Right = &node60;
    node40.Parent = nullptr;
    node40.IsRed = false;

    node20.Left = &node10;
    node20.Right = &node30;
    node20.Parent = &node40;
    node20.IsRed = false;

    node60.Left = &node50;
    node60.Right = &node80;
    node60.Parent = &node40;
    node60.IsRed = false;

    node10.Left = nullptr;
    node10.Right = nullptr;
    node10.Parent = &node20;
    node10.IsRed = false;

    node30.Left = nullptr;
    node30.Right = nullptr;
    node30.Parent = &node20;
    node30.IsRed = false;

    node50.Left = nullptr;
    node50.Right = nullptr;
    node50.Parent = &node60;
    node50.IsRed = false;

    node80.Left = &node70;
    node80.Right = &node90;
    node80.Parent = &node60;
    node80.IsRed = true;

    node70.Left = nullptr;
    node70.Right = nullptr;
    node70.Parent = &node80;
    node70.IsRed = false;

    node90.Left = nullptr;
    node90.Right = &node100;
    node90.Parent = &node80;
    node90.IsRed = false;

    node100.Left = nullptr;
    node100.Right = nullptr;
    node100.Parent = &node90;
    node100.IsRed = true;

    for (const Tree_T::Node& node : testDataInsertionOrder) {
        tree.insert(node.Data);
    }

    ASSERT_TRUE(tree.size() == capacity);
    testRedBlackTreeNodes<TestData_T>(&node40, tree.find(node40.Data));

    TestData_T newData = 35;
    const Tree_T::Node::Iterator replacedNode = tree.find(node60.Data);
    ASSERT_TRUE(*replacedNode == node60.Data);
    tree.replace(node60.Data, newData);
    ASSERT_TRUE(*replacedNode == newData);
    ASSERT_TRUE(tree.size() == capacity);

    // Tree should look like this:
    //                                     40B
    //                        /                           \
    //                      20B                           70B
    //              /                \              /              \
    //            10B                30B          50B              90R
    //          /      \           /    \       /     \        /         \
    //        Null    Null       Null   35R   Null    Null   80B         100B
    //                                /    \                /   \       /    \
    //                              Null   Null           Null  Null  Null   Null

    Tree_T::Node node35{};
    node35.Data = 35;
    node35.Left = nullptr;
    node35.Right = nullptr;
    node35.Parent = &node30;
    node35.IsRed = true;

    node30.Right = &node35;

    node40.Right = &node70;
    node70.Parent = &node40;
    node70.Left = &node50;
    node70.Right = &node90;
    node50.Parent = &node70;
    node90.Parent = &node70;
    node90.Left = &node80;
    node90.IsRed = true;
    node80.Parent = &node90;
    node80.Left = nullptr;
    node80.Right = nullptr;
    node80.IsRed = false;
    node100.IsRed = false;

    testRedBlackTreeNodes<TestData_T>(&node40, tree.find(node40.Data));
}

TEST(AxrRedBlackTree_Pool, Clear) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    constexpr TestData_T testData[capacity]{
        1,
        63,
        80,
        51,
        287,
        6,
        5,
        987,
        12,
        43,
    };

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });

    Tree_T tree(&allocator);
    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(allocator.empty());

    for (const TestData_T& i : testData) {
        tree.insert(i);
    }

    ASSERT_TRUE(tree.size() == capacity);
    ASSERT_TRUE(allocator.size() == capacity);

    tree.clear();

    ASSERT_TRUE(tree.size() == 0);
    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(allocator.empty());
}

TEST(AxrRedBlackTree_Pool, OutOfScopeCleanup) {
    using TestData_T = uint32_t;
    using Tree_T = AxrRedBlackTree_Pool<TestData_T>;;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (Tree_T::getItemSize() * capacity) + Tree_T::getItemAlignment();
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Tree_T::Node> allocator(AxrMemoryBlock{
        .Memory = memory,
        .Size = allocatorSize,
        .Deallocator = callback,
    });
    ASSERT_TRUE(allocator.empty());

    {
        Tree_T tree(&allocator);

        constexpr TestData_T testData[capacity]{
            1,
            63,
            80,
            51,
            287,
            6,
            5,
            987,
            12,
            43,
        };

        for (const TestData_T& i : testData) {
            tree.insert(i);
        }

        ASSERT_TRUE(!allocator.empty());
    }

    ASSERT_TRUE(allocator.empty());
}
