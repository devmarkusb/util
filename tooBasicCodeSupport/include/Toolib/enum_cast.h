// Markus Borris, 2014
// This file is part of Toolib library. Open source.

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
    constexpr auto as_number(Enumeration const value)
        -> typename std::underlying_type<Enumeration>::type
    {
        static_assert(std::is_enum<Enumeration>::value, "parameter is not of type enum or enum class");
        return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }
}

#endif
