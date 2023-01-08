#include "ul/profiler.h"
#include "gtest/gtest.h"
#include <thread>

namespace ul = mb::ul;

TEST(profilerTest, basics)
{
    const auto before{ul::profiler_now()};
    std::this_thread::sleep_for(std::chrono::milliseconds(42));
    const auto after{ul::profiler_now()};
    EXPECT_GT(after, before);
    const auto diff{ul::profiler_diff_s(before, after)};
    EXPECT_GT(0.062, diff);
    EXPECT_LT(0.022, diff);
}
