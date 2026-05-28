//! \file Implements default new+delete as example allocation strategy.

#ifndef EXAMPLE_H_SDUFHI3GN78325TX7635TN27
#define EXAMPLE_H_SDUFHI3GN78325TX7635TN27

#include "mb/ul/buildenv/warnings.hpp"
#include "mb/ul/mem/types.hpp"
#include <cstdint>
#include <new>

namespace mb::ul::mem::alloc {
class Example {
public:
    UL_PRAGMA_WARNINGS_PUSH

    UL_WARNING_DISABLE_CLANG(allocator-wrappers)

    static uint8_t* allocate(Bytes size) {
        return reinterpret_cast<uint8_t*>(::operator new(size.value));
    }

    UL_PRAGMA_WARNINGS_POP

    static void deallocate(uint8_t* p, Bytes /*unused*/ = {}) noexcept {
        ::operator delete(p);
    }
};
} // namespace mb::ul::mem::alloc

#endif
