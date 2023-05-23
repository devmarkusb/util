//! \file

#ifndef PT_R2_REF_H_SJHDXFGGBERZ38F2G36YGFN123F9V

#include "assert.h"

namespace mb::ul {
//! Converts pointer to reference.
template <typename T>
inline T& ptr2ref(T* p_t) {
    UL_EXPECT(p_t);
    return *p_t;
}

//! Const version of ptr2ref().
template <typename T>
inline const T& ptr2refc(const T* p_t) {
    UL_EXPECT(p_t);
    return *p_t;
}

//! Converts reference to pointer.
//!@{

template <typename T>
T* ref2ptr(T& t) {
    return &t;
}

template <typename T>
T* ref2ptr(T* t) {
    return t;
}

template <typename T>
const T* ref2ptrc(const T& t) {
    return &t;
}

template <typename T>
const T* ref2ptrc(const T* t) {
    return t;
}

//!@}
} // namespace mb::ul

#endif
