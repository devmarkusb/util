// 2019

//! \file

#ifndef PHYSMEMUSAGE_RSS_H_fuigh76x3nfg3
#define PHYSMEMUSAGE_RSS_H_fuigh76x3nfg3

#include "types.h"
#include "ul/debug.h"
#include "ul/error.h"
#include "ul/ignore_unused.h"
#include "ul/macros.h"
#include <fstream>
#include <string>

#if UL_OS_LINUX
#include <unistd.h>
#endif


namespace mb::ul::mem
{
/** Returns virtual memory usage and physical memory usage (more precisely RSS, resident set size )
    via virtual_mem_usage and physical_mem_usage.*/
inline void usage(double& virtual_mem_usage, double& physical_mem_usage)
{
#if UL_OS_LINUX
    virtual_mem_usage = {};
    physical_mem_usage = {};

    std::ifstream stat_stream("/proc/self/stat", std::ios_base::in);

    struct UL_DUMMY
    {
        std::string pid, comm, state, ppid, pgrp, session, tty_nr;
        std::string tpgid, flags, minflt, cminflt, majflt, cmajflt;
        std::string utime, stime, cutime, cstime, priority, nice;
        std::string O, itrealvalue, starttime;
    } dummy;

    double vsize{};
    double rss{};

    stat_stream >> dummy.pid >> dummy.comm >> dummy.state >> dummy.ppid >> dummy.pgrp >> dummy.session >> dummy.tty_nr
        >> dummy.tpgid >> dummy.flags >> dummy.minflt >> dummy.cminflt >> dummy.majflt >> dummy.cmajflt >> dummy.utime
        >> dummy.stime >> dummy.cutime >> dummy.cstime >> dummy.priority >> dummy.nice >> dummy.O >> dummy.itrealvalue
        >> dummy.starttime >> vsize >> rss;

    double page_size_kb = static_cast<double>(sysconf(_SC_PAGE_SIZE)) / ul::mem::KiB;
    virtual_mem_usage = vsize / ul::mem::KiB;
    physical_mem_usage = rss * page_size_kb;
#else
    ul::ignore_unused(virtual_mem_usage);
    ul::ignore_unused(physical_mem_usage);
    throw ul::not_implemented{UL_LOCATION " ul::mem::usage not yet for non-Linux"};
#endif
}
} // namespace mb::ul::mem

#endif
