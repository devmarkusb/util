// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef ATOMIC_H_e7tzx3785tz78g2xh4h2gg
#define ATOMIC_H_e7tzx3785tz78g2xh4h2gg

#include "toolib/PPDEFS.h"
#include <atomic>


namespace too::thread::atomic
{
template <typename T>
void updateMaximum(std::atomic<T>& maximum_value, const T& value) noexcept
{
    T prev_value = maximum_value;
    while (prev_value < value &&
           !maximum_value.compare_exchange_weak(prev_value, value))
    {
        TOO_NOOP;
    }
}
} // too::thread::atomic

#endif
