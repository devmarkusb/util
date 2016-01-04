// Markus Borris, 2014
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef STD_EXTENSIONS_H_kjhsdnxuwezkngfywzui6r
#define STD_EXTENSIONS_H_kjhsdnxuwezkngfywzui6r

#include <memory>
#include <type_traits>
#include "../PPDEFS.h"

//! Unfortunately vc12 doesn't support constexpr and noexcept yet, so...
#if TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800
#undef constexpr
#define constexpr

#undef noexcept
#define noexcept
#endif

namespace std
{
#if TOO_COMP_MINGW && TOO_COMP_MINGW_VER <= 40901
template <typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&& ...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

#if !(TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER >= 1800)
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
template <class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <class T>
using add_cv_t       = typename add_cv<T>::type;
template <class T>
using add_const_t    = typename add_const<T>::type;
template <class T>
using add_volatile_t = typename add_volatile<T>::type;
template <class T>
using remove_cv_t       = typename remove_cv<T>::type;
template <class T>
using remove_const_t    = typename remove_const<T>::type;
template <class T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <bool B, class T = void>
using enable_if_t = typename enable_if<B,T>::type;

template <bool B, class T, class F>
using conditional_t = typename conditional<B,T,F>::type;

template <class T>
using add_pointer_t = typename add_pointer<T>::type;

template <class T>
using decay_t = typename decay<T>::type;

template <class T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

template <class T>
using remove_reference_t = typename remove_reference<T>::type;
#endif

#if TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1800
template<class InputIt, class T, class BinaryOperation>
//	requires todo
T accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
{
	for (; first != last; ++first) {
		init = op(init, *first);
	}
	return init;
}
#endif
}

#endif
