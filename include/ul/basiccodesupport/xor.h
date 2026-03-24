//! \file

#ifndef XOR_H_ERUIGHX784ZUG84BVG283BVGSADF
#define XOR_H_ERUIGHX784ZUG84BVG283BVGSADF

#include <cstddef>

namespace mb::ul {
//! Equivalence. True if and only if both are equally true or false.
template <typename T1, typename T2>
bool xnor(const T1& lhs, const T2& rhs) {
    return !lhs == !rhs;
}

//! Impl. note: some compilers complain not converting nullptr_t to bool easily, that's why
//! we use that strange workaround.
//!@{
template <typename T1>
bool xnor(const T1& lhs, const std::nullptr_t& /*pass_nullptr*/) {
    return !lhs;
}

template <typename T2>
bool xnor(const std::nullptr_t& /*pass_nullptr*/, const T2& rhs) {
    return !rhs;
}

//!@}

template <>
inline bool xnor(const bool& lhs, const bool& rhs) {
    return lhs == rhs;
}

//! Exclusive or.
template <typename T1, typename T2>
bool xor_bool(const T1& lhs, const T2& rhs) {
    return !xnor(lhs, rhs);
}

template <>
inline bool xor_bool(const bool& lhs, const bool& rhs) {
    return !xnor(lhs, rhs);
}
} // namespace mb::ul

#endif
