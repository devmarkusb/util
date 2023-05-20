//! \file

#ifndef LEX_CAST_H_SOPAIEM3P2OIUM32OMO
#define LEX_CAST_H_SOPAIEM3P2OIUM32OMO

#include "ul/config.h"
#include "ul/macros.h"
#include <exception>
#include <sstream>

namespace mb::ul {
class ErrBadLexCast : public std::bad_cast {};

//! Converts 'arbitrary' types from or to std::string. There is also the non-throwing version lex_cast.
/** It is even possible to cast from float or int or such. Although one might
    need to be sceptic about the use-case of trying such conversions the
    lexicographical way. They also would need to be fail-safe input-wise,
    that is 42.1f doesn't successfully convert to 42 or whatever.*/
template <typename T, typename S>
T lex_cast_throw(const S& s) {
    std::stringstream strs;
    strs << s;
    T t;
    strs >> t;
    if (strs)
        return t;
    else
        throw ErrBadLexCast();
}

template <typename T, typename S>
T lex_cast(const S& s) noexcept {
    try {
        return lex_cast_throw<T, S>(s);
    } catch (const ErrBadLexCast&) {
        UL_NOOP;
    }
    return T{};
}
} // namespace mb::ul

UL_HEADER_END

#endif
