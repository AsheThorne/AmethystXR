#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include <concepts>
#include <cstddef>

// ----------------------------------------- //
// Concepts
// ----------------------------------------- //

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

template<typename Type>
concept AxrIsChar8Like =
    std::is_same_v<std::remove_cv_t<std::remove_reference_t<Type>>, const char8_t*> ||
    (std::is_array_v<std::remove_reference_t<Type>> &&
     std::is_same_v<std::remove_cv_t<std::remove_extent_t<std::remove_reference_t<Type>>>, char8_t>);

// ----------------------------------------- //
// Functions
// ----------------------------------------- //

/// Check if the given type is of type `char[]`
template<typename Type>
[[nodiscard]] constexpr bool axrIsTypeCharArray() {
    return std::is_array_v<Type> && std::is_same_v<std::remove_extent_t<Type>, char>;
}

/// Check if the given type is of type `const char*`
template<typename Type>
[[nodiscard]] constexpr bool axrIsTypeConstCharPtr() {
    return std::is_same_v<std::remove_extent_t<Type>, const char*>;
}

/// If this type is an array, get the number of elements it holds
/// @return The number of elements the `Type` array holds. Or 0 if it's not an array.
template<typename Type>
[[nodiscard]] constexpr size_t axrGetArrayLength() {
    if constexpr (std::is_array_v<Type>) {
        return sizeof(Type) / sizeof(std::remove_extent_t<Type>);
    }
    return 0;
}

/// Call the destructor for the given item
/// @param item item to destruct
template<typename Type>
void axrCallDestructor(Type& item) {
    if constexpr (std::is_array_v<Type>) {
        for (size_t i = 0; i < axrGetArrayLength<Type>(); ++i) {
            axrCallDestructor(item[i]);
        }
    } else {
        item.~Type();
    }
}
