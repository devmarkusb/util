//! \file Specialities that were observed to be compiler implementation specific.

#ifndef COMPILER_QUIRKS_H_45NTX7358732T3HX783TM4
#define COMPILER_QUIRKS_H_45NTX7358732T3HX783TM4

#include "types.h"
#include "ul/macros.h"

namespace mb::ul::mem::quirk::vector {
/** Quite 'funny' that msvc allocates a certain amount when constructing a vector,
    at least capacity still being zero. But, no offense at all: this is debug-only
    behavior*/
constexpr ul::mem::Bytes constr_heap_alloc_size {
#if UL_COMP_MS_VISUAL_STUDIO_CPP && UL_DEBUG
    16
#else
    0
#endif
};
} // namespace mb::ul::mem::quirk::vector

#endif
