//! \file Hardware diagnostics.

#ifndef HW_H_gi4uhx548hnx4875h7xg785h
#define HW_H_gi4uhx548hnx4875h7xg785h

#include "ul/macros.h"
#include <thread>

#if UL_OS_LINUX
#include <unistd.h>
#endif


namespace mb::ul::thread
{
//! \return number of available logical cores and 0 on error.
inline unsigned int numAvailLogicalCores() noexcept
{
    return std::thread::hardware_concurrency();
}
} // namespace mb::ul::thread

#endif
