//! \file

#ifndef PIN_H_viehrxgzuh34782tg5
#define PIN_H_viehrxgzuh34782tg5

#include "../assert.h"
#include "../debug.h"
#include "../error.h"
#include "../ignore_unused.h"

#include "ul/macros.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>

#if UL_OS_UNIX
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#else
// not implemented
#endif
#if UL_OS_MAC
#include <mach/mach_types.h>
#include <mach/thread_act.h>
#include <sys/sysctl.h>
#endif

namespace mb::ul::thread {
#if UL_OS_MAC
namespace mac {
const auto SYSCTL_CORE_COUNT = "machdep.cpu.core_count";

struct cpu_set_t {
    uint32_t count;
};

inline void CPU_ZERO(cpu_set_t* cs) {
    cs->count = 0;
}

inline void CPU_SET(int num, cpu_set_t* cs) {
    cs->count |= (1 << num);
}

inline int CPU_ISSET(int num, cpu_set_t* cs) {
    return (cs->count & (1 << num));
}

int sched_getaffinity(pid_t, size_t, cpu_set_t* cpu_set) {
    int32_t core_count = 0;
    size_t len = sizeof(core_count);
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

int pthread_setaffinity_np(pthread_t thread, size_t cpu_size, cpu_set_t* cpu_set) {
    thread_port_t mach_thread;
    int core = 0;

    for (core = 0; core < 8 * cpu_size; core++) {
        if (CPU_ISSET(core, cpu_set))
            break;
    }
    printf("binding to core %d\n", core);
    thread_affinity_policy_data_t policy = {core};
    mach_thread = pthread_mach_thread_np(thread);
    thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, 1);
    return 0;
}
} // namespace mac
#endif

#if UL_OS_UNIX
using native_handle = pthread_t;
#else
// not implemented
using native_handle = int;
#endif

/** Pins (sets affinity of) executing thread with native handle h to CPU number logicalCoreIdx (0-based).
    Also cf. doc. of function overload.
    \return 0 on success and a certain error code otherwise (with errno set).*/
inline int pinToLogicalCore([[maybe_unused]] native_handle h, [[maybe_unused]] int logicalCoreIdx)
#if UL_OS_MAC
    noexcept
#endif
{
    UL_EXPECT(h);
    UL_EXPECT(logicalCoreIdx >= 0);

#if UL_OS_LINUX
    throw ul::not_implemented{UL_LOCATION " pinToLogicalCore for arbitrary handle not yet for Linux"};
#elif UL_OS_MAC
    const auto nh = static_cast<pthread_t>(h);
    mac::cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(logicalCoreIdx, &cpuset);
    return mac::pthread_setaffinity_np(nh, sizeof(mac::cpu_set_t), &cpuset);
#else
    ul::ignore_unused(h);
    ul::ignore_unused(logicalCoreIdx);
    throw ul::not_implemented{UL_LOCATION " pinToLogicalCore not yet for non-Unix"};
#endif
}

//! Like pinToLogicalCore but current thread only.
inline int pinToLogicalCore(int logicalCoreIdx) {
#if UL_OS_LINUX
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(logicalCoreIdx, &cpuset);
    return sched_setaffinity(gettid(), sizeof(cpu_set_t), &cpuset);
#else
    ul::ignore_unused(logicalCoreIdx);
    throw ul::not_implemented{UL_LOCATION " pinToLogicalCore"};
#endif
}

/** Pins (sets affinity of) executing thread t to CPU number logicalCoreIdx (0-based).
    Further note that in an example setup of 4 cores where 2 of them (which ones?) are only hyperthreading
    partners, you might not always get the performance gain you expect, by pinning to all cores. Half
    of them might be sufficient sometimes then.
    \throws std::runtime_error on error. Note: an empty constructed std::thread won't do.*/
inline void pinToLogicalCore(std::thread& t, int logicalCoreIdx) {
    UL_EXPECT(t.joinable()); // thread needs to be executing; you might have passed an empty constructed t
    UL_EXPECT(logicalCoreIdx >= 0);

#if UL_OS_LINUX || UL_OS_MAC
    const auto nh = static_cast<native_handle>(t.native_handle());
    const auto err = pinToLogicalCore(nh, logicalCoreIdx);
    if (err) {
        std::stringstream ss;
        ss << "thread_pinToCPU failed with code " << err;
        throw std::runtime_error{ss.str()};
    }
#else
    ul::ignore_unused(t);
    ul::ignore_unused(logicalCoreIdx);
    throw ul::not_implemented{UL_LOCATION " pinToLogicalCore not yet for non-Unix"};
#endif
}

//! \return number of current, this_thread's, CPU (0-based) and -1 on error (with errno set).
inline int numLogicalCores()
#if UL_OS_LINUX || UL_OS_MAC
    noexcept
#endif
{
#if UL_OS_LINUX
    return sched_getcpu();
#elif UL_OS_MAC
    mac::cpu_set_t cpuset{};
    const auto ok = sched_getaffinity({}, {}, &cpuset);
    return ok <= 0 ? -1 : cpuset.count;
#else
    throw ul::not_implemented{UL_LOCATION " numLogicalCores not yet for non-Unix"};
#endif
}
} // namespace mb::ul::thread

#endif
