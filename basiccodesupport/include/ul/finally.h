//! \file Taken from / inspired by the Cpp Guidelines Support Library GSL.

#ifndef FINALLY_H_SLKHNUIGCHERX8G3H54XX7TH24XT3NX
#define FINALLY_H_SLKHNUIGCHERX8G3H54XX7TH24XT3NX

#include "std/std_extensions.h"

namespace mb::ul {
//! It's not so convenient to use this directly; call the associated helper function instead.
template <class F>
class FinalAct {
public:
    explicit FinalAct(F f) noexcept
        : f_{std::move(f)} {
    }

    FinalAct(FinalAct&& other) noexcept
        : f_{std::move(other.f_)}
        , invoke_{other.invoke_} {
        other.invoke_ = false;
    }

    FinalAct(const FinalAct&) = delete;
    FinalAct& operator=(const FinalAct&) = delete;

    ~FinalAct() noexcept {
        if (this->invoke_)
            this->f_();
    }

private:
    F f_;
    bool invoke_{true};
};

//! Most conveniently called with a lambda.
/** Usage:
\code
{
        auto do_sth_finally = finally([&](){act();});
        // other stuff
} // here act() is called
\endcode
Note that you need to assign the return value to a local variable, otherwise only
a temporary is created, which gets destructed immediately. The mechanism is
obviously connected to the lifetime of the variable assigned to.
*/
template <class F>
inline FinalAct<F> finally(const F& f) noexcept {
    return FinalAct<F>(f);
}

//! Cf. finally() above.
template <class F>
inline FinalAct<F> finally(F&& f) noexcept {
    return FinalAct<F>(std::forward<F>(f));
}
} // namespace mb::ul

#endif
