//! \file

#ifndef PTR2REF_H_sjhdxfggberz38f2g36ygfn123f9v
#define PTR2REF_H_sjhdxfggberz38f2g36ygfn123f9v

#include "assert.h"

namespace mb::ul
{
//! Converts pointer to reference.
template <typename T>
inline T& ptr2Ref(T* pT)
{
    UL_EXPECT(pT);
    return *pT;
}

//! Const version of ptr2Ref().
template <typename T>
inline const T& ptr2RefC(const T* pT)
{
    UL_EXPECT(pT);
    return *pT;
}

//! Converts reference to pointer.
//!@{

template <typename T>
T* ref2Ptr(T& t)
{
    return &t;
}

template <typename T>
T* ref2Ptr(T* t)
{
    return t;
}

template <typename T>
const T* ref2PtrC(const T& t)
{
    return &t;
}

template <typename T>
const T* ref2PtrC(const T* t)
{
    return t;
}

//!@}
} // namespace mb::ul

#endif
