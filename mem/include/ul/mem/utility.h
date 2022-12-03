// 2018

//!
/**
 */
//! \file

#ifndef UTILITY_H_ui3gnt3784xgh25g7523gyg
#define UTILITY_H_ui3gnt3784xgh25g7523gyg

#include <cstddef>


namespace mb::ul
{
namespace mem
{
inline constexpr size_t pad_up(size_t n, size_t alignment) noexcept
{
    return (n + (alignment - 1)) & ~(alignment - 1);
}
} // namespace mem
} // namespace mb::ul
#endif
