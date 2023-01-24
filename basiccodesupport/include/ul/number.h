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

namespace mb::ul::math
{
enum class ENumSys : unsigned char
{
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16,
};

template <typename T>
unsigned char getDigitCount(T Number, ENumSys Base = ENumSys::DEC)
{
    static_assert(std::is_integral<T>::value, "only integral numbers are allowed as input");
    unsigned char count = 0;
    do
    {
        ++count;
        Number /= ul::as_number(Base);
    } while (Number != 0);
    return count;
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

template <typename T>
std::string toLeadingZeros(T x, int digits)
{
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
ul::opt<ArithType> isPowerOf(ArithType x, ArithType base)
{
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
