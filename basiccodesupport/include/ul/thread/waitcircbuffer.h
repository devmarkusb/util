// 2019-22

//! \file

#ifndef WAITCIRCBUFFER_H_4z894527892xh5
#define WAITCIRCBUFFER_H_4z894527892xh5

#include "ul/error.h"
#include "ul/ignore_unused.h"
#include "ul/std/circular_buffer.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <type_traits>

namespace mb::ul::thread
{
namespace detail::waitcircbuf_impl_container
{
template <typename T, size_t staticCapacity>
class CapacityCompiletime
{
protected:
    ul::CircularBuffer<T, staticCapacity> buf_;
};

template <typename T>
class CapacityRuntime
{
protected:
    ul::CircularBuffer<T> buf_;

    explicit CapacityRuntime(size_t capacity)
        : buf_{capacity}
    {
        UL_EXPECT(capacity > 0);
    }
};

template <typename T, size_t staticCapacity>
using Base = std::conditional_t<staticCapacity == 0, CapacityRuntime<T>, CapacityCompiletime<T, staticCapacity>>;
} // namespace detail::waitcircbuf_impl_container

//! A queue that exclusively provides methods that are safe to use in a multi-producer / multi-consumer context.
/** Note, unlike the behavior of CircularBuffer this WaitCircularBuffer protects against overwrites.*/
template <typename T, size_t staticCapacity = 0>
class WaitCircularBuffer : private detail::waitcircbuf_impl_container::Base<T, staticCapacity>
{
public:
    using Base = detail::waitcircbuf_impl_container::Base<T, staticCapacity>;

    //! Expects capacity > 0.
    template <typename = std::enable_if<staticCapacity == 0>>
    explicit WaitCircularBuffer(size_t capacity)
        : Base{capacity}
    {
    }

    template <typename = std::enable_if<staticCapacity != 0>>
    WaitCircularBuffer() noexcept
    {
    } // = default, not working here

    template <typename T_>
    bool push(T_&& elem)
    {
        static_assert(std::is_convertible_v<T_, T>);

        {
            std::unique_lock<std::mutex> lock(mutex_);

            if (Base::buf_.full())
                return false;

            Base::buf_.push(std::forward<T_>(elem));
        }
        conditionVariable_.notify_one();
        return true;
    }

    //! Don't use! Doesn't work yet. Implementation of CircularBuffer is the guilty one.
    /** (At least not working in the 'dynamic' staticCapacity == 0 case, whereas in the 'static' capacity != 0 case
        there is no difference to push.)*/
    template <typename T_>
    bool emplace(T_&& elem)
    {
        static_assert(std::is_convertible_v<T_, T>);

        ul::ignore_unused(elem);
        throw ul::not_implemented{"WaitCircularBuffer::emplace"};
#if 0
        {
            std::unique_lock<std::mutex> lock(mutex_);

            if (Base::buf_.full())
                return false;

            Base::buf_.emplace(std::forward<T_>(elem);
        }
        conditionVariable_.notify_one();
        return true;
#endif
    }

    //! \return false if the queue has been stopped (this interrupts waiting even if the queue is empty).
    bool waitAndPop(T& poppedElem)
    {
        std::unique_lock<std::mutex> lk(mutex_);
        while (Base::buf_.empty() && !stopped_)
            conditionVariable_.wait(lk);

        if (stopped_)
            return false;

        UL_VERIFY(Base::buf_.tryPop(poppedElem));

        return true;
    }

    void stop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stopped_ = true;
        conditionVariable_.notify_all();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return Base::buf_.size();
    }

    constexpr size_t capacity() const noexcept
    {
        return Base::buf_.capacity();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable conditionVariable_;
    bool stopped_{false};
};
} // namespace mb::ul::thread

#endif
