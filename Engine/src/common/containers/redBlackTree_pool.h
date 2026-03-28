#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../memory/poolAllocator.h"

/// A red black tree that uses a pool allocator
/// @tparam Type Data type to use
template<typename Type>
class AxrRedBlackTree_Pool {
public:
    // ----------------------------------------- //
    // Node
    // ----------------------------------------- //

    class Node {
    public:
        // ----------------------------------------- //
        // Public Variables
        // ----------------------------------------- //
        Node* Left{};
        Node* Right{};
        Node* Parent{};
        Type Data{};
        bool IsRed{};

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        // Default constructor
        Node() = default;

        /// Constructor
        /// @param data Data to store in the node
        explicit Node(const Type& data) :
            Data(data),
            IsRed(true) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if this node is on the left of the binary tree
        /// @return True if this node is on the left of the binary tree
        [[nodiscard]] bool isLeftNode() const {
            return Parent != nullptr && this == Parent->Left;
        }

        /// Check if this node is on the right of the binary tree
        /// @return True if this node is on the right of the binary tree
        [[nodiscard]] bool isRightNode() const {
            return Parent != nullptr && this == Parent->Right;
        }

        /// Get the uncle node relative to this node
        /// @return The uncle node
        Node* getUncle() {
            if (Parent == nullptr || Parent->Parent == nullptr) {
                return nullptr;
            }

            if (Parent->isLeftNode()) {
                return Parent->Parent->Right;
            }

            return Parent->Parent->Left;
        }

        /// Get the sibling node relative to this node
        /// @return The sibling node
        Node* getSibling() {
            if (Parent == nullptr) {
                return nullptr;
            }

            if (isLeftNode()) {
                return Parent->Right;
            }

            return Parent->Left;
        }

        /// Check if this node has a red child
        /// @return True if this node has a red child
        [[nodiscard]] bool hasRedChild() const {
            return (Left != nullptr && Left->IsRed) || (Right != nullptr && Right->IsRed);
        }

        // ----------------------------------------- //
        // Node Iterator
        // ----------------------------------------- //
        class Iterator {
        public:
            // ----------------------------------------- //
            // Special Functions
            // ----------------------------------------- //

            // ---- Constructors ----

            /// Default Constructor
            Iterator() = default;

            /// Constructor
            /// @param rootNode The root node of the tree
            /// @param node The node for this iterator to point to
            explicit Iterator(const Node* rootNode, const Node* node) :
                m_RootNode(rootNode),
                m_Node(node) {
            }

            // ---- Operator Overloads ----

            /// Prefix increment operator overload
            /// @return The iterator, incremented by 1
            Iterator& operator++() {
                // We're already at the end. cannot increment;
                if (m_Node == nullptr) [[unlikely]] {
                    return *this;
                }

                // First, we try finding the node's successor
                if (m_Node->Right != nullptr) {
                    const Node* currentNode = m_Node->Right;
                    while (currentNode->Left != nullptr) {
                        currentNode = currentNode->Left;
                    }
                    m_Node = currentNode;
                    return *this;
                }

                // If there's no successor, then the next node in the sequence will be the parent of the next left
                // parent node
                const Node* currentNode = m_Node;
                while (currentNode->isRightNode()) {
                    currentNode = currentNode->Parent;
                }
                if (currentNode != nullptr) {
                    m_Node = currentNode->Parent;
                    return *this;
                }

                // If we get here, then we've reached the end of the tree. Don't modify the node
                return *this;
            }

            /// Postfix increment operator overload
            /// @return the current iterator, before incrementing it by 1
            Iterator operator++(int) {
                Iterator returnValue = *this;
                operator++();

                return returnValue;
            }

            /// Prefix decrement operator overload
            /// @return The iterator, decremented by 1
            Iterator& operator--() {
                if (m_Node == nullptr) [[unlikely]] {
                    // Find the last value in the tree

                    if (m_RootNode == nullptr) [[unlikely]] {
                        // Unable to find the last value
                        return *this;
                    }

                    m_Node = m_RootNode;

                    while (m_Node->Right != nullptr) {
                        m_Node = m_Node->Right;
                    }

                    return *this;
                }

                // First, we try finding the node's predecessor
                if (m_Node->Left != nullptr) {
                    const Node* currentNode = m_Node->Left;
                    while (currentNode->Right != nullptr) {
                        currentNode = currentNode->Right;
                    }
                    m_Node = currentNode;
                    return *this;
                }

                // If there's no predecessor, then the next node in the sequence will be the parent of the next right
                // parent node
                const Node* currentNode = m_Node;
                while (currentNode->isLeftNode()) {
                    currentNode = currentNode->Parent;
                }
                if (currentNode != nullptr) {
                    m_Node = currentNode->Parent;
                    return *this;
                }

                // If we get here, then we've reached the end of the tree. Don't modify the node
                return *this;
            }

            /// Postfix decrement operator overload
            /// @return the current iterator, before decrementing it by 1
            Iterator operator--(int) {
                Iterator returnValue = *this;
                operator--();

                return returnValue;
            }

            /// Equality operator overload
            /// @other Iterator to compare against
            /// @return True if both iterators point to the same node
            bool operator==(const Iterator& other) const {
                return m_RootNode == other.m_RootNode && m_Node == other.m_Node;
            }

            /// Inequality operator overload
            /// @other Iterator to compare against
            /// @return True if both iterators point to different nodes
            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

            /// Get the underlining data for this iterator
            /// @return The underlining data for this iterator
            const Type& operator*() const {
                return m_Node->Data;
            }

        private:
            // ----------------------------------------- //
            // Private Variables
            // ----------------------------------------- //
            const Node* m_RootNode{};
            const Node* m_Node{};
        };
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrRedBlackTree_Pool() = default;

    /// Constructor
    /// @param poolAllocator The pool allocator to use
    explicit AxrRedBlackTree_Pool(AxrPoolAllocator<Node>* poolAllocator) :
        m_PoolAllocator(poolAllocator) {
    }

    /// Copy Constructor
    /// @param src Source AxrRedBlackTree_Pool to copy from
    AxrRedBlackTree_Pool(const AxrRedBlackTree_Pool& src) = delete;

    /// Move Constructor
    /// @param src Source AxrRedBlackTree_Pool to move from
    AxrRedBlackTree_Pool(AxrRedBlackTree_Pool&& src) noexcept {
        move_internal(std::move(src));
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrRedBlackTree_Pool() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrRedBlackTree_Pool to copy from
    AxrRedBlackTree_Pool& operator=(const AxrRedBlackTree_Pool& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrRedBlackTree_Pool to move from
    AxrRedBlackTree_Pool& operator=(AxrRedBlackTree_Pool&& src) noexcept {
        if (this != &src) {
            cleanup();

            move_internal(std::move(src));
        }
        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Find the node with the given data
    /// @param data Data to search for
    /// @return The node with the given data. Or nullptr if the data wasn't found.
    [[nodiscard]] const Node* findNode(const Type& data) const {
        Node* currentNode = m_RootNode;

        while (currentNode != nullptr && currentNode->Data != data) {
            if (data < currentNode->Data) {
                currentNode = currentNode->Left;
            } else {
                currentNode = currentNode->Right;
            }
        }

        return currentNode;
    }

    /// Find the next largest value after the given value. The given value doesn't need to exist in the tree.
    /// @param data Find the next node which is larger than this value
    /// @return The next largest node's iterator. Or the iterator end if there is no node larger than the given value
    [[nodiscard]] Node::Iterator findNextLargest(const Type& data) const {
        Node* currentNode = m_RootNode;
        Node* previousNode = nullptr;

        // Keep going down the tree until we find the closest leaf node to the given value. The leaf node will be stored
        // in the `previousNode`
        while (currentNode != nullptr) {
            if (data < currentNode->Data) {
                previousNode = currentNode;
                currentNode = currentNode->Left;
            } else {
                previousNode = currentNode;
                currentNode = currentNode->Right;
            }
        }

        typename Node::Iterator closestIterator(m_RootNode, previousNode);
        // At this point, we've just found the leaf node that's closest to the given value. We don't know if it's
        // actually larger than it. So all we need to do is check if the value we're searching for is larger than our
        // iterator. If it is, the next largest node is the next largest node. If not, then we're already on the next
        // largest node
        if (data > *closestIterator) {
            ++closestIterator;
        }

        return closestIterator;
    }

    /// Check if the given data exists in this tree
    /// @param data Data to check
    /// @return True if the data exists
    [[nodiscard]] bool exists(const Type& data) const {
        return findNode(data) != nullptr;
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to insert data into the red black tree. "
    /// Insert data into the tree
    /// @param data Data to insert
    /// @return The inserted node. Or nullptr if we failed to insert the data
    Node* insert(const Type& data) {
        if (m_PoolAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Pool allocator is null.");
            return nullptr;
        }

        Node* node{};
        AxrResult axrResult = m_PoolAllocator->allocate(node);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to allocate memory.");
            return nullptr;
        }

        *node = Node(data);

        axrResult = insertNode(node);
        if (AXR_FAILED(axrResult)) {
            m_PoolAllocator->deallocate(node);
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to insert the node.");
            return nullptr;
        }

        ++m_Size;
        return node;
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to remove data from the red black tree. "
    /// Remove data from the tree
    /// @param data Data to remove
    void remove(const Type& data) {
        Node* node = findNode_internal(data);
        if (node == nullptr) {
            return;
        }

        if (m_PoolAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Pool allocator is null.");
            return;
        }

        const AxrResult axrResult = removeNode(node);
        if (AXR_FAILED(axrResult)) {
            return;
        }

        --m_Size;
        m_PoolAllocator->deallocate(node);
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to replace data in the red black tree. "
    /// Replace the given data with new data. Position it in the tree correctly for the new data but reuse the same
    /// memory address as the original data.
    /// @param originalData Original data to replace
    /// @param newData New data to replace the original data with
    void replace(const Type& originalData, const Type& newData) {
        Node* node = findNode_internal(originalData);
        if (node == nullptr) {
            return;
        }

        AxrResult axrResult = removeNode(node);
        if (AXR_FAILED(axrResult)) {
            return;
        }

        *node = Node(newData);

        axrResult = insertNode(node);
        if (AXR_FAILED(axrResult)) {
            m_PoolAllocator->deallocate(node);
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to insert the node.");
            return;
        }
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Deallocate all nodes and clear the red black tree
    void clear() {
        if (m_RootNode == nullptr) {
            return;
        }

        // We assert instead of logging an error because if we trigger this, then a memory leak occurred.
        // It should never happen in normal circumstances
        assert(m_PoolAllocator != nullptr);

        Node* currentNode = m_RootNode;
        while (currentNode != nullptr) {
            if (currentNode->Left != nullptr) {
                currentNode = currentNode->Left;
                continue;
            }
            if (currentNode->Right != nullptr) {
                currentNode = currentNode->Right;
                continue;
            }

            Node* parent = currentNode->Parent;
            if (parent != nullptr) {
                if (currentNode->isLeftNode()) {
                    parent->Left = nullptr;
                } else {
                    parent->Right = nullptr;
                }
            }
            m_PoolAllocator->deallocate(currentNode);

            currentNode = parent;
        }

        m_RootNode = nullptr;
        m_Size = 0;
    }

    /// Get the beginning iterator
    /// @return The beginning iterator
    Node::Iterator begin() const {
        Node* currentNode = m_RootNode;

        while (currentNode->Left != nullptr) {
            currentNode = currentNode->Left;
        }

        return typename Node::Iterator(m_RootNode, currentNode);
    }

    /// Get the end iterator
    /// @return The end iterator
    Node::Iterator end() const {
        return typename Node::Iterator(m_RootNode, nullptr);
    }

    /// Check if this red black tree is empty
    /// @return True if this red black tree is empty
    [[nodiscard]] bool empty() const {
        return m_RootNode == nullptr;
    }

    /// Get the number of items in this red black tree
    /// @return The number of items in this red black tree
    [[nodiscard]] size_t size() const {
        return m_Size;
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrPoolAllocator<Node>* m_PoolAllocator{};
    Node* m_RootNode{};
    size_t m_Size{};

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        clear();

        m_PoolAllocator = {};
        m_RootNode = {};
        m_Size = {};
    }

    /// Move the given AxrRedBlackTree_Pool to this class
    /// @param src AxrRedBlackTree_Pool to move
    void move_internal(AxrRedBlackTree_Pool&& src) {
        m_PoolAllocator = src.m_PoolAllocator;
        m_RootNode = src.m_RootNode;
        m_Size = src.m_Size;

        src.m_PoolAllocator = {};
        src.m_RootNode = {};
        src.m_Size = {};
    }

    /// Find the node with the given data
    /// @param data Data to search for
    /// @return The node with the given data. Or nullptr if the data wasn't found.
    [[nodiscard]] Node* findNode_internal(const Type& data) {
        Node* currentNode = m_RootNode;

        while (currentNode != nullptr && currentNode->Data != data) {
            if (data < currentNode->Data) {
                currentNode = currentNode->Left;
            } else {
                currentNode = currentNode->Right;
            }
        }

        return currentNode;
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to perform a standard bst insertion for the red black tree. "
    /// Insert the given node into the tree, via standard binary search tree methods.
    /// @param node Node to insert
    /// @returns AXR_SUCCESS if the function succeeded.
    /// AXR_ERROR_DUPLICATE if the given node's data already exists
    [[nodiscard]] AxrResult bstInsert(Node* node) {
        assert(node != nullptr);

        Node** nodeLocation = &m_RootNode;
        Node* parentNode = nullptr;

        // Find where to insert the new data
        while (*nodeLocation != nullptr && (*nodeLocation)->Data != node->Data) {
            if (node->Data < (*nodeLocation)->Data) {
                parentNode = *nodeLocation;
                nodeLocation = &(*nodeLocation)->Left;
            } else {
                parentNode = *nodeLocation;
                nodeLocation = &(*nodeLocation)->Right;
            }
        }

        if (*nodeLocation != nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Data already exists.");
            return AXR_ERROR_DUPLICATE;
        }

        *nodeLocation = node;
        node->Parent = parentNode;

        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Fix all red red violations for the given, newly inserted, node
    /// @param node Newly inserted node
    void fixRedRedViolations(Node* node) {
        assert(node != nullptr);

        // Keep in mind, if a node is null, it's considered to be black.

        // We aren't explicitly checking if the node is *not* the root, since the root has no parent.
        // So by making sure that there is a parent, it can't be the root node.
        // It just makes more sense for code readability to check if something is null, before accessing its
        // data. Compared to checking if the node is the root, then checking the node's data.
        while (node->IsRed && node->Parent != nullptr && node->Parent->IsRed) {
            Node* parentNode = node->Parent;
            Node* grandparent = parentNode->Parent;
            Node* uncle = node->getUncle();

            if (uncle != nullptr && uncle->IsRed) {
                grandparent->IsRed = true;
                parentNode->IsRed = false;
                uncle->IsRed = false;

                // Repeat the loop with the grandparent
                node = grandparent;
                continue;
            }

            // If we get here, then uncle is black

            if (parentNode->isLeftNode()) {
                if (node->isRightNode()) {
                    rotateLeft(parentNode);
                    node = parentNode;
                    parentNode = node->Parent;
                }

                rotateRight(grandparent);
            } else {
                if (node->isLeftNode()) {
                    rotateRight(parentNode);
                    node = parentNode;
                    parentNode = node->Parent;
                }

                rotateLeft(grandparent);
            }

            swapColors(parentNode, grandparent);

            // Repeat the loop with the parent
            node = parentNode;
        }
    }

    /// Insert the given node into the tree
    /// @param node Node to insert
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult insertNode(Node* node) {
        // Red black tree insertion algorithm references:
        // https://www.geeksforgeeks.org/dsa/insertion-in-red-black-tree/
        // https://www.geeksforgeeks.org/dsa/c-program-red-black-tree-insertion/

        const AxrResult axrResult = bstInsert(node);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            return axrResult;
        }

        fixRedRedViolations(node);

        // Make sure the root is always black.
        m_RootNode->IsRed = false;

        return AXR_SUCCESS;
    }

    /// Rotates the given node to the right
    /// @param node Node to rotate
    void rotateRight(Node* node) {
        assert(node != nullptr);

        Node* newParent = node->Left;

        if (node == m_RootNode) {
            m_RootNode = newParent;
        }

        if (node->Parent != nullptr) {
            if (node->isLeftNode()) {
                node->Parent->Left = newParent;
            } else {
                node->Parent->Right = newParent;
            }
        }
        newParent->Parent = node->Parent;
        node->Parent = newParent;

        node->Left = newParent->Right;
        if (newParent->Right != nullptr) {
            newParent->Right->Parent = node;
        }

        newParent->Right = node;
    }

    /// Rotates the given node to the left
    /// @param node Node to rotate
    void rotateLeft(Node* node) {
        assert(node != nullptr);

        Node* newParent = node->Right;

        if (node == m_RootNode) {
            m_RootNode = newParent;
        }

        if (node->Parent != nullptr) {
            if (node->isLeftNode()) {
                node->Parent->Left = newParent;
            } else {
                node->Parent->Right = newParent;
            }
        }
        newParent->Parent = node->Parent;
        node->Parent = newParent;

        node->Right = newParent->Left;
        if (newParent->Left != nullptr) {
            newParent->Left->Parent = node;
        }

        newParent->Left = node;
    }

    /// Swap the colors between node a and b
    /// @param a First node
    /// @param b Second node
    static void swapColors(Node* a, Node* b) {
        bool temp = a->IsRed;
        a->IsRed = b->IsRed;
        b->IsRed = temp;
    };

    /// Swap the nodes within the tree between node a and b
    /// @param a First node
    /// @param b Second node
    void swapNodes(Node* a, Node* b) {
        assert(a != nullptr && b != nullptr);

        Node* aParent = a->Parent;
        Node* aLeft = a->Left;
        Node* aRight = a->Right;
        bool aIsRed = a->IsRed;

        bool aWasLeftNode = a->isLeftNode();
        bool bWasLeftNode = b->isLeftNode();

        // Make sure we don't create an infinite loop by connecting 'a' to itself in any situation. It should point to
        // 'b' since that's where 'a' used to be.
        b->Parent == a ? a->Parent = b : a->Parent = b->Parent;
        b->Left == a ? a->Left = b : a->Left = b->Left;
        b->Right == a ? a->Right = b : a->Right = b->Right;
        a->IsRed = b->IsRed;

        // Make sure we don't create an infinite loop by connecting 'b' to itself in any situation. It should point to
        // where
        aParent == b ? b->Parent = a : b->Parent = aParent;
        aLeft == b ? b->Left = a : b->Left = aLeft;
        aRight == b ? b->Right = a : b->Right = aRight;
        b->IsRed = aIsRed;

        // Update connected nodes
        if (a->Parent != nullptr && a->Parent != b) {
            // We check if 'b' was the left node because that's the one we're replacing
            bWasLeftNode ? a->Parent->Left = a : a->Parent->Right = a;
        }
        if (a->Left != nullptr && a->Left != b) {
            a->Left->Parent = a;
        }
        if (a->Right != nullptr && a->Right != b) {
            a->Right->Parent = a;
        }
        if (b->Parent != nullptr && b->Parent != a) {
            // We check if 'a' was the left node because that's the one we're replacing
            aWasLeftNode ? b->Parent->Left = b : b->Parent->Right = b;
        }
        if (b->Left != nullptr && b->Left != a) {
            b->Left->Parent = b;
        }
        if (b->Right != nullptr && b->Right != a) {
            b->Right->Parent = b;
        }

        if (m_RootNode == b) {
            m_RootNode = a;
        } else if (m_RootNode == a) {
            m_RootNode = b;
        }
    }

    /// Get the successor for the given node
    /// @param node Node to get the successor for
    /// @return The node's successor
    [[nodiscard]] Node* getSuccessor(Node* node) {
        assert(node != nullptr);

        Node* currentNode = node;
        while (currentNode->Left != nullptr) {
            currentNode = currentNode->Left;
        }

        return currentNode;
    }

    /// Get the node to replace the given node, when the given node is deleted
    /// @param node Node to replace
    /// @return Node to replace the given node with
    [[nodiscard]] Node* getNodeToReplace(Node* node) {
        assert(node != nullptr);

        // When the node has 2 children
        if (node->Left != nullptr && node->Right != nullptr) {
            return getSuccessor(node->Right);
        }

        /// When the node is a leaf node
        if (node->Left == nullptr && node->Right == nullptr) {
            return nullptr;
        }

        // When the node has a single child
        if (node->Left != nullptr) {
            return node->Left;
        }
        return node->Right;
    }

    /// Fix double black violations for the given node
    /// @param node Node to fix violations for
    void fixDoubleBlackViolation(Node* node) {
        assert(node != nullptr);

        if (node == m_RootNode)
            // Reached root
            return;

        Node* sibling = node->getSibling();
        Node* parent = node->Parent;

        if (sibling == nullptr) {
            // No sibling, push the double black up to the parent
            fixDoubleBlackViolation(parent);
        } else {
            if (sibling->IsRed) {
                parent->IsRed = true;
                sibling->IsRed = false;

                if (sibling->isLeftNode()) {
                    rotateRight(parent);
                } else {
                    rotateLeft(parent);
                }

                fixDoubleBlackViolation(node);
            } else {
                if (sibling->hasRedChild()) {
                    if (sibling->Left != nullptr && sibling->Left->IsRed) {
                        if (sibling->isLeftNode()) {
                            // left left
                            sibling->Left->IsRed = sibling->IsRed;
                            sibling->IsRed = parent->IsRed;
                            rotateRight(parent);
                        } else {
                            // right left
                            sibling->Left->IsRed = parent->IsRed;
                            rotateRight(sibling);
                            rotateLeft(parent);
                        }
                    } else {
                        if (sibling->isLeftNode()) {
                            // left right
                            sibling->Right->IsRed = parent->IsRed;
                            rotateLeft(sibling);
                            rotateRight(parent);
                        } else {
                            // right right
                            sibling->Right->IsRed = sibling->IsRed;
                            sibling->IsRed = parent->IsRed;
                            rotateLeft(parent);
                        }
                    }
                    parent->IsRed = false;
                } else {
                    // 2 black children
                    sibling->IsRed = true;
                    if (parent->IsRed == false) {
                        fixDoubleBlackViolation(parent);
                    } else {
                        parent->IsRed = false;
                    }
                }
            }
        }
    }

    /// Remove the given node from the tree without deallocating it
    /// @param nodeToRemove Node to remove
    /// @return AXR_SUCCESS if the function succeeded and the node can be deallocated
    [[nodiscard]] AxrResult removeNode(Node* nodeToRemove) {
        // Red black tree deletion algorithm reference:
        // https://www.geeksforgeeks.org/dsa/deletion-in-red-black-tree/

        assert(nodeToRemove != nullptr);

        Node* nodeToReplace = getNodeToReplace(nodeToRemove);

        // When nodeToRemove and nodeToReplace are both black, a 'double black' violation has occurred
        const bool doubleBlackViolation = ((nodeToReplace == nullptr || !nodeToReplace->IsRed) && !nodeToRemove->IsRed);

        if (nodeToReplace == nullptr) {
            // If we get here, nodeToRemove must be a leaf node
            if (nodeToRemove == m_RootNode) {
                m_RootNode = nullptr;
            } else {
                if (doubleBlackViolation) {
                    // nodeToRemove is a leaf node, so we fix the double black at nodeToRemove
                    fixDoubleBlackViolation(nodeToRemove);
                } else {
                    // If we get here, nodeToRemove is red, and nodeToReplace is black
                    Node* sibling = nodeToRemove->getSibling();
                    if (sibling != nullptr) {
                        sibling->IsRed = true;
                    }
                }

                // Remove nodeToRemove from the tree
                if (nodeToRemove->isLeftNode()) {
                    nodeToRemove->Parent->Left = nullptr;
                } else {
                    nodeToRemove->Parent->Right = nullptr;
                }
            }

            return AXR_SUCCESS;
        }

        if (nodeToRemove->Left == nullptr || nodeToRemove->Right == nullptr) {
            // If we get here, nodeToRemove has 1 child

            if (nodeToRemove == m_RootNode) {
                nodeToReplace->Parent = nullptr;
                m_RootNode = nodeToReplace;
                m_RootNode->IsRed = false;
                return AXR_SUCCESS;
            }

            // Detach nodeToRemove from tree and move nodeToReplace up
            if (nodeToRemove->isLeftNode()) {
                nodeToRemove->Parent->Left = nodeToReplace;
            } else {
                nodeToRemove->Parent->Right = nodeToReplace;
            }
            nodeToReplace->Parent = nodeToRemove->Parent;

            if (doubleBlackViolation) {
                fixDoubleBlackViolation(nodeToReplace);
            } else {
                // If we got here, either nodeToRemove or nodeToReplace was red.
                // So make sure nodeToReplace is black.
                nodeToReplace->IsRed = false;
            }

            return AXR_SUCCESS;
        }

        // If we get here, nodeToRemove has 2 children.

        // Swap nodes rather than just swap data because there could be external pointers to that node, and we don't
        // want to change the data for those external pointers unexpectedly.
        swapNodes(nodeToReplace, nodeToRemove);
        return removeNode(nodeToRemove);
    }
};
