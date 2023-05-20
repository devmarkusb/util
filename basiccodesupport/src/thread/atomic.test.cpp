#include "ul/thread/atomic.h"
#include "gtest/gtest.h"
#include <atomic>

namespace ul = mb::ul;

namespace {
constexpr auto some_number_choice{42};
} // namespace

TEST(atomic_updateMaximum, update) {
    std::atomic<int> m{some_number_choice};
    ul::thread::atomic::update_maximum(m, some_number_choice + 1);
    EXPECT_EQ(m.load(), some_number_choice + 1);
}

TEST(atomic_updateMaximum, no_update_eq) {
    std::atomic<int> m{some_number_choice};
    ul::thread::atomic::update_maximum(m, some_number_choice);
    EXPECT_EQ(m.load(), some_number_choice);
}

TEST(atomic_updateMaximum, no_update_less) {
    std::atomic<int> m{some_number_choice};
    ul::thread::atomic::update_maximum(m, some_number_choice - 1);
    EXPECT_EQ(m.load(), some_number_choice);
}
