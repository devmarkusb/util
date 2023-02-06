#include "ul/thread/pin.h"
#include "ul/macros.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>

#if UL_OS_LINUX
namespace ul = mb::ul;
#endif

class pinToCPUTest : public testing::Test
{
protected:
    struct Ready2go
    {
        std::mutex m;
        std::condition_variable cv;
        bool ok{false};
    };

    void SetUp() override
    {
    }
};

#if !UL_OS_LINUX // mac also not working yet
TEST_F(pinToCPUTest, DISABLED_twoThreadsPinnedToFirstTwoCPUs_okFor10msCheckedEach2ms)
{
}
#else
TEST_F(pinToCPUTest, twoThreadsPinnedToFirstTwoCPUs_okFor10msCheckedEach2ms)
{
    const auto num_threads = std::min(2u, std::thread::hardware_concurrency());
    ASSERT_GE(num_threads, 2u); // get some modern system for development!

    std::vector<std::thread> threads(num_threads);
    std::vector<Ready2go> ready2go(num_threads);
    for (auto i = decltype(num_threads){0}; i < num_threads; ++i)
    {
        threads[i] = std::thread(
            [i, &ready2go]
            {
                std::unique_lock<std::mutex> lk(ready2go[i].m);
                ready2go[i].cv.wait(
                    lk,
                    [i, &ready2go]
                    {
                        return ready2go[i].ok;
                    });
                lk.unlock();
                for (auto j = 0; j < 5; ++j)
                {
                    EXPECT_EQ(static_cast<int>(i), ul::thread::numLogicalCores());
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            });
        ul::thread::pinToLogicalCore(threads[i], static_cast<int>(i));
        {
            std::lock_guard<std::mutex> lk(ready2go[i].m);
            ready2go[i].ok = true;
        }
        ready2go[i].cv.notify_one();
    }
    for (auto& t : threads)
    {
        t.join();
    }
}
#endif

#if !UL_OS_LINUX // mac also not working yet
TEST_F(pinToCPUTest, DISABLED_twoThreadsPinnedToSecondCPUOnly_okFor10msCheckedEach2ms)
{
}
#else
TEST_F(pinToCPUTest, twoThreadsPinnedToSecondCPUOnly_okFor10msCheckedEach2ms)
{
    const auto num_threads = std::min(2u, std::thread::hardware_concurrency());
    ASSERT_GE(num_threads, 2u); // get some modern system for development!

    std::vector<std::thread> threads(num_threads);
    std::vector<Ready2go> ready2go(num_threads);
    for (auto i = decltype(num_threads){0}; i < num_threads; ++i)
    {
        threads[i] = std::thread(
            [i, &ready2go]
            {
                std::unique_lock<std::mutex> lk(ready2go[i].m);
                ready2go[i].cv.wait(
                    lk,
                    [i, &ready2go]
                    {
                        return ready2go[i].ok;
                    });
                lk.unlock();
                for (auto j = 0; j < 5; ++j)
                {
                    EXPECT_EQ(1, ul::thread::numLogicalCores());
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            });
        ul::thread::pinToLogicalCore(threads[i], 1);
        {
            std::lock_guard<std::mutex> lk(ready2go[i].m);
            ready2go[i].ok = true;
        }
        ready2go[i].cv.notify_one();
    }
    for (auto& t : threads)
    {
        t.join();
    }
}
#endif

#if !UL_OS_LINUX // mac also not working yet
TEST_F(pinToCPUTest, DISABLED_twoThreadsPinnedToFirstTwoCPUsSwitchedAfter3rdCheck_okFor50msCheckedEach5ms)
{
}
#else
TEST_F(pinToCPUTest, twoThreadsPinnedToFirstTwoCPUsSwitchedAfter3rdCheck_okFor50msCheckedEach5ms)
{
    const auto num_threads = std::min(2u, std::thread::hardware_concurrency());
    ASSERT_GE(num_threads, 2u); // get some modern system for development!

    std::vector<std::thread> threads(num_threads);
    std::vector<Ready2go> ready2go(num_threads);
    bool switched{};
    std::mutex switched_m;
    auto switchCores = [&threads, &num_threads]()
    {
        for (auto i = decltype(num_threads){0}; i < num_threads; ++i)
        {
            ul::thread::pinToLogicalCore(threads[i], static_cast<int>(i + 1 % num_threads));
        }
    };
    for (auto i = decltype(num_threads){0}; i < num_threads; ++i)
    {
        threads[i] = std::thread(
            [i, &ready2go, &switched, &switched_m, &switchCores, &num_threads]
            {
                std::unique_lock<std::mutex> lk(ready2go[i].m);
                ready2go[i].cv.wait(
                    lk,
                    [i, &ready2go]
                    {
                        return ready2go[i].ok;
                    });
                lk.unlock();
                for (auto j = 0; j < 10; ++j)
                {
                    {
                        std::lock_guard<std::mutex> switch_lk(switched_m);
                        if (!switched)
                        {
                            EXPECT_EQ(static_cast<int>(i), ul::thread::numLogicalCores());
                            if (j == 2)
                            {
                                switched = true;
                                switchCores();
                            }
                        }
                        else
                        {
                            EXPECT_EQ(static_cast<int>(i + 1u % num_threads), ul::thread::numLogicalCores());
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            });
        ul::thread::pinToLogicalCore(threads[i], static_cast<int>(i));
        {
            std::lock_guard<std::mutex> lk(ready2go[i].m);
            ready2go[i].ok = true;
        }
        ready2go[i].cv.notify_one();
    }
    for (auto& t : threads)
    {
        t.join();
    }
}
#endif

#if !UL_OS_LINUX // mac also not working yet
TEST_F(pinToCPUTest, DISABLED_maxThreadsPinnedToSeparateCPUs_okFor10msCheckedEach2ms)
{
}
#else
TEST_F(pinToCPUTest, maxThreadsPinnedToSeparateCPUs_okFor10msCheckedEach2ms)
{
    const auto num_threads = std::thread::hardware_concurrency();
    ASSERT_GT(num_threads, 0u); // hardware_concurrency failed

    std::vector<std::thread> threads(num_threads);
    std::vector<Ready2go> ready2go(num_threads);
    for (auto i = decltype(num_threads){0}; i < num_threads; ++i)
    {
        threads[i] = std::thread(
            [i, &ready2go]
            {
                std::unique_lock<std::mutex> lk(ready2go[i].m);
                ready2go[i].cv.wait(
                    lk,
                    [i, &ready2go]
                    {
                        return ready2go[i].ok;
                    });
                lk.unlock();
                for (auto j = 0; j < 5; ++j)
                {
                    EXPECT_EQ(static_cast<int>(i), ul::thread::numLogicalCores());
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            });
        ul::thread::pinToLogicalCore(threads[i], static_cast<int>(i));
        {
            std::lock_guard<std::mutex> lk(ready2go[i].m);
            ready2go[i].ok = true;
        }
        ready2go[i].cv.notify_one();
    }
    for (auto& t : threads)
    {
        t.join();
    }
}
#endif

#if !UL_OS_LINUX // mac also not working yet
TEST_F(pinToCPUTest, DISABLED_invalidCPUNr_throws)
{
}
#else
TEST_F(pinToCPUTest, invalidCPUNr_throws)
{
    std::thread thread(
        []
        {
        });
    EXPECT_THROW(
        ul::thread::pinToLogicalCore(thread, 1 + static_cast<int>(std::thread::hardware_concurrency())),
        std::runtime_error);
    thread.join();
}
#endif
