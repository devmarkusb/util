// Markus Borris, 2014
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef VECTOR_H_sixe38grf38gxfn34x8gfn4
#define VECTOR_H_sixe38grf38gxfn34x8gfn4

#include <stddef.h>
#include <vector>


namespace too
{
namespace vector
{
//! Only makes sense for dst not within [start, start + length).
/** Whereas dst == start + length (also, for this function) means doing nothing.*/
template <typename T>
inline void move_range(size_t start, size_t length, size_t dst, std::vector<T>& v)
{
    const size_t end = start + length;
    if (dst >= start && dst <= end)
        return;
    if (end > v.size())
        return;
    const size_t final_dst = dst > start ? dst - length : dst;
    const size_t orig_size = v.size();
    std::vector<T> tmp(std::make_move_iterator(v.begin() + start), std::make_move_iterator(v.begin() + start + length));
    v.erase(v.begin() + start, v.begin() + start + length);
    if (dst >= orig_size)
        v.insert(v.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
    else
        v.insert(v.begin() + final_dst, std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
}
} // namespace vector
} // namespace too

#endif
