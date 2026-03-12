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
                                  const typename AxrRedBlackTree_Pool<Type>::Node* actualNodes) {
    if (testNodes == nullptr) {
        ASSERT_TRUE(actualNodes == nullptr);
        return;
    }
    // If testNodes isn't null, actualNodes shouldn't be either
    if (actualNodes == nullptr) {
        ASSERT_TRUE(false);
        return;
    }

    ASSERT_TRUE(actualNodes->Data == testNodes->Data);
    ASSERT_TRUE(actualNodes->Count == testNodes->Count);
    ASSERT_TRUE(actualNodes->IsRed == testNodes->IsRed);
    testRedBlackTreeNodes<Type>(testNodes->Left, actualNodes->Left);
    testRedBlackTreeNodes<Type>(testNodes->Right, actualNodes->Right);
}

// ----------------------------------------- //
// Tests
// ----------------------------------------- //

TEST(AxrRedBlackTree_Pool, Initialization) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    const AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
    ASSERT_TRUE(tree.size() == 0);
    ASSERT_TRUE(tree.empty());
}

TEST(AxrRedBlackTree_Pool, Insert_One) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 16;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //          25B
    //         /   \
    //       Null   Null
    Node_T node1{};
    node1.Left = nullptr;
    node1.Right = nullptr;
    node1.Parent = nullptr;
    node1.Count = 1;
    node1.Data = 25;
    node1.IsRed = false;

    ASSERT_FALSE(tree.exists(node1.Data));
    tree.insert(node1.Data);
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.exists(node1.Data));
    testRedBlackTreeNodes<TestData_T>(&node1, tree.find(node1.Data));
}

TEST(AxrRedBlackTree_Pool, Find_Exists) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);

    constexpr TestData_T testData = 10;
    tree.insert(1);
    tree.insert(testData);
    tree.insert(2);

    const Node_T* foundNode = tree.find(testData);
    ASSERT_TRUE(foundNode != nullptr);
    if (foundNode != nullptr) {
        ASSERT_TRUE(foundNode->Data == testData);
    }
}

TEST(AxrRedBlackTree_Pool, Find_DoesntExist) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);

    tree.insert(1);
    tree.insert(15);
    tree.insert(2);

    const Node_T* foundNode = tree.find(10);
    ASSERT_TRUE(foundNode == nullptr);
}

TEST(AxrRedBlackTree_Pool, Insert_All) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 11;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Add 12
    // Tree should look like this:
    //          12B
    //         /   \
    //       Null   Null
    Node_T node12{};
    node12.Left = nullptr;
    node12.Right = nullptr;
    node12.Parent = nullptr;
    node12.Count = 1;
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
    Node_T node20{};
    node20.Left = nullptr;
    node20.Right = nullptr;
    node20.Parent = &node12;
    node20.Count = 1;
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

    Node_T node9{};
    node9.Left = nullptr;
    node9.Right = nullptr;
    node9.Parent = &node12;
    node9.Count = 1;
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

    Node_T node17{};
    node17.Left = nullptr;
    node17.Right = nullptr;
    node17.Parent = &node20;
    node17.Count = 1;
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

    Node_T node18{};
    node18.Left = &node17;
    node18.Right = &node20;
    node18.Parent = &node12;
    node18.Count = 1;
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

    Node_T node32{};
    node32.Left = nullptr;
    node32.Right = nullptr;
    node32.Parent = &node20;
    node32.Count = 1;
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

    Node_T node27{};
    node27.Left = &node20;
    node27.Right = &node32;
    node27.Parent = &node18;
    node27.Count = 1;
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

    Node_T node42{};
    node42.Left = nullptr;
    node42.Right = nullptr;
    node42.Parent = &node32;
    node42.Count = 1;
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

    Node_T node80{};
    node80.Left = nullptr;
    node80.Right = nullptr;
    node80.Parent = &node42;
    node80.Count = 1;
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

    Node_T node4{};
    node4.Left = nullptr;
    node4.Right = nullptr;
    node4.Parent = &node9;
    node4.Count = 1;
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

    Node_T node2{};
    node2.Left = nullptr;
    node2.Right = nullptr;
    node2.Parent = &node9;
    node2.Count = 1;
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

TEST(AxrRedBlackTree_Pool, Insert_Duplicate) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //                                 63B
    //                  /                             \
    //                 6R                            287B
    //          /             \                  /         \
    //         1B             43B               80R        987R
    //       /    \      /          \         /    \      /    \
    //     5R    Null   12R         51R     Null  Null  Null  Null
    //   /   \         /   \       /   \
    // Null  Null    Null  Null  Null  Null

    Node_T node1{};
    node1.Count = 2;
    node1.Data = 1;
    Node_T node63{};
    node63.Count = 1;
    node63.Data = 63;
    Node_T node80{};
    node80.Count = 1;
    node80.Data = 80;
    Node_T node51{};
    node51.Count = 1;
    node51.Data = 51;
    Node_T node287{};
    node287.Count = 1;
    node287.Data = 287;
    Node_T node6{};
    node6.Count = 1;
    node6.Data = 6;
    Node_T node5{};
    node5.Count = 1;
    node5.Data = 5;
    Node_T node987{};
    node987.Count = 1;
    node987.Data = 987;
    Node_T node12{};
    node12.Count = 1;
    node12.Data = 12;
    Node_T node43{};
    node43.Count = 1;
    node43.Data = 43;

    const Node_T testDataInsertionOrder[capacity]{
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

    for (const Node_T& node : testDataInsertionOrder) {
        for (uint32_t i = 0; i < node.Count; ++i) {
            tree.insert(node.Data);
        }
    }

    // It's capacity + 1 because 1 node has a count of 2 at this point.
    size_t expectedSize = capacity + 1;
    ASSERT_TRUE(tree.size() == expectedSize);
    testRedBlackTreeNodes<TestData_T>(&node63, tree.find(node63.Data));

    node63.Count = 2;
    node43.Count = 2;
    node80.Count = 4;

    tree.insert(node63.Data);
    tree.insert(node43.Data);

    tree.insert(node80.Data);
    tree.insert(node80.Data);
    tree.insert(node80.Data);

    expectedSize += 5;
    ASSERT_TRUE(tree.size() == expectedSize);
    testRedBlackTreeNodes<TestData_T>(&node63, tree.find(node63.Data));
}

TEST(AxrRedBlackTree_Pool, Insert_TooMany) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //                                 63B
    //                  /                             \
    //                 6R                            287B
    //          /             \                  /         \
    //         1B             43B               80R        987R
    //       /    \      /          \         /    \      /    \
    //     5R    Null   12R         51R     Null  Null  Null  Null
    //   /   \         /   \       /   \
    // Null  Null    Null  Null  Null  Null

    Node_T node1{};
    node1.Count = 1;
    node1.Data = 1;
    Node_T node63{};
    node63.Count = 1;
    node63.Data = 63;
    Node_T node80{};
    node80.Count = 1;
    node80.Data = 80;
    Node_T node51{};
    node51.Count = 1;
    node51.Data = 51;
    Node_T node287{};
    node287.Count = 1;
    node287.Data = 287;
    Node_T node6{};
    node6.Count = 1;
    node6.Data = 6;
    Node_T node5{};
    node5.Count = 1;
    node5.Data = 5;
    Node_T node987{};
    node987.Count = 1;
    node987.Data = 987;
    Node_T node12{};
    node12.Count = 1;
    node12.Data = 12;
    Node_T node43{};
    node43.Count = 1;
    node43.Data = 43;

    const Node_T testDataInsertionOrder[capacity]{
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

    for (const Node_T& node : testDataInsertionOrder) {
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
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 10;

    constexpr TestData_T testData = 43;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
    ASSERT_TRUE(tree.empty());

    tree.insert(testData);

    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.find(testData) != nullptr);

    tree.remove(testData);

    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.find(testData) == nullptr);
}

TEST(AxrRedBlackTree_Pool, Remove_All) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
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

    Node_T node40{};
    node40.Count = 1;
    node40.Data = 40;
    Node_T node20{};
    node20.Count = 1;
    node20.Data = 20;
    Node_T node60{};
    node60.Count = 1;
    node60.Data = 60;
    Node_T node10{};
    node10.Count = 1;
    node10.Data = 10;
    Node_T node30{};
    node30.Count = 1;
    node30.Data = 30;
    Node_T node50{};
    node50.Count = 1;
    node50.Data = 50;
    Node_T node80{};
    node80.Count = 1;
    node80.Data = 80;
    Node_T node70{};
    node70.Count = 1;
    node70.Data = 70;
    Node_T node90{};
    node90.Count = 1;
    node90.Data = 90;
    Node_T node100{};
    node100.Count = 1;
    node100.Data = 100;

    const Node_T testDataInsertionOrder[capacity]{
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

    for (const Node_T& node : testDataInsertionOrder) {
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

TEST(AxrRedBlackTree_Pool, Remove_Duplicate) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
    ASSERT_TRUE(tree.empty());

    // Tree should look like this:
    //                                 63B
    //                  /                             \
    //                 6R                            287B
    //          /             \                  /         \
    //         1B             43B               80R        987R
    //       /    \      /          \         /    \      /    \
    //     5R    Null   12R         51R     Null  Null  Null  Null
    //   /   \         /   \       /   \
    // Null  Null    Null  Null  Null  Null

    Node_T node1{};
    node1.Count = 2;
    node1.Data = 1;
    Node_T node63{};
    node63.Count = 2;
    node63.Data = 63;
    Node_T node80{};
    node80.Count = 4;
    node80.Data = 80;
    Node_T node51{};
    node51.Count = 1;
    node51.Data = 51;
    Node_T node287{};
    node287.Count = 1;
    node287.Data = 287;
    Node_T node6{};
    node6.Count = 1;
    node6.Data = 6;
    Node_T node5{};
    node5.Count = 1;
    node5.Data = 5;
    Node_T node987{};
    node987.Count = 1;
    node987.Data = 987;
    Node_T node12{};
    node12.Count = 1;
    node12.Data = 12;
    Node_T node43{};
    node43.Count = 2;
    node43.Data = 43;

    const Node_T testDataInsertionOrder[capacity]{
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

    for (const Node_T& node : testDataInsertionOrder) {
        for (uint32_t i = 0; i < node.Count; ++i) {
            tree.insert(node.Data);
        }
    }

    // It's capacity + 6 because node1 has a count of 2, node63 has a count of 2, node43 has a count of 2 and node80 has
    // a count of 4 at this point.
    size_t expectedSize = capacity + 6;
    ASSERT_TRUE(tree.size() == expectedSize);
    testRedBlackTreeNodes<TestData_T>(&node63, tree.find(node63.Data));

    node63.Count = 1;
    node43.Count = 1;
    node80.Count = 2;

    tree.remove(node63.Data);
    tree.remove(node43.Data);

    tree.remove(node80.Data);
    tree.remove(node80.Data);

    expectedSize -= 4;
    ASSERT_TRUE(tree.size() == expectedSize);
    testRedBlackTreeNodes<TestData_T>(&node63, tree.find(node63.Data));
}

TEST(AxrRedBlackTree_Pool, Clear) {
    using TestData_T = uint32_t;
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
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

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);

    AxrRedBlackTree_Pool<TestData_T> tree(&allocator);
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
    using Node_T = AxrRedBlackTree_Pool<TestData_T>::Node;
    constexpr uint32_t capacity = 10;

    AxrDeallocateBlock callback;
    callback.connect<deallocateCallback>();

    constexpr size_t allocatorSize = (sizeof(Node_T) * capacity) + alignof(Node_T);
    void* memory = malloc(allocatorSize);
    AxrPoolAllocator<Node_T> allocator(memory, allocatorSize, callback);
    ASSERT_TRUE(allocator.empty());

    {
        AxrRedBlackTree_Pool<TestData_T> tree(&allocator);

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
