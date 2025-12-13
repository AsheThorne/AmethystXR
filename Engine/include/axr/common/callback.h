#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "utils.h"

#include <functional>
#include <type_traits>

template<typename>
class AxrCallback;

/// Generic callback function type
/// A simplified version of entt::delegate<>. Works the same except we can't use shorter function signatures
/// (https://github.com/skypjack/entt/wiki/Events,-signals-and-everything-in-between#delegate)
/// @tparam Return_T Return type
/// @tparam Args Callback arguments
template<typename Return_T, typename... Args>
class AxrCallback<Return_T(Args...)> {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //
    using ReturnNoConst_T = std::remove_const_t<Return_T>;
    using Function_T = ReturnNoConst_T(const void* instance, Args... args);

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    // ---- Constructors ----

    /// Constructor
    AxrCallback() = default;

    // ---- Operator overloads ----

    /// bool Operator overload
    /// @returns True if a function pointer has been defined
    explicit operator bool() const {
        return m_Function != nullptr;
    }

    /// == Operator overload
    /// @param other Other callback to check equality with
    /// @returns True if this callback and the other callback are equal
    bool operator==(const AxrCallback<Return_T(Args...)>& other) const {
        return m_Function == other.m_Function && m_Instance == other.m_Instance;
    }

    /// != Operator overload
    /// @param other Other callback to check equality with
    /// @returns True if this callback and the other callback are not equal
    bool operator!=(const AxrCallback<Return_T(Args...)>& other) const {
        return !(*this == other);
    }

    /// () Operator overload
    /// @param args Function arguments
    Return_T operator()(Args... args) const {
        if (m_Function == nullptr)
            return static_cast<Return_T>(0);

        return m_Function(m_Instance, std::forward<Args>(args)...);
    }

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Connect a function pointer to this callback
    /// @tparam Candidate Function pointer
    template<auto Candidate>
    void connect() {
        m_Instance = nullptr;
        m_Function = [](const void*, Args... args) -> ReturnNoConst_T {
            return Return_T(std::invoke(Candidate, std::forward<Args>(args)...));
        };
    }

    /// Connect a function pointer to this callback
    /// @tparam Candidate Function pointer
    /// @tparam Type Instance type
    template<auto Candidate, typename Type>
    void connect(Type& instance) {
        m_Instance = &instance;
        m_Function = [](const void* payload, Args... args) -> ReturnNoConst_T {
            Type* curr = static_cast<Type*>(const_cast<AxrConstnessAs_T<void, Type>*>(payload));
            return Return_T(std::invoke(Candidate, *curr, std::forward<Args>(args)...));
        };
    }

    /// Connect a function pointer to this callback
    /// @tparam Candidate Function pointer
    /// @tparam Type Instance type
    template<auto Candidate, typename Type>
    void connect(Type* instance) {
        m_Instance = instance;
        m_Function = [](const void* payload, Args... args) -> ReturnNoConst_T {
            Type* curr = static_cast<Type*>(const_cast<AxrConstnessAs_T<void, Type>*>(payload));
            return Return_T(std::invoke(Candidate, curr, std::forward<Args>(args)...));
        };
    }

    /// Reset the callback function pointer
    void reset() {
        m_Instance = nullptr;
        m_Function = nullptr;
    }

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    const void* m_Instance = nullptr;
    Function_T* m_Function = nullptr;
};
