// Markus Borris, 2018
// This file is part of tfl library.

//!
/**
*/
//! \file

#ifndef UTILITY_H_ui3gnt3784xgh25g7523gyg
#define UTILITY_H_ui3gnt3784xgh25g7523gyg

#include <cstddef>


namespace too
{
namespace mem
{
inline constexpr size_t pad_up(size_t n, size_t alignment) noexcept
{
    return (n + (alignment - 1)) & ~(alignment - 1);
}
} // mem
} // too
#endif