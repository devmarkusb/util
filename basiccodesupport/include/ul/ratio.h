//! \file

#ifndef RATIO_H_louiuzlik79hi965gi6
#define RATIO_H_louiuzlik79hi965gi6

#include "almost_equal.h"
#include "assert.h"

#include "ul/config.h"
#include <cstdint>
#include <initializer_list>
#include <ratio>
#include <string>
#include <type_traits>

namespace mb::ul::math
{
//! Greatest common divisor. Expects at least on of a, b to be > 0.
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type gcd(T a, T b)
{
    UL_EXPECT(a > T() || b > T());
    T c = 0;
    while (a != 0)
    {
        c = a;
        a = b % a;
        b = c;
    }
    return b;
}

//! Lowest common multiple.
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type lcm(T a, T b)
{
    UL_EXPECT(a > T() || b > T());
    return a * (b / gcd(a, b));
}


struct Rational;

//! Obviously ensures common denom ofs one, two after the call.
inline void make_common_denom(Rational& one, Rational& two);

//! Value type for rational numbers.
/** Since member denom is publicly accessible, the user is responsible
        a) to keep denom != 0 always, and
        b) to  represent negative numbers by setting num < 0 only,
            so denom is even expected to be > 0 always.
    Otherwise there is undefined behavior.*/
struct Rational
{
    using ValueType = std::intmax_t;

    //! Numerator.
    ValueType num{0};
    //! Denominator.
    ValueType denom{1}; // expected to be > 0 always

    //! Constructs a 0 (numerator 0, denominator 1).
    Rational() = default;

    //! Initialization expects either
    //!     a) exactly one numerator value (denominator being implicitly 1), or
    //!     b) two values, the first denoting the numerator and the second the denominator, or
    //!     c) an empty list, corresponding to numerator 0 and denominator 1.
    /** Otherwise behavior is undefined. As it is of course also, if denominator is passed as 0.
        Note the ugly implementation detail: I need this init-list constructor to initialize conveniently
        because otherwise the default constructor seems to disturb.*/
    Rational(std::initializer_list<ValueType> init)
    {
        UL_EXPECT(init.size() <= 2);
        auto it = init.begin();
        if (it == init.end())
        {
            num = {};
            return;
        }
        num = *it;
        ++it;
        if (it != init.end())
        {
            denom = *it;
            UL_EXPECT(denom > 0);
        }
    }

    template <intmax_t N, intmax_t D>
    explicit constexpr Rational(std::ratio<N, D>)
        : num{N}
        , denom{D}
    {
        static_assert(D > 0, "denominator for Rational expected to be > 0");
    }

    ~Rational() = default; // gcc needs this to be trivial for the use of Rational in constexpr's

    //     {
    //         // documenting the important invariant
    //         UL_EXPECT(denom > 0);
    //     }

    //! Note that this ensures the invariant of having denom > 0 always
    void inverse()
    {
        std::swap(this->num, this->denom);
        if (this->denom < 0)
        {
            this->denom = -this->denom;
            this->num = -this->num;
        }
        UL_ENSURE(denom > 0);
    }

    template <typename T>
    [[nodiscard]] typename std::enable_if<std::is_floating_point<T>::value, T>::type asFloatingPoint() const
    {
        return T(this->num) / T(this->denom);
    }

    void reduce()
    {
        const auto d = gcd(this->num, this->denom);
        this->num /= d;
        this->denom /= d;
    }

    // as soon as explicit is soundly supported by the standard here...
    // explicit operator bool() const
    //{
    //    return this->num != ValueType{};
    //}

    bool operator!() const
    {
        return is_null();
    }

    [[nodiscard]] bool is_null() const
    {
        return this->num == ValueType{};
    }

    Rational operator-() const
    {
        Rational tmp{*this};
        tmp.num = -this->num;
        return tmp;
    }

    Rational& operator+=(const Rational& rhs)
    {
        Rational r = rhs;
        make_common_denom(*this, r);
        this->num += r.num;
        return *this;
    }

    Rational& operator-=(const Rational& rhs)
    {
        Rational r = rhs;
        make_common_denom(*this, r);
        this->num -= r.num;
        return *this;
    }

    Rational& operator*=(const Rational& rhs)
    {
        this->num *= rhs.num;
        this->denom *= rhs.denom;
        return *this;
    }

    Rational& operator/=(const Rational& rhs)
    {
        Rational r = rhs;
        r.inverse();
        *this *= r;
        return *this;
    }
};

inline void make_common_denom(Rational& one, Rational& two)
{
    const auto m = lcm(one.denom, two.denom);
    one.num *= m / one.denom;
    one.denom = m;
    two.num *= m / two.denom;
    two.denom = m;

    UL_ENSURE(one.denom == two.denom);
}

inline Rational operator+(Rational lhs, const Rational& rhs)
{
    lhs += rhs;
    return lhs;
}

inline Rational operator-(Rational lhs, const Rational& rhs)
{
    lhs -= rhs;
    return lhs;
}

inline Rational operator*(Rational lhs, const Rational& rhs)
{
    lhs *= rhs;
    return lhs;
}

inline Rational operator/(Rational lhs, const Rational& rhs)
{
    lhs /= rhs;
    return lhs;
}

inline bool operator==(const Rational& lhs, const Rational& rhs)
{
    if (lhs.denom == rhs.denom)
        return lhs.num == rhs.num;
    return ul::almost_equal(lhs.asFloatingPoint<long double>(), rhs.asFloatingPoint<long double>());
}

inline bool operator!=(const Rational& lhs, const Rational& rhs)
{
    return !operator==(lhs, rhs);
}

inline bool operator<(const Rational& lhs, const Rational& rhs)
{
    if (lhs.denom == rhs.denom)
        return lhs.num < rhs.num;
    if (lhs.num == rhs.num)
        return lhs.denom > rhs.denom;
    return lhs.asFloatingPoint<long double>() < rhs.asFloatingPoint<long double>();
}

inline bool operator>(const Rational& lhs, const Rational& rhs)
{
    return operator<(rhs, lhs);
}

inline bool operator<=(const Rational& lhs, const Rational& rhs)
{
    return !operator>(lhs, rhs);
}

inline bool operator>=(const Rational& lhs, const Rational& rhs)
{
    return !operator<(lhs, rhs);
}

//todo just use constexpr here
#if UL_HAS_NO_CONSTEXPR_STD_RATIO
#define UL_TEMPCONSTEXPR
#else
#define UL_TEMPCONSTEXPR constexpr
#endif
// clang-format off
UL_TEMPCONSTEXPR const Rational atto  {std::atto{}};
UL_TEMPCONSTEXPR const Rational femto {std::femto{}};
UL_TEMPCONSTEXPR const Rational pico  {std::pico{}};
UL_TEMPCONSTEXPR const Rational nano  {std::nano{}};
UL_TEMPCONSTEXPR const Rational micro {std::micro{}};
UL_TEMPCONSTEXPR const Rational milli {std::milli{}};
UL_TEMPCONSTEXPR const Rational centi {std::centi{}};
UL_TEMPCONSTEXPR const Rational deci  {std::deci{}};
UL_TEMPCONSTEXPR const Rational one   {std::ratio<1, 1>{}};
UL_TEMPCONSTEXPR const Rational deca  {std::deca{}};
UL_TEMPCONSTEXPR const Rational hecto {std::hecto{}};
UL_TEMPCONSTEXPR const Rational kilo  {std::kilo{}};
UL_TEMPCONSTEXPR const Rational mega  {std::mega{}};
UL_TEMPCONSTEXPR const Rational giga  {std::giga{}};
UL_TEMPCONSTEXPR const Rational tera  {std::tera{}};
UL_TEMPCONSTEXPR const Rational peta  {std::peta{}};
UL_TEMPCONSTEXPR const Rational exa   {std::exa{}};

const std::string  atto_symb = "a";
const std::string femto_symb = "f";
const std::string  pico_symb = "p";
const std::string  nano_symb = "n";
const std::string micro_symb = "\xC2\xB5";
const std::string milli_symb = "m";
const std::string centi_symb = "c";
const std::string  deci_symb = "d";
const std::string   one_symb/* = ""*/;
const std::string  deka_symb = "da";
const std::string hecto_symb = "h";
const std::string  kilo_symb = "k";
const std::string  mega_symb = "M";
const std::string  giga_symb = "G";
const std::string  tera_symb = "T";
const std::string  peta_symb = "P";
const std::string   exa_symb = "E";

UL_TEMPCONSTEXPR const Rational one_twelveth     {std::ratio<1, 12>{}};
UL_TEMPCONSTEXPR const Rational one_seventh      {std::ratio<1, 7>{}};
UL_TEMPCONSTEXPR const Rational one_twentyfourth {std::ratio<1, 24>{}};
UL_TEMPCONSTEXPR const Rational one_sixtyth      {std::ratio<1, 60>{}};
UL_TEMPCONSTEXPR const Rational sixtytimes       {std::ratio<60, 1>{}};
UL_TEMPCONSTEXPR const Rational twentyfourtimes  {std::ratio<24, 1>{}};
UL_TEMPCONSTEXPR const Rational seventimes       {std::ratio<7, 1>{}};
UL_TEMPCONSTEXPR const Rational twelvetimes      {std::ratio<12, 1>{}};
// clang-format on
#undef UL_TEMPCONSTEXPR
} // namespace mb::ul::math

UL_HEADER_END

#endif
