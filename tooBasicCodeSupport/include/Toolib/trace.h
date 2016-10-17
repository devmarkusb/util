// Markus Borris, 2013
// This file is part of Toolib library.

//!
/**
*/
//! \file

#pragma once
#ifndef TRACE_H_INCL_dfsgjn854gcnz782x5g7813sdyfwh
#define TRACE_H_INCL_dfsgjn854gcnz782x5g7813sdyfwh

#include "Toolib/PPDEFS.h"
#include <sstream>
#if TOO_OS_WINDOWS
#include <windows.h>
#endif


namespace too
{
// todo other platforms
#if TOO_OS_WINDOWS
//! Usage: std::ostringstream os; os << "bla" << 2 << "blabla"; trace(os);
//! Only supported for Windows so far.
inline void trace(const std::ostringstream& os) { OutputDebugStringA(os.str().c_str()); }
inline void trace(const std::wostringstream& os) { OutputDebugStringW(os.str().c_str()); }
#else
inline void trace(const std::ostringstream&) {}
#endif
}

#endif
