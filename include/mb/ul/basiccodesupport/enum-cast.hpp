//! \file

#ifndef ENUM_CAST_H_YDJGFRENXZUIGFNEWGYF8E3Q2YGN4
#define ENUM_CAST_H_YDJGFRENXZUIGFNEWGYF8E3Q2YGN4

#include "mb/ul/buildenv/config.hpp"

#include "concepts.hpp"

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

UL_HEADER_END

#endif
