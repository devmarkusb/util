#include "toolib/thread/waitcircbuffer.h"
#include "gtest/gtest.h"
#include <atomic>
#include <set>
#include <thread>


TEST(thread_WaitCircularBuffer, basics)
{
    too::thread::WaitCircularBuffer<int> q{2};

    ASSERT_TRUE(q.push(42));
    int elem{};
    q.waitAndPop(elem);
    EXPECT_EQ(elem, 42);

    ASSERT_TRUE(q.push(43));
    ASSERT_TRUE(q.push(44));
    ASSERT_FALSE(q.push(45));
    q.waitAndPop(elem);
    EXPECT_EQ(elem, 43);
    q.waitAndPop(elem);
    EXPECT_EQ(elem, 44);
}

TEST(thread_WaitCircularBuffer, massive_parallel)
{
    too::thread::WaitCircularBuffer<int> q{100};
    std::atomic<int> current{};
    const size_t threadCount{100};
    std::vector<std::thread> producer(threadCount);
    for (auto& p : producer)
    {
        p = std::thread{[&q, &current]() {
            q.push(++current);
        }};
    }

    std::set<int> poppedValues;
    std::mutex mutex;
    std::vector<std::thread> consumer(threadCount - 1);
    for (auto& c : consumer)
    {
        c = std::thread{[&q, &mutex, &poppedValues]() {
            int elem{};
            q.waitAndPop(elem);
            std::lock_guard<std::mutex> lk{mutex};
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

TEST(thread_WaitCircularBuffer, stop)
{
    too::thread::WaitCircularBuffer<int> q{2};

    q.stop();

    int elem{};
    q.waitAndPop(elem);
    // just testing that we aren't caught in an endless loop with the previous line
    // (which we were without the stop)
}
