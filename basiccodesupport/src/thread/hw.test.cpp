#include "ul/thread/hw.h"
#include "gtest/gtest.h"
#include <thread>

namespace ul = mb::ul;

TEST(thread_numAvailLogicalCores, consistency) {
    EXPECT_EQ(std::thread::hardware_concurrency(), ul::thread::numAvailLogicalCores());
}
