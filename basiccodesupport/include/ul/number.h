//! \file

#ifndef NUMBER_H_F29JH8HNF238HRXZ23
#define NUMBER_H_F29JH8HNF238HRXZ23

#include "almost_equal.h"
#include "assert.h"
#include "enum_cast.h"
#include "floating_point.h"
#include "narrow.h"

#include "ul/comp_bwds.h"
#include "ul/config.h"
#include <cmath>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>

namespace mb::ul::math {
//! For values of single digits.
using BaseDigitType = unsigned char;

//! Base or radix, here just positive integers.
enum class NumBase : BaseDigitType {
    bin = 2,
    oct = 8,
    dec = 10,
    hex = 16,
    sgs = 60, //!< sexagesimal
};

template <typename T>
T get_digit_count(T number, NumBase base = NumBase::dec) {
    static_assert(std::is_integral_v<T>, "only integral numbers are allowed as input");
    T count = 0;
    do {
        ++count;
        number /= enum_cast(base);
    } while (number != 0);
    return count;
}

//! \param digit_idx counted from the right (least significant digit) 0-based
template <typename T>
BaseDigitType get_digit(T number, T digit_idx, NumBase base = NumBase::dec) {
    static_assert(std::is_integral_v<T>, "only integral numbers are allowed as input");
    return narrow_cast<BaseDigitType>(
        number / static_cast<T>(std::pow(ul::enum_cast(base), digit_idx)) % ul::enum_cast(base));
}

template <typename T>
int sgn(T val) {
    return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
}

template <typename T>
std::string to_leading_zeros(T x, int digits) {
    static_assert(std::is_integral_v<T>);
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
std::optional<ArithType> is_power_of(ArithType x, ArithType base) {
    static_assert(std::is_arithmetic_v<ArithType>, "only arithmetic numbers are allowed as input");
    UL_EXPECT(x > ArithType{});
    UL_EXPECT(base > ArithType{});
    UL_EXPECT(!almost_equal_alltypes(base, static_cast<ArithType>(1)));

    /** Impl. notes:
            If you wonder, whether this could be implemented using std::modf instead of the rounding check,
            then no, that doesn't seem to be the right function here. It doesn't guarantee to round to the
            nearest integer. It only ensures that the sum of integral and fractional part gives the original
            value. Under mingw I got a test-case where 4.9999... did not yield 5 as integral part.
            Though one could improve the hard-coded 1e-12 (std::numeric_limits<long double>::min() is
            much too small).*/
    const long double exp = std::log(x) / std::log(base);
    const int64_t intpart = llround(exp);
    const auto intpart_dbl = narrow_cast<long double>(intpart);

    if (constexpr auto sufficiently_small_deviation{1e-12L};
        !approx_equal(intpart_dbl, exp, sufficiently_small_deviation))
        return {};
    return narrow_cast<ArithType>(intpart);
}
} // namespace mb::ul::math

UL_HEADER_END

#endif
