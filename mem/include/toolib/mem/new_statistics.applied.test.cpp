#define TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "toolib/mem/new_statistics.h"
#undef TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE
#include "toolib/PPDEFS.h"
#include "toolib/format_number.h"
#include "gtest/gtest.h"


namespace
{
struct AGlobalDestructor
{
    ~AGlobalDestructor()
    {
        const auto& memstats = too::mem::Statistics::instance();

        // mem/allocator.test.cpp is the evil one, that still has leaks according to the statistics
        // But nevertheless the test is ok for now as it can still detect (and prevent) any new leaks.
        std::cout << "new calls: " << memstats.newCalls() << "\n";
        std::cout << "delete calls: " << memstats.deleteCalls() << "\n";
#if TOO_LINUX
        EXPECT_EQ(memstats.newCalls() - memstats.deleteCalls(), 2737 - 2572);
#endif
        std::cout << "allocated size minus deallocated size: ";
        const auto allocDeallocDiff = memstats.allocatedSize() -
                                      memstats.deallocatedSize();
#if !TOO_OS_MAC
        EXPECT_EQ(memstats.allocatedSize(),
                  memstats.deallocatedSize() + allocDeallocDiff);
#endif
#if !TOO_OS_MAC && !TOO_OS_WINDOWS
        // strange, under Windows this yields an exception 'Invalid address specified to RtlValidateHeap'
        std::cout << too::fmt::groupThousands(allocDeallocDiff);
#else
        std::cout << allocDeallocDiff;
#endif
        std::cout << "\n";
        const auto peakSize = memstats.peakSize();
        std::cout << "peak mem usage: " <<
#if !TOO_OS_MAC && !TOO_OS_WINDOWS
            too::fmt::groupThousands(peakSize)
#else
            peakSize
#endif
            << "\n";
    }
} global;
} // namespace
