#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "../../memory/dynamicAllocator.h"
#include "../../utils.h"

// Resources used/referenced:
// https://www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation
// https://gist.github.com/ssylvan/5538011
// https://codecapsule.com/2013/11/17/robin-hood-hashing-backward-shift-deletion/#ref
// https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1214/lectures/18/Slides18.pdf

// TODO (Ashe): Resize the map when the load factor reaches 90%. Simply double the capacity.

/// Axr Unordered Map that uses a dynamic allocator
/// @tparam Key_T Key type
/// @tparam Value_T Value type
template<typename Key_T, typename Value_T>
class AxrUnorderedMap_Dynamic {
private:
    // ----------------------------------------- //
    // Private Structs
    // ----------------------------------------- //

    class Item {
    public:
        using Hash_T = uint32_t;

        Key_T Key{};
        Value_T Value{};
        Hash_T Hash{};
        static constexpr Hash_T const& UninitializedHashValue = 0;

        // ----------------------------------------- //
        // Special Functions
        // ----------------------------------------- //

        // ---- Constructors ----

        /// Default Constructor
        Item() = default;

        /// Constructor
        /// @param key Key
        /// @param value Value
        /// @param hash hash
        template<typename V_T>
        Item(const Key_T& key, V_T&& value, const Hash_T hash) :
            Key(key),
            Value(std::forward<V_T>(value)),
            Hash(hash) {
        }

        /// Copy Constructor
        /// @param src Source Item to copy from
        Item(const Item& src) = delete;

        /// Move Constructor
        /// @param src Source Item to move from
        Item(Item&& src) noexcept {
            move_internal(std::move(src));
        }

        // ---- Destructor ----

        /// Destructor
        ~Item() {
            cleanup();
        }

        // ---- Operator Overloads ----

        /// Copy Assignment Operator
        /// @param src Source Item to copy from
        Item& operator=(const Item& src) = delete;

        /// Move Assignment Operator
        /// @param src Source Item to move from
        Item& operator=(Item&& src) noexcept {
            if (this != &src) {
                cleanup();

                move_internal(std::move(src));
            }
            return *this;
        }

    private:
        // ----------------------------------------- //
        // Private Functions
        // ----------------------------------------- //

        /// Clean up this class
        void cleanup() {
            Key.~Key_T();
            Value.~Value_T();

            // Reset the hash value so we know the slot this item is in is empty
            Hash = UninitializedHashValue;
        }

        /// Move the given Item to this class
        /// @param src Item to move
        void move_internal(Item&& src) {
            Key = std::move(src.Key);
            Value = std::move(src.Value);
            Hash = std::move(src.Hash);
        }
    };

public:
    // ----------------------------------------- //
    // Iterator
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
        /// @param item The item for this iterator to point to
        explicit Iterator(const Item* item) :
            m_Item(item) {
        }

        // ---- Operator Overloads ----

        /// Prefix increment operator overload
        /// @return The iterator, incremented by 1
        Iterator& operator++() {
            if (m_Item == nullptr) [[unlikely]] {
                return *this;
            }

            do {
                ++m_Item;
            } while (m_Item->Hash == 0);

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
            if (m_Item == nullptr) [[unlikely]] {
                return *this;
            }

            do {
                --m_Item;
            } while (m_Item->Hash == 0);

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
            return m_Item == other.m_Item;
        }

        /// Inequality operator overload
        /// @other Iterator to compare against
        /// @return True if both iterators point to different nodes
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        /// Get the underlining data for this iterator
        /// @return The underlining data for this iterator
        std::pair<Key_T, const Value_T&> operator*() const {
            return std::pair<Key_T, const Value_T&>(m_Item->Key, m_Item->Value);
        }

    private:
        // ----------------------------------------- //
        // Private Variables
        // ----------------------------------------- //
        const Item* m_Item{};
    };

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrUnorderedMap_Dynamic() = default;

#define AXR_FUNCTION_FAILED_STRING "Failed to initialize AxrUnorderedMap_Dynamic. "
    /// Constructor
    /// @param capacity The max number of items this map can hold
    /// @param allocator Dynamic allocator to use
    explicit AxrUnorderedMap_Dynamic(const size_t capacity, AxrDynamicAllocator* allocator) :
        m_DynamicAllocator(allocator),
        m_Capacity(capacity),
        m_IndexWraparoundMask(capacity - 1) {
        // Make sure capacity is a power of two.
        // We need it to be so we can use `hash & (capacity - 1)` instead of `index = capacity % hash` for better
        // performance.
        assert(capacity > 0 && (capacity & (capacity - 1)) == 0);

        if (AXR_FAILED(allocateData())) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "`allocateData()` failed.");
            return;
        }
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Copy Constructor
    /// @param src Source AxrUnorderedMap_Dynamic to copy from
    AxrUnorderedMap_Dynamic(const AxrUnorderedMap_Dynamic& src) = delete;

    /// Move Constructor
    /// @param src Source AxrUnorderedMap_Dynamic to move from
    AxrUnorderedMap_Dynamic(AxrUnorderedMap_Dynamic&& src) noexcept {
        move_internal(std::move(src));
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrUnorderedMap_Dynamic() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrUnorderedMap_Dynamic to copy from
    AxrUnorderedMap_Dynamic& operator=(const AxrUnorderedMap_Dynamic& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrUnorderedMap_Dynamic to move from
    AxrUnorderedMap_Dynamic& operator=(AxrUnorderedMap_Dynamic&& src) noexcept {
        if (this != &src) {
            cleanup();

            move_internal(std::move(src));
        }
        return *this;
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Check if the map has been allocated
    /// @return True if the map has been allocated
    [[nodiscard]] bool allocated() const {
        return m_DataHandle != nullptr;
    }

    /// Get the map's capacity
    /// @return The map's capacity
    [[nodiscard]] size_t capacity() const {
        return m_Capacity;
    }

    /// Get the map's size
    /// @return The map's size
    [[nodiscard]] size_t size() const {
        return m_Size;
    }

    /// Check if the map is empty
    /// @return True if the map is empty
    [[nodiscard]] bool empty() const {
        return m_Size == 0;
    }

    /// Get the beginning iterator
    /// @return The beginning iterator
    [[nodiscard]] Iterator begin() const {
        return Iterator(&m_DataHandle[0]);
    }

    /// Get the end iterator
    /// @return The end iterator
    [[nodiscard]] Iterator end() const {
        return Iterator(&m_DataHandle[m_Capacity]);
    }

    /// Find the item with the given key
    /// @param key Key to search for
    /// @return The item with the given key's iterator. Or the iterator end if the key wasn't found.
    [[nodiscard]] Iterator find(const Key_T& key) const {
        const size_t foundIndex = findIndex(key);

        if (foundIndex == SIZE_MAX) {
            return end();
        }

        return Iterator(&m_DataHandle[foundIndex]);
    }

    /// Check if the given key exists in this map
    /// @param key Key to check
    /// @return True if the key exists
    [[nodiscard]] bool exists(const Key_T& key) const {
        return find(key) != end();
    }

    /// Insert a new key value pair
    /// @param key Key to insert
    /// @param value Value to insert
    /// @return The inserted item iterator. Or the iterator end if we failed to insert.
    Iterator insert(const Key_T& key, const Value_T& value) {
        return insert_internal(key, value);
    }

    /// Insert a new key value pair
    /// @param key Key to insert
    /// @param value Value to insert
    /// @return The inserted item iterator. Or the iterator end if we failed to insert.
    Iterator insert(const Key_T& key, Value_T&& value) {
        return insert_internal(key, std::move(value));
    }

    /// Remove the item with the given key
    /// @param key Key to remove
    void remove(const Key_T& key) {
        const typename Item::Hash_T hash = hashKey(key);
        const int foundIndex = findIndex(key);

        if (foundIndex == SIZE_MAX) [[unlikely]] {
            // Key doesn't exist
            return;
        }

        axrCallDestructor(m_DataHandle[foundIndex]);

        size_t previousPosition = foundIndex;
        size_t position = (foundIndex + 1) & m_IndexWraparoundMask;
        // Continue to shift back each item until the slot is either empty or we reach an item that is already where it
        // wants to be.
        while (m_DataHandle[position].Hash != Item::UninitializedHashValue && getProbeDistance(position) != 0) {
            std::swap(m_DataHandle[previousPosition], m_DataHandle[position]);
            previousPosition = position;
            position = (position + 1) & m_IndexWraparoundMask;
        }

        --m_Size;
    }

    /// Clear all items from this map
    void clear() {
        if (empty()) {
            return;
        }

        for (size_t i = 0; i < m_Capacity; ++i) {
            if (m_DataHandle[i].Hash == 0) {
                continue;
            }
            axrCallDestructor(m_DataHandle[i]);
        }

        m_Size = 0;
    }

    static constexpr size_t getItemSize() {
        return sizeof(Item);
    }

    static constexpr size_t getItemAlignment() {
        return alignof(Item);
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    AxrDynamicAllocator* m_DynamicAllocator{};
    AxrHandle<Item> m_DataHandle{};
    size_t m_Capacity{};
    size_t m_Size{};
    size_t m_IndexWraparoundMask{};

    /// Clean up this class
    void cleanup() {
        clear();
        deallocateData();

        m_DynamicAllocator = {};
        m_Capacity = {};
        m_Size = {};
        m_IndexWraparoundMask = {};
    }

    /// Move the given AxrUnorderedMap_Dynamic to this class
    /// @param src AxrUnorderedMap_Dynamic to move
    void move_internal(AxrUnorderedMap_Dynamic&& src) {
        m_DataHandle = std::move(src.m_DataHandle);

        m_DynamicAllocator = src.m_DynamicAllocator;
        m_Capacity = src.m_Capacity;
        m_Size = src.m_Size;
        m_IndexWraparoundMask = src.m_IndexWraparoundMask;

        src.m_DynamicAllocator = {};
        src.m_Capacity = {};
        src.m_Size = {};
        src.m_IndexWraparoundMask = {};
    }

#define AXR_FUNCTION_FAILED_STRING "Failed to insert a new item into the AxrUnorderedMap_Dynamic. "
    /// Insert a new key value pair
    /// @param key Key to insert
    /// @param value Value to insert
    /// @return The inserted item iterator. Or the iterator end if we failed to insert.
    template<typename V_T>
    Iterator insert_internal(const Key_T& key, V_T&& value) {
        if (m_Size + 1 > m_Capacity) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Unordered map is full.");
            return end();
        }

        const typename Item::Hash_T hash = hashKey(key);
        size_t position = getDesiredIndex(hash);
        size_t probeDistance = 0;

        Item itemToInsert(key, std::forward<V_T>(value), hash);

        size_t indexInserted = SIZE_MAX;

        // (Ashe) - This looks like it might infinitely loop, but it won't. It will only infinitely loop if there are no
        // free slots. But because we check above to make sure there is space, we're ok.
        while (true) {
            // If the position is free, use it for the new item
            if (m_DataHandle[position].Hash == Item::UninitializedHashValue) {
                ::new (&m_DataHandle[position]) Item(std::move(itemToInsert));
                ++m_Size;

                // Keep track of where we inserted the original item that was asked to be inserted.
                // If it is SIZE_MAX, then we haven't set it yet. So this must be the new item that is being inserted.
                if (indexInserted == SIZE_MAX) {
                    indexInserted = position;
                }

                return Iterator(&m_DataHandle[indexInserted]);
            }

            // If the existing item has the same key, then fail the insert
            if (m_DataHandle[position].Key == itemToInsert.Key) {
                axrLogError(AXR_FUNCTION_FAILED_STRING "Key already exists.");
                return end();
            }

            // If the existing element has probed less than us, then swap the existing item with the new item and loop
            // again to find a new slot for the existing item.
            const size_t existingElementProbeDistance = getProbeDistance(position);
            if (existingElementProbeDistance < probeDistance) {
                std::swap(m_DataHandle[position], itemToInsert);
                probeDistance = existingElementProbeDistance;

                // Keep track of where we inserted the original item that was asked to be inserted.
                // If it is SIZE_MAX, then we haven't set it yet. So this must be the new item that is being inserted.
                if (indexInserted == SIZE_MAX) {
                    indexInserted = position;
                }
            }

            position = (position + 1) & m_IndexWraparoundMask;
            ++probeDistance;
        }
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to allocate AxrUnorderedMap_Dynamic data. "
    /// Allocate the data we need
    /// @return AXR_SUCCESS if the function succeeded
    [[nodiscard]] AxrResult allocateData() {
        assert(m_DynamicAllocator != nullptr);

        if (m_DataHandle != nullptr) [[unlikely]] {
            axrLogWarning("Data has already been allocated.");
            return AXR_SUCCESS;
        }

        const AxrResult axrResult = m_DynamicAllocator->allocate(m_Capacity, m_DataHandle);
        if (AXR_FAILED(axrResult)) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Failed to allocate memory.");
            return axrResult;
        }

        return AXR_SUCCESS;
    }
#undef AXR_FUNCTION_FAILED_STRING

#define AXR_FUNCTION_FAILED_STRING "Failed to deallcoate AxrUnorderedMap_Dynamic data. "
    /// Deallocate the data block.
    void deallocateData() {
        if (m_DataHandle == nullptr) {
            return;
        }

        if (m_DynamicAllocator == nullptr) [[unlikely]] {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Memory leak detected. DynamicAllocator is null.");
            return;
        }

        m_DynamicAllocator->deallocate(m_DataHandle);
    }
#undef AXR_FUNCTION_FAILED_STRING

    /// Hash the given key
    /// @param key Key to hash
    /// @return The hashed key
    [[nodiscard]] Item::Hash_T hashKey(const Key_T& key) const {
        const std::hash<Key_T> hasher;
        auto hash = static_cast<Item::Hash_T>(hasher(key));

        // If the hash is the same as an uninitialized hash, set it to 1 instead.
        hash |= hash == Item::UninitializedHashValue;

        return hash;
    }

    /// Get the desired position index for the given hash
    /// @param hash Hash to use
    /// @return The desired position index
    [[nodiscard]] size_t getDesiredIndex(const Item::Hash_T hash) const {
        return hash & m_IndexWraparoundMask;
    }

    /// Get the distance the given index is from its desired index
    /// @param index Index to probe
    /// @return The distance from the desired index
    [[nodiscard]] size_t getProbeDistance(const size_t index) const {
        return (index + m_Capacity - getDesiredIndex(m_DataHandle[index].Hash)) & m_IndexWraparoundMask;
    }

    /// Find the index for the item with the given key
    /// @param key Key to search for
    /// @return Array index location. Or `SIZE_MAX` if the key doesn't exist.
    [[nodiscard]] size_t findIndex(const Key_T& key) const {
        const typename Item::Hash_T hash = hashKey(key);
        size_t position = getDesiredIndex(hash);
        size_t probeDistance = 0;

        // Continue the loop until we either return early or have searched every element
        while (probeDistance < m_Capacity - 1) {
            if (m_DataHandle[position].Hash == Item::UninitializedHashValue) {
                // Found an empty slot. Key doesn't exist.
                return SIZE_MAX;
            }

            if (probeDistance > getProbeDistance(position)) {
                // Encountered an item with a different desired index. Key doesn't exist.
                return SIZE_MAX;
            }

            if (m_DataHandle[position].Key == key) {
                return position;
            }

            position = (position + 1) & m_IndexWraparoundMask;
            ++probeDistance;
        }

        // Searched the entire map and failed to find the given key
        return SIZE_MAX;
    }
};

#undef AXR_UNINITIALIZED_HASH
