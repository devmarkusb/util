/** \file
    Usage I: The following steps are for a library (shared or static) as intended target.
    (1) In the build configuration of your library define sth. like YOURLIB_LIBRARY. This is
    the indicator, that a library should be built from the code. Of course you don't do this in
    the application which later uses the library.
    (2) Within your library code base create your own YourlibDEF.h among the shared include subdir
    which includes this file here and place your own library import/export define in there like
    \code
    // ...don't forget include guard...
    #include "ul/linklib/LINKLIB_DEFS.h"
    #if defined(YOURLIB_LIBRARY)
    #define YOURLIBSHARED_EXPORT    UL_DECL_EXPORT
    #else
    #define YOURLIBSHARED_EXPORT    UL_DECL_IMPORT
    #endif
    \endcode
    // ...
    (3) Include your YourlibDEF.h into all your library headers that export things and ought to be used
    from the outside, too. Of course, if you have only one header to share, you can put the code of YourlibDEF.h
    right into there and spare that additional header.
    (4) Exporting things then goes like this:
    \code
    #include "YourlibDEF.h"
    class YOURLIBSHARED_EXPORT CExample {...};
    YOURLIBSHARED_EXPORT inline void f(...) {...}
    YOURLIBSHARED_EXPORT void g(...);
    YOURLIBSHARED_EXPORT int i;
    \endcode
    (5) Remark: Do not use a DllMain function. This is Windows-only.
    (7) Add UL_EXTERN_C_DECLS (next to YOURLIBSHARED_EXPORT) if you intend to allow your library users the
    choice of late binding.

    Usage II: The following documentation is for application code which likes to call or link your library.
    - Just linking (statically or early binding).
        Nothing to do. Just include your library headers with the exported symbols that you like to use.
    - Note: Never use late binding, if not absolutely necessary. There are just more pitfalls.
*/

#ifndef LINKLIB_DEFS_H_KJDSNFXIWZGFNY38TGN38G
#define LINKLIB_DEFS_H_KJDSNFXIWZGFNY38TGN38G

#include "ul/macros.h"


//####################################################################################################################

#if UL_OS_WINDOWS == 1
//! Perhaps UL_LINKLIB_IMPL_CHOICE_OWN_CFG_STATICLIB has to be defined when you compile a library as static lib and
//! also when you use it.
#if !defined(UL_LINKLIB_IMPL_CHOICE_OWN_CFG_STATICLIB)
#define UL_DECL_EXPORT __declspec(dllexport)
#define UL_DECL_IMPORT __declspec(dllimport)
#elif UL_LINKLIB_IMPL_CHOICE_OWN_CFG_STATICLIB == 1
#define UL_DECL_EXPORT
#define UL_DECL_IMPORT
#endif
#elif UL_OS_LINUX == 1
#define UL_DECL_EXPORT
#define UL_DECL_IMPORT
#else
// untested, don't think this alone works for all remaining platforms ;)
#define UL_DECL_EXPORT
#define UL_DECL_IMPORT
#endif


//! Declare the general dll calling convention.
#if defined(_STDCALL_SUPPORTED)
#define UL_CALLINGCONVENTION __stdcall
#else
#define UL_CALLINGCONVENTION __cdecl
#endif


//! Declare the dll calling convention for DllMain, Windows.
#if UL_OS_WINDOWS == 1 && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
#define UL_WINAPI_DLLMAIN __stdcall
#else
#define UL_WINAPI_DLLMAIN
#endif
#define UL_APIENTRY WINAPI_DLLMAIN


//####################################################################################################################

//! Put UL_EXTERN_C_DECLS in front of your YOURLIBSHARED_EXPORT-define
//! (being switch-wise UL_DECL_EXPORT/UL_DECL_IMPORT) if appropriate.
#if UL_LANG_STANDARD_CPP
#define UL_EXTERN_C_DECLS       extern "C"
#define UL_EXTERN_C_DECLS_BEGIN extern "C" {
#define UL_EXTERN_C_DECLS_END   }
#else
#define UL_EXTERN_C_DECLS
#define UL_EXTERN_C_DECLS_BEGIN
#define UL_EXTERN_C_DECLS_END
#endif


//####################################################################################################################

#endif
