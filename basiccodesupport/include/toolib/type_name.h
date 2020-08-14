// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef TYPE_NAME_H_4387t73824t378t371y47ym47
#define TYPE_NAME_H_4387t73824t378t371y47ym47

#include "debug.h"
#include "error.h"
#include <string_view>

#include "toolib/macros.h"


namespace too
{
//! Prints type of variable objectOfType in the form of a compile error.
#define TOO_COMPILETIME_TYPE_NAME(objectOfType) decltype(objectOfType)::type_of_##objectOfType##_is = {};

/** \tparam T required to be any type you want to print the name of.
    \return name of the type T.
    \see https://stackoverflow.com/a/20170989/636486*/
template <typename T>
constexpr std::string_view type_name()
{
    using std::string_view;
    string_view p = TOO_FUNCTIONSIGN;
#if TOO_COMP_CLANG
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif TOO_COMP_GNU_CPP
#if TOO_LANG_STANDARD_CPP < TOO_LANG_STANDARD_CPP14
    return string_view(p.data() + 36, p.size() - 36 - 1);
#else
    throw too::not_implemented{"type_name"};
    //return string_view(p.data() + 49, p.find(';', 49) - 49);
#endif
#elif defined(_MSC_VER)
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}
} // namespace too

#include "toolib/macros_end.h"

#endif
