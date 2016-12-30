// Markus Borris, 2014-16
// This file is part of Toolib library.

//!
/** 'Extending namespace std' - not really, we but it in namespace too to be safe - with stuff that isn't
    there yet for certain compilers. We only allow those things to be placed here that have a good chance
    to make it into some standard C++xx.
    */
//! \file

#ifndef STD_EXTENSIONS_H_kjhsdnxuwezkngfywzui6r
#define STD_EXTENSIONS_H_kjhsdnxuwezkngfywzui6r

#include "Toolib/PPDEFS.h"
#include <algorithm> // included for the most probable find of std::accumulate (if ever)
#include <memory>
#include <numeric>   // included for the most probable find of std::accumulate (if ever)
#include <type_traits>


//! Unfortunately some compiler whose name doesn't want to be called doesn't support constexpr and
//! noexcept yet, so...
/** This header serves as a shutdown of these keywords beyond its boundaries.*/
#if TOO_HAS_NO_CPP11_CONSTEXPR
#undef constexpr
#define constexpr
#endif

#if TOO_HAS_NO_CPP11_NOEXCEPT
#undef noexcept
#define noexcept throw()
#endif

namespace too
{

//! A must have.
#if TOO_HAS_NO_CPP14_MAKE_UNIQUE
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#else
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}
#endif

//! Lots of nice to have type_traits.
//!@{

#if TOO_HAS_NO_CPP14_TYPE_TRAITS__T_SHORTCUTS
template <class T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;
template <class T>
using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;

template <class T>
using add_cv_t = typename std::add_cv<T>::type;
template <class T>
using add_const_t = typename std::add_const<T>::type;
template <class T>
using add_volatile_t = typename std::add_volatile<T>::type;
template <class T>
using remove_cv_t = typename std::remove_cv<T>::type;
template <class T>
using remove_const_t = typename std::remove_const<T>::type;
template <class T>
using remove_volatile_t = typename std::remove_volatile<T>::type;

template <bool B, class T = void>
using enable_if_t         = typename std::enable_if<B, T>::type;

template <bool B, class T, class F>
using conditional_t = typename std::conditional<B, T, F>::type;

template <class T>
using add_pointer_t = typename std::add_pointer<T>::type;

template <class T>
using decay_t = typename std::decay<T>::type;

template <class T>
using remove_all_extents_t = typename std::remove_all_extents<T>::type;

template <class T>
using remove_reference_t = typename std::remove_reference<T>::type;

#else

template <class T>
using add_lvalue_reference_t = std::add_lvalue_reference_t<T>;
template <class T>
using add_rvalue_reference_t = std::add_rvalue_reference_t<T>;

template <class T>
using add_cv_t = std::add_cv_t<T>;
template <class T>
using add_const_t = std::add_const_t<T>;
template <class T>
using add_volatile_t = std::add_volatile_t<T>;
template <class T>
using remove_cv_t = std::remove_cv_t<T>;
template <class T>
using remove_const_t = std::remove_const_t<T>;
template <class T>
using remove_volatile_t = std::remove_volatile_t<T>;

template <bool B, class T = void>
using enable_if_t         = std::enable_if_t<B, T>;

template <bool B, class T, class F>
using conditional_t = std::conditional_t<B, T, F>;

template <class T>
using add_pointer_t = std::add_pointer_t<T>;

template <class T>
using decay_t = std::decay_t<T>;

template <class T>
using remove_all_extents_t = std::remove_all_extents_t<T>;

template <class T>
using remove_reference_t = std::remove_reference_t<T>;

#endif

//!@}


#if TOO_HAS_NO_ACCUMULATE_ALG
//! A missing algorithm, accumulating a range by an arbitrary operation (e.g. summing up by +).
template <class InputIt, class T, class BinaryOperation>
//	requires // cf. cpp doc
T accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
{
    for (; first != last; ++first)
    {
        init = op(init, *first);
    }
    return init;
}
#else
template <class InputIt, class T, class BinaryOperation>
T accumulate(InputIt&& first, InputIt&& last, T&& init, BinaryOperation&& op)
{
    return std::accumulate(std::forward<InputIt>(first), std::forward<InputIt>(last), std::forward<T>(init),
        std::forward<BinaryOperation>(op));
}
#endif
}

#endif
