#if !UL_OS_MAC
#include "ul/mem/physmemusage-rss.h"
#include "ul/macros.h"
#include "ul/mem/types.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace ul = mb::ul;

TEST(physmemusage, usage) {
#if UL_OS_LINUX
    const size_t memsize{1 * ul::mem::mb};

    double vm_initial{};
    double pm_initial{};
    ul::mem::usage(vm_initial, pm_initial);
    std::cout << "initial: virtmem: " << vm_initial << ", physmem: " << pm_initial << "\n";

    auto waste = reinterpret_cast<char*>(std::malloc(memsize));
    std::memset(waste, 1, memsize);
    double vm{};
    double pm{};
    ul::mem::usage(vm, pm);
    std::cout << "after malloc: virtmem: " << vm << ", physmem: " << pm << "\n";
    EXPECT_TRUE(vm > vm_initial || pm > pm_initial);

    std::free(waste);
    double vm_final{};
    double pm_final{};
    ul::mem::usage(vm_final, pm_final);
    std::cout << "after free: virtmem: " << vm_final << ", physmem: " << pm_final << "\n";
    EXPECT_TRUE(vm > vm_final || pm > pm_final);
#endif
}

#endif
