//! \file

#ifndef WAITCIRCBUFFER_H_4Z894527892XH5
#define WAITCIRCBUFFER_H_4Z894527892XH5

#include "../error.h"
#include "../ignore_unused.h"
#include "../std/circular_buffer.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <type_traits>

namespace mb::ul::thread {
namespace detail::waitcircbuf_impl_container {
template <typename T, size_t static_capacity>
class CapacityCompiletime {
protected:
    ul::CircularBuffer<T, static_capacity> buf_;
};

template <typename T>
class CapacityRuntime {
protected:
    ul::CircularBuffer<T> buf_;

    explicit CapacityRuntime(size_t capacity)
        : buf_{capacity} {
        UL_EXPECT(capacity > 0);
    }
};

template <typename T, size_t static_capacity>
using Base = std::conditional_t<static_capacity == 0, CapacityRuntime<T>, CapacityCompiletime<T, static_capacity>>;
} // namespace detail::waitcircbuf_impl_container

//! A queue that exclusively provides methods that are safe to use in a multi-producer / multi-consumer context.
/** Note, unlike the behavior of CircularBuffer this WaitCircularBuffer protects against overwrites.*/
template <typename T, size_t static_capacity = 0>
class WaitCircularBuffer : private detail::waitcircbuf_impl_container::Base<T, static_capacity> {
public:
    using Base = detail::waitcircbuf_impl_container::Base<T, static_capacity>;

    //! Expects capacity > 0.
    template <typename = std::enable_if<static_capacity == 0>>
    explicit WaitCircularBuffer(size_t capacity)
        : Base{capacity} {
    }

    WaitCircularBuffer() noexcept = default;

    template <typename U>
    bool push(U&& elem) {
        static_assert(std::is_convertible_v<U, T>);

        {
            const std::unique_lock<std::mutex> lock(mutex_);

            if (Base::buf_.full())
                return false;

            Base::buf_.push(std::forward<U>(elem));
        }
        condition_variable_.notify_one();
        return true;
    }

    //! Don't use! Doesn't work yet. Implementation of CircularBuffer is the guilty one.
    /** (At least not working in the 'dynamic' staticCapacity == 0 case, whereas in the 'static' capacity != 0 case
        there is no difference to push.)*/
    template <typename U>
    bool emplace(U&& elem) {
        static_assert(std::is_convertible_v<U, T>);

        ul::ignore_unused(std::forward<U>(elem));
        throw NotImplemented{"WaitCircularBuffer::emplace"};
        // {
        //     std::unique_lock<std::mutex> lock(mutex_);
        //
        //     if (Base::buf_.full())
        //         return false;
        //
        //     Base::buf_.emplace(std::forward<U>(elem);
        // }
        // conditionVariable_.notify_one();
        // return true;
    }

    //! \return false if the queue has been stopped (this interrupts waiting even if the queue is empty).
    bool wait_and_pop(T& popped_elem) {
        std::unique_lock<std::mutex> lk(mutex_);
        while (Base::buf_.empty() && !stopped_)
            condition_variable_.wait(lk);

        if (stopped_)
            return false;

        UL_VERIFY(Base::buf_.try_pop(popped_elem));

        return true;
    }

    void stop() {
        const std::unique_lock<std::mutex> lock(mutex_);
        stopped_ = true;
        condition_variable_.notify_all();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return Base::buf_.size();
    }

    constexpr size_t capacity() const noexcept {
        return Base::buf_.capacity();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_variable_;
    bool stopped_{false};
};
} // namespace mb::ul::thread

#endif
