//! \file

#ifndef TYPE_NAME_H_4387t73824t378t371y47ym47
#define TYPE_NAME_H_4387t73824t378t371y47ym47

#include "debug.h"
#include "error.h"
#include "ul/macros.h"
#include <string_view>

namespace mb::ul {
//! Prints type of variable objectOfType in the form of a compile error.
#define UL_COMPILETIME_TYPE_NAME(objectOfType) decltype(objectOfType)::type_of_##objectOfType##_is = {};

//todo
/** WARNING: Don't use, this doesn't work anymore.
    \tparam T required to be any type you want to print the name of.
    \return name of the type T.
    \see https://stackoverflow.com/a/20170989/636486*/
template <typename T>
constexpr std::string_view typeName() {
    using std::string_view;
    const string_view p{UL_FUNCTIONSIGN}; // NOLINT
#if UL_COMP_CLANG
    return {p.data() + 34, p.size() - 34 - 1}; // NOLINT // magic number
#elif UL_COMP_GNU_CPP
#if UL_LANG_STANDARD_CPP < UL_LANG_STANDARD_CPP14
    return string_view(p.data() + 36, p.size() - 36 - 1);
#else
    throw ul::not_implemented{"type_name"};
    //return string_view(p.data() + 49, p.find(';', 49) - 49);
#endif
#elif defined(_MSC_VER)
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}
} // namespace mb::ul

#endif
