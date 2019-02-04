// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef PIN_H_viehrxgzuh34782tg5
#define PIN_H_viehrxgzuh34782tg5

#include "toolib/assert.h"
#include "toolib/PPDEFS.h"
#include <sstream>
#include <stdexcept>

#if TOO_OS_LINUX
#include <pthread.h>
#else
#error "not implemented"
#endif


namespace too::thread
{
#if TOO_OS_LINUX
using native_handle = pthread_t;
#else
#error "not implemented"
#endif

/** Pins (sets affinity of) executing thread with native handle \param h to CPU number \param logicalCoreIdx (0-based).
    \Returns 0 on success and a certain error code otherwise (with errno set). Also cf. doc. of function overload.*/
inline int pinToLogicalCore(native_handle h, int logicalCoreIdx) noexcept
{
    TOO_EXPECT(h);
    TOO_EXPECT(logicalCoreIdx >= 0);

#if TOO_OS_LINUX
    const auto nh = static_cast<pthread_t>(h);
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(logicalCoreIdx, &cpuset);
    return pthread_setaffinity_np(nh, sizeof(cpu_set_t), &cpuset);
#else
#error "not implemented"
    return -1;
#endif
}

/** Pins (sets affinity of) executing thread \param t to CPU number \param logicalCoreIdx (0-based).
    \Throws std::runtime_error on error. Note: an empty constructed std::thread won't do.
    Further note that in an example setup of 4 cores where 2 of them (which ones?) are only hyperthreading
    partners, you might not always get the performance gain you expect, by pinning to all cores. Half
    of them might be sufficient sometimes then.*/
inline void pinToLogicalCore(std::thread &t, int logicalCoreIdx)
{
    TOO_EXPECT(t.joinable()); // thread needs to be executing; you might have passed an empty constructed t
    TOO_EXPECT(logicalCoreIdx >= 0);

    const auto nh = static_cast<native_handle>(t.native_handle());
    const auto err = pinToLogicalCore(nh, logicalCoreIdx);
    if (err)
    {
        std::stringstream ss;
        ss << "thread_pinToCPU failed with code " << err;
        throw std::runtime_error{ss.str()};
    }
}

//! \Returns number of current, this_thread's, CPU (0-based) and -1 on error (with errno set).
inline int numLogicalCores() noexcept
{
#if TOO_OS_LINUX
    return sched_getcpu();
#else
#error "not implemented"
    return -1;
#endif
}
} // too::thread
#endif
