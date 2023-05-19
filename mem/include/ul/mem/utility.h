//! \file

#ifndef UTILITY_H_UI3GNT3784XGH25G7523GYG
#define UTILITY_H_UI3GNT3784XGH25G7523GYG

#include <cstddef>

namespace mb::ul::mem {
constexpr size_t pad_up(size_t n, size_t alignment) noexcept {
    return (n + (alignment - 1)) & ~(alignment - 1);
}
} // namespace mb::ul::mem

#endif
