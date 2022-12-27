// 2018-19

//! \file

#ifndef TYPES_H_eroui342zt347g8hx3xg713h4
#define TYPES_H_eroui342zt347g8hx3xg713h4

#include <cstddef>
#include <ostream>

#include "ul/macros.h"

namespace mb::ul::mem
{
const size_t KB = 1000;
constexpr size_t MB = 1000 * KB;
constexpr size_t GB = 1000 * MB;

const size_t KiB = 1024;
constexpr size_t MiB = 1024 * KB;
constexpr size_t GiB = 1024 * MB;

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

    constexpr /*implicit*/ operator bool() const noexcept
    {
        return value;
    }
};

constexpr Bytes operator+(Bytes a, Bytes b) noexcept
{
    a += b;
    return a;
}

constexpr Bytes operator-(Bytes a, Bytes b) noexcept
{
    a -= b;
    return a;
}

constexpr bool operator==(Bytes lhs, Bytes rhs) noexcept
{
    return lhs.value == rhs.value;
}

constexpr bool operator!=(Bytes lhs, Bytes rhs) noexcept
{
    return !operator==(lhs, rhs);
}

constexpr bool operator<(Bytes lhs, Bytes rhs) noexcept
{
    return lhs.value < rhs.value;
}

constexpr bool operator>(Bytes lhs, Bytes rhs) noexcept
{
    return operator<(rhs, lhs);
}

constexpr bool operator<=(Bytes lhs, Bytes rhs) noexcept
{
    return !operator>(lhs, rhs);
}

constexpr bool operator>=(Bytes lhs, Bytes rhs) noexcept
{
    return !operator<(lhs, rhs);
}

namespace impl
{
class ThousandsSep : public std::numpunct<char>
{
protected:
    char do_thousands_sep() const override
    {
        return '\'';
    }

    std::string do_grouping() const override
    {
        return "\003";
    }
};
} // namespace impl

inline std::ostream& operator<<(std::ostream& os, Bytes x)
{
    // strangely under Windows this crashes (Mac the same or otherwise malfunctioning)
#if UL_OS_LINUX
    os.imbue(std::locale(os.getloc(), new impl::ThousandsSep));
#endif
    os << x.value << " B";
    return os;
}
} // namespace mb::ul::mem

#include "ul/macros_end.h"

#endif
