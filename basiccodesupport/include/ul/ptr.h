// 2015

//! \file Taken from / inspired by the Cpp Guidelines Support Library GSL.

#ifndef PTR_H_dfzg87c3tdcn872z3tcx3349xn3gx2f7y
#define PTR_H_dfzg87c3tdcn872z3tcx3349xn3gx2f7y

#include "ul/assert.h"
#include "ul/std/std_extensions.h"
#include <type_traits>

#include "ul/macros.h"


namespace mb::ul
{
//! Mark a raw pointer as owner of the memory. Should be rarely needed, in low level code at the most.
/** Usage:
\code
ul::owner<int*> owning_the_mem = new int;
int* just_an_alias_non_owning = owning_the_mem;
delete owning_the_mem;
\endcode
*/
template <typename T>
using owner = T;


//! Restricts a pointer or smart pointer to only hold non-null values.
/** (Cf. Cpp Guidelines Support Library GSL) Has zero size overhead over T.
    If T is a pointer (i.e. T == U*) then
    - allow construction from U* or U&
    - disallow construction from nullptr_t
    - disallow default construction
    - ensure construction from U* fails with nullptr
    - allow implicit conversion to U*

    Usage:
    \code
    void f(ul::not_null<int*> param)
    {
    // no nullptr-check necessary
    param;
    }
    \endcode
    */
template <class T>
class not_null
{
    static_assert(std::is_assignable<T&, std::nullptr_t>::value, "T cannot be assigned nullptr.");

public:
    explicit not_null(T t)
        : ptr(t)
    {
        ensure_invariant();
    }
    not_null& operator=(const T& t)
    {
        this->ptr = t;
        ensure_invariant();
        return *this;
    }

    not_null(const not_null&) = default;
    not_null& operator=(const not_null&) = default;

#if UL_HAS_CPP11_DEFAULT_MOVES
    not_null(not_null&&) noexcept = default;
    not_null& operator=(not_null&&) noexcept = default;
#endif

    template <typename U, typename Dummy = ul::enable_if_t<std::is_convertible<U, T>::value>>
    explicit not_null(const not_null<U>& other)
    {
        *this = other;
    }

    template <typename U, typename Dummy = ul::enable_if_t<std::is_convertible<U, T>::value>>
    not_null& operator=(const not_null<U>& other)
    {
        this->ptr = other.get();
        return *this;
    }

    // prevents compilation when someone attempts to assign a nullptr
    not_null(std::nullptr_t) = delete;
    not_null(int) = delete;
    not_null<T>& operator=(std::nullptr_t) = delete;
    not_null<T>& operator=(int) = delete;

    T get() const
    {
#if UL_COMP_MS_VISUAL_STUDIO_CPP
        __assume(ptr != nullptr); // the assume() should help the optimizer
#endif
        return ptr;
    }

    explicit operator T() const
    {
        return get();
    }
    T operator->() const
    {
        return get();
    }

    bool operator==(const T& rhs) const
    {
        return this->ptr == rhs;
    }
    bool operator!=(const T& rhs) const
    {
        return !(*this == rhs);
    }

    // unwanted operators...pointers only point to single objects!
    // untested, ensure all arithmetic ops on this type are unavailable, also list still incomplete
    not_null<T>& operator++() = delete;
    not_null<T>& operator--() = delete;
    not_null<T> operator++(int) = delete;
    not_null<T> operator--(int) = delete;
    not_null<T>& operator+(size_t) = delete;
    not_null<T>& operator+=(size_t) = delete;
    not_null<T>& operator-(size_t) = delete;
    not_null<T>& operator-=(size_t) = delete;

private:
    T ptr;

    // we assume that the compiler can hoist/prove away most of the checks inlined from this function
    // if not, we could make them optional via conditional compilation
    void ensure_invariant() const
    {
        UL_EXPECT_THROW(this->ptr != nullptr);
    }
};
} // namespace mb::ul

#include "ul/macros_end.h"

#endif
