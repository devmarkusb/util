// 2019

//!
/**
 */
//! \file

#ifndef ATOMIC_H_e7tzx3785tz78g2xh4h2gg
#define ATOMIC_H_e7tzx3785tz78g2xh4h2gg

#include <atomic>

#include "ul/macros.h"


namespace mb::ul::thread::atomic
{
template <typename T>
void updateMaximum(std::atomic<T>& maximum_value, const T& value) noexcept
{
    T prev_value = maximum_value;
    while (prev_value < value && !maximum_value.compare_exchange_weak(prev_value, value))
    {
        UL_NOOP;
    }
}
} // namespace mb::ul::thread::atomic

#include "ul/macros_end.h"

#endif
