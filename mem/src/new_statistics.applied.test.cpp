#define UL_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef UL_I_AM_SURE_TO_REPLACE_NEW_DELETE
#include "ul/error.h"
#include "ul/format_number.h"
#include "ul/macros.h"
#include <iostream>

namespace ul = mb::ul;

namespace {
struct AGlobalDestructor {
    AGlobalDestructor() {
        auto& memstats = ul::mem::Statistics::instance();

        memstats.reset();
    }

    ~AGlobalDestructor() {
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
#if !UL_DEBUG
        UL_ASSERT_TERMINATE(newCalls - deleteCalls == 0);
#endif
#else
        // untested
        UL_ASSERT_TERMINATE(newCalls - deleteCalls == 0);
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
#if !UL_OS_MAC
        UL_ASSERT_TERMINATE(allocDeallocDiff == ul::mem::Bytes{});
#else
        // untested
        UL_ASSERT_TERMINATE(allocDeallocDiff == ul::mem::Bytes{});
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

int main(int, char**) {
    // Our definition of 'global'. By the way, a real global data's construction and destruction time is beyond
    // our control, so that would never yield proper new/delete statistics.
    AGlobalDestructor global;
    return ul::prog_exit_success;
}
