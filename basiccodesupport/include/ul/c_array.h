//! \file

#ifndef C_ARRAY_H_lnm45hxn73rmnfy2738yrn873rg
#define C_ARRAY_H_lnm45hxn73rmnfy2738yrn873rg

#include "assert.h"

namespace mb::ul
{
/**
Usage:
\code
int x[5] = {};
for (size_t i = 0; i < ul::c_array_length(x); ++i)
    ; // do sth
\endcode
*/
// NOLINTBEGIN
template <class T, size_t N>
size_t c_array_length(T (&)[N])
{
    return N;
}

// NOLINTEND
} // namespace mb::ul

#endif
