// Markus Borris, 2014
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef PTR2REF_H_sjhdxfggberz38f2g36ygfn123f9v
#define PTR2REF_H_sjhdxfggberz38f2g36ygfn123f9v

#include "Toolib/assert.h"


namespace too
{
//! Converts pointer to reference.
template <typename T>
inline T& Ptr2Ref(T* pT)
{
    TOO_EXPECT(pT);
    return *pT;
}

//! Const version of Ptr2Ref().
template <typename T>
inline const T& Ptr2RefC(const T* pT)
{
    TOO_EXPECT(pT);
    return *pT;
}


//! Converts reference to pointer.
//!@{

template <typename T>
T* Ref2Ptr(T& t)
{
    return &t;
}

template <typename T>
T* Ref2Ptr(T* t)
{
    return t;
}

template <typename T>
const T* Ref2PtrC(const T& t)
{
    return &t;
}

template <typename T>
const T* Ref2PtrC(const T* t)
{
    return t;
}

//!@}
}

#endif
