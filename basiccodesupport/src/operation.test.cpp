#include "ul/operation.h"
#include "ul/assert.h"

#include "gtest/gtest.h"

namespace {}

TEST(power_opt2Test, misc) {
    EXPECT_EQ(ul::power_pos(2, 4, std::multiplies<int>{}), 16);
    EXPECT_EQ(
        ul::power_pos(
            2, 4,
            [](int a, int b) {
                return a * b;
            }),
        16);
    // confused, why this wrapping is necessary, endless loop in release mode?!
#ifndef NDEBUG
    std::string msg;
#if UL_OS_WINDOWS && UL_COMP_MS_VISUAL_STUDIO_CPP
    msg = "";
#else
    msg = "> 0";
#endif
    EXPECT_DEBUG_DEATH(ul::power_pos(3, 0, std::multiplies<int>{}), msg);
#endif
    EXPECT_EQ(ul::power_pos(3, 1, std::multiplies<int>{}), 3);
    EXPECT_EQ(ul::power_pos(3, 2, std::multiplies<int>{}), 9);
}
