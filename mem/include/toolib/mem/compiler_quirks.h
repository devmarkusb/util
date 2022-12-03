// 2019

//! Specialities that were observed to be compiler implementation specific.
/**
 */
//! \file

#ifndef COMPILER_QUIRKS_H_45ntx7358732t3hx783tm4
#define COMPILER_QUIRKS_H_45ntx7358732t3hx783tm4

#include "toolib/mem/types.h"

#include "toolib/macros.h"


namespace too::mem::quirk
{
namespace vector
{
/** Quite 'funny' that msvc allocates a certain amount when constructing a vector,
    at least capacity still being zero. But, no offense at all: this is debug-only
    behavior*/
constexpr too::mem::Bytes constr_heap_alloc_size
{
#if TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_DEBUG
    16
#else
    0
#endif
};
} // namespace vector
} // namespace too::mem::quirk

#include "toolib/macros_end.h"

#endif
