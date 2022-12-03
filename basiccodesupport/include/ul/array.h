// 2019

//!
/**
 */
//! \file

#ifndef ARRAY_H_uixngt3h478gh389xgh3984g
#define ARRAY_H_uixngt3h478gh389xgh3984g

#include <array>
#include <cstddef>


namespace mb::ul
{
namespace detail
{
template <typename T, std::size_t N, typename Container>
constexpr T ct_accumulate(Container&& arr, T init_val)
{
    T sum{init_val};

    for (size_t i = 0; i < N; ++i)
        sum += arr[i];

    return sum;
}
} // namespace detail

//! Compile-time equivalent of std::accumulate at least for array.
template <typename T, std::size_t N>
constexpr T ct_accumulate(const std::array<T, N>& arr, T init_val)
{
    return detail::ct_accumulate<T, N>(arr, init_val);
}

//! C-array version. Cf. other ct_accumulate.
template <typename T, std::size_t N>
constexpr T ct_accumulate(const T (&arr)[N], T init_val)
{
    return detail::ct_accumulate<T, N>(arr, init_val);
}
} // namespace mb::ul
#endif
