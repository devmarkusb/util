// Markus Borris, 2018-19
// This file is part of tfl library.

//!
/** Implements the most simple (but perhaps also most efficient) linear memory allocation arena/strategy.
    The idea is that physical allocation and deallocation happen at the beginning and the end only, but nowhere in
    between.
    Important notes:
    - For now the class should only be used for allocating objects of a single type. For performance reasons,
    as there is no alignment handling after the preallocation anymore.
    - You should be aware that deallocate here does nothing (except for corner cases when deallocating
    a memory range allocated immediately before). So you will use up a lot more memory than you think, if you are
    erasing a lot. Since erasing doesn't decrease the counter of memory used.*/
//! \file

#ifndef LINEAR_H_jsdkdbhfzu34gt2837tnyg13
#define LINEAR_H_jsdkdbhfzu34gt2837tnyg13

#include "toolib/assert.h"
#include "toolib/class/non_copyable.h"
#include "toolib/mem/alloc/statistics.h"
#include "toolib/mem/types.h"
#include <cstddef>
#include <new>


namespace too
{
namespace mem
{
namespace alloc
{
template <typename StatisticsPolicy = NoStatistics>
class Linear
    : private too::non_copyable
    , public StatisticsPolicy
{
public:
    explicit Linear(Bytes capacity, Bytes alignment = Bytes{__STDCPP_DEFAULT_NEW_ALIGNMENT__})
        : alignment_{static_cast<std::align_val_t>(alignment.value)}
        , buf_{static_cast<uint8_t*>(capacity ? ::operator new(capacity.value, alignment_) : nullptr)}
        , curr_offset_{}
        , capacity_{capacity}
    {
    }

    ~Linear()
    {
        ::operator delete(buf_, alignment_);
    }

    Bytes size() const noexcept
    {
        return curr_offset_;
    }

    /** If you know for sure you will never need to access a certain known amount size of the latest
        previously allocated memory again, you can shrink (only the shrinking direction makes sense) the used up
        memory to the original size again. This amounts to more capacity being left over to allocate.*/
    void resize(Bytes size) noexcept
    {
        TOO_EXPECT(size <= curr_offset_);

        curr_offset_ = size;
    }

    void reset() noexcept
    {
        resize({});
    }

    uint8_t* allocate(Bytes size)
    {
        const auto old_offset = curr_offset_;
        const auto new_offset = curr_offset_ + size;

        if (new_offset > capacity_)
        {
            throw std::bad_alloc{};
        }

        curr_offset_ = new_offset;

        this->statsCollect_currentSize(curr_offset_);

        return buf_ + old_offset.value;
    }

    void deallocate(uint8_t* p, Bytes size) noexcept
    {
        if (p + size.value == buf_ + curr_offset_.value)
            curr_offset_ -= size;
    }

private:
    std::align_val_t alignment_{};
    uint8_t* buf_{};
    Bytes curr_offset_{};
    Bytes capacity_{};
};
} // namespace alloc
} // namespace mem
} // namespace too
#endif
