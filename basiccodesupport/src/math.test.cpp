#include "ul/math.h"
#include "ul/almost_equal.h"
#include "ul/floating_point.h"
#include "ul/warnings.h"
#include "gtest/gtest.h"
#include <cmath> // NOLINT

UL_PRAGMA_WARNINGS_PUSH
// clang-format off
UL_WARNING_DISABLE_CLANG(reserved-id-macro)
UL_WARNING_DISABLE_CLANG(unused-macros)
// clang-format on
#define _USE_MATH_DEFINES // NOLINT
// needs to be the first include for the upper macro to work
#include <cmath> // NOLINT
#undef _USE_MATH_DEFINES
UL_PRAGMA_WARNINGS_POP

#ifndef M_PI //3.141592653589793
#define M_PI 3.141592653589793
#endif

TEST(math_constsTest, test) {
    const auto pi = M_PI;
    EXPECT_DOUBLE_EQ(pi, ul::math::consts::pi<double>());
    EXPECT_TRUE(ul::almost_equal(pi, ul::math::consts::pi<double>()));

    const auto pi_f = static_cast<float>(M_PI);
    EXPECT_FLOAT_EQ(pi_f, ul::math::consts::pi<float>());
    EXPECT_TRUE(ul::almost_equal(pi_f, ul::math::consts::pi<float>()));

    const auto pi_ld = static_cast<long double>(M_PI);
    EXPECT_TRUE(ul::math::approx_equal(pi_ld, ul::math::consts::pi<long double>(), 1e-15L));
}

#undef M_PI // anyway

// NOLINTBEGIN

TEST(math_is_power_of_two, some) {
    static_assert(!ul::math::is_power_of_two(0));
    static_assert(ul::math::is_power_of_two(1));
    static_assert(ul::math::is_power_of_two(2));
    static_assert(!ul::math::is_power_of_two(3));
    static_assert(ul::math::is_power_of_two(4));
    static_assert(ul::math::is_power_of_two(256));
    static_assert(!ul::math::is_power_of_two(257));

    static_assert(!ul::math::is_power_of_two(-1));
    static_assert(!ul::math::is_power_of_two(-2));
    static_assert(!ul::math::is_power_of_two(-256));
}

#if !UL_COMP_MS_VISUAL_STUDIO_CPP
TEST(math_NextGreaterOrEqPow2, int_zero) {
    static_assert(ul::math::NextGreaterOrEqPow2<int, 0>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 0>::value() != 0);
}

TEST(math_NextGreaterOrEqPow2, ints) {
    static_assert(ul::math::NextGreaterOrEqPow2<int, 1>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 2>::value() == 2);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 3>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 4>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 5>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 6>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 7>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 8>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 9>::value() == 16);
    static_assert(ul::math::NextGreaterOrEqPow2<int, 10>::value() == 16);
}

TEST(math_NextGreaterOrEqPow2, int_neg) {
    static_assert(ul::math::NextGreaterOrEqPow2<int, -1>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int, -2>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int, -1000>::value() == 1);
}

TEST(math_NextGreaterOrEqPow2, uint8_t_zero) {
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 0>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 0>::value() != 0);
}

TEST(math_NextGreaterOrEqPow2, uint8_ts) {
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 1>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 2>::value() == 2);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 3>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 4>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 5>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 6>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 7>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 8>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 9>::value() == 16);
    static_assert(ul::math::NextGreaterOrEqPow2<uint8_t, 10>::value() == 16);
}

TEST(math_NextGreaterOrEqPow2, uint64_t_zero) {
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 0>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 0>::value() != 0);
}

TEST(math_NextGreaterOrEqPow2, int64_ts) {
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 1>::value() == 1);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 2>::value() == 2);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 3>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 4>::value() == 4);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 5>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 6>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 7>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 8>::value() == 8);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 9>::value() == 16);
    static_assert(ul::math::NextGreaterOrEqPow2<int64_t, 10>::value() == 16);
}

TEST(math_NextGreaterPow2, int_zero) {
    static_assert(ul::math::NextGreaterPow2<int, 0>::value() == 1);
    static_assert(ul::math::NextGreaterPow2<int, 0>::value() != 0);
}

TEST(math_NextGreaterPow2, ints) {
    static_assert(ul::math::NextGreaterPow2<int, 1>::value() == 2);
    static_assert(ul::math::NextGreaterPow2<int, 2>::value() == 4);
    static_assert(ul::math::NextGreaterPow2<int, 3>::value() == 4);
    static_assert(ul::math::NextGreaterPow2<int, 4>::value() == 8);
    static_assert(ul::math::NextGreaterPow2<int, 5>::value() == 8);
    static_assert(ul::math::NextGreaterPow2<int, 6>::value() == 8);
    static_assert(ul::math::NextGreaterPow2<int, 7>::value() == 8);
    static_assert(ul::math::NextGreaterPow2<int, 8>::value() == 16);
    static_assert(ul::math::NextGreaterPow2<int, 9>::value() == 16);
}

TEST(math_NextGreaterPow2, int_neg) {
    static_assert(ul::math::NextGreaterPow2<int, -1>::value() == 1);
    static_assert(ul::math::NextGreaterPow2<int, -2>::value() == 1);
    static_assert(ul::math::NextGreaterPow2<int, -1000>::value() == 1);
}
#endif

TEST(math_next_greater_or_eq_pow2, some) {
    static_assert(ul::math::next_greater_or_eq_pow2(1) == 1);
    static_assert(ul::math::next_greater_or_eq_pow2(2) == 2);
    static_assert(ul::math::next_greater_or_eq_pow2(3) == 4);
    static_assert(ul::math::next_greater_or_eq_pow2(4) == 4);
    static_assert(ul::math::next_greater_or_eq_pow2(5) == 8);
    static_assert(ul::math::next_greater_or_eq_pow2(6) == 8);
    static_assert(ul::math::next_greater_or_eq_pow2(7) == 8);
    static_assert(ul::math::next_greater_or_eq_pow2(8) == 8);
    static_assert(ul::math::next_greater_or_eq_pow2(9) == 16);
    static_assert(ul::math::next_greater_or_eq_pow2(10) == 16);

    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(1) == 1);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(2) == 2);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(3) == 4);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(4) == 4);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(5) == 8);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(6) == 8);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(7) == 8);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(8) == 8);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(9) == 16);
    EXPECT_TRUE(ul::math::next_greater_or_eq_pow2(10) == 16);
}

// NOLINTEND
