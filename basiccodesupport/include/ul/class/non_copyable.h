// 2011-18

//!
/**
 */
//! \file

#ifndef NON_COPYABLE_H_imcutoiu34o5
#define NON_COPYABLE_H_imcutoiu34o5

#include "ul/std/std_extensions.h"

#include "ul/macros.h"


namespace mb::ul
{
//! Provides a shortcut ultility to declare classes that are supposed to forbid copying.
/** Usage: \code class SomeClassThatShallForbidCopy : private non_copyable {};\endcode
        Note that the inheritance has to be qualified private.*/
class non_copyable
{
protected:
    constexpr non_copyable() noexcept = default;
    ~non_copyable() = default;
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;
#if TOO_HAS_CPP11_DEFAULT_MOVES
    non_copyable(non_copyable&&) noexcept = default;
    non_copyable& operator=(non_copyable&&) noexcept = default;
#endif
};
} // namespace mb::ul

#include "ul/macros_end.h"

#endif