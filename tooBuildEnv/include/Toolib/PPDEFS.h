// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/** Most common and basic preprocessor defines.
*/
//! \file

#ifndef PPDEFS_H_INCL_o85nzumvt424cmt75tn
#define PPDEFS_H_INCL_o85nzumvt424cmt75tn


//##############################################################################################################
// Operating Systems

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__TOS_WIN__)
#define TOO_OS_WINDOWS		1
#if defined(_WIN64)
#define TOO_OS_WINDOWS_64	1
#endif
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#define TOO_OS_UNIX         1
#endif

#if defined(linux) || defined(__linux)
#define TOO_OS_LINUX		1
#endif

#if defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
#define TOO_OS_MAC			1
#endif

#if defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
#define TOO_OS_MSDOS		1
#endif

#if defined(_WIN32_WCE)
#define TOO_OS_WINDOWS_CE	1
#endif

#if defined(__FreeBSD__)
#define TOO_OS_FREEBSD      1
#endif

#if ((TOO_OS_LINUX || TOO_OS_MAC || TOO_OS_FREEBSD) && (!TOO_OS_WINDOWS))
#define TOO_OS_UNIX         1
#endif

#if defined(__ANDROID__)
#define TOO_OS_ANDROID      1
#endif


//##############################################################################################################
// Compilers

#if defined(_MSC_VER)
#define TOO_COMP_MS_VISUAL_STUDIO_CPP   1
#define TOO_COMP_MS_VS_VER              _MSC_VER
#endif

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
#define TOO_COMP_BORLAND_CPP            1
#endif

#if defined(__GNUC__)
#define TOO_COMP_GNU_CPP                1
#define GCC_VERSION                     (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define TOO_COMP_GNU_CPP_VER            GCC_VERSION
#endif

#if defined(__clang__) && (__clang__ == 1)
#define TOO_COMP_CLANG                  1
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define TOO_COMP_MINGW                  1
#define TOO_COMP_MINGW_VER              GCC_VERSION
#endif

#if defined(__CYGWIN__) && (__CYGWIN__ == 1)
#define TOO_COMP_CYGWIN                 1
#endif

#if defined(__INTEL_COMPILER)
#define TOO_COMP_INTEL                  1
#endif


//##############################################################################################################
// Languages

#ifdef __STDC__
#define TOO_LANG_STANDARD_C			1
#define TOO_LANG_STANDARD_C89		1
#if __STDC_VERSION__ >= 199901L
#define TOO_LANG_STANDARD_C99		1
#endif
#endif

#ifdef __cplusplus
#define TOO_LANG_STANDARD_CPP		1
#define TOO_LANG_STANDARD_CPP98		1
#endif

#ifdef __cplusplus_cli
#define TOO_LANG_STANDARD_CPP_CLI	1
#define TOO_LANG_STANDARD_CPP_CLI04	1
#endif

#ifdef __embedded_cplusplus
#define TOO_LANG_STANDARD_EMBCPP    1
#endif


//##############################################################################################################
// Just commenting definitions for switching on/off diverse library usages within the implementations of Toolib.

#if TOO_NOT_JUST_COMMENTING_LIBRARY_SWITCH_DEFINITIONS_BUT_ACTUALLY_USE_SOME_ARBITRARY_DEFAULTS // dummy
//! Use this definition to switch off any external lib dependencies.
/** The idea is that a good deal of Toolib is implemented just by standard methods.*/
#define TOO_NO_DEPENDENCIES
#endif


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
// Some common general preprocessor magic (as little as possible)

#define TOO_CONCATENATE_DIRECT(s1, s2)		s1##s2
#define TOO_CONCATENATE_INDIRECT(s1, s2)	TOO_CONCATENATE_DIRECT(s1, s2)
#define TOO_ANONYMOUS_VARIABLE_IMPL(str)	TOO_CONCATENATE_INDIRECT(str, __LINE__)
#define TOO_ANONYMOUS_VARIABLE				TOO_ANONYMOUS_VARIABLE_IMPL(anonymous_variable_)

#define TOO_STRINGIFY_IMPL(s)				#s
#define TOO_STRINGIFY(s)					TOO_STRINGIFY_IMPL(s)


//! Only for information. Since #error is plain standard you should just use it!
/** Actually it doesn't seem to be technically possible to define sth. like this
    \code
    #define TOO_COMPILER_ERROR(x)       #error x
    \endcode
    Which is ok, but the same applies to sth. like \code #pragma warning \endcode.*/


#define TOO_NOOP (void)0


//##############################################################################################################
#endif
