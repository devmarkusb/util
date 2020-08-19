// Markus Borris, 2016
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef OPTIONAL_H_lfjuoijv5ijmzhc453457tnx387
#define OPTIONAL_H_lfjuoijv5ijmzhc453457tnx387

#include "toolib/assert.h"
#include "toolib/std/std_extensions.h"
#include <cstddef>
#include <memory>
#include <utility>

#include "toolib/macros.h"


namespace too
{
using none_t = std::nullptr_t;
//! Used to set some \code too::opt<T> x \endcode variable to empty.
/** Note that this unfortunately does not always work well when being passed as return value.
    It is recommended to use the much clearer `return {};` then.*/
const none_t none = nullptr;

//! Optional type.
/** Known shortcomings:
        -doesn't implicitly convert well to complex types, e.g. std::string,
        when assigning or passing to function; construction is ok
        -streaming operators >> << would have to be implemented on demand yet

    And probably many more unknown ones. The implementation is kept extremely
    simple. It is just done in a way as if you would use bool* instead of
    too::opt<bool> and did everything manually. But for everyday live it should
    just be absolutely sufficient.
    Usage:
    \code
    void f(const too::opt<bool>& prop) // const& just to demonstrate where to put it
    {
        if (prop)
        {
           if (*prop)
               ;// do sth.
           else
               ;// do other things
        }
        else
           // do different things, not expecting any prop value
    }
    \endcode
*/
template <typename T>
struct opt
{
    opt()
    {
    }
    ~opt() = default;

    opt(const opt<T>& other)
        : holder(other.holder ? too::make_unique<T>(*other.holder) : nullptr)
    {
    }
    opt<T>& operator=(const opt<T>& other)
    {
        holder = other.holder ? too::make_unique<T>(*other.holder) : nullptr;
        return *this;
    }

    explicit opt(const none_t&)
        : holder(nullptr)
    {
    }
    explicit opt(none_t&&)
        : holder(nullptr)
    {
    }

#if TOO_HAS_CPP11_DEFAULT_MOVES
    opt(opt<T>&&) = default;
    opt& operator=(opt<T>&&) = default;
#endif

    opt(const T& x)
        : holder(too::make_unique<T>(x))
    {
    }
    opt(T&& x)
        : holder(too::make_unique<T>(std::move(x)))
    {
    }

    operator T*() const
    {
        return this->holder.get();
    }

// otherwise we get warnings of implicit conversions to bool (performance issue, theoretically)
#if TOO_COMP_MS_VISUAL_STUDIO_CPP
//! Change to explicit cast, as soon as possible!
///*explicit */ operator bool() const { return reinterpret_cast<bool>(this->holder.get()); }
#endif

    opt<T>& operator=(const T& x)
    {
        this->holder = too::make_unique<T>(x);
        return *this;
    }

    opt<T>& operator=(T&& x)
    {
        this->holder = too::make_unique<T>(std::move(x));
        return *this;
    }

    opt<T>& operator=(const none_t&)
    {
        this->holder.reset();
        return *this;
    }

    opt<T>& operator=(none_t&&)
    {
        this->holder.reset();
        return *this;
    }

    void reset()
    {
        this->holder.reset();
    }

private:
    std::unique_ptr<T> holder;
};

template <typename T>
bool operator==(const opt<T>& lhs, const opt<T>& rhs)
{
    if (lhs && rhs)
        return *lhs == *rhs;
    else if (!lhs && !rhs)
        return true;
    else
        return false;
}

template <typename T>
bool operator!=(const opt<T>& lhs, const opt<T>& rhs)
{
    return !operator==(lhs, rhs);
}

//! Note: too::none is interpreted to be the smallest.
template <typename T>
bool operator<(const opt<T>& lhs, const opt<T>& rhs)
{
    if (lhs && rhs)
        return *lhs < *rhs;
    else if (!lhs && !rhs)
        return false;
    else
        return !lhs && rhs ? true : false; // decide none to be the smallest
}

template <typename T>
bool operator>(const opt<T>& lhs, const opt<T>& rhs)
{
    return operator<(rhs, lhs);
}

template <typename T>
bool operator<=(const opt<T>& lhs, const opt<T>& rhs)
{
    return !operator>(lhs, rhs);
}

template <typename T>
bool operator>=(const opt<T>& lhs, const opt<T>& rhs)
{
    return !operator<(lhs, rhs);
}
} // namespace too

#include "toolib/macros_end.h"

#endif
