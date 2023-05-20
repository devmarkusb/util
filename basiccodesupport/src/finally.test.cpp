#include "ul/finally.h"
#include "ul/macros.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(FinallyTest, Test) {
    bool is_changed = false;
    {
        const auto ul_anonymous_variable = ul::finally([is_changed]() {
            is_changed = true;
        });
        is_changed = false;
    }
    EXPECT_TRUE(is_changed);
}
