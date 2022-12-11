// 2011-22

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
class NonCopyable
{
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    constexpr NonCopyable() noexcept = default;
    ~NonCopyable() = default;
#if UL_HAS_CPP11_DEFAULT_MOVES
    NonCopyable(NonCopyable&&) noexcept = default;
    NonCopyable& operator=(NonCopyable&&) noexcept = default;
#endif
};
} // namespace mb::ul

#include "ul/macros_end.h"

#endif
