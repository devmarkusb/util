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

#ifndef CIRCULAR_BUFFER_H_GFHCIO489CGHX4M9GH39U8HX3GH
#define CIRCULAR_BUFFER_H_GFHCIO489CGHX4M9GH39U8HX3GH

#include "../assert.h"
#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

namespace mb::ul {
namespace detail::circbuf_impl_container {
template <typename T, size_t static_capacity>
class Array {
protected:
    std::array<T, static_capacity> buf_{};

    template <typename U, typename Buffer>
    void emplace(U&& item, Buffer&& buf, size_t head) noexcept {
        static_assert(std::is_convertible_v<U, T>);

        buf[head] = std::forward<U>(item);
    }
};

template <typename T>
class Vector {
protected:
    std::vector<T> buf_;
    size_t capacity_{};

    explicit Vector(size_t capacity)
        : capacity_{capacity} {
        UL_EXPECT(capacity > 0);

        buf_.resize(capacity);
    }

    template <typename U, typename Buffer>
    void emplace(U&& item, Buffer&& buf, size_t head) {
        static_assert(std::is_convertible_v<U, T>);

        auto it = std::begin(buf);
        std::advance(it, head);
        buf.emplace(it, std::forward<U>(item));
    }
};

template <typename T, size_t static_capacity>
using Base = std::conditional_t<static_capacity == 0, Vector<T>, Array<T, static_capacity>>;
} // namespace detail::circbuf_impl_container

/** If you know the (always fixed) capacity of the circular buffer at compile time you should definitely prefer
    to pass it as template staticCapacity != 0. Then there is only a default constructor (no parameters).
    Otherwise for a capacity only known as soon as runtime, you keep staticCapacity at 0 and pass the desired
    capacity as constructor parameter.*/
template <typename T, size_t static_capacity = 0>
class CircularBuffer : private detail::circbuf_impl_container::Base<T, static_capacity> {
public:
    using Base = detail::circbuf_impl_container::Base<T, static_capacity>;

    //! Expects capacity > 0.
    template <typename = std::enable_if<static_capacity == 0>>
    explicit CircularBuffer(size_t capacity)
        : Base{capacity} {
    }

    CircularBuffer() noexcept = default;

    template <typename U>
    void push(U&& item) noexcept {
        static_assert(std::is_convertible_v<U, T>);

        if (full_)
            tail_ = (tail_ + 1) % capacity();

        Base::buf_[head_] = std::forward<U>(item); // NOLINT

        head_ = (head_ + 1) % capacity();

        full_ = head_ == tail_;
    }

    //! Don't use! Doesn't work yet.
    /** (At least not working in the 'dynamic' staticCapacity == 0 case, whereas in the 'static' capacity != 0 case
        there is no difference to push.)*/
    template <typename U>
    void emplace(U&& item) {
        static_assert(std::is_convertible_v<U, T>);

        if (full_)
            tail_ = (tail_ + 1) % capacity();

        Base::emplace(std::forward<U>(item), Base::buf_, head_);

        head_ = (head_ + 1) % capacity();

        full_ = head_ == tail_;
    }

    bool try_front(T& front_item) noexcept {
        if (empty())
            return false;

        front_item = Base::buf_[tail_];
        return true;
    }

    bool try_pop(T& popped_item) noexcept {
        if (empty())
            return false;

        popped_item = Base::buf_[tail_]; // NOLINT
        full_ = false;
        tail_ = (tail_ + 1) % capacity();

        return true;
    }

    void reset() noexcept {
        head_ = tail_;
        full_ = false;
    }

    [[nodiscard]] bool empty() const noexcept {
        return (!full_ && (head_ == tail_));
    }

    [[nodiscard]] bool full() const noexcept {
        return full_;
    }

    [[nodiscard]] constexpr size_t capacity() const noexcept {
        if constexpr (static_capacity > 0)
            return static_capacity;
        else
            return Base::capacity_;
    }

    [[nodiscard]] size_t size() const noexcept {
        size_t size = capacity();

        if (!full_) {
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
} // namespace mb::ul

#endif
