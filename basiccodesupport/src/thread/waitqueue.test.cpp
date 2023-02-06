#include "ul/thread/waitqueue.h"
#include "gtest/gtest.h"
#include <atomic>
#include <set>
#include <thread>

namespace ul = mb::ul;

namespace
{
constexpr auto threadCount{100};
} // namespace

TEST(thread_WaitQueue, basics)
{
    ul::thread::WaitQueue<int> q{2};

    ASSERT_TRUE(q.emplace(42));
    int elem{};
    q.waitAndPop(elem);
    EXPECT_EQ(elem, 42);

    ASSERT_TRUE(q.emplace(43));
    ASSERT_TRUE(q.emplace(44));
    ASSERT_FALSE(q.emplace(45));
    q.waitAndPop(elem);
    EXPECT_EQ(elem, 43);
    q.waitAndPop(elem);
    EXPECT_EQ(elem, 44);
}

TEST(thread_WaitQueue, massive_parallel)
{
    ul::thread::WaitQueue<int> q{threadCount};
    std::atomic<int> current{};
    std::vector<std::thread> producer(threadCount);
    for (auto& p : producer)
    {
        p = std::thread{[&q, &current]()
                        {
                            q.emplace(++current);
                        }};
    }

    std::set<int> poppedValues;
    std::mutex mutex;
    std::vector<std::thread> consumer(threadCount - 1);
    for (auto& c : consumer)
    {
        c = std::thread{[&q, &mutex, &poppedValues]()
                        {
                            int elem{};
                            q.waitAndPop(elem);
                            const std::lock_guard<std::mutex> lk{mutex};
                            EXPECT_FALSE(poppedValues.count /*contains*/ (elem));
                            poppedValues.insert(elem);
                        }};
    }

    for (auto& p : producer)
    {
        p.join();
    }
    for (auto& c : consumer)
    {
        c.join();
    }

    EXPECT_EQ(poppedValues.size(), threadCount - 1);
    int elem{};
    q.waitAndPop(elem);
    EXPECT_FALSE(poppedValues.count /*contains*/ (elem));
}

TEST(thread_WaitQueue, stop)
{
    ul::thread::WaitQueue<int> q{2};

    q.stop();

    int elem{};
    q.waitAndPop(elem);
    // just testing that we aren't caught in an endless loop with the previous line
    // (which we were without the stop)
}
