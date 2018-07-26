// Markus Borris, 2014-2016
// This file is part of tfl library.

//!
/**
*/
//! \file

#ifndef IGNORE_ARG_H_kasgfuznfgzwngfz27gwfnsdfsd
#define IGNORE_ARG_H_kasgfuznfgzwngfz27gwfnsdfsd

#include "toolib/PPDEFS.h"


namespace too
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
    TOO_NOOP;
}
} // too

#endif
