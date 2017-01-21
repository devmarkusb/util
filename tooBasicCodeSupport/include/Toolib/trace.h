// Markus Borris, 2013-17
// This file is part of Toolib library.

//!
/**
*/
//! \file

#pragma once
#ifndef TRACE_H_INCL_dfsgjn854gcnz782x5g7813sdyfwh
#define TRACE_H_INCL_dfsgjn854gcnz782x5g7813sdyfwh

#include "Toolib/class/non_copyable.h"
#include "Toolib/PPDEFS.h"
#include "Toolib/std/std_extensions.h"
#include <memory>
#include <sstream>
#if TOO_OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif


namespace too
{
namespace trace
{
struct Enabled
{
};
struct Disabled
{
};
struct OutputToIDEWindow
{
};
struct NoOutputToIDEWindow
{
};
struct OutputToConsole
{
};
struct NoOutputToConsole
{
};
struct CheckConsoleOpen
{
};
struct DontCheckConsoleOpen
{
};

struct Tracer
{
    virtual ~Tracer() {}
    virtual void trace(const std::ostringstream&) const = 0;
};


template <class T>
inline Tracer& operator<<(Tracer& t, const T& x)
{
    std::ostringstream ss;
    ss << x;
    t.trace(ss);
    return t;
}

struct End
{
};

template <>
inline Tracer& operator<<(Tracer& t, const End&)
{
    std::ostringstream ss;
    ss << '\n';
    t.trace(ss);
    return t;
}

template <class EnabledIfPolicy, class OutputToIDEWindowPolicy = OutputToIDEWindow,
    class OutputToConsolePolicy = NoOutputToConsole, class CheckConsoleOpenPolicy = DontCheckConsoleOpen>
struct Tracer_impl : public Tracer
{
    virtual void trace(const std::ostringstream& ss) const override { std::cerr << ss.str(); }

private:
    std::ostringstream m_buffer;
};


template <class EnabledIfPolicy, class OutputToIDEWindowPolicy = OutputToIDEWindow,
    class OutputToConsolePolicy = NoOutputToConsole, class CheckConsoleOpenPolicy = DontCheckConsoleOpen>
inline void init()
{
    if (TracerWrapperSingleton::getInstance().tracer())
        throw std::runtime_error{"trace init called more often than once"};
    TracerWrapperSingleton::getInstance().tracer() = too::make_unique<
        Tracer_impl<EnabledIfPolicy, OutputToIDEWindowPolicy, OutputToConsolePolicy, CheckConsoleOpenPolicy>>();
};

struct TracerWrapperSingleton
{
    static TracerWrapperSingleton& getInstance()
    {
        static TracerWrapperSingleton inst;
        return inst;
    }

    std::unique_ptr<Tracer>& tracer() { return t; }

private:
    std::unique_ptr<Tracer> t;

    TracerWrapperSingleton() = default;
};

inline Tracer& stream()
{
    // if you crash here, you probably forgot to call too::trace::init before your first trace
    return *TracerWrapperSingleton::getInstance().tracer();
}

struct AutoEnd : private too::non_copyable
{
private:
    mutable Tracer& m_obj;

public:
    explicit AutoEnd(Tracer& obj) : m_obj(obj) {}
    ~AutoEnd() { m_obj << End{}; }

    Tracer& obj() const { return m_obj; }
};

template <typename T>
const AutoEnd& operator<<(const AutoEnd& ae, T&& arg)
{
    ae.obj() << std::forward<T>(arg);
    return ae;
}

#define TOO_TRACE /*(level)*/ too::trace::AutoEnd(too::trace::stream(/*TOO_STRINGIFY(level)*/))
} // trace


namespace deprecated
{
#if TOO_OS_WINDOWS
//! Usage: std::ostringstream os; os << "bla" << 2 << "blabla"; trace(os);
//! Only supported for Windows so far.
/** Impl. notes: Also tried std::cerr and std::clog under Windows - without success.*/
inline void trace(const std::ostringstream& os) { OutputDebugStringA(os.str().c_str()); }
inline void trace(const std::wostringstream& os) { OutputDebugStringW(os.str().c_str()); }
#else
inline void trace(const std::ostringstream&) {}
#endif
} // deprecated
} // too

#endif
