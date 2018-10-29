// Markus Borris, 2018
// This file is part of tfl library.

//!
/** Implements the most simple (but perhaps also most efficient) linear memory allocation arena/strategy.
    The idea is that physical allocation and deallocation happen at the beginning and the end only, but nowhere in
    between.*/
//! \file

#ifndef LINEAR_H_jsdkdbhfzu34gt2837tnyg13
#define LINEAR_H_jsdkdbhfzu34gt2837tnyg13

#include "toolib/assert.h"
#include "toolib/mem/types.h"
#include "toolib/class/non_copyable.h"
#include <cstddef>
#include <new>


namespace too
{
namespace mem
{
namespace alloc
{
class Linear : private too::non_copyable
{
public:
    //! This is *not* done automatically on construction.
    void preallocate(Bytes capacity, Bytes alignment)
    {
        TOO_ASSERT(!capacity_);
        TOO_ASSERT(!buf_);
        TOO_ASSERT(!curr_offset_);

        capacity_ = capacity;
        buf_ = static_cast<uint8_t*>(
                ::operator new(capacity.value, static_cast<std::align_val_t>(alignment.value)));
        curr_offset_ = Bytes{0};
    }

    //! This would also be done automatically on destruction.
    void free() noexcept
    {
        ::operator delete(buf_);
        capacity_ = Bytes{0};
        buf_ = nullptr;
        curr_offset_ = Bytes{0};
    }

    ~Linear()
    {
        free();
    }

    Bytes size() const noexcept
    {
        return curr_offset_;
    }

    /** If you know you will never need to access the previously, since @param size, allocated memory again,
        you can shrink (only the shrinking direction makes sense) the used up memory to the original size again.
        This amounts to more capacity being left over to allocate.*/
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

        if (new_offset > capacity_) {
            throw std::bad_alloc{};
        }

        curr_offset_ = new_offset;

        return buf_ + old_offset.value;
    }

    void deallocate(uint8_t*, Bytes size) noexcept
    {
        TOO_EXPECT(size <= curr_offset_);

        curr_offset_ -= size;
    }

private:
    uint8_t* buf_{};
    Bytes curr_offset_{};
    Bytes capacity_{};
};
} // alloc
} // mem
} // too
#endif
