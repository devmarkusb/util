// Markus Borris, 2011-17
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef NON_COPYABLE_H_INCL_imcutoiu34o5
#define NON_COPYABLE_H_INCL_imcutoiu34o5

#include "Toolib/PPDEFS.h"
#include "Toolib/std/std_extensions.h"


namespace too
{
//! Provides a shortcut ultility to declare classes that are supposed to forbid copying.
/** Usage: \code class SomeClassThatShallForbidCopy : private non_copyable {};\endcode
        Note that the inheritance has to be qualified private.*/
class non_copyable
{
protected:
    constexpr non_copyable() = default;
    ~non_copyable() = default;
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;
#if TOO_HAS_CPP11_DEFAULT_MOVES
    non_copyable(non_copyable&&) = default;
    non_copyable& operator=(non_copyable&&) = default;
#endif
};
}

#endif
