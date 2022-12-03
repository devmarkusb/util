// 2014-19

//!
/**
 */
//! \file

#ifndef ENUM_CAST_H_ydjgfrenxzuigfnewgyf8e3q2ygn4
#define ENUM_CAST_H_ydjgfrenxzuigfnewgyf8e3q2ygn4

#include "std/std_extensions.h"


namespace too
{
template <typename Enumeration>
constexpr too::enable_if_t<std::is_enum<Enumeration>::value, too::underlying_type_t<Enumeration>> as_number(
    const Enumeration value) noexcept
{
    return static_cast<too::underlying_type_t<Enumeration>>(value);
}
} // namespace too

#endif
