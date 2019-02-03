// Markus Borris, 2019
// This file is part of tfl library.

//! Circular buffer structure(s).
/** They make a perfect replacement for queue if the needed capacity is known beforehand.
    The behavior for further pushes in case of a full buffer is simple overwriting of the oldest elements.
    There are two kinds of implementations provided: CircularBuffer, which dynamically allocates the
    buffer memory, and CircularBuffer_static which does it statically (on stack ideally). The latter
    obviously should be preferred if the needed capacity is known at compile time.*/
//! \file

#ifndef CIRCULAR_BUFFER_H_gfhcio489cghx4m9gh39u8hx3gh
#define CIRCULAR_BUFFER_H_gfhcio489cghx4m9gh39u8hx3gh

#include "toolib/assert.h"
#include <array>
#include <cstddef>
#include <memory>
#include <vector>


namespace too
{
namespace detail
{
template <typename T>
class CircularBuffer_common_impl
{
protected:
    template <typename T_, typename Buffer>
    void push(T_&& item, Buffer&& buf, size_t capacity) noexcept
    {
        if (full_)
            tail_ = (tail_ + 1) % capacity;

        buf[head_] = std::forward<T_>(item);

        head_ = (head_ + 1) % capacity;

        full_ = head_ == tail_;
    }

    //! Don't use, not yet working correctly.
    template <typename T_, typename Buffer>
    void emplace(T_&& item, Buffer&& buf, size_t capacity) noexcept
    {
        if (full_)
            tail_ = (tail_ + 1) % capacity;

        auto it = std::begin(buf);
        std::advance(it, head_);
        buf.emplace(it, std::forward<T_>(item));

        head_ = (head_ + 1) % capacity;

        full_ = head_ == tail_;
    }

    template <typename Buffer>
    bool try_pop(T& poppedItem, Buffer&& buf, size_t capacity) noexcept
    {
        if (empty())
            return false;

        poppedItem = buf[tail_];
        full_      = false;
        tail_      = (tail_ + 1) % capacity;

        return true;
    }

    void reset() noexcept
    {
        head_ = tail_;
        full_ = false;
    }

    bool empty() const noexcept
    {
        return (!full_ && (head_ == tail_));
    }

    bool full() const noexcept
    {
        return full_;
    }

    size_t size(size_t capacity) const noexcept
    {
        size_t size = capacity;

        if (!full_)
        {
            if (head_ >= tail_)
                size = head_ - tail_;
            else
                size = capacity + head_ - tail_;
        }

        return size;
    }

private:
    size_t head_{0};
    size_t tail_{0};
    bool full_{};
};
} // detail

template <typename T>
class CircularBuffer : private detail::CircularBuffer_common_impl<T>
{
public:
    using Base = detail::CircularBuffer_common_impl<T>;

    //! Expects \param capacity > 0.
    explicit CircularBuffer(size_t capacity) : capacity_(capacity)
    {
        TOO_EXPECT(capacity > 0);

        buf_.resize(capacity);
    }

    template <typename T_>
    void push(T_&& item) noexcept
    {
        Base::push(std::forward<T_>(item), buf_, capacity_);
    }

    //! Don't use, not yet working correctly.
    template <typename T_>
    void emplace(T_&& item) noexcept
    {
        Base::emplace(std::forward<T_>(item), buf_, capacity_);
    }

    bool try_pop(T& poppedItem) noexcept
    {
        return Base::try_pop(poppedItem, buf_, capacity_);
    }

    //void reset() noexcept
    using Base::reset;

    //bool empty() const noexcept
    using Base::empty;

    //bool full() const noexcept
    using Base::full;

    size_t capacity() const noexcept { return capacity_; }

    size_t size() const noexcept
    {
        return Base::size(capacity_);
    }

private:
    std::vector<T> buf_;
    size_t capacity_{};
};


//! Expects \param capacity > 0. The "_static" means static allocation.
template <typename T, size_t capacity_>
class CircularBuffer_static : private detail::CircularBuffer_common_impl<T>
{
public:
    static_assert(capacity_ > 0);

    using Base = detail::CircularBuffer_common_impl<T>;

    template <typename T_>
    void push(T_&& item) noexcept
    {
        Base::push(std::forward<T_>(item), buf_, capacity_);
    }

    //! No difference to push here.
    template <typename T_>
    void emplace(T_&& item) noexcept
    {
        Base::push(std::forward<T_>(item), buf_, capacity_);
    }

    bool try_pop(T& poppedItem) noexcept
    {
        return Base::try_pop(poppedItem, buf_, capacity_);
    }

    //void reset() noexcept
    using Base::reset;

    //bool empty() const noexcept
    using Base::empty;

    //bool full() const noexcept
    using Base::full;

    size_t capacity() const noexcept { return capacity_; }

    size_t size() const noexcept
    {
        return Base::size(capacity_);
    }

private:
    std::array<T, capacity_> buf_;
};
} // too
#endif
