//! \file

#ifndef ENUM_CAST_H_YDJGFRENXZUIGFNEWGYF8E3Q2YGN4
#define ENUM_CAST_H_YDJGFRENXZUIGFNEWGYF8E3Q2YGN4

#include "ul/config.h"

#if __has_include(<concepts>) && __cpp_concepts && __cpp_lib_concepts
#include "concepts.h"
#include <concepts>

namespace mb::ul {
template <EnumConcept E>
constexpr std::underlying_type_t<E> enum_cast(E value) noexcept {
    return static_cast<std::underlying_type_t<E>>(value);
}

//! Synonym of enum_cast (of that direction).
template <EnumConcept E>
[[deprecated("use enum_cast")]] constexpr std::underlying_type_t<E> as_number(E value) noexcept {
    return enum_cast(value);
}

template <EnumConcept E>
constexpr E enum_cast(std::underlying_type_t<E> value) noexcept {
    return static_cast<E>(value);
}
} // namespace mb::ul
#else
#include "std/std_extensions.h"

namespace mb::ul {
template <typename Enumeration>
constexpr enable_if_t<std::is_enum<Enumeration>::value, underlying_type_t<Enumeration>> enum_cast(
    const Enumeration value) noexcept {
    return static_cast<underlying_type_t<Enumeration>>(value);
}

template <typename Enumeration>
constexpr enable_if_t<std::is_enum<Enumeration>::value, Enumeration> enum_cast(
    const std::integral auto value) noexcept {
    return static_cast<Enumeration>(value);
}

template <typename Enumeration>
constexpr enable_if_t<std::is_enum<Enumeration>::value, underlying_type_t<Enumeration>> as_number(
    const Enumeration value) noexcept {
    return enum_cast<Enumeration>(value);
}
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
