//! \file Implements default new+delete as example allocation strategy.

#ifndef EXAMPLE_H_SDUFHI3GN78325TX7635TN27
#define EXAMPLE_H_SDUFHI3GN78325TX7635TN27

#include "ul/mem/types.h"
#include <cstdint>
#include <new>

namespace mb::ul::mem::alloc {
class Example {
public:
    static uint8_t* allocate(Bytes size) {
        return reinterpret_cast<uint8_t*>(::operator new(size.value));
    }

    static void deallocate(uint8_t* p, Bytes /*unused*/ = {}) noexcept {
        ::operator delete(p);
    }
};
} // namespace mb::ul::mem::alloc

#endif
