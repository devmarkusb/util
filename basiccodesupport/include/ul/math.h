//! \file

#ifndef MATH_H_SKUY3478NGT8247GG2
#define MATH_H_SKUY3478NGT8247GG2

#include "ul/macros.h"
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace mb::ul {
#if __cpp_concepts && __cpp_lib_concepts
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;
#endif

namespace math {
namespace consts {
template <typename T>
constexpr T pi() {
    return std::acos(static_cast<T>(-1));
}
} // namespace consts

/** \return true if the number number is a power of two, e.g. 1, 2, 4, 8, ..., and false otherwise.
    This is done at compile-time (if possible).
    Negative numbers are handled as well.*/
template <typename NumberType>
constexpr bool is_power_of_two(NumberType number) noexcept {
    static_assert(std::is_integral<NumberType>::value, "NumberType must be of integral type");
    return number && ((number & (number - 1)) == 0); // NOLINT
}

#if !UL_COMP_MS_VISUAL_STUDIO_CPP
namespace detail {
// Impl. notes: Workaround for partially specializing non-type template parameters of dependent type.
// https://stackoverflow.com/a/22486607
// The cast (NumberType)0 instead of NumberType{} is a workaround for GCC.
template <typename NumberType, typename Number>
struct NextGreaterOrEqPow2;

template <typename NumberType, NumberType number>
struct NextGreaterOrEqPow2<NumberType, std::integral_constant<NumberType, number>> {
    static constexpr NumberType value =
        NextGreaterOrEqPow2<NumberType, std::integral_constant<NumberType, number / NumberType{2}>>::value
        * NumberType{2};
};

template <typename NumberType>
struct NextGreaterOrEqPow2<NumberType, std::integral_constant<NumberType, NumberType{0}>> {
    static constexpr NumberType value = NumberType{1};
};
} // namespace detail

/** Next greater or equal power of two. Found at compile-time only.
    Function value returns a power of two that is greater than or equal to the passed number.
    Negative numbers are handled as well.
    Examples:
        static_assert(ul::math::NextGreaterOrEqualPowerOfTwo<int, 4>::value() == 4);
        static_assert(ul::math::NextGreaterOrEqualPowerOfTwo<int, 5>::value() == 8);*/
template <typename NumberType, NumberType number>
struct NextGreaterOrEqPow2
    : public detail::NextGreaterOrEqPow2<NumberType, std::integral_constant<NumberType, number>> {
    static_assert(std::is_integral<NumberType>::value, "NumberType must be of integral type");

    static constexpr NumberType value() noexcept {
        if constexpr (is_power_of_two(number)) {
            return number;
        } else {
            // yes, this still keeps the function compile-time, cf. unit tests
            if (number < NumberType{}) {
                return NumberType{1};
            }
            return detail::NextGreaterOrEqPow2<NumberType, std::integral_constant<NumberType, number>>::value;
        }
    }
};

/** Next greater power of two. Found at compile-time only.
    Function value returns a power of two that is greater than the passed number.
    Negative numbers are handled as well.
    Examples:
        static_assert(util::math::NextGreaterOrEqualPowerOfTwo<int, 3>::value() == 4);
        static_assert(util::math::NextGreaterOrEqualPowerOfTwo<int, 4>::value() == 8);*/
template <typename NumberType, NumberType number>
struct NextGreaterPow2 : public detail::NextGreaterOrEqPow2<NumberType, std::integral_constant<NumberType, number>> {
    static_assert(std::is_integral<NumberType>::value, "NumberType must be of integral type");

    static constexpr NumberType value() noexcept {
        if (number < NumberType{}) {
            return NumberType{1};
        }
        return detail::NextGreaterOrEqPow2<NumberType, std::integral_constant<NumberType, number>>::value;
    }
};
#endif

/** Like NextGreaterOrEqualPowerOfTwo (except for negative number support), but working at runtime as well.
    Cf. https://stackoverflow.com/questions/1322510#1322548*/
constexpr uint64_t next_greater_or_eq_pow2(uint64_t v) noexcept {
    --v;
    v |= v >> 1u;
    // NOLINTBEGIN
    v |= v >> 2u;
    v |= v >> 4u;
    v |= v >> 8u;
    v |= v >> 16u;
    v |= v >> 32u;
    // NOLINTEND
    return v + 1;
}
} // namespace math
} // namespace mb::ul

#endif
