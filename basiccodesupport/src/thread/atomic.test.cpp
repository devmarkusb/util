#include "ul/thread/atomic.h"
#include "gtest/gtest.h"
#include <atomic>

namespace ul = mb::ul;

namespace {
constexpr auto someNumberChoice{42};
} // namespace

TEST(atomic_updateMaximum, update) {
    std::atomic<int> m{someNumberChoice};
    ul::thread::atomic::updateMaximum(m, someNumberChoice + 1);
    EXPECT_EQ(m.load(), someNumberChoice + 1);
}

TEST(atomic_updateMaximum, no_update_eq) {
    std::atomic<int> m{someNumberChoice};
    ul::thread::atomic::updateMaximum(m, someNumberChoice);
    EXPECT_EQ(m.load(), someNumberChoice);
}

TEST(atomic_updateMaximum, no_update_less) {
    std::atomic<int> m{someNumberChoice};
    ul::thread::atomic::updateMaximum(m, someNumberChoice - 1);
    EXPECT_EQ(m.load(), someNumberChoice);
}
