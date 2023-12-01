#include "ul/operation.h"
#include "ul/assert.h"

#include "gtest/gtest.h"

namespace {}

TEST(power_opt2Test, misc) {
    EXPECT_EQ(ul::power_opt2(2, 4, std::multiplies<int>{}), 16);
    EXPECT_EQ(
        ul::power_opt2(
            2, 4,
            [](int a, int b) {
                return a * b;
            }),
        16);
    // confused, why this wrapping is necessary, endless loop in release mode?!
#ifndef NDEBUG
    EXPECT_DEBUG_DEATH(ul::power_opt2(3, 0, std::multiplies<int>{}), "> 0");
#endif
    EXPECT_EQ(ul::power_opt2(3, 1, std::multiplies<int>{}), 3);
    EXPECT_EQ(ul::power_opt2(3, 2, std::multiplies<int>{}), 9);
}
