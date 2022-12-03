// 2018-19

//!
/** Implements allocation on the stack as arena/strategy.
    Important note:
    You should be aware that deallocate here does nothing (except for corner cases when deallocating
    a memory range allocated immediately before). So you will use up a lot more memory than you think, if you are
    erasing a lot. Since erasing doesn't decrease the counter of memory used.*/
//! \file

#ifndef ONSTACK_H_eourtz3478xth378tgh
#define ONSTACK_H_eourtz3478xth378tgh

#include "toolib/assert.h"
#include "toolib/class/non_copyable.h"
#include "toolib/mem/alloc/statistics.h"
#include "toolib/mem/types.h"
#include "toolib/mem/utility.h"
#include "toolib/warnings.h"
#include <cstddef>
#include <cstdint>


namespace too::mem::alloc
{
/** You should adapt max_alignment_in_bytes to at least the alignment of the largest type you're going to
    allocate frequently. But not larger as this wastes space due to padding.*/
template <size_t capacity_in_bytes, size_t max_alignment_in_bytes, typename StatisticsPolicy = NoStatistics>
class OnStack
    : private too::non_copyable
    , public StatisticsPolicy
{
public:
    uint8_t* allocate(Bytes size)
    {
        const auto padded_size = pad_up(size.value, max_alignment_in_bytes);
        if (static_cast<decltype(padded_size)>(buf_ + capacity_in_bytes - curr_memptr_) < padded_size)
            throw std::bad_alloc{};

        auto old_memptr = curr_memptr_;
        curr_memptr_ += padded_size;

        this->statsCollect_currentSize(this->size());

        return old_memptr;
    }

    void deallocate(uint8_t* p, Bytes size) noexcept
    {
        size.value = pad_up(size.value, max_alignment_in_bytes);
        if (p + size.value == curr_memptr_)
            curr_memptr_ = p;
    }

    static constexpr Bytes capacity() noexcept
    {
        return Bytes{capacity_in_bytes};
    }
    Bytes size() const noexcept
    {
        return Bytes{static_cast<size_t>(curr_memptr_ - buf_)};
    }
    void reset() noexcept
    {
        curr_memptr_ = buf_;
    }

private:
    TOO_PRAGMA_WARNINGS_PUSH
    TOO_WARNING_DISABLE_MSVC(4324) // structure was padded due to __declspec(align())
    alignas(max_alignment_in_bytes) uint8_t buf_[capacity_in_bytes];
    TOO_PRAGMA_WARNINGS_POP
    uint8_t* curr_memptr_{buf_};
};
} // namespace too::mem::alloc

#endif
