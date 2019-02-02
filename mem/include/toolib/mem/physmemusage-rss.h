// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef PHYSMEMUSAGE_RSS_H_fuigh76x3nfg3
#define PHYSMEMUSAGE_RSS_H_fuigh76x3nfg3

#include "types.h"
#include "toolib/ignore_arg.h"
#include "toolib/PPDEFS.h"
#include <fstream>
#include <string>

#if TOO_OS_LINUX
#include <unistd.h>
#endif

namespace too::mem
{
/** Returns virtual memory usage and physical memory usage (more precisely RSS, resident set size )
    via \param virtual_mem_usage and \param physical_mem_usage.*/
void usage(double& virtual_mem_usage, double& physical_mem_usage)
{
#if TOO_OS_LINUX
    virtual_mem_usage  = {};
    physical_mem_usage = {};

    std::ifstream stat_stream("/proc/self/stat", std::ios_base::in);

    struct TOO_DUMMY
    {
        std::string pid, comm, state, ppid, pgrp, session, tty_nr;
        std::string tpgid, flags, minflt, cminflt, majflt, cmajflt;
        std::string utime, stime, cutime, cstime, priority, nice;
        std::string O, itrealvalue, starttime;
    } dummy;

    unsigned long vsize{};
    long rss{};

    stat_stream >> dummy.pid >> dummy.comm >> dummy.state >> dummy.ppid >> dummy.pgrp >> dummy.session >>
        dummy.tty_nr >> dummy.tpgid >> dummy.flags >> dummy.minflt >> dummy.cminflt >> dummy.majflt >> dummy.cmajflt >>
        dummy.utime >> dummy.stime >> dummy.cutime >> dummy.cstime >> dummy.priority >> dummy.nice >> dummy.O >>
        dummy.itrealvalue >> dummy.starttime >> vsize >> rss;

    long page_size_kb  = sysconf(_SC_PAGE_SIZE) / too::mem::KiB;
    virtual_mem_usage  = vsize / static_cast<double>(too::mem::KiB);
    physical_mem_usage = rss * page_size_kb;
#else
    too::ignore_arg()
#error "not implemented"
#endif
}
} // too::mem
#endif
