#pragma once

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "callback.h"

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <unordered_set>

/// Generic event handler
/// @tparam Args Callback arguments
template <typename... Args>
class AxrEventHandler {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    /// Event handler callback function signature
    using Callback_T = AxrCallback<void, Args...>;

    // ----------------------------------------- //
    // Public Functions
    // ----------------------------------------- //

    /// Add a new callback function
    /// @param callback Callback to add
    void addCallback(const Callback_T& callback);
    /// Remove a callback function
    /// @param function Callback function to remove
    void removeCallback(const typename Callback_T::CallbackFunction_T& function);
    /// Invoke all callback functions
    /// @param params Parameters to use for the callback functions
    void invoke(Args... params);
    /// Clear all callback functions
    void clear();

private:
    // ----------------------------------------- //
    // Private Variables
    // ----------------------------------------- //
    std::vector<Callback_T> m_Callbacks;
};

template <typename... Args>
void AxrEventHandler<Args...>::addCallback(const Callback_T& callback) {
    for (const Callback_T& cb : m_Callbacks) {
        // Don't add a callback that already exists
        if (cb == callback) {
            return;
        }
    }
    m_Callbacks.push_back(callback);
}

template <typename... Args>
void AxrEventHandler<Args...>::removeCallback(const typename Callback_T::CallbackFunction_T& function) {
    for (auto it = m_Callbacks.begin(); it != m_Callbacks.end(); ++it) {
        if ((*it).Function == function) {
            m_Callbacks.erase(it);
            return;
        }
    }
}

template <typename... Args>
void AxrEventHandler<Args...>::invoke(Args... params) {
    for (const Callback_T& callback : m_Callbacks) {
        callback(std::forward<Args>(params)...);
    }
}

template <typename... Args>
void AxrEventHandler<Args...>::clear() {
    m_Callbacks.clear();
}
