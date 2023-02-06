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
        const auto* it = init.begin();
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
    explicit constexpr Rational(std::ratio<N, D> /*unused*/)
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
        return static_cast<T>(this->num) / static_cast<T>(this->denom);
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

// clang-format off
constexpr const Rational atto  {std::atto{}};
constexpr const Rational femto {std::femto{}};
constexpr const Rational pico  {std::pico{}};
constexpr const Rational nano  {std::nano{}};
constexpr const Rational micro {std::micro{}};
constexpr const Rational milli {std::milli{}};
constexpr const Rational centi {std::centi{}};
constexpr const Rational deci  {std::deci{}};
constexpr const Rational one   {std::ratio<1, 1>{}};
constexpr const Rational deca  {std::deca{}};
constexpr const Rational hecto {std::hecto{}};
constexpr const Rational kilo  {std::kilo{}};
constexpr const Rational mega  {std::mega{}};
constexpr const Rational giga  {std::giga{}};
constexpr const Rational tera  {std::tera{}};
constexpr const Rational peta  {std::peta{}};
constexpr const Rational exa   {std::exa{}};

constexpr std::string_view atto_symb{"a"};
constexpr std::string_view femto_symb{"f"};
constexpr std::string_view pico_symb{"p"};
constexpr std::string_view nano_symb{"n"};
constexpr std::string_view micro_symb{"\xC2\xB5"};
constexpr std::string_view milli_symb{"m"};
constexpr std::string_view centi_symb{"c"};
constexpr std::string_view deci_symb{"d"};
constexpr std::string_view one_symb{""};
constexpr std::string_view deka_symb{"da"};
constexpr std::string_view hecto_symb{"h"};
constexpr std::string_view kilo_symb{"k"};
constexpr std::string_view mega_symb{"M"};
constexpr std::string_view giga_symb{"G"};
constexpr std::string_view tera_symb{"T"};
constexpr std::string_view peta_symb{"P"};
constexpr std::string_view exa_symb{"E"};

constexpr const Rational one_twelveth     {std::ratio<1, 12>{}};
constexpr const Rational one_seventh      {std::ratio<1, 7>{}};
constexpr const Rational one_twentyfourth {std::ratio<1, 24>{}};
constexpr const Rational one_sixtyth      {std::ratio<1, 60>{}};
constexpr const Rational sixtytimes       {std::ratio<60, 1>{}};
constexpr const Rational twentyfourtimes  {std::ratio<24, 1>{}};
constexpr const Rational seventimes       {std::ratio<7, 1>{}};
constexpr const Rational twelvetimes      {std::ratio<12, 1>{}};
// clang-format on
} // namespace mb::ul::math

UL_HEADER_END

#endif
