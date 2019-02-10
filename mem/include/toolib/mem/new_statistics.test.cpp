#define TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "new_statistics.h"
#undef TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE
#include "toolib/assert.h"
#include "toolib/error.h"
#include <iostream>
#include <vector>


int main()
{
    std::cout << "Running main() from " << __FILE__ << "\n";

    TOO_ASSERT_THROW(too::mem::Statistics::instance().newCalls() == 0);
    TOO_ASSERT_THROW(too::mem::Statistics::instance().deleteCalls() == 0);
    TOO_ASSERT_THROW(too::mem::Statistics::instance().peakSize() == 0);

    {
        int* p1 = new int;
        TOO_ASSERT_THROW(too::mem::Statistics::instance().newCalls() == 1);
        TOO_ASSERT_THROW(too::mem::Statistics::instance().peakSize() == 4);

        {
            std::vector<int> l;
            l.reserve(10);
            TOO_ASSERT_THROW(too::mem::Statistics::instance().newCalls() == 2);
            TOO_ASSERT_THROW(too::mem::Statistics::instance().peakSize() == 44);
            for (int i = 0; i < 5; ++i)
                l.push_back(i);
        }
        TOO_ASSERT_THROW(too::mem::Statistics::instance().deleteCalls() == 1);

        delete p1;
        TOO_ASSERT_THROW(too::mem::Statistics::instance().deleteCalls() == 2);

        TOO_ASSERT_THROW(too::mem::Statistics::instance().peakSize() == 44);

        int* p2 = new int[10];
        TOO_ASSERT_THROW(too::mem::Statistics::instance().newCalls() == 3);
        delete[] p2;
        TOO_ASSERT_THROW(too::mem::Statistics::instance().deleteCalls() == 3);
    }

    TOO_ASSERT_THROW(too::mem::Statistics::instance().newCalls() == 3);
    TOO_ASSERT_THROW(too::mem::Statistics::instance().deleteCalls() == 3);
    TOO_ASSERT_THROW(too::mem::Statistics::instance().peakSize() == 44);

    std::cout << "PASSED (" << __FILE__ << ")\n";

    return too::prog_exit_success;
}
