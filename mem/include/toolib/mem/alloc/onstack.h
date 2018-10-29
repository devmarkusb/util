// Markus Borris, 2018
// This file is part of tfl library.

//!
/** Implements allocation on the stack as arena/strategy.*/
//! \file

#ifndef ONSTACK_H_eourtz3478xth378tgh
#define ONSTACK_H_eourtz3478xth378tgh

#include "toolib/assert.h"
#include "toolib/class/non_copyable.h"
#include "toolib/mem/types.h"
#include "toolib/mem/utility.h"
#include <cstddef>
#include <cstdint>


namespace too
{
namespace mem
{
namespace alloc
{
/** You should adapt @tparam max_alignment_in_bytes to at least the alignment of the largest type you're going to
    allocate frequently. But not larger as this wastes space due to padding.*/
template <size_t capacity_in_bytes, size_t max_alignment_in_bytes>
class OnStack : private too::non_copyable
{
public:
    static_assert(capacity_in_bytes % max_alignment_in_bytes == 0, "capacity needs to be a multiple of alignment");

    uint8_t* allocate(Bytes size)
    {
        const auto padded_size = pad_up(size.value, max_alignment_in_bytes);
        if (static_cast<decltype(padded_size)>(buf_ + capacity_in_bytes - curr_memptr_) < padded_size)
            throw std::bad_alloc{};

        auto old_memptr = curr_memptr_;
        curr_memptr_ += padded_size;
        return old_memptr;
    }

    void deallocate(uint8_t* p, Bytes size) noexcept
    {
        size.value = pad_up(size.value, max_alignment_in_bytes);
        if (p + size.value == curr_memptr_)
            curr_memptr_ = p;
    }

    static constexpr Bytes capacity() noexcept { return Bytes{capacity_in_bytes}; }
    Bytes size() const noexcept { return Bytes{static_cast<size_t>(curr_memptr_ - buf_)}; }
    void reset() noexcept { curr_memptr_ = buf_; }

private:
    alignas(max_alignment_in_bytes) uint8_t buf_[capacity_in_bytes];
    uint8_t* curr_memptr_{buf_};
};
} // alloc
} // mem
} // too
#endif