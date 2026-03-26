#if !UL_OS_MAC
#include "mb/ul/mem/physmemusage-rss.hpp"
#include "mb/ul/buildenv/macros.hpp"
#include "mb/ul/mem/types.hpp"
#include "gtest/gtest.h"
#include <cstring>
#include <iostream>
#if UL_OS_LINUX
#include <sys/mman.h>
#endif

namespace ul = mb::ul;

TEST(physmemusage, usage) {
#if UL_OS_LINUX
    const size_t memsize{1 * ul::mem::mb};

    double vm_initial{};
    double pm_initial{};
    ul::mem::usage(vm_initial, pm_initial);
    std::cout << "initial: virtmem: " << vm_initial << ", physmem: " << pm_initial << "\n";

    // glibc malloc may satisfy a multi-page request from an existing arena without growing the
    // map, so VmSize/RSS can stay flat (flaky). Anonymous mmap always adds a new mapping.
    void* const mapped = ::mmap(nullptr, memsize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ASSERT_NE(mapped, MAP_FAILED);
    auto* waste = static_cast<char*>(mapped);
    std::memset(waste, 1, memsize);
#if defined(__GNUC__) || defined(__clang__)
    __asm__ __volatile__("" ::: "memory");
#endif

    double vm{};
    double pm{};
    ul::mem::usage(vm, pm);
    std::cout << "after mmap: virtmem: " << vm << ", physmem: " << pm << "\n";
#if !defined(MB_DEVENV_SANITIZER)
    EXPECT_TRUE(vm > vm_initial || pm > pm_initial);
#endif

    ASSERT_EQ(::munmap(mapped, memsize), 0);
    double vm_final{};
    double pm_final{};
    ul::mem::usage(vm_final, pm_final);
    std::cout << "after munmap: virtmem: " << vm_final << ", physmem: " << pm_final << "\n";
    // RSS may not drop immediately after unmap; no strict assertion vs. the peak sample above.
    // (vm > vm_final || pm > pm_final might not hold).
#endif
}

#endif
