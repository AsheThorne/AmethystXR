#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <concepts>

/// Determine if each type is unique
template<typename...>
struct AxrUniqueTypesStruct : std::true_type {};

/// Determine if each type is unique
template<typename Type, typename... Types>
struct AxrUniqueTypesStruct<Type, Types...> :
    std::bool_constant<(!std::same_as<Type, Types> && ...) && AxrUniqueTypesStruct<Types...>::value> {};

/// Determine if each type is unique
template<typename... Types>
concept AxrUniqueTypes = AxrUniqueTypesStruct<Types...>::value;
