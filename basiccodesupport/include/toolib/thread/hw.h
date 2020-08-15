// Markus Borris, 2019
// This file is part of tfl library.

//! Hardware diagnostics.
/**
 */
//! \file

#ifndef HW_H_gi4uhx548hnx4875h7xg785h
#define HW_H_gi4uhx548hnx4875h7xg785h

#include <thread>

#include "toolib/macros.h"

#if TOO_OS_LINUX
#include <unistd.h>
#endif


namespace too::thread
{
//! \return number of available logical cores and 0 on error.
inline unsigned int numAvailLogicalCores() noexcept
{
#if TOO_OS_LINUX // for safety, not changing existing code (yet)
    const auto nproc = sysconf(_SC_NPROCESSORS_ONLN);
    const auto ret = static_cast<unsigned int>(nproc == -1 ? 0u : nproc);
    return ret;
#else
    return std::thread::hardware_concurrency();
#endif
}
} // namespace too::thread

#include "toolib/macros_end.h"

#endif
