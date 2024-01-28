//! \file

#ifndef ALMOST_EQUAL_H_SDRUNTXG83G83UMHXERUASFR
#define ALMOST_EQUAL_H_SDRUNTXG83G83UMHXERUASFR

#include "assert.h"
#include "ignore_unused.h"
#include "ul/macros/UNDEF_MIN_MAX.h"
#include <cmath>

namespace mb::ul {
/** Expects ulp >= 1.
    \return true, if x and y are almost equal.*/
template <typename FloatType>
std::enable_if_t<std::is_floating_point_v<FloatType>, bool> almost_equal(FloatType x, FloatType y, int ulp = 1) {
    UL_EXPECT(ulp >= 1);
    return std::abs(x - y) < std::numeric_limits<FloatType>::epsilon() * std::abs(x + y) * static_cast<FloatType>(ulp)
           || std::abs(x - y) < std::numeric_limits<FloatType>::min();
}

//! Same as almost_equal, but also falls back to '==' if T is an integer type.
/** Useful if ArithmeticType is already a more general template parameter in your context.*/
template <typename ArithmeticType>
std::enable_if_t<std::is_floating_point_v<ArithmeticType>, bool> almost_equal_alltypes(
    ArithmeticType x, ArithmeticType y, int ulp = 1) {
    return almost_equal(x, y, ulp);
}

//! Cf. other declaration of almost_equal_alltypes.
template <typename ArithmeticType>
std::enable_if_t<std::is_integral_v<ArithmeticType>, bool> almost_equal_alltypes(
    ArithmeticType x, ArithmeticType y, int ulp = 1) {
    ignore_unused(ulp);
    return x == y;
}
} // namespace mb::ul

#endif
