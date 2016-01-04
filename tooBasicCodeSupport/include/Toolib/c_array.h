// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef C_ARRAY_H_INCL_lnm45hxn73rmnfy2738yrn873rg
#define C_ARRAY_H_INCL_lnm45hxn73rmnfy2738yrn873rg

#include "debug.h"

namespace too
{

	template<typename T> inline size_t ARRAY_LENGTH(T t)
	{
		size_t divisor = sizeof(t[0]);
		TOO_ASSERT(divisor);
		return sizeof(t) / divisor;
	}

}

#endif
