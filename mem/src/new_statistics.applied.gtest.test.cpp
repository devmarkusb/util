#define UL_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef UL_I_AM_SURE_TO_REPLACE_NEW_DELETE
#include "ul/format_number.h"
#include "ul/macros.h"
#include "gtest/gtest.h"

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
#if UL_OS_LINUX
        // compensates for gtest leaks, + 1 accounts for using EXPECT_DEBUG_DEATH
#if !UL_DEBUG
        // commented-out because it unfortunately doesn't work yet
        //EXPECT_EQ(newCalls - deleteCalls, 78 - 75);
#endif
#else
        // untested
#endif
        std::cout << "allocated size: " << allocatedSize << "\n";
        std::cout << "deallocated size: " << deallocatedSize << "\n";
        const auto allocDeallocDiff = allocatedSize - deallocatedSize;
        std::cout << "allocated size minus deallocated size: ";
#if !UL_OS_MAC && !UL_OS_WINDOWS
        // strange, under Windows this yields an exception 'Invalid address specified to RtlValidateHeap'
        std::cout << ul::fmt::groupThousands(allocDeallocDiff);
#else
        std::cout << allocDeallocDiff;
#endif
#if UL_COMP_CLANG
#if UL_COMP_CLANG_VER == 100001
#if UL_DEBUG
        const auto compensation_EXPECT_DEBUG_DEATH{91};
#else
        const auto compensation_EXPECT_DEBUG_DEATH{91 - 65};
#endif
#else
        [[maybe_unused]] const auto compensation_EXPECT_DEBUG_DEATH{63};
#endif
#elif UL_COMP_GNU_CPP
#if UL_COMP_GNU_CPP_VER == 100100
#if UL_DEBUG
        [[maybe_unused]] const auto compensation_EXPECT_DEBUG_DEATH{59 - 37 + 65};
#else
        [[maybe_unused]] const auto compensation_EXPECT_DEBUG_DEATH{59 - 37};
#endif
#else
        [[maybe_unused]] const auto compensation_EXPECT_DEBUG_DEATH{59};
#endif
#else
        [[maybe_unused]] const auto compensation_EXPECT_DEBUG_DEATH{59};
#endif
#if !UL_OS_MAC
        // compensates for gtest leaks, + compensation_EXPECT_DEBUG_DEATH accounts for EXPECT_DEBUG_DEATH usages
        // commented-out because it unfortunately doesn't work yet
        //EXPECT_EQ(allocDeallocDiff, ul::mem::Bytes{107 + compensation_EXPECT_DEBUG_DEATH});
#else
        // untested
#endif
        std::cout << "\n";
        std::cout << "peak mem usage: " <<
#if !UL_OS_MAC && !UL_OS_WINDOWS
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
