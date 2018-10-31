// Markus Borris, 2018
// This file is part of tfl library.

//!
/**
*/
//! \file

#ifndef TYPES_H_eroui342zt347g8hx3xg713h4
#define TYPES_H_eroui342zt347g8hx3xg713h4

#include <cstddef>
#include <ostream>


namespace too
{
namespace mem
{
struct Bytes
{
    size_t value{};

    constexpr Bytes& operator+=(Bytes rhs) noexcept
    {
        value += rhs.value;
        return *this;
    }

    constexpr Bytes& operator-=(Bytes rhs) noexcept
    {
        value -= rhs.value;
        return *this;
    }

    constexpr operator bool() const noexcept
    {
        return value;
    }
};

inline constexpr Bytes operator+(Bytes a, Bytes b) noexcept
{
    a += b;
    return a;
}

inline constexpr Bytes operator-(Bytes a, Bytes b) noexcept
{
    a -= b;
    return a;
}

inline constexpr bool operator==(Bytes lhs, Bytes rhs) noexcept { return lhs.value == rhs.value; }
inline constexpr bool operator!=(Bytes lhs, Bytes rhs) noexcept { return !operator==(lhs, rhs); }
inline constexpr bool operator<(Bytes lhs, Bytes rhs) noexcept { return lhs.value < rhs.value; }
inline constexpr bool operator>(Bytes lhs, Bytes rhs) noexcept { return operator<(rhs, lhs); }
inline constexpr bool operator<=(Bytes lhs, Bytes rhs) noexcept { return !operator>(lhs, rhs); }
inline constexpr bool operator>=(Bytes lhs, Bytes rhs) noexcept { return !operator<(lhs, rhs); }

namespace impl
{
class ThousandsSep : public std::numpunct<char>
{
protected:
    char do_thousands_sep() const override { return '\''; }
    std::string do_grouping() const override { return "\003"; }
};
} // impl

inline std::ostream& operator<<(std::ostream& os, Bytes x)
{
    os.imbue(std::locale(os.getloc(), new impl::ThousandsSep));
    os << x.value << " B";
    return os;
}
} // mem
} // too

#endif
