//! \file

#ifndef FLOATING_POINT_H_09458t87t5twd2874t57rbn6
#define FLOATING_POINT_H_09458t87t5twd2874t57rbn6

#include "assert.h"

#include "ul/comp_bwds.h"
#include "ul/config.h"
#include "ul/macros/UNDEF_MIN_MAX.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <type_traits>

namespace mb::ul::math {
/** Rough version of almost_equal, where you can pass a user defined eps(ilon) within which x and y are
    understood approx. equal.
    HINT: a common mistake leading to compiler error is not providing eps with explicit type FloatType, as the
    other params.*/
template <typename FloatType>
typename std::enable_if<std::is_floating_point<FloatType>::value, bool>::type approx_equal(
    FloatType x, FloatType y, FloatType eps) {
    return std::abs(x - y) < eps;
}

//! Cf. to_string functions.
enum class FloatFormat {
    default_,
    fixed,
    scientific,
};

namespace impl {
template <typename FloatType, FloatFormat FF>
struct ToStringConverter;
} // namespace impl

//! \return a string of the floating point number x.
/** \param precision controls either
        a) the count of significant digits for FF `default_`, or
        b) the decimal places for FF `fixed` or `scientific`.
    FF scientific leads to exponential formatting.
    If don't want to pass precision and use a default one (e.g. 6) and use FF default_, just use std::to_string.*/
template <FloatFormat FF = FloatFormat::default_, typename FloatType = double>
//  FloatType expected as floating point
typename std::enable_if<std::is_floating_point<FloatType>::value, std::string>::type to_string(
    FloatType x, int precision) {
    UL_EXPECT(precision >= 0);
    return impl::ToStringConverter<FloatType, FF>::convert(x, precision);
}

//! \return a string of the floating point number x.
/** \tparam FF selects the formatting: `default_` being equivalent to a call of std::to_string,
    `fixed` meaning fixed count of decimal places and `scientific` an exponential formatting.*/
template <FloatFormat FF = FloatFormat::default_, typename FloatType = double>
//  FloatType expected as floating point
typename std::enable_if<std::is_floating_point<FloatType>::value, std::string>::type to_string(FloatType x) {
    return impl::ToStringConverter<FloatType, FF>::convert(x);
}

namespace impl {
template <typename FloatType, FloatFormat FF>
struct ToStringConverter {};

template <typename FloatType>
struct ToStringConverter<FloatType, FloatFormat::default_> {
    static std::string convert(FloatType x) {
        return ul::to_string(x);
    }

    static std::string convert(FloatType x, int precision) {
        UL_EXPECT(precision >= 0);
        std::ostringstream ret;
        ret << std::setprecision(precision) << x;
        return ret.str();
    }
};

template <typename FloatType>
struct ToStringConverter<FloatType, FloatFormat::fixed> {
    static std::string convert(FloatType x) {
        std::ostringstream ret;
        ret << std::fixed << x;
        return ret.str();
    }

    static std::string convert(FloatType x, int precision) {
        UL_EXPECT(precision >= 0);
        std::ostringstream ret;
        ret << std::fixed << std::setprecision(precision) << x;
        return ret.str();
    }
};

template <typename FloatType>
struct ToStringConverter<FloatType, FloatFormat::scientific> {
    static std::string convert(FloatType x) {
        std::ostringstream ret;
        ret << std::scientific << x;
        return ret.str();
    }

    static std::string convert(FloatType x, int precision) {
        UL_EXPECT(precision >= 0);
        std::ostringstream ret;
        ret << std::scientific << std::setprecision(precision) << x;
        return ret.str();
    }
};
} // namespace impl
} // namespace mb::ul::math

UL_HEADER_END

#endif
