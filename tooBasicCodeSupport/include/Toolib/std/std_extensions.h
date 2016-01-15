// Markus Borris, 2014
// This file is part of Toolib library. Open source.

//!
/** Extending namespace std with stuff that isn't there yet for certain compilers.
    We only allow those things to be placed here that have a good chance to make it into some standard C++xx.
    */
//! \file

#ifndef STD_EXTENSIONS_H_kjhsdnxuwezkngfywzui6r
#define STD_EXTENSIONS_H_kjhsdnxuwezkngfywzui6r

#include <memory>
#include <type_traits>
#include <algorithm> // included for the most probable find of std::accumulate (if ever)
#include "Toolib/PPDEFS.h"


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

namespace std
{
//! A must have.
#if TOO_HAS_NO_CPP1_MAKE_UNIQUE
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

//! Lots of nice to have type_traits.
//!@{

#if TOO_HAS_NO_CPP14_TYPE_TRAITS__T_SHORTCUTS
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
template <class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <class T>
using add_cv_t = typename add_cv<T>::type;
template <class T>
using add_const_t = typename add_const<T>::type;
template <class T>
using add_volatile_t = typename add_volatile<T>::type;
template <class T>
using remove_cv_t = typename remove_cv<T>::type;
template <class T>
using remove_const_t = typename remove_const<T>::type;
template <class T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <bool B, class T = void>
using enable_if_t         = typename enable_if<B, T>::type;

template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

template <class T>
using add_pointer_t = typename add_pointer<T>::type;

template <class T>
using decay_t = typename decay<T>::type;

template <class T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

template <class T>
using remove_reference_t = typename remove_reference<T>::type;
#endif

//!@}


#if TOO_HAS_NO_ACCUMULATE_ALG
//! A missing algorithm, accumulating a range by an arbitrary operation (e.g. summing up by +).
template <class InputIt, class T, class BinaryOperation>
//	requires //todo
T accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
{
    for (; first != last; ++first)
    {
        init = op(init, *first);
    }
    return init;
}
#endif
}

#endif
