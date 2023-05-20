//! \file

#ifndef ENUM_CAST_H_YDJGFRENXZUIGFNEWGYF8E3Q2YGN4
#define ENUM_CAST_H_YDJGFRENXZUIGFNEWGYF8E3Q2YGN4

#include "std/std_extensions.h"

namespace mb::ul {
template <typename Enumeration>
constexpr ul::enable_if_t<std::is_enum<Enumeration>::value, ul::underlying_type_t<Enumeration>> enum_cast(
    const Enumeration value) noexcept {
    return static_cast<ul::underlying_type_t<Enumeration>>(value);
}

template <typename Enumeration>
constexpr ul::enable_if_t<std::is_enum<Enumeration>::value, ul::underlying_type_t<Enumeration>> as_number(
    const Enumeration value) noexcept {
    return enum_cast<Enumeration>(value);
}
} // namespace mb::ul

#endif
