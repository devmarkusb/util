// Markus Borris, 2011
// This file is part of Toolib library.

//!
/** Most common and basic preprocessor defines.
*/
//! \file

#ifndef PPDEFS_H_INCL_o85nzumvt424cmt75tn
#define PPDEFS_H_INCL_o85nzumvt424cmt75tn

//##############################################################################################################
//! Operating Systems
//!@{

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__TOS_WIN__)
#define TOO_OS_WINDOWS 1
#if defined(_WIN64)
#define TOO_OS_WINDOWS_64 1
#endif
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#define TOO_OS_UNIX 1
#endif

#if defined(linux) || defined(__linux)
#define TOO_OS_LINUX 1
#endif

#if defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
#define TOO_OS_MAC 1
#endif

#if defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
#define TOO_OS_MSDOS 1
#endif

#if defined(_WIN32_WCE)
#define TOO_OS_WINDOWS_CE 1
#endif

#if defined(__FreeBSD__)
#define TOO_OS_FREEBSD 1
#endif

#if ((TOO_OS_LINUX || TOO_OS_MAC || TOO_OS_FREEBSD) && (!TOO_OS_WINDOWS))
#define TOO_OS_UNIX 1
#endif

#if defined(__ANDROID__)
#define TOO_OS_ANDROID 1
#endif

//!@}

//##############################################################################################################
//! Compilers
//!@{

#if defined(_MSC_VER)
#define TOO_COMP_MS_VISUAL_STUDIO_CPP 1
#define TOO_COMP_MS_VS_VER _MSC_VER
#endif

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
#define TOO_COMP_BORLAND_CPP 1
#endif

#if defined(__GNUC__)
#define TOO_COMP_GNU_CPP 1
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define TOO_COMP_GNU_CPP_VER GCC_VERSION
#endif

#if defined(__clang__) && (__clang__ == 1)
#define TOO_COMP_CLANG 1
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define TOO_COMP_MINGW 1
#define TOO_COMP_MINGW_VER GCC_VERSION
#endif

#if defined(__CYGWIN__) && (__CYGWIN__ == 1)
#define TOO_COMP_CYGWIN 1
#endif

#if defined(__INTEL_COMPILER)
#define TOO_COMP_INTEL 1
#endif

//!@}

//##############################################################################################################
//! Languages
//!@{

#ifdef __STDC__
#define TOO_LANG_STANDARD_C 1
#define TOO_LANG_STANDARD_C89 1
#if __STDC_VERSION__ >= 199901L
#define TOO_LANG_STANDARD_C99 1
#endif
#endif

#ifdef __cplusplus
#define TOO_LANG_STANDARD_CPP 1
#define TOO_LANG_STANDARD_CPP98 1
#endif

#ifdef __cplusplus_cli
#define TOO_LANG_STANDARD_CPP_CLI 1
#define TOO_LANG_STANDARD_CPP_CLI04 1
#endif

#ifdef __embedded_cplusplus
#define TOO_LANG_STANDARD_EMBCPP 1
#endif

//!@}

//##############################################################################################################
//! Main debug-macro definition. Note that it's perfectly fine to use NDEBUG
//! which is standard instead of
//! TOO_DEBUG.
//!@{

#if TOO_MS_VISUAL_STUDIO_CPP
#ifdef _DEBUG
#define TOO_DEBUG 1
#else
#define TOO_DEBUG 0
#endif
#else
#ifdef NDEBUG
#define TOO_DEBUG 0
#else
#define TOO_DEBUG 1
#endif
#endif

//!@}

//##############################################################################################################
//! Language/Compiler/OS/... feature defines.
//!
//!@{

#if (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800)
#define TOO_HAS_NO_CPP11_U8 1
#else
#define TOO_HAS_NO_CPP11_U8 0
#endif

#if (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800)
#define TOO_HAS_CPP11_DEFAULT_MOVES 0
#else
#define TOO_HAS_CPP11_DEFAULT_MOVES 1
#endif

#if (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800)
#define TOO_HAS_NO_CPP11_CONSTEXPR 1
#else
#define TOO_HAS_NO_CPP11_CONSTEXPR 0
#endif

#if (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800)
#define TOO_HAS_NO_CPP11_NOEXCEPT 1
#else
#define TOO_HAS_NO_CPP11_NOEXCEPT 0
#endif

#if !(TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER >= 1800)
#define TOO_HAS_NO_CPP14_TYPE_TRAITS__T_SHORTCUTS 1
#else
#define TOO_HAS_NO_CPP14_TYPE_TRAITS__T_SHORTCUTS 0
#endif

#if TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800
#define TOO_HAS_NO_ACCUMULATE_ALG 1
#else
#define TOO_HAS_NO_ACCUMULATE_ALG 0
#endif

#if TOO_COMP_MINGW && TOO_COMP_MINGW_VER <= 40902
#define TOO_HAS_NO_CPP1_MAKE_UNIQUE 1 // shame on you
#else
#define TOO_HAS_NO_CPP1_MAKE_UNIQUE 0
#endif

#if TOO_COMP_MINGW && TOO_COMP_MINGW_VER <= 40902
#define TOO_HAS_NO_CODECVT 1
#else
#define TOO_HAS_NO_CODECVT 0
#endif

#if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
#define TOO_SIZEOF_WCHAR_T 4
static_assert(sizeof(wchar_t) == 4, "You might adapt the above conditionals to your platform");
#else
#define TOO_SIZEOF_WCHAR_T 2
static_assert(sizeof(wchar_t) == 2, "You might adapt the above conditionals to your platform");
#endif

//!@}

//##############################################################################################################
// Some common general preprocessor magic (as little as possible)

#define TOO_CONCATENATE_DIRECT(s1, s2) s1##s2
#define TOO_CONCATENATE_INDIRECT(s1, s2) TOO_CONCATENATE_DIRECT(s1, s2)
#define TOO_ANONYMOUS_VARIABLE_IMPL(str) TOO_CONCATENATE_INDIRECT(str, __LINE__)
//! Whenever you don't want to think of a name of an identifier,
//! TOO_ANONYMOUS_VARIABLE or
//! interchangeably TOO_NONAME are your friends.
/**	One note: it is possible to use TOO_NONAME in the same source code
   line, but no beyond. But this probably
    won't be good practice.*/
//!@{
#define TOO_ANONYMOUS_VARIABLE TOO_ANONYMOUS_VARIABLE_IMPL(anonymous_variable_)
#define TOO_NONAME TOO_ANONYMOUS_VARIABLE
//!@}

#define TOO_STRINGIFY_IMPL(notYetString) #notYetString
//! Usage:
/**
\code
TOO_STRINGIFY(I want this to be in double quotes)
\endcode
*/
#define TOO_STRINGIFY(notYetString) TOO_STRINGIFY_IMPL(notYetString)

//! Only for information. Since #error is plain standard you should just use it!
/** Actually it doesn't seem to be technically possible to define sth. like this
    \code
    #define TOO_COMPILER_ERROR(x)       #error x
    \endcode
    Which is ok, but the same applies to sth. like \code #pragma warning
   \endcode.*/

//! Let compiler print a message. Should be supported by all decent compilers.
/** Note that there is no way of letting the message pop up only once.*/
//#pragma message("Just a hint...")

//! Let compiler print a warning. Unfortunately not possible portably.
//#warning "oh no!"

//! To express an empty statement.
#define TOO_NOOP (void)0

//##############################################################################################################
#endif
