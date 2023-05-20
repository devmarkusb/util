//! \file

#ifndef ATOMIC_H_E7TZX3785TZ78G2XH4H2GG
#define ATOMIC_H_E7TZX3785TZ78G2XH4H2GG

#include "ul/macros.h"
#include <atomic>

namespace mb::ul::thread::atomic {
template <typename T>
void update_maximum(std::atomic<T>& maximum_value, const T& value) noexcept {
    T prev_value = maximum_value;
    while (prev_value < value && !maximum_value.compare_exchange_weak(prev_value, value)) {
        UL_NOOP;
    }
}
} // namespace mb::ul::thread::atomic

#endif
