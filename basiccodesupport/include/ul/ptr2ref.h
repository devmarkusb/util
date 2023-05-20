//! \file

#ifndef PT_R2_REF_H_SJHDXFGGBERZ38F2G36YGFN123F9V

#include "assert.h"

namespace mb::ul {
//! Converts pointer to reference.
template <typename T>
inline T& ptr2_ref(T* pT) {
    UL_EXPECT(pT);
    return *pT;
}

//! Const version of ptr2Ref().
template <typename T>
inline const T& ptr2_ref_c(const T* pT) {
    UL_EXPECT(pT);
    return *pT;
}

//! Converts reference to pointer.
//!@{

template <typename T>
T* ref2_ptr(T& t) {
    return &t;
}

template <typename T>
T* ref2_ptr(T* t) {
    return t;
}

template <typename T>
const T* ref2_ptr_c(const T& t) {
    return &t;
}

template <typename T>
const T* ref2_ptr_c(const T* t) {
    return t;
}

//!@}
} // namespace mb::ul

#endif
