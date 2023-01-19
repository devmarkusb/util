/** \file
    Disable/enable compiler/linker warnings.
    UL_PRAGMA_WARNINGS_PUSH saves the current warning settings, UL_PRAGMA_WARNINGS_POP resets the
    warning settings to the state before calling UL_PRAGMA_WARNINGS_PUSH.
    UL_WARNING_DISABLE_<compiler>(w) disables warning w for a specific compiler.
    UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC is a specialty. First it's the same as UL_PRAGMA_WARNINGS_PUSH for
    every compiler. But for MSVC this additionally disables *all* all warnings (useful when including 3rd party headers,
    but this problem might also be solvable in another way).
*/

#ifndef WARNINGS_H_erufh35782xz3785th183
#define WARNINGS_H_erufh35782xz3785th183

#include "macros.h"

#if UL_COMP_MS_VISUAL_STUDIO_CPP
#define UL_PRAGMA_WARNINGS_PUSH                      UL_PRAGMA(warning(push))
#define UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC UL_PRAGMA(warning(push, 0))
#define UL_PRAGMA_WARNINGS_POP                       UL_PRAGMA(warning(pop))

//! E.g. UL_WARNING_DISABLE_MSVC(4702) disables warning number 4702 about unreachable code
#define UL_WARNING_DISABLE_MSVC(w) UL_PRAGMA(warning(disable : w))
#define UL_WARNING_DISABLE_CLANG(w)
#define UL_WARNING_DISABLE_CLANG_ALL
#define UL_WARNING_DISABLE_GCC(w)

#elif UL_COMP_CLANG
#define UL_PRAGMA_WARNINGS_PUSH                      UL_PRAGMA(clang diagnostic push)
#define UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC UL_PRAGMA_WARNINGS_PUSH
#define UL_PRAGMA_WARNINGS_POP                       UL_PRAGMA(clang diagnostic pop)

#define UL_WARNING_DISABLE_MSVC(w)
//! E.g. UL_WARNING_DISABLE_CLANG(sign-conversion) is equivalent to compiler flag -Wno-sign-conversion
#define UL_WARNING_DISABLE_CLANG(w)  UL_PRAGMA(clang diagnostic ignored UL_STRINGIFY_VALUE(UL_CONCAT_2(-W, w)))
#define UL_WARNING_DISABLE_CLANG_ALL UL_WARNING_DISABLE_CLANG(everything)
#define UL_WARNING_DISABLE_GCC(w)

#elif UL_COMP_GNU_CPP
#define UL_PRAGMA_WARNINGS_PUSH                      UL_PRAGMA(GCC diagnostic push)
#define UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC UL_PRAGMA_WARNINGS_PUSH
#define UL_PRAGMA_WARNINGS_POP                       UL_PRAGMA(GCC diagnostic pop)

#define UL_WARNING_DISABLE_MSVC(w)
#define UL_WARNING_DISABLE_CLANG(w)
#define UL_WARNING_DISABLE_CLANG_ALL
//! E.g. UL_WARNING_DISABLE_GCC(unused-variable) is equivalent to compiler flag -Wno-unused-variable
#define UL_WARNING_DISABLE_GCC(w) UL_PRAGMA(GCC diagnostic ignored UL_STRINGIFY_VALUE(UL_CONCAT_2(-W, w)))
// not implementable right now it seems
//#define UL_WARNING_DISABLE_GCC_ALL

#endif

#endif
