#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <type_traits>

// ----------------------------------------- //
// Structs
// ----------------------------------------- //

/// Make `To` the same constness as `From`
/// Remove const from `To` if `From` isn't const
template<typename To, typename From>
struct AxrConstnessAs {
    using Type = std::remove_const_t<To>;
};

/// Make `To` the same constness as `From`
/// Make `To` const if `From` is const and `To` isn't
template<typename To, typename From>
struct AxrConstnessAs<To, const From> {
    using Type = const To;
};

// ----------------------------------------- //
// Types
// ----------------------------------------- //

/// Get the type of `To` with the same constantness as `From`
template<typename To, typename From>
using AxrConstnessAs_T = typename AxrConstnessAs<To, From>::Type;
