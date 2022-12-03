// 2014

//!
/**
 */
//! \file

#ifndef PTR2REF_H_sjhdxfggberz38f2g36ygfn123f9v
#define PTR2REF_H_sjhdxfggberz38f2g36ygfn123f9v

#include "ul/assert.h"


namespace mb::ul
{
//! Converts pointer to reference.
template <typename T>
inline T& Ptr2Ref(T* pT)
{
    UL_EXPECT(pT);
    return *pT;
}

//! Const version of Ptr2Ref().
template <typename T>
inline const T& Ptr2RefC(const T* pT)
{
    UL_EXPECT(pT);
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
} // namespace mb::ul

#endif
