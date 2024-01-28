//! \file

#ifndef ARRAY_H_UIXNGT3H478GH389XGH3984G
#define ARRAY_H_UIXNGT3H478GH389XGH3984G

#include <array>
#include <cstddef>

namespace mb::ul {
namespace detail {
template <typename T, size_t n, typename Container>
constexpr T ct_accumulate(const Container& arr, T init_val) {
    T sum{init_val};

    for (size_t i = 0; i < n; ++i)
        sum += arr[i]; // NOLINT

    return sum;
}
} // namespace detail

//! Compile-time equivalent of std::accumulate at least for array.
template <typename T, size_t n>
constexpr T ct_accumulate(const std::array<T, n>& arr, T init_val) {
    return detail::ct_accumulate<T, n>(arr, init_val);
}

//! C-array version. Cf. other ct_accumulate.
template <typename T, size_t n>
constexpr T ct_accumulate(const T (&arr)[n], T init_val) // NOLINT
{
    return detail::ct_accumulate<T, n>(arr, init_val);
}
} // namespace mb::ul
#endif
