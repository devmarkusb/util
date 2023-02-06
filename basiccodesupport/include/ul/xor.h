//! \file

#ifndef XOR_H_eruighx784zug84bvg283bvgsadf
#define XOR_H_eruighx784zug84bvg283bvgsadf

#include <cstddef>

namespace mb::ul
{
//! Equivalence. True if and only if both are equally true or false.
template <typename T1, typename T2>
bool XNOR(const T1& lhs, const T2& rhs)
{
    return !lhs == !rhs;
}

//! Impl. note: some compilers complain not converting nullptr_t to bool easily, that's why
//! we use that strange workaround.
//!@{
template <typename T1>
bool XNOR(const T1& lhs, const std::nullptr_t& /*pass_nullptr*/)
{
    return !lhs;
}

template <typename T2>
bool XNOR(const std::nullptr_t& /*pass_nullptr*/, const T2& rhs)
{
    return !rhs;
}

//!@}

template <>
inline bool XNOR(const bool& lhs, const bool& rhs)
{
    return lhs == rhs;
}

//! Exclusive or.
template <typename T1, typename T2>
bool XOR(const T1& lhs, const T2& rhs)
{
    return !XNOR(lhs, rhs);
}

template <>
inline bool XOR(const bool& lhs, const bool& rhs)
{
    return !XNOR(lhs, rhs);
}

} // namespace mb::ul

#endif
