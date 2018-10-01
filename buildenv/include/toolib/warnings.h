// Markus Borris, 2018
// This file is part of tfl library.

//!
/** Disable/enable compiler/linker warnings.
*/
//! \file

#ifndef WARNINGS_H_erufh35782xz3785th183
#define WARNINGS_H_erufh35782xz3785th183

#include "toolib/PPDEFS.h"

#if TOO_COMP_MS_VISUAL_STUDIO_CPP

#define TOO_PRAGMA_WARNINGS_PUSH __pragma(warning(push))
#define TOO_PRAGMA_WARNINGS_PUSH_DISABLE_ALL __pragma(warning(push, 0))
#define TOO_PRAGMA_WARNINGS_POP __pragma(warning(pop))

#define TOO_PRAGMA_WARNING_NO_assignment_operator_not_generated __pragma(warning(disable : 4512))
#define TOO_PRAGMA_WARNING_NO_comma
#define TOO_PRAGMA_WARNING_NO_constant_condition_expression __pragma(warning(disable : 4127))
#define TOO_PRAGMA_WARNING_NO_deprecated_declarations
#define TOO_PRAGMA_WARNING_NO_dll_interface_needed __pragma(warning(disable : 4251))
#define TOO_PRAGMA_WARNING_NO_header_hygiene
#define TOO_PRAGMA_WARNING_NO_reserved_id_macro
#define TOO_PRAGMA_WARNING_NO_sign_conversion
#define TOO_PRAGMA_WARNING_NO_unreachable_code __pragma(warning(disable : 4702))
#define TOO_PRAGMA_WARNING_NO_unsafe_parameters __pragma(warning(disable : 4996))
#define TOO_PRAGMA_WARNING_NO_unused_local_typedefs
#define TOO_PRAGMA_WARNING_NO_unused_variable
#define TOO_PRAGMA_WARNING_NO_used_but_marked_unused

#elif TOO_COMP_CLANG

#define TOO_PRAGMA_WARNINGS_PUSH _Pragma("clang diagnostic push")
#define TOO_PRAGMA_WARNINGS_PUSH_DISABLE_ALL
#define TOO_PRAGMA_WARNINGS_POP _Pragma("clang diagnostic pop")

#define TOO_PRAGMA_WARNING_NO_assignment_operator_not_generated
#define TOO_PRAGMA_WARNING_NO_comma _Pragma("clang diagnostic ignored \"-Wcomma\"")
#define TOO_PRAGMA_WARNING_NO_constant_condition_expression
#define TOO_PRAGMA_WARNING_NO_deprecated_declarations
#define TOO_PRAGMA_WARNING_NO_dll_interface_needed
#define TOO_PRAGMA_WARNING_NO_header_hygiene _Pragma("clang diagnostic ignored \"-Wheader-hygiene\"")
#define TOO_PRAGMA_WARNING_NO_reserved_id_macro _Pragma("clang diagnostic ignored \"-Wreserved-id-macro\"")
#define TOO_PRAGMA_WARNING_NO_sign_conversion _Pragma("clang diagnostic ignored \"-Wsign-conversion\"")
#define TOO_PRAGMA_WARNING_NO_unreachable_code
#define TOO_PRAGMA_WARNING_NO_unsafe_parameters
#define TOO_PRAGMA_WARNING_NO_unused_local_typedefs
#define TOO_PRAGMA_WARNING_NO_unused_variable
#define TOO_PRAGMA_WARNING_NO_used_but_marked_unused _Pragma("clang diagnostic ignored \"-Wused-but-marked-unused\"")

#elif TOO_COMP_GNU_CPP

#define TOO_PRAGMA_WARNINGS_PUSH _Pragma("GCC diagnostic push")
#define TOO_PRAGMA_WARNINGS_PUSH_DISABLE_ALL
#define TOO_PRAGMA_WARNINGS_POP _Pragma("GCC diagnostic pop")

#define TOO_PRAGMA_WARNING_NO_assignment_operator_not_generated
#define TOO_PRAGMA_WARNING_NO_comma
#define TOO_PRAGMA_WARNING_NO_constant_condition_expression
#define TOO_PRAGMA_WARNING_NO_deprecated_declarations _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#define TOO_PRAGMA_WARNING_NO_dll_interface_needed
#define TOO_PRAGMA_WARNING_NO_header_hygiene
#define TOO_PRAGMA_WARNING_NO_reserved_id_macro
#define TOO_PRAGMA_WARNING_NO_sign_conversion
#define TOO_PRAGMA_WARNING_NO_unreachable_code
#define TOO_PRAGMA_WARNING_NO_unsafe_parameters
#define TOO_PRAGMA_WARNING_NO_unused_local_typedefs _Pragma("GCC diagnostic ignored \"-Wunused-local-typedefs\"")
#define TOO_PRAGMA_WARNING_NO_unused_variable _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")
#define TOO_PRAGMA_WARNING_NO_used_but_marked_unused

#endif


#endif
