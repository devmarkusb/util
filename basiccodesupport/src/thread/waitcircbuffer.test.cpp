#include "ul/thread/waitcircbuffer.h"
#include "gtest/gtest.h"
#include <atomic>
#include <set>
#include <thread>

namespace ul = mb::ul;

namespace {
constexpr auto some_number_choice{42};
constexpr auto thread_count{100};
} // namespace

TEST(thread_WaitCircularBuffer, basics) {
    ul::thread::WaitCircularBuffer<int> q{2};

    ASSERT_TRUE(q.push(some_number_choice));
    int elem{};
    q.waitAndPop(elem);
    EXPECT_EQ(elem, some_number_choice);

    ASSERT_TRUE(q.push(some_number_choice + 1));
    ASSERT_TRUE(q.push(some_number_choice + 2));
    ASSERT_FALSE(q.push(some_number_choice + 3));
    q.waitAndPop(elem);
    EXPECT_EQ(elem, some_number_choice + 1);
    q.waitAndPop(elem);
    EXPECT_EQ(elem, some_number_choice + 2);
}

TEST(thread_WaitCircularBuffer, massive_parallel) {
    ul::thread::WaitCircularBuffer<int> q{thread_count};
    std::atomic<int> current{};
    std::vector<std::thread> producer(thread_count);
    for (auto& p : producer) {
        p = std::thread{[&q, &current]() {
            q.push(++current);
        }};
    }

    std::set<int> popped_values;
    std::mutex mutex;
    std::vector<std::thread> consumer(thread_count - 1);
    for (auto& c : consumer) {
        c = std::thread{[&q, &mutex, &popped_values]() {
            int elem{};
            q.waitAndPop(elem);
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
    q.waitAndPop(elem);
    EXPECT_FALSE(popped_values.count /*contains*/ (elem));
}

TEST(thread_WaitCircularBuffer, stop) {
    ul::thread::WaitCircularBuffer<int> q{2};

    q.stop();

    int elem{};
    q.waitAndPop(elem);
    // just testing that we aren't caught in an endless loop with the previous line
    // (which we were without the stop)
}
