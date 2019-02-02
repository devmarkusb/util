#include "physmemusage-rss.h"
#include "toolib/mem/types.h"
#include "toolib/PPDEFS.h"
#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

TEST(physmemusage, usage)
{
#if TOO_OS_LINUX
    const size_t memsize{1 * too::mem::MB};

    double vm_initial{};
    double pm_initial{};
    too::mem::usage(vm_initial, pm_initial);
    std::cout << "initial: virtmem: " << vm_initial << ", physmem: " << pm_initial << "\n";

    auto waste = reinterpret_cast<char*>(std::malloc(memsize));
    std::memset(waste, 1, memsize);
    double vm{};
    double pm{};
    too::mem::usage(vm, pm);
    std::cout << "after malloc: virtmem: " << vm << ", physmem: " << pm << "\n";
    EXPECT_TRUE(vm > vm_initial || pm > pm_initial);

    std::free(waste);
    double vm_final{};
    double pm_final{};
    too::mem::usage(vm_final, pm_final);
    std::cout << "after free: virtmem: " << vm_final << ", physmem: " << pm_final << "\n";
    EXPECT_TRUE(vm > vm_final || pm > pm_final);
#endif
}