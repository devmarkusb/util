//! \file

#ifndef IGNORE_UNUSED_H_fklh3874gy78342
#define IGNORE_UNUSED_H_fklh3874gy78342

#include "ul/macros.h"

namespace mb::ul
{
/** To 'suppress' compiler warning of an unused function parameter. Beyond that, it is
    even usable for a function itself (e.g. gcc shows a warning for unused functions).
    Example:
    \code
    f(int i) { ignore_unused(i); }
    \endcode
    Or:
    \code
    f() { ignore_unused(f); }
    \endcode*/
template <typename T>
void ignore_unused(T&&)
{
    // intentionally doing nothing; ideally the compiler optimizes away any call of this function
    UL_NOOP;
}
} // namespace mb::ul

#endif
