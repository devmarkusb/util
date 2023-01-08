//! \file Bucket for unexpectedly needed compiler backwards compatibility.

#ifndef COMP_BWDS_H_uiregb74n398734gf478gy8734h
#define COMP_BWDS_H_uiregb74n398734gf478gy8734h

#include "config.h"
#include "macros.h"

#if UL_COMP_GNU_CPP && UL_COMP_GNU_CPP_VER < 41000
#define UL_HAS_LLROUND 0
#else
#define UL_HAS_LLROUND 1
#endif

#if UL_COMP_GNU_CPP && UL_COMP_GNU_CPP_VER < 41000
#define UL_HAS_STOI 0
#else
#define UL_HAS_STOI 1
#endif

#define UL_HAS_FROM_STRING 0

#if UL_COMP_GNU_CPP && UL_COMP_GNU_CPP_VER < 41000
#define UL_HAS_TO_STRING 0
#else
#define UL_HAS_TO_STRING 1
#endif

#if UL_HAS_LLROUND
#include <cmath>
#else
#include <math.h>
#endif
#if !UL_HAS_STOI
#include <cstdlib>
#endif
#if !UL_HAS_TO_STRING || !UL_HAS_FROM_STRING
#include <sstream>
#endif
#include <string>

namespace mb::ul
{
template <typename FloatingPointNumber>
inline long long llround(FloatingPointNumber x)
{
#if UL_HAS_LLROUND
    return std::llround(x);
#else
    return static_cast<long long>(x < 0.0f ? x - 0.5f : x + 0.5f);
#endif
}

template <typename StringStreamable>
StringStreamable from_string(const std::string& s)
{
#if UL_HAS_FROM_STRING
    return std::from_string(s);
#else
    std::stringstream ss(s);
    StringStreamable ret;
    ss >> ret;
    return ret;
#endif
}

inline int stoi(const std::string& s)
{
#if UL_HAS_STOI
    return std::stoi(s);
#else
    return atoi(s.c_str());
#endif
}

inline long double stold(const std::string& s)
{
#if UL_HAS_STOI
    return std::stold(s);
#else
    return from_string<long double>(s);
#endif
}

template <typename StringStreamable>
std::string to_string(const StringStreamable& x)
{
#if UL_HAS_TO_STRING
    return std::to_string(x);
#else
    std::stringstream ss;
    ss << x;
    return ss.str();
#endif
}
} // namespace mb::ul

UL_HEADER_END

#endif
