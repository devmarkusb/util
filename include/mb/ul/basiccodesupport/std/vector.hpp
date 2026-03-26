//! \file

#ifndef VECTOR_H_SIXE38GRF38GXFN34X8GFN4
#define VECTOR_H_SIXE38GRF38GXFN34X8GFN4

#include <cstddef>
#include <vector>

namespace mb::ul::vector {
//! Only makes sense for dst not within [start, start + length).
/** Whereas dst == start + length (also, for this function) means doing nothing.*/
template <typename T>
void move_range(size_t start, size_t length, size_t dst, std::vector<T>& v) {
    const size_t end = start + length;
    if (dst >= start && dst <= end)
        return;
    if (end > v.size())
        return;
    const size_t final_dst = dst > start ? dst - length : dst;
    const size_t orig_size = v.size();
    std::vector<T> tmp(
        std::make_move_iterator(v.begin() + static_cast<ptrdiff_t>(start)),
        std::make_move_iterator(v.begin() + static_cast<ptrdiff_t>(start + length)));
    v.erase(v.begin() + static_cast<ptrdiff_t>(start), v.begin() + static_cast<ptrdiff_t>(start + length));
    if (dst >= orig_size)
        v.insert(v.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
    else
        v.insert(
            v.begin() + static_cast<ptrdiff_t>(final_dst), std::make_move_iterator(tmp.begin()),
            std::make_move_iterator(tmp.end()));
}
} // namespace mb::ul::vector

#endif
