//! \file

#ifndef NON_COPYABLE_H_IMCUTOIU34O5
#define NON_COPYABLE_H_IMCUTOIU34O5

#include "../std/std_extensions.h"

#include "ul/macros.h"

namespace mb::ul {
//! Provides a shortcut ultility to declare classes that are supposed to forbid copying.
/** Usage: \code class SomeClassThatShallForbidCopy : private non_copyable {};\endcode
        Note that the inheritance has to be qualified private.*/
class NonCopyable {
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

#endif
