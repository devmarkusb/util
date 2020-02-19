// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef PIN_H_viehrxgzuh34782tg5
#define PIN_H_viehrxgzuh34782tg5

#include "toolib/assert.h"
#include "toolib/debug.h"
#include "toolib/error.h"
#include "toolib/ignore_arg.h"
#include "toolib/PPDEFS.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>

#if TOO_OS_UNIX
#include <pthread.h>
#else
// not implemented
#endif
#if TOO_OS_MAC
#include <mach/mach_types.h>
#include <mach/thread_act.h>
#include <sys/sysctl.h>
#endif

namespace too::thread
{
#if TOO_OS_MAC
namespace mac
{
const auto SYSCTL_CORE_COUNT = "machdep.cpu.core_count";

struct cpu_set_t
{
    uint32_t count;
};

inline void CPU_ZERO(cpu_set_t* cs)
{
    cs->count = 0;
}

inline void CPU_SET(int num, cpu_set_t* cs) { cs->count |= (1 << num); }

inline int CPU_ISSET(int num, cpu_set_t* cs) { return (cs->count & (1 << num)); }

int sched_getaffinity(pid_t, size_t, cpu_set_t *cpu_set)
{
    int32_t core_count = 0;
    size_t  len = sizeof(core_count);
    int ret = sysctlbyname(SYSCTL_CORE_COUNT, &core_count, &len, 0, 0);
    if (ret) {
        printf("error while get core count %d\n", ret);
        return -1;
    }
    cpu_set->count = 0;
    for (int i = 0; i < core_count; i++) {
        cpu_set->count |= (1 << i);
    }

    return 0;
}

int pthread_setaffinity_np(pthread_t thread, size_t cpu_size,
                           cpu_set_t *cpu_set)
{
    thread_port_t mach_thread;
    int core = 0;

    for (core = 0; core < 8 * cpu_size; core++) {
        if (CPU_ISSET(core, cpu_set)) break;
    }
    printf("binding to core %d\n", core);
    thread_affinity_policy_data_t policy = { core };
    mach_thread = pthread_mach_thread_np(thread);
    thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                      (thread_policy_t)&policy, 1);
    return 0;
}
} // mac
#endif

#if TOO_OS_UNIX
using native_handle = pthread_t;
#else
// not implemented
using native_handle = int;
#endif

/** Pins (sets affinity of) executing thread with native handle \param h to CPU number \param logicalCoreIdx (0-based).
    \Returns 0 on success and a certain error code otherwise (with errno set). Also cf. doc. of function overload.*/
inline int pinToLogicalCore(native_handle h, int logicalCoreIdx)
#if TOO_OS_LINUX || TOO_OS_MAC
    noexcept
#endif
{
    TOO_EXPECT(h);
    TOO_EXPECT(logicalCoreIdx >= 0);

#if TOO_OS_LINUX
    const auto nh = static_cast<pthread_t>(h);
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(logicalCoreIdx, &cpuset);
    return pthread_setaffinity_np(nh, sizeof(cpu_set_t), &cpuset);
#elif TOO_OS_MAC
    const auto nh = static_cast<pthread_t>(h);
    mac::cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(logicalCoreIdx, &cpuset);
    return mac::pthread_setaffinity_np(nh, sizeof(mac::cpu_set_t), &cpuset);
#else
    too::ignore_arg(h);
    too::ignore_arg(logicalCoreIdx);
    throw too::not_implemented{TOO_LOCATION" pinToLogicalCore not yet for non-Unix"};
#endif
}

/** Pins (sets affinity of) executing thread \param t to CPU number \param logicalCoreIdx (0-based).
    \Throws std::runtime_error on error. Note: an empty constructed std::thread won't do.
    Further note that in an example setup of 4 cores where 2 of them (which ones?) are only hyperthreading
    partners, you might not always get the performance gain you expect, by pinning to all cores. Half
    of them might be sufficient sometimes then.*/
inline void pinToLogicalCore(std::thread& t, int logicalCoreIdx)
{
    TOO_EXPECT(t.joinable()); // thread needs to be executing; you might have passed an empty constructed t
    TOO_EXPECT(logicalCoreIdx >= 0);

#if TOO_OS_LINUX || TOO_OS_MAC
    const auto nh = static_cast<native_handle>(t.native_handle());
    const auto err = pinToLogicalCore(nh, logicalCoreIdx);
    if (err)
    {
        std::stringstream ss;
        ss << "thread_pinToCPU failed with code " << err;
        throw std::runtime_error{ss.str()};
    }
#else
    too::ignore_arg(t);
    too::ignore_arg(logicalCoreIdx);
    throw too::not_implemented{TOO_LOCATION" pinToLogicalCore not yet for non-Unix"};
#endif
}

//! \Returns number of current, this_thread's, CPU (0-based) and -1 on error (with errno set).
inline int numLogicalCores()
#if TOO_OS_LINUX || TOO_OS_MAC
    noexcept
#endif
{
#if TOO_OS_LINUX
    return sched_getcpu();
#elif TOO_OS_MAC
    mac::cpu_set_t cpuset{};
    const auto ok = sched_getaffinity({}, {}, &cpuset);
    return ok <= 0 ? -1 : cpuset.count;
#else
    throw too::not_implemented{TOO_LOCATION" numLogicalCores not yet for non-Unix"};
#endif
}
} // too::thread
#endif
