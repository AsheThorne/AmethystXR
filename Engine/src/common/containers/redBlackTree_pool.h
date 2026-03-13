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
        /// @param parent Parent node
        explicit Node(const Type& data, Node* parent) :
            Parent(parent),
            Data(data),
            IsRed(true) {
        }

        // ----------------------------------------- //
        // Public Functions
        // ----------------------------------------- //

        /// Check if this node is on the left of the binary tree
        /// @return True if this node is on the left of the binary tree
        bool isLeftNode() {
            return Parent != nullptr && this == Parent->Left;
        }

        /// Check if this node is on the right of the binary tree
        /// @return True if this node is on the right of the binary tree
        bool isRightNode() {
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
        bool hasRedChild() {
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

            /// Constructor
            explicit Iterator(Node* node) :
                m_Node(node) {
                assert(m_Node != nullptr);
            }

            // ---- Operator Overloads ----

            /// Prefix increment operator overload
            /// @return The iterator, incremented by 1
            Iterator& operator++() {
                // First, we try finding the node's successor
                if (m_Node->Right != nullptr) {
                    Node* currentNode = m_Node->Right;
                    while (currentNode->Left != nullptr) {
                        currentNode = currentNode->Left;
                    }
                    m_Node = currentNode;
                    return *this;
                }

                // If there's no successor, then the next node in the sequence will be the parent of the next left
                // parent node
                Node* currentNode = m_Node;
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
                // First, we try finding the node's predecessor
                if (m_Node->Left != nullptr) {
                    Node* currentNode = m_Node->Left;
                    while (currentNode->Right != nullptr) {
                        currentNode = currentNode->Right;
                    }
                    m_Node = currentNode;
                    return *this;
                }

                // If there's no predecessor, then the next node in the sequence will be the parent of the next right
                // parent node
                Node* currentNode = m_Node;
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
                return m_Node == other.m_Node;
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
            Node* m_Node;
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
        m_PoolAllocator = src.m_PoolAllocator;
        m_RootNode = src.m_RootNode;
        m_Size = src.m_Size;

        src.m_PoolAllocator = {};
        src.m_RootNode = {};
        src.m_Size = {};
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

            m_PoolAllocator = src.m_PoolAllocator;
            m_RootNode = src.m_RootNode;
            m_Size = src.m_Size;

            src.m_PoolAllocator = {};
            src.m_RootNode = {};
            src.m_Size = {};
        }
        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Find the node with the given data
    /// @param data Data to search for
    /// @return The node with the given data. Or nullptr if the data wasn't found.
    [[nodiscard]] const Node* find(const Type& data) const {
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

    /// Check if the given data exists in this tree
    /// @param data Data to check
    /// @return True if the data exists
    [[nodiscard]] bool exists(const Type& data) const {
        return find(data) != nullptr;
    }

    /// Insert data into the tree
    /// @param data Data to insert
    void insert(const Type& data) {
        // Red black tree insertion algorithm references:
        // https://www.geeksforgeeks.org/dsa/insertion-in-red-black-tree/
        // https://www.geeksforgeeks.org/dsa/c-program-red-black-tree-insertion/

        Node* node = bstInsert(data);
        if (node == nullptr) [[unlikely]] {
            return;
        }

        fixRedRedViolations(node);
        ++m_Size;

        // Make sure the root is always black.
        m_RootNode->IsRed = false;
    }

    /// Remove data from the tree
    /// @param data Data to remove
    void remove(const Type& data) {
        // Red black tree deletion algorithm reference:
        // https://www.geeksforgeeks.org/dsa/deletion-in-red-black-tree/

        Node* node = findNode(data);
        if (node == nullptr) {
            return;
        }

        const AxrResult axrResult = removeNode(node);
        if (AXR_SUCCEEDED(axrResult)) {
            --m_Size;
        }
    }

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

        return typename Node::Iterator(currentNode);
    }

    /// Get the end iterator
    /// @return The end iterator
    Node::Iterator end() const {
        Node* currentNode = m_RootNode;

        while (currentNode->Right != nullptr) {
            currentNode = currentNode->Right;
        }

        return typename Node::Iterator(currentNode);
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

    /// Find the node with the given data
    /// @param data Data to search for
    /// @return The node with the given data. Or nullptr if the data wasn't found.
    [[nodiscard]] Node* findNode(const Type& data) {
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
    /// Insert the given data into the tree, via standard binary search tree methods.
    /// @param data Data to insert
    /// @returns The new node that was inserted. Or nullptr if this function failed.
    [[nodiscard]] Node* bstInsert(const Type& data) {
        if (m_PoolAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Pool allocator is null.");
            return nullptr;
        }

        Node** node = &m_RootNode;
        Node* parentNode = nullptr;

        // Find where to insert the new data
        while (*node != nullptr && (*node)->Data != data) {
            if (data < (*node)->Data) {
                parentNode = *node;
                node = &(*node)->Left;
            } else {
                parentNode = *node;
                node = &(*node)->Right;
            }
        }

        if (*node != nullptr) {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Data already exists.");
            return nullptr;
        }

        // Create the new node
        const AxrResult axrResult = m_PoolAllocator->allocate(*node);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to allocate memory.");
            return nullptr;
        }

        **node = Node(data, parentNode);

        return *node;
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

    /// Swap the data between node a and b
    /// @param a First node
    /// @param b Second node
    static void swapData(Node* a, Node* b) {
        assert(a != nullptr && b != nullptr);

        Type tempData = a->Data;
        a->Data = b->Data;
        b->Data = tempData;
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

#define AXR_FUNCTION_FAILED_STRING "Failed to remove a node from the red black tree. "
    /// Delete the given node
    /// @param nodeToDelete Node to delete
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult removeNode(Node* nodeToDelete) {
        assert(nodeToDelete != nullptr);

        if (m_PoolAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Pool allocator is null.");
            return AXR_ERROR_VALIDATION_FAILED;
        }

        Node* nodeToReplace = getNodeToReplace(nodeToDelete);

        // When nodeToDelete and nodeToReplace are both black, a 'double black' violation has occurred
        const bool doubleBlackViolation = ((nodeToReplace == nullptr || !nodeToReplace->IsRed) && !nodeToDelete->IsRed);

        if (nodeToReplace == nullptr) {
            // If we get here, nodeToDelete must be a leaf node
            if (nodeToDelete == m_RootNode) {
                m_RootNode = nullptr;
            } else {
                if (doubleBlackViolation) {
                    // nodeToDelete is a leaf node, so we fix the double black at nodeToDelete
                    fixDoubleBlackViolation(nodeToDelete);
                } else {
                    // If we get here, nodeToDelete is red, and nodeToReplace is black
                    Node* sibling = nodeToDelete->getSibling();
                    if (sibling != nullptr) {
                        sibling->IsRed = true;
                    }
                }

                // Delete nodeToDelete from the tree
                if (nodeToDelete->isLeftNode()) {
                    nodeToDelete->Parent->Left = nullptr;
                } else {
                    nodeToDelete->Parent->Right = nullptr;
                }
            }

            m_PoolAllocator->deallocate(nodeToDelete);
            return AXR_SUCCESS;
        }

        if (nodeToDelete->Left == nullptr || nodeToDelete->Right == nullptr) {
            // If we get here, nodeToDelete has 1 child

            if (nodeToDelete == m_RootNode) {
                // Replace the root node data
                nodeToDelete->Data = nodeToReplace->Data;
                // Since we've already established that nodeToDelete only had 1 child, we can safely just set both
                // children to null, since we know that the child can't have any children without violating red black
                // tree properties
                nodeToDelete->Left = nullptr;
                nodeToDelete->Right = nullptr;

                m_PoolAllocator->deallocate(nodeToReplace);
                return AXR_SUCCESS;
            }

            // Detach nodeToDelete from tree and move nodeToReplace up
            if (nodeToDelete->isLeftNode()) {
                nodeToDelete->Parent->Left = nodeToReplace;
            } else {
                nodeToDelete->Parent->Right = nodeToReplace;
            }
            nodeToReplace->Parent = nodeToDelete->Parent;

            if (doubleBlackViolation) {
                fixDoubleBlackViolation(nodeToReplace);
            } else {
                // If we got here, either nodeToDelete or nodeToReplace was red.
                // So make sure nodeToReplace is black.
                nodeToReplace->IsRed = false;
            }

            m_PoolAllocator->deallocate(nodeToDelete);
            return AXR_SUCCESS;
        }

        // If we get here, nodeToDelete has 2 children.
        // Swap data with nodeToReplace and recurse for nodeToReplace (with the data of nodeToDelete)
        swapData(nodeToReplace, nodeToDelete);
        // nodeToReplace will either be a leaf node or have 1 child at this point. So we will never need to recur more
        // than once since we only recur when the node has 2 children.
        return removeNode(nodeToReplace);
    }
#undef AXR_FUNCTION_FAILED_STRING
};
