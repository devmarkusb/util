// Markus Borris, 2011-20
// This file is part of tfl library.

//!
/** Most common and basic preprocessor defines.
*   If you include this file within a header you should also include macros_end.h when finished.
*/
//! \file

// no include guard intentionally

//####################################################################################################################
//! Operating Systems
//!@{
	
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__TOS_WIN__)
#define TOO_OS_WINDOWS 1
#if defined(_WIN64)
#define TOO_OS_WINDOWS_64 1
#endif
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#define TOO_OS_WINDOWS_DESKTOP_GE_WIN81 1
#define TOO_OS_WINDOWS_DESKTOP          1
#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#define TOO_OS_WINDOWS_UWP_APP  1
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PC_APP)
#define TOO_OS_WINDOWS_UWP_DESKTOP_APP  1
#endif
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE_APP)
#define TOO_OS_WINDOWS_UWP_PHONE_APP  1
#endif
#else
#define TOO_OS_WINDOWS_DESKTOP_LT_WIN81 1
#define TOO_OS_WINDOWS_DESKTOP          1
#endif
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#define TOO_OS_UNIX 1
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
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
#if !TOO_OS_LINUX
#error "unexpected, Android should imply Linux"
#endif
#endif

//!@}


//####################################################################################################################
//! Compilers
//!@{

#if defined(_MSC_VER)
#define TOO_COMP_MS_VISUAL_STUDIO_CPP 1
#define TOO_COMP_MS_VS_VER _MSC_VER
#endif

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
#define TOO_COMP_BORLAND_CPP 1
#endif

#if defined(__clang__) && (__clang__ == 1)
#define TOO_COMP_CLANG 1
#define TOO_COMP_CLANG_VER (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#endif

#if defined(__GNUC__) && !TOO_COMP_CLANG
#define TOO_COMP_GNU_CPP 1
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define TOO_COMP_GNU_CPP_VER GCC_VERSION
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


//#####################################################################################################################
//! Standard library
//!@{

#if TOO_COMP_MS_VISUAL_STUDIO_CPP
#define TOO_STDLIB_MSVC				1
#else
#if __GLIBCXX__
#define TOO_STDLIB_GNU_LIBSTDCPP	1
#else
#define TOO_STDLIB_CLANG_LIBCPP		1
#endif
#endif

//!@}


//####################################################################################################################
//! Languages
/** Note, it's very much common and ok to use __cplusplus directly of course (instead of TOO_LANG_STANDARD_CPP).
*/
//!@{

#ifdef __STDC__
#define TOO_LANG_STANDARD_C 1
#define TOO_LANG_STANDARD_C89 1
#if __STDC_VERSION__ >= 199901L
#define TOO_LANG_STANDARD_C99 1
#endif
#endif

#ifdef __cplusplus
#if __cplusplus != 0
#define TOO_LANG_STANDARD_CPP __cplusplus
#else
#define TOO_LANG_STANDARD_CPP 1
#endif
#define TOO_LANG_STANDARD_CPP98 199711L
#define TOO_LANG_STANDARD_CPP11 201103L
#define TOO_LANG_STANDARD_CPP14 201402L
#define TOO_LANG_STANDARD_CPP17 201703L
#endif

#ifdef __cplusplus_cli
#define TOO_LANG_STANDARD_CPP_CLI 1
#define TOO_LANG_STANDARD_CPP_CLI04 1
#endif

#ifdef __embedded_cplusplus
#define TOO_LANG_STANDARD_EMBCPP 1
#endif

//!@}


//####################################################################################################################
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


//####################################################################################################################
//! Language/Compiler/OS/... feature defines.
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
#define TOO_HAS_NO_CPP14_TYPE_TRAITS_T_SHORTCUTS 1
#else
#define TOO_HAS_NO_CPP14_TYPE_TRAITS_T_SHORTCUTS 0
#endif

#if 0 // TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800
#define TOO_HAS_NO_ACCUMULATE_ALG 1
#else
#define TOO_HAS_NO_ACCUMULATE_ALG 0
#endif

#if (TOO_COMP_MINGW && TOO_COMP_MINGW_VER <= 50300) || (TOO_COMP_GNU_CPP && TOO_COMP_GNU_CPP_VER <= 60201)
#define TOO_HAS_NO_CPP14_MAKE_UNIQUE 1 // shame on you
#else
#define TOO_HAS_NO_CPP14_MAKE_UNIQUE 0
#endif

#if (TOO_COMP_MINGW && TOO_COMP_MINGW_VER <= 50300) || (TOO_COMP_GNU_CPP && TOO_COMP_GNU_CPP_VER <= 40900) \
	|| (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER >= 1914)
#define TOO_HAS_NO_CODECVT 1
#else
#define TOO_HAS_NO_CODECVT 0
#endif

#if (TOO_COMP_MINGW && TOO_COMP_MINGW_VER <= 50300) || (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1900)
#define TOO_HAS_NO_CONSTEXPR_STD_RATIO 1
#else
#define TOO_HAS_NO_CONSTEXPR_STD_RATIO 0
#endif

#if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
#define TOO_SIZEOF_WCHAR_T 4
static_assert(sizeof(wchar_t) == 4, "You might adapt the above conditionals to your platform");
#else
#define TOO_SIZEOF_WCHAR_T 2
static_assert(sizeof(wchar_t) == 2, "You might adapt the above conditionals to your platform");
#endif

#if (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER >= 1800)
#define TOO_HAS_BRACE_INIT_MEMBER_NON_STATIC 0
#else
#define TOO_HAS_BRACE_INIT_MEMBER_NON_STATIC 1
#endif

#if (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800)
#define TOO_HAS_COMPILER_WARNING_INHERITS_BY_DOMINANCE 1
#else
#define TOO_HAS_COMPILER_WARNING_INHERITS_BY_DOMINANCE 0
#endif

#if TOO_COMP_GNU_CPP && !TOO_OS_WINDOWS
#ifndef __cdecl
#define __cdecl
#endif
#endif

#if (TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER >= 1900)
#define TOO_CAN_BUILD_WINRT_WRL_UWP 1
#else
#define TOO_CAN_BUILD_WINRT_WRL_UWP 0
#endif

//!@}


//####################################################################################################################
// Some common general preprocessor magic (as little as possible)

/** Usage:
	\code
	TOO_STRINGIFY(I want this to be in double quotes)
	// or
	#define SOMETHING 42
	#pragma message TOO_STRINGIFY(SOMETHING) // prints SOMETHING
	#pragma message TOO_STRINGIFY_VALUE(SOMETHING) // prints 42
	\endcode
*/
//!@{
#define TOO_STRINGIFY_VALUE(MACRO)	TOO_STRINGIFY(MACRO)
#define TOO_STRINGIFY(MACRO)		#MACRO
//!@}

/** If you have for example:
    \code
         #define ONETHING 42
         #define OTHERTHING 43
    \endcode
    Then TOO_CONCAT_2(ONETHING, OTHERTHING) will be replaced by ONETHINGOTHERTHING and
    TOO_CONCAT_VALUES_2(ONETHING, OTHERTHING) by 4243 at preprocessing stage.
    Note that you can even combine TOO_STRINGIFY_VALUE(TOO_CONCAT_2(...)) or TOO_STRINGIFY_VALUE(TOO_CONCAT_VALUES_2(...)), which
    additionally puts "" around. (TOO_STRINGIFY probably doesn't make much sense here.)*/
//!@{
#define TOO_CONCAT_2(a, b)              a##b
#define TOO_CONCAT_VALUES_2(a, b)       TOO_CONCAT_2(a, b)
//!@}

//! \See TOO_CONCAT_2, TOO_CONCAT_VALUES_2, just for 3 values.
//!@{
#define TOO_CONCAT_3(a, b, c)           a##b##c
#define TOO_CONCAT_VALUES_3(a, b, c)    TOO_CONCAT_3(a, b, c)
//!@}

#define TOO_ANONYMOUS_IDENTIFIER_IMPL(str)  TOO_CONCAT_VALUES_2(str, __LINE__)
/** TOO_ANONYMOUS_IDENTIFIER and convenient short version TOO_DUMMY are e.g. for cases where you don't need to reference
    a variable ever again and don't want to think of a name but need a unique identifier.
    Except for usage within a macro definition: since this is expanded on a single line always, you can refer to
    the (implicitly same) variable multiple times.*/
//!@{
#define TOO_ANONYMOUS_IDENTIFIER            TOO_ANONYMOUS_IDENTIFIER_IMPL(anonymous_)
#define TOO_DUMMY                           TOO_ANONYMOUS_IDENTIFIER
//!@}

#if TOO_COMP_MS_VISUAL_STUDIO_CPP
//! You shouldn't use this. It might not be compiler-portable and as just an implementation detail of TOO_PRAGMA.
#define TOO_PRAGMA_STR(str) __pragma(str)
//! Used for planting a pragma within a macro. \Param nostr has to be passed without surrounding "".
#define TOO_PRAGMA(nostr)   __pragma(nostr)
#elif TOO_COMP_CLANG || TOO_COMP_GNU_CPP || TOO_COMP_MINGW
//! You shouldn't use this. It might not be compiler-portable and as just an implementation detail of TOO_PRAGMA.
#define TOO_PRAGMA_STR(str) _Pragma(str)
//! Used for planting a pragma within a macro. \Param nostr has to be passed without surrounding "".
#define TOO_PRAGMA(nostr)   TOO_PRAGMA_STR(TOO_STRINGIFY_VALUE(nostr))
#else
#error "not implemented"
#endif

//! Only for information. Since #error is plain standard you should just use it!
/** Actually it doesn't seem to be technically possible to define sth. like this
    \code
    #define TOO_COMPILER_ERROR(x)       #error x
    \endcode
    Which is ok, but the same applies to sth. like \code #pragma warning \endcode.*/

//! Let compiler print a message. Should be supported by all decent compilers.
/** Note that there is no way of letting the message pop up only once.*/
//#pragma message("Just a hint...")

//! Let compiler print a warning. Unfortunately not possible portably.
//#warning "oh no!"

//! To express an empty statement.
#define TOO_NOOP (void)0

#ifdef __cplusplus
#define TOO_DEPRECATED [[deprecated]]
#else
#define TOO_DEPRECATED
#endif