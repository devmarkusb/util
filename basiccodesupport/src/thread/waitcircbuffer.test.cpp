#include "ul/thread/waitcircbuffer.h"
#include "gtest/gtest.h"
#include <atomic>
#include <set>
#include <thread>

namespace ul = mb::ul;

namespace {
constexpr auto someNumberChoice{42};
constexpr auto threadCount{100};
} // namespace

TEST(thread_WaitCircularBuffer, basics) {
    ul::thread::WaitCircularBuffer<int> q{2};

    ASSERT_TRUE(q.push(someNumberChoice));
    int elem{};
    q.waitAndPop(elem);
    EXPECT_EQ(elem, someNumberChoice);

    ASSERT_TRUE(q.push(someNumberChoice + 1));
    ASSERT_TRUE(q.push(someNumberChoice + 2));
    ASSERT_FALSE(q.push(someNumberChoice + 3));
    q.waitAndPop(elem);
    EXPECT_EQ(elem, someNumberChoice + 1);
    q.waitAndPop(elem);
    EXPECT_EQ(elem, someNumberChoice + 2);
}

TEST(thread_WaitCircularBuffer, massive_parallel) {
    ul::thread::WaitCircularBuffer<int> q{threadCount};
    std::atomic<int> current{};
    std::vector<std::thread> producer(threadCount);
    for (auto& p : producer) {
        p = std::thread{[&q, &current]() {
            q.push(++current);
        }};
    }

    std::set<int> poppedValues;
    std::mutex mutex;
    std::vector<std::thread> consumer(threadCount - 1);
    for (auto& c : consumer) {
        c = std::thread{[&q, &mutex, &poppedValues]() {
            int elem{};
            q.waitAndPop(elem);
            const std::lock_guard<std::mutex> lk{mutex};
            EXPECT_FALSE(poppedValues.count /*contains*/ (elem));
            poppedValues.insert(elem);
        }};
    }

    for (auto& p : producer) {
        p.join();
    }
    for (auto& c : consumer) {
        c.join();
    }

    EXPECT_EQ(poppedValues.size(), threadCount - 1);
    int elem{};
    q.waitAndPop(elem);
    EXPECT_FALSE(poppedValues.count /*contains*/ (elem));
}

TEST(thread_WaitCircularBuffer, stop) {
    ul::thread::WaitCircularBuffer<int> q{2};

    q.stop();

    int elem{};
    q.waitAndPop(elem);
    // just testing that we aren't caught in an endless loop with the previous line
    // (which we were without the stop)
}
