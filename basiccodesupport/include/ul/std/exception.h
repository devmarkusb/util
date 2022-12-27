// 2019-22

//! \file

#ifndef EXCEPTION_H_afsfjdig3478tg4
#define EXCEPTION_H_afsfjdig3478tg4

#include <exception>

namespace mb::ul::exception
{
//! This class can e.g. be used to capture exceptions in (OpenMP) threads and rethrow them afterwards.
class Capture final
{
public:
    //! Construct the object with the code that may throw an exception but shouldn't yet.
    template <typename Callable, typename... Args>
    explicit Capture(Callable&& f, Args... args) noexcept
    {
        try
        {
            f(args...);
        }
        catch (...)
        {
            captureException();
        }
    }

    //! Call this method later when it's safe to throw. If an exception was caught, it will be rethrown.
    /** Note that you can rethrow it again and again, no reset is done.*/
    void rethrow() const
    {
        if (exceptionPtr_)
            std::rethrow_exception(exceptionPtr_);
    }

private:
    void captureException() noexcept
    {
        exceptionPtr_ = std::current_exception();
    }

    std::exception_ptr exceptionPtr_;
};
} // namespace mb::ul::exception

#endif
