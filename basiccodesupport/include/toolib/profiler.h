// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
*/
//! \file

#ifndef PROFILER_H_958x2g58sdjkfh23789hgxt34
#define PROFILER_H_958x2g58sdjkfh23789hgxt34

#include <chrono>
#include <cstdint>


namespace too
{
using ProfilerTimePoint = int64_t;

inline ProfilerTimePoint profiler_now()
{
    const auto now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
    return now.time_since_epoch().count();
}

inline double profiler_diff_s(ProfilerTimePoint start, ProfilerTimePoint end)
{
    return static_cast<double>(end - start) / 1e9;
}
} // too

#endif