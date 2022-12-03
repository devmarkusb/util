// 2014-19

//!
/**
 */
//! \file

#ifndef ENUM_CAST_H_ydjgfrenxzuigfnewgyf8e3q2ygn4
#define ENUM_CAST_H_ydjgfrenxzuigfnewgyf8e3q2ygn4

#include "std/std_extensions.h"


namespace mb::ul
{
template <typename Enumeration>
constexpr ul::enable_if_t<std::is_enum<Enumeration>::value, ul::underlying_type_t<Enumeration>> as_number(
    const Enumeration value) noexcept
{
    return static_cast<ul::underlying_type_t<Enumeration>>(value);
}
} // namespace mb::ul

#endif
