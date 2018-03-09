// Markus Borris, 2017-18
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef XOR_H_eruighx784zug84bvg283bvgsadf
#define XOR_H_eruighx784zug84bvg283bvgsadf


namespace too
{
//! Equivalence. True if and only if both are equally true or false.
template <typename T1, typename T2>
bool XNOR(const T1& lhs, const T2& rhs)
{
    return lhs == rhs;
}

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

} // too

#endif
