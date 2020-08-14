// Markus Borris, 2013-17
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef TRACE_H_dfsgjn854gcnz782x5g7813sdyfwh
#define TRACE_H_dfsgjn854gcnz782x5g7813sdyfwh

#include "toolib/class/non_copyable.h"
#include "toolib/error.h"
#include "toolib/PPDEFS.h"
#include "toolib/std/std_extensions.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#if TOO_OS_WINDOWS
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif


namespace too
{
//! Purpose of too::trace is a quick&dirty substitute for logging. That is you can print to console window
//! and/or IDE output window. For GUI applications you can even open an console window and redirect cout and cerr
//! calls there. The initialization of the tracing is highly configurable, cf. too::trace::init.
/** Usage:
        #include "toolib/trace.h" // for the file you want to trace something.

        int main()
        {
            // first call init (and only once)
            too::tracer::init(); // cf. function doc. for configuration possiblities
            too::trace("ERROR") << "not so good, " << 42;
            too::trace() << "not so good, " << 42; // does the same, since "ERROR" is also default
            }
    It is recommended to use level strings "ERROR", "WARN", "INFO", "TRACE" in that order of decreasing severity.
    Note that you don't need to append '\n' to your trace line, it's done automatically.
    Also note, that "ERROR" is default and can be left out. If you don't want to show a level, pass an empty string.*/
struct trace;

namespace tracer
{
struct OutputToConsole
{
    static bool trace(const std::ostringstream& ss)
    {
        std::cerr << ss.str();
        return true;
    }

    static bool is_activated()
    {
        return true;
    }
};

struct NoOutputToConsole
{
    static bool trace(const std::ostringstream&)
    {
        return false;
    }

    static bool is_activated()
    {
        return false;
    }
};

struct OutputToIDEWindow
{
    template <class OutputToConsolePolicy>
    static void trace(const std::ostringstream& ss)
    {
#if !(TOO_OS_WINDOWS && TOO_COMP_MS_VISUAL_STUDIO_CPP)
        if (!OutputToConsolePolicy::is_activated())
#endif
#if TOO_OS_WINDOWS
            OutputDebugStringA(ss.str().c_str());
#else
        std::cerr << ss.str();
#endif
    }
};

struct NoOutputToIDEWindow
{
    template <class>
    static void trace(const std::ostringstream&)
    {
        // Note: e.g. for mingw we can't prevent output from showing up in IDE window
        TOO_NOOP;
    }
};

struct DontAlsoBindStdout
{
    static void bind(bool& ret_bound)
    {
        ret_bound = false;
    }
};

struct AlsoBindStdout
{
    static void bind(bool& ret_bound)
    {
#if TOO_OS_WINDOWS
        ret_bound = false;
        FILE* nsp{};
        const auto open_failed = freopen_s(&nsp, "CONOUT$", "w", stdout);
        if (!open_failed && nsp)
            ret_bound = true;
        else
            TOO_ASSERT(false);
#else
        ret_bound = true;
#endif
    }
};

struct CheckConsoleOpen
{
    template <class AlsoBindStdoutToNewConsolePolicy = DontAlsoBindStdout>
    static void openConsoleIfNecessary(bool& ret_stderrBound, bool& ret_stdoutBound)
    {
#if TOO_OS_WINDOWS
        ret_stderrBound = false;
        ret_stdoutBound = false;

        const auto alloc_ok = AllocConsole();
        if (!alloc_ok)
        {
            TOO_ASSERT(false); // you don't need to try to open a new console, you already have one
            return;
        }

        FILE* nsp{};
        const auto open_failed = freopen_s(&nsp, "CONOUT$", "w", stderr);
        if (!open_failed && nsp)
            ret_stderrBound = true;
        else
            TOO_ASSERT(false);

        AlsoBindStdoutToNewConsolePolicy::bind(ret_stdoutBound);
#else
        ret_stderrBound = true;
        ret_stdoutBound = true;
#endif
    }
};

struct DontCheckConsoleOpen
{
    template <class>
    static void openConsoleIfNecessary(bool&, bool&)
    {
        TOO_NOOP;
    }
};

struct Enabled
{
    template <class OutputToIDEWindowPolicy, class OutputToConsolePolicy>
    static void trace(const std::ostringstream& ss)
    {
        OutputToIDEWindowPolicy::template trace<OutputToConsolePolicy>(ss);
        OutputToConsolePolicy::trace(ss);
    }

    static const bool isEnabled{true};
};

struct Disabled
{
    template <class, class>
    static void trace(const std::ostringstream&)
    {
        TOO_NOOP;
    }

    static const bool isEnabled{false};
};

namespace detail_impl
{
struct StreamTracer
{
    virtual ~StreamTracer()
    {
    }
    virtual void trace(const std::ostringstream&) const = 0;
};

template <class T>
inline StreamTracer& operator<<(StreamTracer& t, const T& x)
{
    std::ostringstream ss;
    ss << x;
    t.trace(ss);
    return t;
}

struct StreamTraceEndOfLine
{
};

template <>
inline StreamTracer& operator<<(StreamTracer& t, const StreamTraceEndOfLine&)
{
    std::ostringstream ss;
    ss << '\n';
    t.trace(ss);
    return t;
}

template <class EnabledIfPolicy, class OutputToIDEWindowPolicy = OutputToIDEWindow,
    class OutputToConsolePolicy = NoOutputToConsole>
struct StreamTracer_impl : public StreamTracer
{
    StreamTracer_impl(bool close_stderr, bool close_stdout)
        : close_stderr{close_stderr}
        , close_stdout{close_stdout}
    {
    }

    ~StreamTracer_impl() override
    {
        if (close_stderr)
            fclose(stderr);
        if (close_stdout)
            fclose(stdout);
    }

    void trace(const std::ostringstream& ss) const override
    {
        EnabledIfPolicy::template trace<OutputToIDEWindowPolicy, OutputToConsolePolicy>(ss);
    }

private:
    bool close_stderr{};
    bool close_stdout{};
};

struct StreamTracerWrapperSingleton
{
    static StreamTracerWrapperSingleton& getInstance()
    {
        static StreamTracerWrapperSingleton inst;
        return inst;
    }

    std::unique_ptr<StreamTracer>& tracer()
    {
        return t;
    }

private:
    std::unique_ptr<StreamTracer> t;

    StreamTracerWrapperSingleton() = default;
};

inline StreamTracer& stream()
{
    auto& ret = StreamTracerWrapperSingleton::getInstance().tracer();
    // if you crash shortly after, you probably forgot to call too::tracer::init before your first trace
    TOO_EXPECT(ret);
    return *ret;
}
} // namespace detail_impl

//! This needs to be called before any call to too::trace and only once.
//! That is yopu can fix your configuration only once and for all for the whole program.
/** Configuration options in detail:
        EnabledIfPolicy: switches tracing on/off entirely, e.g. useful to be bound to a
                            macro define specifying the case of a release or deployment build
        OutputToIDEWindowPolicy: choose whether you want to trace to your IDE output window;
                            note that you can't switch off those traces in some environments if
                            you switched on console traces at the same time
                            (mingw+QtCreator is an example);
                            also note that the implementation tries to take care of avoiding
                            duplicated output in certain environments, when output to console
                            *and* output to IDE window are switched on
        OutputToConsolePolicy: choose whether to trace to a console window or not
        CheckConsoleOpenPolicy: a check+open only makes sense for non-console GUI application;
                            there you can open an additional console window to see traces if
                            switched on
        AlsoBindStdoutToNewConsolePolicy: only makes sense if you checked+opened a new console
                            window by the previous policy; then youn have the option to not only
                            bind stderr (cerr) - which is done by default and used for traces
                            to the console - but also stdout (cout) to the console window output.*/
template <class EnabledIfPolicy = Enabled, class OutputToIDEWindowPolicy = OutputToIDEWindow,
    class OutputToConsolePolicy = NoOutputToConsole, class CheckConsoleOpenPolicy = DontCheckConsoleOpen,
    class AlsoBindStdoutToNewConsolePolicy = DontAlsoBindStdout>
// EnabledIfPolicy expected to be Enabled or Disabled
// OutputToIDEWindowPolicy expected to be OutputToIDEWindow or NoOutputToIDEWindow
// OutputToConsolePolicy expected to be OutputToConsole or NoOutputToConsole
// CheckConsoleOpenPolicy expected to be CheckConsoleOpen or DontCheckConsoleOpen
// AlsoBindStdoutToNewConsolePolicy expected to be AlsoBindStdout or DontAlsoBindStdout
inline void init()
{
    if (detail_impl::StreamTracerWrapperSingleton::getInstance().tracer())
        throw std::runtime_error{"trace init called more often than once"};

    bool ret_stderrBound{};
    bool ret_stdoutBound{};
    if (EnabledIfPolicy::isEnabled)
        CheckConsoleOpenPolicy::template openConsoleIfNecessary<AlsoBindStdoutToNewConsolePolicy>(
            ret_stderrBound, ret_stdoutBound);

    detail_impl::StreamTracerWrapperSingleton::getInstance().tracer() = too::make_unique<
        detail_impl::StreamTracer_impl<EnabledIfPolicy, OutputToIDEWindowPolicy, OutputToConsolePolicy>>(
        ret_stderrBound, ret_stdoutBound);
}
} // namespace tracer

struct trace : private too::non_copyable
{
    explicit trace(const std::string& level = "ERROR")
        : stream_{&too::tracer::detail_impl::stream()}
    {
        std::ostringstream tmp;
        tmp << std::left << std::setw(level.empty() ? 0 : 6) << level;
        *this->stream_ << tmp.str();
    }

    ~trace()
    {
        *this->stream_ << tracer::detail_impl::StreamTraceEndOfLine{};
    }

    tracer::detail_impl::StreamTracer& stream() const
    {
        return *this->stream_;
    }

private:
    mutable tracer::detail_impl::StreamTracer* stream_;
};

template <typename T>
const trace& operator<<(const trace& t, T&& arg)
{
    t.stream() << std::forward<T>(arg);
    return t;
}


namespace deprecated
{
#if TOO_OS_WINDOWS
//! Usage: std::ostringstream os; os << "bla" << 2 << "blabla"; trace(os);
//! Only supported for Windows so far.
/** Impl. notes: Also tried std::cerr and std::clog under Windows - without success.*/
inline void trace(const std::ostringstream& os)
{
    OutputDebugStringA(os.str().c_str());
}
inline void trace(const std::wostringstream& os)
{
    OutputDebugStringW(os.str().c_str());
}
#else
inline void trace(const std::ostringstream&)
{
}
#endif
} // namespace deprecated
} // namespace too

#endif
