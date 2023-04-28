//! \file

#ifndef ATOMIC_H_e7tzx3785tz78g2xh4h2gg
#define ATOMIC_H_e7tzx3785tz78g2xh4h2gg

#include "ul/macros.h"
#include <atomic>

namespace mb::ul::thread::atomic {
template <typename T>
void updateMaximum(std::atomic<T>& maximum_value, const T& value) noexcept {
    T prev_value = maximum_value;
    while (prev_value < value && !maximum_value.compare_exchange_weak(prev_value, value)) {
        UL_NOOP;
    }
}
} // namespace mb::ul::thread::atomic

#endif
