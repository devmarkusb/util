// Markus Borris, 2018
// This file is part of tfl library.

//!
/**
*/
//! \file

#ifndef ALMOST_EQUAL_H_sdruntxg83g83umhxeruasfr
#define ALMOST_EQUAL_H_sdruntxg83g83umhxeruasfr

#include "toolib/assert.h"
#include "toolib/ignore_arg.h"


namespace too
{
//! \Returns true, if \params x and y are almost equal. Expects \param ulp >= 1.
template<typename FloatType>
typename std::enable_if<std::is_floating_point<FloatType>::value, bool>::type almost_equal(
        FloatType x, FloatType y, int ulp = 1)
{
    TOO_EXPECT(ulp >= 1);
    return std::abs(x - y) < std::numeric_limits<FloatType>::epsilon() * std::abs(x + y) * ulp ||
           std::abs(x - y) < std::numeric_limits<FloatType>::min();
}

//! Same as almost_equal, but also falls back to '==' if T is an integer type.
/** Useful if ArithmeticType is already a more general template parameter in your context.*/
template <typename ArithmeticType>
typename std::enable_if<std::is_floating_point<ArithmeticType>::value, bool>::type almost_equal_alltypes(
        ArithmeticType x, ArithmeticType y, int ulp = 1)
{
    return almost_equal(x, y, ulp);
}

//! Cf. other declaration of almost_equal_alltypes.
template <typename ArithmeticType>
typename std::enable_if<std::is_integral<ArithmeticType>::value, bool>::type almost_equal_alltypes(
        ArithmeticType x, ArithmeticType y, int ulp = 1)
{
    too::ignore_arg(ulp);
    return x == y;
}
} // too

#endif