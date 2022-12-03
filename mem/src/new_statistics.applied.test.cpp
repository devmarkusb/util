#define TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE
#include "ul/format_number.h"
#include "gtest/gtest.h"

#include "ul/macros.h"

namespace ul = mb::ul;

namespace
{
struct AGlobalDestructor
{
    AGlobalDestructor()
    {
        auto& memstats = ul::mem::Statistics::instance();

        memstats.reset();
    }

    ~AGlobalDestructor()
    {
        const auto& memstats = ul::mem::Statistics::instance();

        const auto newCalls{memstats.newCalls()};
        const auto deleteCalls{memstats.deleteCalls()};
        const auto allocatedSize{memstats.allocatedSize()};
        const auto deallocatedSize{memstats.deallocatedSize()};
        const auto peakSize = memstats.peakSize();

        // mem/allocator.test.cpp is the evil one, that still has leaks according to the statistics
        // But nevertheless the test is ok for now as it can still detect (and prevent) any new leaks.
        std::cout << "new calls: " << newCalls << "\n";
        std::cout << "delete calls: " << deleteCalls << "\n";
#if TOO_OS_LINUX
        // compensates for gtest leaks, + 1 accounts for using EXPECT_DEBUG_DEATH
#if !TOO_DEBUG
        EXPECT_EQ(newCalls - deleteCalls, 78 - 75);
#endif
#else
        // untested
        EXPECT_EQ(newCalls - deleteCalls, 78 - 75 + 1);
#endif
        std::cout << "allocated size: " << allocatedSize << "\n";
        std::cout << "deallocated size: " << deallocatedSize << "\n";
        const auto allocDeallocDiff = allocatedSize - deallocatedSize;
        std::cout << "allocated size minus deallocated size: ";
#if !TOO_OS_MAC && !TOO_OS_WINDOWS
        // strange, under Windows this yields an exception 'Invalid address specified to RtlValidateHeap'
        std::cout << ul::fmt::groupThousands(allocDeallocDiff);
#else
        std::cout << allocDeallocDiff;
#endif
#if TOO_COMP_CLANG
#if TOO_COMP_CLANG_VER == 100001
#if TOO_DEBUG
        const auto compensation_EXPECT_DEBUG_DEATH{91};
#else
        const auto compensation_EXPECT_DEBUG_DEATH{91 - 65};
#endif
#else
        const auto compensation_EXPECT_DEBUG_DEATH{63};
#endif
#elif TOO_COMP_GNU_CPP
#if TOO_COMP_GNU_CPP_VER == 100100
#if TOO_DEBUG
        const auto compensation_EXPECT_DEBUG_DEATH{59 - 37 + 65};
#else
        const auto compensation_EXPECT_DEBUG_DEATH{59 - 37};
#endif
#else
        const auto compensation_EXPECT_DEBUG_DEATH{59};
#endif
#else
        const auto compensation_EXPECT_DEBUG_DEATH{59};
#endif
#if !TOO_OS_MAC
        // compensates for gtest leaks, + compensation_EXPECT_DEBUG_DEATH accounts for EXPECT_DEBUG_DEATH usages
        EXPECT_EQ(allocDeallocDiff, ul::mem::Bytes{107 + compensation_EXPECT_DEBUG_DEATH});
#else
        // untested
        EXPECT_EQ(allocDeallocDiff, ul::mem::Bytes{107 + compensation_EXPECT_DEBUG_DEATH});
#endif
        std::cout << "\n";
        std::cout << "peak mem usage: " <<
#if !TOO_OS_MAC && !TOO_OS_WINDOWS
            ul::fmt::groupThousands(peakSize)
#else
            peakSize
#endif
                  << "\n";
    }
};
} // namespace

int main(int argc, char** argv)
{
    // Our definition of 'global'. By the way, a real global data's construction and destruction time is beyond
    // our control, so that would never yield proper new/delete statistics. The next best 3rd party lib will mess
    // that up, as gtest does at least.
    // Gtest even leaks 3 deletes and 107 B by the following innocent code.
    AGlobalDestructor global;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
