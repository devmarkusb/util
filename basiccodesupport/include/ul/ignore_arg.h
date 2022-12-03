// 2014-2016

//! DEPRECATED! Use ignore_unused.
/**
 */
//! \file

#ifndef IGNORE_ARG_H_kasgfuznfgzwngfz27gwfnsdfsd
#define IGNORE_ARG_H_kasgfuznfgzwngfz27gwfnsdfsd

#include "ul/macros.h"


namespace mb::ul
{
//! To 'suppress' compiler warning of an unused function parameter. Beyond that, it is
//! even usable for a function itself (e.g. gcc shows a warning for unused functions).
/** Example:
    \code
    f(int i) { ignore_arg(i); }
    \endcode
    Or:
    \code
    f() { ignore_arg(f); }
    \endcode*/
template <typename T>
void ignore_arg(T&&)
{
    // intentionally doing nothing; ideally the compiler optimizes away any call of this function
    UL_NOOP;
}
} // namespace mb::ul

#include "ul/macros_end.h"

#endif
