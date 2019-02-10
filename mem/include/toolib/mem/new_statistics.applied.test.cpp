#define TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "toolib/mem/new_statistics.h"
#undef TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE
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
        EXPECT_EQ(too::mem::Statistics::instance().newCalls(),
            too::mem::Statistics::instance().deleteCalls() + (2737 - 2572));
        EXPECT_EQ(too::mem::Statistics::instance().allocatedSize(),
                  too::mem::Statistics::instance().deallocatedSize() + 16'802);
        std::cout << "new calls: " << too::mem::Statistics::instance().newCalls() << "\n";
        std::cout << "delete calls: " << too::mem::Statistics::instance().deleteCalls() << "\n";
        std::cout << "allocated size minus deallocated size: "
                  << too::fmt::groupThousands(too::mem::Statistics::instance().allocatedSize() -
                         too::mem::Statistics::instance().deallocatedSize())
                  << "\n";
        std::cout << "peak mem usage: " << too::fmt::groupThousands(too::mem::Statistics::instance().peakSize())
                  << "\n";
    }
} global;
} // namespace
