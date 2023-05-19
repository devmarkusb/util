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
#if !UL_DEBUG
        UL_ASSERT_TERMINATE(new_calls - delete_calls == 0);
#endif
#else
        // untested
        UL_ASSERT_TERMINATE(new_calls - delete_calls == 0);
#endif
        std::cout << "allocated size: " << allocated_size << "\n";
        std::cout << "deallocated size: " << deallocated_size << "\n";
        const auto alloc_dealloc_diff = allocated_size - deallocated_size;
        std::cout << "allocated size minus deallocated size: ";
#if !UL_OS_MAC && !UL_OS_WINDOWS
        // strange, under Windows this yields an exception 'Invalid address specified to RtlValidateHeap'
        std::cout << ul::fmt::groupThousands(alloc_dealloc_diff);
#else
        std::cout << alloc_dealloc_diff;
#endif
#if !UL_OS_MAC
        UL_ASSERT_TERMINATE(alloc_dealloc_diff == ul::mem::Bytes{});
#else
        // untested
        UL_ASSERT_TERMINATE(alloc_dealloc_diff == ul::mem::Bytes{});
#endif
        std::cout << "\n";
        std::cout << "peak mem usage: " <<
#if !UL_OS_MAC && !UL_OS_WINDOWS
            ul::fmt::groupThousands(peak_size)
#else
            peak_size
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
