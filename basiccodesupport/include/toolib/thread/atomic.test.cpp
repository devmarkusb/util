#include "atomic.h"
#include "gtest/gtest.h"
#include <atomic>


TEST(atomic_updateMaximum, update)
{
    std::atomic<int> m{42};
    too::thread::atomic::updateMaximum(m, 43);
    EXPECT_EQ(m.load(), 43);
}

TEST(atomic_updateMaximum, no_update_eq)
{
    std::atomic<int> m{42};
    too::thread::atomic::updateMaximum(m, 42);
    EXPECT_EQ(m.load(), 42);
}

TEST(atomic_updateMaximum, no_update_less)
{
    std::atomic<int> m{42};
    too::thread::atomic::updateMaximum(m, 41);
    EXPECT_EQ(m.load(), 42);
}