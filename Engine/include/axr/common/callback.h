#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <utility>

/// Generic callback
/// @tparam T Return type
/// @tparam Args Callback arguments
template <typename T, typename... Args>
class AxrCallback {
public:
    // ----------------------------------------- //
    // Types
    // ----------------------------------------- //

    // TODO: Discovered something cool. we can use class functions (meaning we won't need to do the static function fuckery)
    //  by defining the class in the (*) section. like, T(AxrClass::*)(void* userData, Args... args).
    //  So find a way to use that. but it'd be nice to somehow have the option to still use static functions like we are now
    /// Callback function type
    using CallbackFunction_T = T(*)(void* userData, Args... args);

    // ----------------------------------------- //
    // Public Variables
    // ----------------------------------------- //
    void* UserData;
    CallbackFunction_T Function;

    // ----------------------------------------- //
    // Special Functions
    // ----------------------------------------- //

    /// Default Constructor
    AxrCallback():
        UserData(nullptr),
        Function(nullptr) {
    }

    /// Constructor
    /// @param userData User data
    /// @param function Callback function
    AxrCallback(void* userData, const CallbackFunction_T function):
        UserData(userData),
        Function(function) {
    }

    // ---- Operator overloads ----

    /// == Operator overload
    /// @param other Other callback to check equality with
    /// @returns True if this callback and the other callback are equal
    bool operator==(const AxrCallback& other) const {
        return Function == other.Function;
    }

    /// () Operator overload
    /// @param args Function arguments
    T operator()(Args... args) const {
        if (Function == nullptr) return static_cast<T>(0);

        return Function(UserData, std::forward<Args>(args)...);
    }
};
