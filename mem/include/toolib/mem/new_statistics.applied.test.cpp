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
        // mem/allocator.test.cpp is the evil one, that still has leaks according to the statistics
        // But nevertheless the test is ok for now as it can still detect (and prevent) any new leaks.
        std::cout << "new calls: " << too::mem::Statistics::instance().newCalls() << "\n";
        std::cout << "delete calls: " << too::mem::Statistics::instance().deleteCalls() << "\n";
#if !TOO_OS_MAC
        EXPECT_EQ(too::mem::Statistics::instance().newCalls(),
            too::mem::Statistics::instance().deleteCalls() + (2737 - 2572));
#endif
        std::cout << "allocated size minus deallocated size: ";
        const auto allocDeallocDiff = too::mem::Statistics::instance().allocatedSize() -
                                      too::mem::Statistics::instance().deallocatedSize();
#if !TOO_OS_MAC
        EXPECT_EQ(too::mem::Statistics::instance().allocatedSize(),
                  too::mem::Statistics::instance().deallocatedSize() + allocDeallocDiff);
#endif
#if !TOO_OS_MAC
        std::cout << too::fmt::groupThousands(allocDeallocDiff);
#else
        std::cout << allocDeallocDiff;
#endif
        std::cout << "\n";
        const auto peakSize = too::mem::Statistics::instance().peakSize();
        std::cout << "peak mem usage: " <<
#if !TOO_OS_MAC
            too::fmt::groupThousands(peakSize)
#else
            peakSize
#endif
            << "\n";
    }
} global;
} // namespace
