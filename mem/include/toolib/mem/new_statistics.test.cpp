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
    {
        int *p1 = new int;
        delete p1;


        int *p2 = new int[10];
        delete[] p2;

        std::vector<int> l;
        l.reserve(5);
        for (int i = 0; i < 5; ++i)
            l.push_back(i);
    }

    TOO_ASSERT_TERMINATE(too::mem::Statistics::instance().newCalls() == 3);
    TOO_ASSERT_TERMINATE(too::mem::Statistics::instance().deleteCalls() == 3);
    std::cout << "peak bytes: " << too::mem::Statistics::instance().peakSize() << "\n";

    return too::prog_exit_success;
}
