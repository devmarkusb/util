// Markus Borris, 2019
// This file is part of tfl library.

//! Circular buffer structure(s).
/** They make a perfect replacement for queue if the needed capacity is known beforehand.
    The behavior for further pushes in case of a full buffer is simple overwriting of the oldest elements.
    There are two kinds of implementations provided:
        1) CircularBuffer<T>, which dynamically allocates the buffer memory (although only once of the capacity
        passed to the constructor then - like a vector::resize/reserve), and
        2) CircularBuffer<T, size_t> which does it statically (on stack ideally) with the capacity passed as template
        parameter (without the constructor expecting any arguments).
    The latter obviously should be preferred if the needed capacity is known at compile time.*/
//! \file

#ifndef CIRCULAR_BUFFER_H_gfhcio489cghx4m9gh39u8hx3gh
#define CIRCULAR_BUFFER_H_gfhcio489cghx4m9gh39u8hx3gh

#include "toolib/assert.h"
#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>


namespace too
{
namespace detail
{
namespace circbuf_impl_container
{
template <typename T, size_t capacity__>
class Array
{
protected:
    std::array<T, capacity__> buf_;

    template <typename T_, typename Buffer>
    void emplace(T_&& item, Buffer&& buf, size_t head) noexcept
    {
        static_assert(std::is_convertible_v<T_, T>);

        buf[head] = std::forward<T_>(item);
    }
};

template <typename T>
class Vector
{
protected:
    std::vector<T> buf_;
    size_t capacity_{};

    explicit Vector(size_t capacity)
        : capacity_{capacity}
    {
        TOO_EXPECT(capacity > 0);

        buf_.resize(capacity);
    }

    template <typename T_, typename Buffer>
    void emplace(T_&& item, Buffer&& buf, size_t head)
    {
        static_assert(std::is_convertible_v<T_, T>);

        auto it = std::begin(buf);
        std::advance(it, head);
        buf.emplace(it, std::forward<T_>(item));
    }
};

template <typename T, size_t capacity__>
using Base = std::conditional_t<capacity__ == 0, Vector<T>, Array<T, capacity__>>;
} // namespace circbuf_impl_container
} // namespace detail


/** If you know the (always fixed) capacity of the circular buffer at compile time you should definitely prefer
    to pass it as template \param capacity__ != 0. Then there is only a default constructor (no parameters).
    Otherwise for a capacity only known as soon as runtime, you keep \param capacity__ at 0 and pass the desired
    capacity as constructor parameter.*/
template <typename T, size_t capacity__ = 0>
class CircularBuffer : private detail::circbuf_impl_container::Base<T, capacity__>
{
public:
    using Base = detail::circbuf_impl_container::Base<T, capacity__>;

    //! Expects \param capacity > 0.
    template <typename = std::enable_if<capacity__ == 0>>
    explicit CircularBuffer(size_t capacity)
        : Base{capacity}
    {
    }

    template <typename = std::enable_if<capacity__ != 0>>
    CircularBuffer()
    {
    } // = default, not working here

    template <typename T_>
    void push(T_&& item) noexcept
    {
        static_assert(std::is_convertible_v<T_, T>);

        if (full_)
            tail_ = (tail_ + 1) % capacity();

        Base::buf_[head_] = std::forward<T_>(item);

        head_ = (head_ + 1) % capacity();

        full_ = head_ == tail_;
    }

    //! Don't use! Doesn't work yet.
    /** (At least not working in the 'dynamic' capacity__ == 0 case, whereas in the 'static' capacity != 0 case
        there is no difference to push.)*/
    template <typename T_>
    void emplace(T_&& item)
    {
        static_assert(std::is_convertible_v<T_, T>);

        if (full_)
            tail_ = (tail_ + 1) % capacity();

        Base::emplace(std::forward<T_>(item), Base::buf_, head_);

        head_ = (head_ + 1) % capacity();

        full_ = head_ == tail_;
    }

    bool try_front(T& frontItem) noexcept
    {
        if (empty())
            return false;

        frontItem = Base::buf_[tail_];
        return true;
    }

    bool try_pop(T& poppedItem) noexcept
    {
        if (empty())
            return false;

        poppedItem = Base::buf_[tail_];
        full_ = false;
        tail_ = (tail_ + 1) % capacity();

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

    constexpr size_t capacity() const noexcept
    {
        if constexpr (capacity__ > 0)
            return capacity__;
        else
            return Base::capacity_;
    }

    size_t size() const noexcept
    {
        size_t size = capacity();

        if (!full_)
        {
            if (head_ >= tail_)
                size = head_ - tail_;
            else
                size = capacity() + head_ - tail_;
        }

        return size;
    }

private:
    size_t head_{0};
    size_t tail_{0};
    bool full_{};
};
} // namespace too
#endif
