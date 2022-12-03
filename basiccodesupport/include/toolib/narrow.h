// 2015-16

//!
/** Taken from / inspired by the Cpp Guidelines Support Library GSL.
 */
//! \file

#ifndef NARROW_H_sk837xxgnf83xgfn3g8t34sdjn47
#define NARROW_H_sk837xxgnf83xgfn3g8t34sdjn47

#include "almost_equal.h"
#include "std/std_extensions.h"
#include <type_traits>


namespace too
{
//! Nothing else than a usual static_cast, but communicating intent of converting to a smaller type, assuming the value
//! not to change at all.
template <class T, class U>
constexpr T narrow_cast(U u) noexcept
{
    return static_cast<T>(u);
}

//! Exception thrown by narrow() when failing.
struct narrowing_error : public std::exception
{
};

//! Checked version of narrow_cast(), throwing \ref narrowing_error if the cast changed the value.
//! Expects at least one of the casting types to be non floating point.
template <class T, class U>
T narrow(U u)
{
    static_assert(
        !(std::is_floating_point<T>::value && std::is_floating_point<U>::value),
        "narrow expects at least one of the casting types to be non floating point. Use narrow_cast instead.");
    T t = narrow_cast<T>(u);
    if (!too::almost_equal_alltypes(static_cast<U>(t), u))
        throw narrowing_error();
    return t;
}
} // namespace too

#endif
