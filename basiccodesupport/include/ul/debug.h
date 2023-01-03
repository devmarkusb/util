// 2011-19

//! \file Contains useful tools for debugging, like run-time breakpoints.

#ifndef DEBUG_MISC_H_n9283zr823rz832
#define DEBUG_MISC_H_n9283zr823rz832

#include "ul/assert.h"
#include "ul/macros.h"


//####################################################################################################################
//! Define a breakpoint macro for debugging.
/** Just call UL_DEBUG_BREAK_IF(...); with some if-condition as parameter.*/
#if UL_DEBUG
#if UL_OS_WINDOWS && UL_COMP_MS_VISUAL_STUDIO_CPP
#if UL_OS_WINDOWS_64 // using portable common solution for x64 configuration
#include <crtdbg.h>
#define UL_DEBUG_BREAK_IF(_CONDITION_) \
    do \
    { \
        if ((_CONDITION_)) \
        { \
            _CrtDbgBreak(); \
        } \
    } while (false)
#else
#define UL_DEBUG_BREAK_IF(_CONDITION_) \
    do \
    { \
        if ((_CONDITION_)) \
        { \
            __asm int 3 \
        } \
    } while (false)
#endif
#else
#define UL_DEBUG_BREAK_IF(_CONDITION_) \
    do \
    { \
        UL_ASSERT(!(_CONDITION_)); \
    } while (false)
#endif
#define UL_DEBUG_BREAK \
    do \
    { \
        UL_DEBUG_BREAK_IF(true); \
    } while (false)
#else
// no sizeof((_CONDITION_)), as this would produce code in release build
#define UL_DEBUG_BREAK_IF(_CONDITION_)
#define UL_DEBUG_BREAK
#endif

//! Function signature.
#if UL_COMP_MS_VISUAL_STUDIO_CPP
#define UL_FUNCTIONSIGN __FUNCSIG__
#elif UL_COMP_GNU_CPP || UL_COMP_MINGW || UL_COMP_CLANG
#define UL_FUNCTIONSIGN __PRETTY_FUNCTION__
#elif UL_COMP_INTEL
#define UL_FUNCTIONSIGN __FUNCTION__
#elif UL_COMP_BORLAND_CPP
#define UL_FUNCTIONSIGN __FUNC__
#else // for C99, or hopefully compilers including that
#define UL_FUNCTIONSIGN __func__
#endif

//! Use something along the following as string literal: UL_LOCATION" some message"
/** This would evaluate to "...somepath.../ul/debug/debug_misc.h (71) some message". Useful e.g. as #pragma
 * message (UL_LOCATION"some message")*/
#define UL_LOCATION __FILE__ " (" UL_STRINGIFY_VALUE(__LINE__) ")"

#endif
