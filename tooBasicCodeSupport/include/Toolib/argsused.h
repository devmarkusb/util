// Markus Borris, 2014
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef ARGSUSED_H_kasgfuznfgzwngfz27gwfnsdfsd
#define ARGSUSED_H_kasgfuznfgzwngfz27gwfnsdfsd

namespace too
{
//! To 'suppress' compiler warning of an unused function parameter.
/** Example:
    \code
    f(int i) { ignore_arg(i); }
    \endcode
*/
template <typename T>
void ignore_arg(T &&)
{
    // intentionally doing nothing; ideally the compiler optimizes away any call of this function
}
}

#endif
