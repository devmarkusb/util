// Markus Borris, 2019
// This file is part of tfl library.

//! Specialities that were observed to be compiler implementation specific.
/**
 */
 //! \file

#ifndef COMPILER_QUIRKS_H_45ntx7358732t3hx783tm4
#define COMPILER_QUIRKS_H_45ntx7358732t3hx783tm4

#include"toolib/PPDEFS.h"
#include"toolib/mem/types.h"


namespace too::mem::quirk
{
namespace vector
{
/** Quite 'funny' that msvc allocates a certain amount when constructing a vector,
    at least capacity still being zero.*/
constexpr too::mem::Bytes constr_heap_alloc_size{
#if TOO_COMP_MS_VISUAL_STUDIO_CPP
    16
#else
    0
#endif
};
} // vector
} // too::mem::quirk

#endif