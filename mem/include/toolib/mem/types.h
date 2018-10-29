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

    Bytes& operator+=(Bytes rhs)
    {
        value += rhs.value;
        return *this;
    }

    operator bool() const
    {
        return value;
    }
};

inline Bytes operator+(Bytes a, Bytes b)
{
    a += b;
    return a;
}

inline bool operator==(Bytes lhs, Bytes rhs) { return lhs.value == rhs.value; }
inline bool operator!=(Bytes lhs, Bytes rhs) { return !operator==(lhs, rhs); }
inline bool operator<(Bytes lhs, Bytes rhs) { return lhs.value < rhs.value; }
inline bool operator>(Bytes lhs, Bytes rhs) { return operator<(rhs, lhs); }
inline bool operator<=(Bytes lhs, Bytes rhs) { return !operator>(lhs, rhs); }
inline bool operator>=(Bytes lhs, Bytes rhs) { return !operator<(lhs, rhs); }
} // mem
} // too

#endif
