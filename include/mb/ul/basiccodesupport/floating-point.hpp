//! \file

#ifndef FLOATING_POINT_H_09458T87T5TWD2874T57RBN6
#define FLOATING_POINT_H_09458T87T5TWD2874T57RBN6

#include "assert.hpp"

#include "mb/ul/buildenv/comp-bwds.hpp"
#include "mb/ul/buildenv/config.hpp"
#include "mb/ul/buildenv/macros/UNDEF-MIN-MAX.hpp"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <type_traits>

namespace mb::ul::math {
/** Rough version of almost_equal, where you can pass a user defined eps(ilon) within which x and y are
    understood approx. equal.
    HINT: a common mistake leading to compiler error is not providing eps with explicit type FloatType, as the
    other params.*/
template <std::floating_point FloatType>
bool approx_equal(FloatType x, FloatType y, FloatType eps) {
    if (std::isnan(x) || std::isnan(y)) {
        return false;
    }
    if (std::isinf(x) || std::isinf(y)) {
        return std::isinf(x) && std::isinf(y) && std::signbit(x) == std::signbit(y);
    }
    const auto diff = std::abs(x - y);
    return std::fpclassify(diff) == FP_ZERO || diff < eps;
}

//! Cf. to_string functions.
enum class FloatFormat : uint8_t {
    default_choice,
    fixed,
    scientific,
};

namespace impl {
template <std::floating_point FloatType, FloatFormat ff>
struct ToStringConverter;
} // namespace impl

//! \return a string of the floating point number x.
/** \param precision controls either
        a) the count of significant digits for FF `default_`, or
        b) the decimal places for FF `fixed` or `scientific`.
    FF scientific leads to exponential formatting.
    If don't want to pass precision, FF `default_choice` uses fixed-point formatting with 6 digits after the decimal
    (portable; `std::to_string` for floats omits trailing zeros on some platforms).*/
template <FloatFormat ff = FloatFormat::default_choice, std::floating_point FloatType = double>
//  FloatType expected as floating point
std::string to_string(FloatType x, int precision) {
    UL_EXPECT(precision >= 0);
    return impl::ToStringConverter<FloatType, ff>::convert(x, precision);
}

//! \return a string of the floating point number x.
/** \tparam ff selects the formatting: `default_choice` uses fixed-point with 6 fractional digits (see convert impl),
    `fixed` / `scientific` use stream formatting with default or explicit precision.*/
template <FloatFormat ff = FloatFormat::default_choice, std::floating_point FloatType = double>
//  FloatType expected as floating point
std::string to_string(FloatType x) {
    return impl::ToStringConverter<FloatType, ff>::convert(x);
}

namespace impl {
template <std::floating_point FloatType, FloatFormat ff>
struct ToStringConverter {};

template <std::floating_point FloatType>
struct ToStringConverter<FloatType, FloatFormat::default_choice> {
    static std::string convert(FloatType x) {
        // Avoid std::to_string: trailing fractional zeros are not guaranteed (e.g. "4.556" vs "4.556000").
        std::ostringstream ret;
        constexpr auto default_choice_fractional_digits{6};
        ret << std::fixed << std::setprecision(default_choice_fractional_digits) << x;
        return ret.str();
    }

    static std::string convert(FloatType x, int precision) {
        UL_EXPECT(precision >= 0);
        std::ostringstream ret;
        ret << std::setprecision(precision) << x;
        return ret.str();
    }
};

template <std::floating_point FloatType>
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

template <std::floating_point FloatType>
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
