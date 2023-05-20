//! \file Taken from / inspired by the Cpp Guidelines Support Library GSL.

#ifndef PTR_H_DFZG87C3TDCN872Z3TCX3349XN3GX2F7Y
#define PTR_H_DFZG87C3TDCN872Z3TCX3349XN3GX2F7Y

#include "assert.h"
#include "std/std_extensions.h"

#include "ul/macros.h"
#include <type_traits>

namespace mb::ul {
//! Mark a raw pointer as owner of the memory. Should be rarely needed, in low level code at the most.
/** Usage:
\code
ul::Owner<int*> owning_the_mem = new int;
int* just_an_alias_non_owning = owning_the_mem;
delete owning_the_mem;
\endcode
*/
template <typename T>
using Owner = T;

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
    void f(ul::NotNull<int*> param)
    {
    // no nullptr-check necessary
    param;
    }
    \endcode
    */
template <class T>
class NotNull {
    static_assert(std::is_assignable<T&, std::nullptr_t>::value, "T cannot be assigned nullptr.");

public:
    /*implicit*/ NotNull(T t)
        : ptr_(t) {
        ensure_invariant();
    }

    NotNull& operator=(const T& t) {
        this->ptr_ = t;
        ensure_invariant();
        return *this;
    }

    NotNull(const NotNull&) = default;
    NotNull& operator=(const NotNull&) = default;

#if UL_HAS_CPP11_DEFAULT_MOVES
    NotNull(NotNull&&) noexcept = default;
    NotNull& operator=(NotNull&&) noexcept = default;
#endif

    template <typename U, typename Dummy = ul::enable_if_t<std::is_convertible<U, T>::value>>
    explicit NotNull(const NotNull<U>& other) {
        *this = other;
    }

    template <typename U, typename Dummy = ul::enable_if_t<std::is_convertible<U, T>::value>>
    NotNull& operator=(const NotNull<U>& other) {
        this->ptr_ = other.get();
        return *this;
    }

    // prevents compilation when someone attempts to assign a nullptr
    NotNull(std::nullptr_t) = delete;
    NotNull(int) = delete;
    NotNull<T>& operator=(std::nullptr_t) = delete;
    NotNull<T>& operator=(int) = delete;

    [[nodiscard]] T get() const {
#if UL_COMP_MS_VISUAL_STUDIO_CPP
        __assume(ptr_ != nullptr); // the assume() should help the optimizer
#endif
        return ptr_;
    }

    /*implicit*/ operator T() const {
        return get();
    }

    T operator->() const {
        return get();
    }

    bool operator==(const T& rhs) const {
        return this->ptr_ == rhs;
    }

    bool operator!=(const T& rhs) const {
        return !(*this == rhs);
    }

    // unwanted operators...pointers only point to single objects!
    // untested, ensure all arithmetic ops on this type are unavailable, also list still incomplete
    NotNull<T>& operator++() = delete;
    NotNull<T>& operator--() = delete;
    NotNull<T> operator++(int) = delete;
    NotNull<T> operator--(int) = delete;
    NotNull<T>& operator+(size_t) = delete;
    NotNull<T>& operator+=(size_t) = delete;
    NotNull<T>& operator-(size_t) = delete;
    NotNull<T>& operator-=(size_t) = delete;

private:
    T ptr_;

    // we assume that the compiler can hoist/prove away most of the checks inlined from this function
    // if not, we could make them optional via conditional compilation
    void ensure_invariant() const {
        UL_EXPECT_THROW(this->ptr_ != nullptr);
    }
};
} // namespace mb::ul

#endif
