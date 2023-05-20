//! \file

#ifndef WAITQUEUE_H_EJVHNXG853NH8G2H
#define WAITQUEUE_H_EJVHNXG853NH8G2H

#include <condition_variable>
#include <limits>
#include <mutex>
#include <queue>
#include <type_traits>

namespace mb::ul::thread {
//! A queue that exclusively provides methods that are safe to use in a multi-producer / multi-consumer context.
template <typename T>
class WaitQueue {
public:
    explicit WaitQueue(size_t maxElems = std::numeric_limits<size_t>::max())
        : maxElems_(maxElems) {
    }

    template <typename U>
    bool push(U&& elem) {
        static_assert(std::is_convertible_v<U, T>);

        {
            std::unique_lock<std::mutex> lock(mutex_);

            if (queue_.size() > maxElems_)
                return false;

            queue_.push(std::forward<U>(elem));
        }
        conditionVariable_.notify_one();
        return true;
    }

    template <typename U>
    bool emplace(U&& elem) {
        static_assert(std::is_convertible_v<U, T>);

        {
            const std::unique_lock<std::mutex> lock(mutex_);

            if (queue_.size() >= maxElems_)
                return false;

            queue_.emplace(std::forward<U>(elem));
        }
        conditionVariable_.notify_one();
        return true;
    }

    //! \return false if the queue has been stopped (this interrupts waiting even if the queue is empty).
    bool wait_and_pop(T& poppedElem) {
        std::unique_lock<std::mutex> lk(mutex_);
        while (queue_.empty() && !stopped_)
            conditionVariable_.wait(lk);

        if (stopped_)
            return false;

        poppedElem = std::move(queue_.front());
        queue_.pop();

        return true;
    }

    void stop() {
        const std::unique_lock<std::mutex> lock(mutex_);
        stopped_ = true;
        conditionVariable_.notify_all();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable conditionVariable_;
    size_t maxElems_{};
    std::queue<T> queue_;
    bool stopped_{false};
};
} // namespace mb::ul::thread

#endif
