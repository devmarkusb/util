//! \file Taken from / inspired by the Cpp Guidelines Support Library GSL.

#ifndef FINALLY_H_slkhnuigcherx8g3h54xx7th24xt3nx
#define FINALLY_H_slkhnuigcherx8g3h54xx7th24xt3nx

#include "std/std_extensions.h"

namespace mb::ul
{
//! It's not so convenient to use this directly; call the associated helper function instead.
template <class F>
class final_act
{
public:
    explicit final_act(F f) noexcept
        : f_{std::move(f)}
    {
    }

    final_act(final_act&& other) noexcept
        : f_{std::move(other.f_)}
        , invoke_{other.invoke_}
    {
        other.invoke_ = false;
    }

    final_act(const final_act&) = delete;
    final_act& operator=(const final_act&) = delete;

    ~final_act() noexcept
    {
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
inline final_act<F> finally(const F& f) noexcept
{
    return final_act<F>(f);
}

//! Cf. finally() above.
template <class F>
inline final_act<F> finally(F&& f) noexcept
{
    return final_act<F>(std::forward<F>(f));
}
} // namespace mb::ul

#endif
