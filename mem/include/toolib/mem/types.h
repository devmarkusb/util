// Markus Borris, 2018
// This file is part of tfl library.

//!
/**
*/
//! \file

#ifndef TYPES_H_eroui342zt347g8hx3xg713h4
#define TYPES_H_eroui342zt347g8hx3xg713h4

#include <cstddef>


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
} // mem
} // too

#endif
