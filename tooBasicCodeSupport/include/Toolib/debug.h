// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/** Contains useful tools for debugging, like run-time breakpoints.
*/
//! \file


#ifndef DEBUG_MISC_H_INCL_n9283zr823rz832
#define DEBUG_MISC_H_INCL_n9283zr823rz832

#include "Toolib/PPDEFS.h"
#include "Toolib/assert.h"


//##############################################################################################################
//! Define a breakpoint macro for debugging.
/** Just call TOO_DEBUG_BREAK_IF(...); with some if-condition as parameter.*/
#if TOO_DEBUG
	#if TOO_OS_WINDOWS && TOO_COMP_MS_VISUAL_STUDIO_CPP
        #if TOO_OS_WINDOWS_64 // using portable common solution for x64 configuration
			#include <crtdbg.h>
			#define TOO_DEBUG_BREAK_IF(_CONDITION_)		do { if ((_CONDITION_)) { _CrtDbgBreak(); } } while(false)
		#else
			#define TOO_DEBUG_BREAK_IF(_CONDITION_)		do { if ((_CONDITION_)) { __asm int 3 } } while(false)
		#endif
	#else
		#define TOO_DEBUG_BREAK_IF(_CONDITION_)			do { TOO_ASSERT(!(_CONDITION_)); } while(false)
	#endif
		#define TOO_DEBUG_BREAK							do { TOO_DEBUG_BREAK_IF(true); } while(false)
#else
	#define TOO_DEBUG_BREAK_IF(_CONDITION_)				/*sizeof((_CONDITION_)) <- don't want this; produces code in release version */
	#define TOO_DEBUG_BREAK
#endif

//! Function signature.
#if TOO_COMP_MS_VISUAL_STUDIO_CPP
	#define TOO_FUNCTIONSIGN __FUNCSIG__
#elif TOO_COMP_GNU_CPP || TOO_COMP_MINGW
    #define TOO_FUNCTIONSIGN __PRETTY_FUNCTION__
#elif TOO_COMP_INTEL
    #define TOO_FUNCTIONSIGN __FUNCTION__
#elif TOO_COMP_BORLAND_CPP
    #define TOO_FUNCTIONSIGN __FUNC__
#else // for C99, or hopefully compilers including that
    #define TOO_FUNCTIONSIGN __func__
#endif

//! Use something along the following as string literal: TOO_LOCATION"some message"
/** This would evaluate to "...somepath.../Toolib/debug/debug_misc.h (71) : some message". Useful e.g. as #pragma message (TOO_LOCATION"some message")*/
#define TOO_LOCATION __FILE__ " (" TOO_STRINGIFY(__LINE__) ") : "

#endif
