#define TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE
#include "ul/assert.h"
#include "ul/error.h"
#include "ul/mem/compiler_quirks.h"
#include <iostream>
#include <vector>

namespace ul = mb::ul;

int main()
{
    std::cout << "Running main() from " << __FILE__ << "\n";

    const auto& memstats = ul::mem::Statistics::instance();

    TOO_ASSERT_THROW(memstats.newCalls() == 0);
    TOO_ASSERT_THROW(memstats.deleteCalls() == 0);
    TOO_ASSERT_THROW(memstats.peakSize() == 0);

    std::size_t exp_new_calls{};
    std::size_t exp_delete_calls{};
    ul::mem::Bytes exp_peak_size{};
    {
        int* p1 = new int;
        TOO_ASSERT_THROW(memstats.newCalls() == 1);
        TOO_ASSERT_THROW(memstats.peakSize() == ul::mem::Bytes{4});

        auto impl_correction_ofs = memstats.allocatedSize();
        {
            std::vector<int> l;
            impl_correction_ofs = memstats.allocatedSize() - impl_correction_ofs;
            // just out of interest
            TOO_ASSERT_THROW(impl_correction_ofs == ul::mem::quirk::vector::constr_heap_alloc_size);
            TOO_ASSERT_THROW(!l.capacity());
            l.reserve(10);
            exp_new_calls = 2 + (impl_correction_ofs ? 1 : 0);
            TOO_ASSERT_THROW(memstats.newCalls() == exp_new_calls);
            exp_peak_size = ul::mem::Bytes{44} + impl_correction_ofs;
            TOO_ASSERT_THROW(memstats.peakSize() == exp_peak_size);
            for (int i = 0; i < 5; ++i)
                l.push_back(i);
        }
        exp_delete_calls = 1 + (impl_correction_ofs ? 1 : 0);
        TOO_ASSERT_THROW(memstats.deleteCalls() == exp_delete_calls);

        delete p1;
        ++exp_delete_calls;
        TOO_ASSERT_THROW(memstats.deleteCalls() == exp_delete_calls);

        TOO_ASSERT_THROW(memstats.peakSize() == ul::mem::Bytes{44} + impl_correction_ofs);

        int* p2 = new int[10];
        ++exp_new_calls;
        TOO_ASSERT_THROW(memstats.newCalls() == exp_new_calls);
        delete[] p2;
        ++exp_delete_calls;
        TOO_ASSERT_THROW(memstats.deleteCalls() == exp_delete_calls);
    }

    TOO_ASSERT_THROW(memstats.newCalls() == exp_new_calls);
    TOO_ASSERT_THROW(memstats.deleteCalls() == exp_delete_calls);
    TOO_ASSERT_THROW(memstats.peakSize() == exp_peak_size);

    std::cout << "PASSED (" << __FILE__ << ")\n";

    return ul::prog_exit_success;
}
