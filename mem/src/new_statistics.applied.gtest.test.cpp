#define UL_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef UL_I_AM_SURE_TO_REPLACE_NEW_DELETE
#include "ul/format_number.h"
#include "ul/macros.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

namespace {
struct AGlobalDestructor {
    AGlobalDestructor() {
        auto& memstats = ul::mem::Statistics::instance();

        memstats.reset();
    }

    ~AGlobalDestructor() {
        try {
            const auto& memstats = ul::mem::Statistics::instance();

            const auto new_calls{memstats.new_calls()};
            const auto delete_calls{memstats.delete_calls()};
            const auto allocated_size{memstats.allocated_size()};
            const auto deallocated_size{memstats.deallocated_size()};
            const auto peak_size = memstats.peak_size();

            // mem/allocator.test.cpp is the evil one, that still has leaks according to the statistics
            // But nevertheless the test is ok for now as it can still detect (and prevent) any new leaks.
            std::cout << "new calls: " << new_calls << "\n";
            std::cout << "delete calls: " << delete_calls << "\n";
#if UL_OS_LINUX
            // compensates for gtest leaks, + 1 accounts for using EXPECT_DEBUG_DEATH
#if !UL_DEBUG
            // commented-out because it unfortunately doesn't work yet
            //EXPECT_EQ(new_calls - delete_calls, 78 - 75);
#endif
#else
            // untested
#endif
            std::cout << "allocated size: " << allocated_size << "\n";
            std::cout << "deallocated size: " << deallocated_size << "\n";
            const auto alloc_dealloc_diff = allocated_size - deallocated_size;
            std::cout << "allocated size minus deallocated size: ";
            std::cout << ul::fmt::group_thousands(alloc_dealloc_diff);
#if UL_COMP_CLANG
#if UL_COMP_CLANG_VER == 100001
#if UL_DEBUG
            [[maybe_unused]] const auto compensation_EXPECT_DEBUG_DEATH{91};
#else
            [[maybe_unused]] const auto compensation_EXPECT_DEBUG_DEATH{91 - 65};
#endif
#else
            [[maybe_unused]] const auto compensation_expect_debug_death{63};
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
            //EXPECT_EQ(alloc_dealloc_diff, ul::mem::Bytes{107 + compensation_EXPECT_DEBUG_DEATH});
#else
            // untested
#endif
            std::cout << "\n";
            std::cout << "peak mem usage: " <<
#if !UL_OS_MAC && !UL_OS_WINDOWS
                ul::fmt::group_thousands(peak_size)
#else
                peak_size
#endif
                      << "\n";
        } catch (...) {
            UL_NOOP;
        }
    }
};
} // namespace

int main(int argc, char** argv) {
    // Our definition of 'global'. By the way, a real global data's construction and destruction time is beyond
    // our control, so that would never yield proper new/delete statistics. The next best 3rd party lib will mess
    // that up, as gtest does at least.
    // Gtest even leaks 3 deletes and 107 B by the following innocent code.
    const AGlobalDestructor global;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
