#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //

// ----------------------------------------- //
// Forward Declarations
// ----------------------------------------- //
class AxrDynamicAllocator;

/// A pointer handle.
/// Used in memory allocators where the original pointer may be relocated due to factors like defragmentation.
/// @tparam Type The pointer data type
template<typename Type>
class AxrHandle {
public:
    // ----------------------------------------- //
    // Friends
    // ----------------------------------------- //
    friend AxrDynamicAllocator;

    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    using Deallocator_T = AxrCallback<void(AxrHandle<Type>& handle)>;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrHandle() = default;

    /// Constructor
    /// @param data The data handle
    /// @param deallocator The function callback to deallocate the given data
    explicit AxrHandle(Type* const* data, const Deallocator_T& deallocator) :
        m_Data(data),
        m_Deallocator(deallocator) {
    }

    /// Copy Constructor
    /// @param src Source AxrHandle to copy from
    AxrHandle(const AxrHandle& src) = delete;

    /// Move Constructor
    /// @param src Source AxrHandle to move from
    AxrHandle(AxrHandle&& src) noexcept {
        move_internal(std::move(src));
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrHandle() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrHandle to copy from
    AxrHandle& operator=(const AxrHandle& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrHandle to move from
    AxrHandle& operator=(AxrHandle&& src) noexcept {
        if (this != &src) {
            cleanup();

            move_internal(std::move(src));
        }
        return *this;
    }

    /// Null equality operator overload
    /// @return True if this handle is null
    bool operator==(nullptr_t) const noexcept {
        return m_Data == nullptr || *m_Data == nullptr;
    }

    /// Null equality operator overload
    /// @return True if this handle is not null
    bool operator!=(nullptr_t) const noexcept {
        return m_Data != nullptr && *m_Data != nullptr;
    }

    /// Dereference operator overload
    /// @return A reference to this handles data
    Type& operator*() {
        return **m_Data;
    }

    /// Dereference operator overload
    /// @return A reference to this handles data
    const Type& operator*() const {
        return **m_Data;
    }

    /// Pointer operator overload
    /// @return A pointer to this handles data
    Type* operator->() {
        if (m_Data == nullptr) {
            return nullptr;
        }

        return *m_Data;
    }

    /// Pointer operator overload
    /// @return A pointer to this handles data
    const Type* operator->() const {
        if (m_Data == nullptr) {
            return nullptr;
        }

        return *m_Data;
    }

    /// The subscript operator overload
    /// @param index The data array index
    /// @return The data at the given index
    const Type& operator[](size_t index) const {
        return (*m_Data)[index];
    }

    /// The subscript operator overload
    /// @param index The data array index
    /// @return The data at the given index
    Type& operator[](size_t index) {
        return (*m_Data)[index];
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the pointer to the data
    /// @return The pointer to the data
    [[nodiscard]] Type* getDataPtr() {
        if (m_Data == nullptr) {
            return nullptr;
        }

        return *m_Data;
    }

    /// Get the pointer to the data
    /// @return The pointer to the data
    [[nodiscard]] const Type* getDataPtr() const {
        if (m_Data == nullptr) {
            return nullptr;
        }

        return *m_Data;
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    Type* const* m_Data{};
    Deallocator_T m_Deallocator{};

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        m_Deallocator(*this);

        m_Data = nullptr;
        m_Deallocator = {};
    }

    /// Move the given AxrHandle to this class
    /// @param src AxrHandle to move
    void move_internal(AxrHandle&& src) {
        m_Data = src.m_Data;
        m_Deallocator = src.m_Deallocator;

        src.m_Data = {};
        src.m_Deallocator = {};
    }
};

/// A `void` specialization pointer handle.
/// Used in memory allocators where the original pointer may be relocated due to factors like defragmentation.
template<>
class AxrHandle<void> {
public:
    // ----------------------------------------- //
    // Friends
    // ----------------------------------------- //
    friend AxrDynamicAllocator;

    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    using Deallocator_T = AxrCallback<void(AxrHandle<void>& handle)>;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Default Constructor
    AxrHandle() = default;

    /// Constructor
    /// @param data The data handle
    /// @param deallocator The function callback to deallocate the given data
    explicit AxrHandle(void* const* data, const Deallocator_T& deallocator) :
        m_Data(data),
        m_Deallocator(deallocator) {
    }

    /// Copy Constructor
    /// @param src Source AxrHandle to copy from
    AxrHandle(const AxrHandle& src) = delete;

    /// Move Constructor
    /// @param src Source AxrHandle to move from
    AxrHandle(AxrHandle&& src) noexcept {
        move_internal(std::move(src));
    }

    // ---- Destructor ----

    /// Destructor
    ~AxrHandle() {
        cleanup();
    }

    // ---- Operator Overloads ----

    /// Copy Assignment Operator
    /// @param src Source AxrHandle to copy from
    AxrHandle& operator=(const AxrHandle& src) = delete;

    /// Move Assignment Operator
    /// @param src Source AxrHandle to move from
    AxrHandle& operator=(AxrHandle&& src) noexcept {
        if (this != &src) {
            cleanup();

            move_internal(std::move(src));
        }
        return *this;
    }

    // ---- Operator Overloads ----

    /// Dereference operator overload
    /// @return A reference to this handles data
    void operator*() = delete;

    /// Dereference operator overload
    /// @return A reference to this handles data
    void operator*() const = delete;

    /// Pointer operator overload
    /// @return A pointer to this handles data
    void* operator->() = delete;

    /// Pointer operator overload
    /// @return A pointer to this handles data
    const void* operator->() const = delete;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Get the pointer to the data
    /// @return The pointer to the data
    void* getDataPtr() {
        if (m_Data == nullptr) {
            return nullptr;
        }

        return *m_Data;
    }

    /// Get the pointer to the data
    /// @return The pointer to the data
    [[nodiscard]] const void* getDataPtr() const {
        if (m_Data == nullptr) {
            return nullptr;
        }

        return *m_Data;
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    void* const* m_Data{};
    Deallocator_T m_Deallocator{};

    // ----------------------------------------- //
    // Private Functions
    // ----------------------------------------- //

    /// Clean up this class
    void cleanup() {
        m_Deallocator(*this);

        m_Data = nullptr;
        m_Deallocator = {};
    }

    /// Move the given AxrHandle to this class
    /// @param src AxrHandle to move
    void move_internal(AxrHandle&& src) {
        m_Data = src.m_Data;
        m_Deallocator = src.m_Deallocator;

        src.m_Data = {};
        src.m_Deallocator = {};
    }
};
