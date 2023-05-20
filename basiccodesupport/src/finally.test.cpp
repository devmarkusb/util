#include "ul/finally.h"
#include "ul/macros.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(FinallyTest, Test) {
    bool is_changed = false;
    {
        const auto UL_ANONYMOUS_IDENTIFIER = ul::finally([&is_changed]() {
            is_changed = true;
        });
        is_changed = false;
    }
    EXPECT_TRUE(is_changed);
}
