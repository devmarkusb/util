#define UL_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef UL_I_AM_SURE_TO_REPLACE_NEW_DELETE
#include "ul/assert.h"
#include "ul/error.h"
#include "ul/mem/compiler_quirks.h"
#include "ul/mem/types.h"
#include <iostream>
#include <vector>

namespace ul = mb::ul;

int main() {
    try {
        std::cout << "Running main() from " << __FILE__ << "\n";

        const auto& memstats = ul::mem::Statistics::instance();

        UL_ASSERT_THROW(memstats.new_calls() == 0);
        UL_ASSERT_THROW(memstats.delete_calls() == 0);
        UL_ASSERT_THROW(memstats.peak_size() == ul::mem::Bytes{});

        size_t exp_new_calls{};
        size_t exp_delete_calls{};
        ul::mem::Bytes exp_peak_size{};
        constexpr auto n10{10};
        constexpr auto b44{44};
        {
            // volatile preventing the compiler from optimizing out not officially used p1
            int* volatile p1 = new int;
            UL_ASSERT_THROW(memstats.new_calls() == 1u);
            UL_ASSERT_THROW(memstats.peak_size() == ul::mem::Bytes{4});

            auto impl_correction_ofs = memstats.allocated_size();
            {
                std::vector<int> l;
                impl_correction_ofs = memstats.allocated_size() - impl_correction_ofs;
                // just out of interest
                UL_ASSERT_THROW(impl_correction_ofs == ul::mem::quirk::vector::constr_heap_alloc_size);
                UL_ASSERT_THROW(!l.capacity());
                l.reserve(n10);
                exp_new_calls = 2 + (impl_correction_ofs ? 1 : 0);
                UL_ASSERT_THROW(memstats.new_calls() == exp_new_calls);
                exp_peak_size = ul::mem::Bytes{b44} + impl_correction_ofs;
                UL_ASSERT_THROW(memstats.peak_size() == exp_peak_size);
                for (int i = 0; i < n10 / 2; ++i)
                    l.push_back(i);
            }
            exp_delete_calls = 1 + (impl_correction_ofs ? 1 : 0);
            UL_ASSERT_THROW(memstats.delete_calls() == exp_delete_calls);

            delete p1; // NOLINT
            ++exp_delete_calls;
            UL_ASSERT_THROW(memstats.delete_calls() == exp_delete_calls);

            UL_ASSERT_THROW(memstats.peak_size() == ul::mem::Bytes{b44} + impl_correction_ofs);

            int* volatile p2 = new int[n10]; // NOLINT
            ++exp_new_calls;
            UL_ASSERT_THROW(memstats.new_calls() == exp_new_calls);
            delete[] p2; // NOLINT
            ++exp_delete_calls;
            UL_ASSERT_THROW(memstats.delete_calls() == exp_delete_calls);
        }

        UL_ASSERT_THROW(memstats.new_calls() == exp_new_calls);
        UL_ASSERT_THROW(memstats.delete_calls() == exp_delete_calls);
        UL_ASSERT_THROW(memstats.peak_size() == exp_peak_size);

        std::cout << "PASSED (" << __FILE__ << ")\n";

        return ul::prog_exit_success;
    } catch (...) {
        return ul::prog_exit_failure;
    }
}
