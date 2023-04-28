//! \file

#ifndef NUMBER_H_f29jh8hnf238hrxz23
#define NUMBER_H_f29jh8hnf238hrxz23

#include "almost_equal.h"
#include "assert.h"
#include "enum_cast.h"
#include "floating_point.h"
#include "math.h"
#include "narrow.h"
#include "optional.h"

#include "ul/comp_bwds.h"
#include "ul/config.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

namespace mb::ul::math {
//! For values of single digits.
using BaseDigitType = unsigned char;

//! Base or radix, here just positive integers.
enum class NumBase : BaseDigitType {
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16,
    SGS = 60, ///< sexagesimal
};

template <typename T>
T getDigitCount(T number, NumBase base = NumBase::DEC) {
    static_assert(std::is_integral_v<T>, "only integral numbers are allowed as input");
    T count = 0;
    do {
        ++count;
        number /= ul::as_number(base);
    } while (number != 0);
    return count;
}

//! \param digitIdx counted from the right (least significant digit) 0-based
template <typename T>
BaseDigitType getDigit(T number, T digitIdx, NumBase base = NumBase::DEC) {
    static_assert(std::is_integral_v<T>, "only integral numbers are allowed as input");
    return narrow_cast<BaseDigitType>(
        number / static_cast<T>(std::pow(ul::enum_cast(base), digitIdx)) % ul::enum_cast(base));
}

template <typename T>
int sgn(T val) {
    return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
}

template <typename T>
std::string toLeadingZeros(T x, int digits) {
    static_assert(std::is_integral<T>::value);
    UL_EXPECT_THROW(x >= T());
    UL_EXPECT_THROW(digits >= 0);

    std::stringstream ss;
    ss << std::setw(digits) << std::setfill('0') << x;
    return ss.str();
}

//! \return the exponent with whom x is a power of base, if it is an integer one.
/** Expects parameters to be > 0 and base != 1.
    Note that the return value can be < 0, e.g. 0.5 is the -1 power of base 2.*/
template <typename ArithType>
//  ArithType expected to be is_arithmetic
ul::opt<ArithType> isPowerOf(ArithType x, ArithType base) {
    static_assert(std::is_arithmetic<ArithType>::value, "only arithmetic numbers are allowed as input");
    UL_EXPECT(x > ArithType{});
    UL_EXPECT(base > ArithType{});
    UL_EXPECT(!ul::almost_equal_alltypes(base, static_cast<ArithType>(1)));

    /** Impl. notes:
            If you wonder, whether this could be implemented using std::modf instead of the rounding check,
            then no, that doesn't seem to be the right function here. It doesn't guarantee to round to the
            nearest integer. It only ensures that the sum of integral and fractional part gives the original
            value. Under mingw I got a test-case where 4.9999... did not yield 5 as integral part.
            Though one could improve the hard-coded 1e-12 (std::numeric_limits<long double>::min() is
            much too small).*/
    const long double exp = std::log(x) / std::log(base);
    const int64_t intpart = ul::llround(exp);
    const auto intpart_dbl = ul::narrow_cast<long double>(intpart);

    constexpr auto sufficientlySmallDeviation{1e-12L};
    if (!ul::math::approx_equal(intpart_dbl, exp, sufficientlySmallDeviation))
        return {};
    return ul::narrow_cast<ArithType>(intpart);
}

} // namespace mb::ul::math

UL_HEADER_END

#endif
