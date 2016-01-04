// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/** Contains useful tools for debugging, like run-time breakpoints.
*/
//! \file


#ifndef DEBUG_MISC_H_INCL_n9283zr823rz832
#define DEBUG_MISC_H_INCL_n9283zr823rz832

#include "PPDEFS.h"


//##############################################################################################################
// Main debug-macro definition

#if TOO_MS_VISUAL_STUDIO_CPP
#ifdef _DEBUG
#define TOO_DEBUG	1
#else
#define TOO_DEBUG	0
#endif
#else
#ifdef NDEBUG
#define TOO_DEBUG	0
#else
#define TOO_DEBUG	1
#endif
#endif


//##############################################################################################################
//! Define a breakpoint macro for debugging.
/** Just call TOO_DEBUG_BREAK_IF(...); with some if-condition as parameter.*/
#if TOO_DEBUG
	#include <assert.h>
	#define TOO_ASSERT(_CONDITION_)					assert((_CONDITION_))
	#define TOO_VERIFY(_CONDITION_)					assert((_CONDITION_))
    #if TOO_OS_WINDOWS && TOO_MS_VISUAL_STUDIO_CPP
        #if TOO_OS_WINDOWS_64 // using portable common solution for x64 configuration
			#include <crtdbg.h>
			#define TOO_DEBUG_BREAK_IF(_CONDITION_)	if ((_CONDITION_)) { _CrtDbgBreak(); }
		#else
			#define TOO_DEBUG_BREAK_IF(_CONDITION_)	if ((_CONDITION_)) { _asm int 3 }
		#endif
	#else
		#define TOO_DEBUG_BREAK_IF(_CONDITION_)		TOO_ASSERT(!(_CONDITION_))
	#endif
	#define TOO_DEBUG_BREAK							TOO_DEBUG_BREAK_IF(true)
#else
	#define TOO_ASSERT(_CONDITION_)
	#define TOO_VERIFY(_CONDITION_)					sizeof((_CONDITION_))
	#define TOO_DEBUG_BREAK_IF(_CONDITION_)			/*sizeof((_CONDITION_)) <- don't want this; produces code in release version */
	#define TOO_DEBUG_BREAK
#endif

//! Function signature.
#if TOO_COMP_MS_VISUAL_STUDIO_CPP
	#define TOO_FUNCTIONSIGN __FUNCSIG__
#else
	#define TOO_FUNCTIONSIGN ""
#endif

//! Use something along the following as string literal: TOO_LOCATION"some message"
/** This would evaluate to "...somepath.../Toolib/debug/debug_misc.h (71) : some message". Useful e.g. as #pragma message (TOO_LOCATION"some message")*/
#define TOO_AUXDEF_CONCATENATE_DIRECT_WITH(x) #x
#define TOO_AUXDEF_CONCATENATE_INDIRECT_WITH(x) TOO_AUXDEF_CONCATENATE_DIRECT_WITH(x)
#define TOO_LOCATION __FILE__" ("TOO_AUXDEF_CONCATENATE_INDIRECT_WITH(__LINE__)") : "

////! User-defined compiler (warning) message.
//#if TOO_COMP_MS_VISUAL_STUDIO_CPP
//#define TOO_COMPILER_MSG(x) #pragma message(x) // doesn't work; only formal parameter
//#else
//#define TOO_COMPILER_MSG(x)
//#endif

#endif
