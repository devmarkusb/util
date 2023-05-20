//! \file Hardware diagnostics.

#ifndef HW_H_GI4UHX548HNX4875H7XG785H
#define HW_H_GI4UHX548HNX4875H7XG785H

#include "ul/macros.h"
#include <thread>

#if UL_OS_LINUX
#include <unistd.h>
#endif


namespace mb::ul::thread {
//! \return number of available logical cores and 0 on error.
inline unsigned int num_avail_logical_cores() noexcept {
    return std::thread::hardware_concurrency();
}
} // namespace mb::ul::thread

#endif
