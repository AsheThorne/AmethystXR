#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_set>

/// Generic event handler
template <typename... Args>
class AxrEventHandler {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// Event handler callback function signature
    using CallbackFunction_T = void(*)(void* userData, Args... params);

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Add a new callback function
    /// @param userData User data to pass through to the function
    /// @param function Callback function to add
    void addCallback(void* userData, const CallbackFunction_T& function);
    /// Remove a callback function
    /// @param function Callback function to remove
    void removeCallback(const CallbackFunction_T& function);
    /// Invoke all callback functions
    /// @param params Parameters to use for the callback functions
    void invoke(Args... params);
    /// Clear all callback functions
    void clear();

private:
    // ----------------------------------------- //
    // Private Structs
    // ----------------------------------------- //

    /// Callback function data
    class Callback {
    public:
        void* UserData = nullptr;
        CallbackFunction_T Function = nullptr;

        /// == Operator overload
        /// @param other Other callback to check equality with
        /// @returns True if this callback and the other callback are equal
        bool operator==(const Callback& other) const {
            return Function == other.Function;
        }
        
        /// Hash function to use for the callback object
        class HashFunction {
        public:
            /// Hash the given callback
            /// @param callback Callback to hash
            /// @returns The hash value
            size_t operator()(const Callback& callback) const {
                return std::hash<CallbackFunction_T>()(callback.Function);
            }
        };
    };

    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    std::unordered_set<Callback, typename Callback::HashFunction> m_Callbacks;
};

template <typename... Args>
void AxrEventHandler<Args...>::addCallback(void* userData, const CallbackFunction_T& function) {
    m_Callbacks.insert({userData, function});
}

template <typename... Args>
void AxrEventHandler<Args...>::removeCallback(const CallbackFunction_T& function) {
    m_Callbacks.erase({nullptr, function});
}

template <typename... Args>
void AxrEventHandler<Args...>::invoke(Args... params) {
    for (const Callback& callback : m_Callbacks) {
        callback.Function(callback.UserData, params...);
    }
}

template <typename... Args>
void AxrEventHandler<Args...>::clear() {
    m_Callbacks.clear();
}
