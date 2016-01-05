// Markus Borris, 2013
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef TRACE_H_INCL_dfsgjn854gcnz782x5g7813sdyfwh
#define TRACE_H_INCL_dfsgjn854gcnz782x5g7813sdyfwh

#include "Toolib/PPDEFS.h"
#include <sstream>
#ifdef TOO_OS_WINDOWS
#include <windows.h>
#endif TOO_OS_WINDOWS

namespace too
{
#ifdef TOO_OS_WINDOWS
	//! Usage: trace(makestr() << "bla" << 2 << "blabla");
	void trace(const std::ostringstream& os)
	{
		OutputDebugStringA(os.str().c_str());
	}

	void trace(const std::wostringstream& os)
	{
		OutputDebugStringW(os.str().c_str());
	}
#endif TOO_OS_WINDOWS
}

#endif
