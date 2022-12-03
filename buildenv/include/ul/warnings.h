// 2018

//!
/** Disable/enable compiler/linker warnings.
    TOO_PRAGMA_WARNINGS_PUSH saves the current warning settings, TOO_PRAGMA_WARNINGS_POP resets the
    warning settings to the state before calling TOO_PRAGMA_WARNINGS_PUSH.
    TOO_WARNING_DISABLE_<compiler>(w) disables warning w for a specific compiler.
    TOO_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC is a specialty. First it's the same as TOO_PRAGMA_WARNINGS_PUSH for
    every compiler. But for MSVC this additionally disables *all* all warnings (useful when including 3rd party headers,
    but this problem might also be solvable in another way).
*/
//! \file

#ifndef WARNINGS_H_erufh35782xz3785th183
#define WARNINGS_H_erufh35782xz3785th183

#include "ul/macros.h"


#if TOO_COMP_MS_VISUAL_STUDIO_CPP
#define TOO_PRAGMA_WARNINGS_PUSH                      TOO_PRAGMA(warning(push))
#define TOO_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC TOO_PRAGMA(warning(push, 0))
#define TOO_PRAGMA_WARNINGS_POP                       TOO_PRAGMA(warning(pop))

//! E.g. TOO_WARNING_DISABLE_MSVC(4702) disables warning number 4702 about unreachable code
#define TOO_WARNING_DISABLE_MSVC(w) TOO_PRAGMA(warning(disable : w))
#define TOO_WARNING_DISABLE_CLANG(w)
#define TOO_WARNING_DISABLE_GCC(w)

#elif TOO_COMP_CLANG
#define TOO_PRAGMA_WARNINGS_PUSH                      TOO_PRAGMA(clang diagnostic push)
#define TOO_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC TOO_PRAGMA_WARNINGS_PUSH
#define TOO_PRAGMA_WARNINGS_POP                       TOO_PRAGMA(clang diagnostic pop)

#define TOO_WARNING_DISABLE_MSVC(w)
//! E.g. TOO_WARNING_DISABLE_CLANG(sign-conversion) is equivalent to compiler flag -Wno-sign-conversion
#define TOO_WARNING_DISABLE_CLANG(w) TOO_PRAGMA(clang diagnostic ignored TOO_STRINGIFY_VALUE(TOO_CONCAT_2(-W, w)))
#define TOO_WARNING_DISABLE_GCC(w)

#elif TOO_COMP_GNU_CPP
#define TOO_PRAGMA_WARNINGS_PUSH                      TOO_PRAGMA(GCC diagnostic push)
#define TOO_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC TOO_PRAGMA_WARNINGS_PUSH
#define TOO_PRAGMA_WARNINGS_POP                       TOO_PRAGMA(GCC diagnostic pop)

#define TOO_WARNING_DISABLE_MSVC(w)
#define TOO_WARNING_DISABLE_CLANG(w)
//! E.g. TOO_WARNING_DISABLE_GCC(unused-variable) is equivalent to compiler flag -Wno-unused-variable
#define TOO_WARNING_DISABLE_GCC(w) TOO_PRAGMA(GCC diagnostic ignored TOO_STRINGIFY_VALUE(TOO_CONCAT_2(-W, w)))

#endif

#include "ul/macros_end.h"

#endif
