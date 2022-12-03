// 2011

//!
/**
 */
//! \file

#ifndef C_ARRAY_H_lnm45hxn73rmnfy2738yrn873rg
#define C_ARRAY_H_lnm45hxn73rmnfy2738yrn873rg

#include "toolib/assert.h"


namespace too
{
//! Usage:
/**
\code
int x[5] = {};
for (size_t i = 0; i < too::c_array_length(x); ++i)
    ; // do sth
\endcode
*/
template <class T, size_t N>
size_t c_array_length(T (&)[N])
{
    return N;
}
} // namespace too

#endif
