//! \file

#ifndef EXCEPTION_H_AFSFJDIG3478TG4
#define EXCEPTION_H_AFSFJDIG3478TG4

#include <exception>

namespace mb::ul::exception {
//! This class can e.g. be used to capture exceptions in (OpenMP) threads and rethrow them afterwards.
class Capture final {
public:
    //! Construct the object with the code that may throw an exception but shouldn't yet.
    template <typename Callable, typename... Args>
    explicit Capture(const Callable& f, Args... args) noexcept {
        try {
            f(args...);
        } catch (...) {
            capture_exception();
        }
    }

    //! Call this method later when it's safe to throw. If an exception was caught, it will be rethrown.
    /** Note that you can rethrow it again and again, no reset is done.*/
    void rethrow() const {
        if (exception_ptr_)
            std::rethrow_exception(exception_ptr_);
    }

private:
    void capture_exception() noexcept {
        exception_ptr_ = std::current_exception();
    }

    std::exception_ptr exception_ptr_;
};
} // namespace mb::ul::exception

#endif
