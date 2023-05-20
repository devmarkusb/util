#include "ul/thread/waitqueue.h"
#include "gtest/gtest.h"
#include <atomic>
#include <set>
#include <thread>

namespace ul = mb::ul;

namespace {
constexpr auto thread_count{100};
} // namespace

TEST(thread_WaitQueue, basics) {
    ul::thread::WaitQueue<int> q{2};

    ASSERT_TRUE(q.emplace(42));
    int elem{};
    q.wait_and_pop(elem);
    EXPECT_EQ(elem, 42);

    ASSERT_TRUE(q.emplace(43));
    ASSERT_TRUE(q.emplace(44));
    ASSERT_FALSE(q.emplace(45));
    q.wait_and_pop(elem);
    EXPECT_EQ(elem, 43);
    q.wait_and_pop(elem);
    EXPECT_EQ(elem, 44);
}

TEST(thread_WaitQueue, massive_parallel) {
    ul::thread::WaitQueue<int> q{thread_count};
    std::atomic<int> current{};
    std::vector<std::thread> producer(thread_count);
    for (auto& p : producer) {
        p = std::thread{[&q, &current]() {
            q.emplace(++current);
        }};
    }

    std::set<int> popped_values;
    std::mutex mutex;
    std::vector<std::thread> consumer(thread_count - 1);
    for (auto& c : consumer) {
        c = std::thread{[&q, &mutex, &popped_values]() {
            int elem{};
            q.wait_and_pop(elem);
            const std::lock_guard<std::mutex> lk{mutex};
            EXPECT_FALSE(popped_values.count /*contains*/ (elem));
            popped_values.insert(elem);
        }};
    }

    for (auto& p : producer) {
        p.join();
    }
    for (auto& c : consumer) {
        c.join();
    }

    EXPECT_EQ(popped_values.size(), thread_count - 1);
    int elem{};
    q.wait_and_pop(elem);
    EXPECT_FALSE(popped_values.count /*contains*/ (elem));
}

TEST(thread_WaitQueue, stop) {
    ul::thread::WaitQueue<int> q{2};

    q.stop();

    int elem{};
    q.wait_and_pop(elem);
    // just testing that we aren't caught in an endless loop with the previous line
    // (which we were without the stop)
}
