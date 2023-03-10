//! \file

#ifndef OPTIONAL_H_lfjuoijv5ijmzhc453457tnx387
#define OPTIONAL_H_lfjuoijv5ijmzhc453457tnx387

#include "assert.h"
#include "std/std_extensions.h"

#include "ul/macros.h"
#include <cstddef>
#include <memory>
#include <utility>

namespace mb::ul
{
using none_t = std::nullptr_t;
//! Used to set some \code ul::opt<T> x \endcode variable to empty.
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
    ul::opt<bool> and did everything manually. But for everyday live it should
    just be absolutely sufficient.
    Usage:
    \code
    void f(const ul::opt<bool>& prop) // const& just to demonstrate where to put it
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
    opt() = default;
    ~opt() = default;

    opt(const opt<T>& other)
        : holder_(other.holder_ ? ul::make_unique<T>(*other.holder_) : nullptr)
    {
    }

    opt<T>& operator=(const opt<T>& other)
    {
        if (std::addressof(other) == this)
            return *this;
        holder_ = other.holder_ ? ul::make_unique<T>(*other.holder_) : nullptr;
        return *this;
    }

    explicit opt(const none_t& /*unused*/)
        : holder_(nullptr)
    {
    }

    explicit opt(none_t&& /*unused*/)
        : holder_(nullptr)
    {
    }

#if UL_HAS_CPP11_DEFAULT_MOVES
    opt(opt<T>&&) noexcept = default;
    opt& operator=(opt<T>&&) noexcept = default;
#endif

    /*implicit*/ opt(const T& x)
        : holder_(ul::make_unique<T>(x))
    {
    }

    /*implicit*/ opt(T&& x)
        : holder_(ul::make_unique<T>(std::move(x)))
    {
    }

    /*implicit*/ operator T*() const
    {
        return this->holder_.get();
    }

// otherwise we get warnings of implicit conversions to bool (performance issue, theoretically)
#if UL_COMP_MS_VISUAL_STUDIO_CPP
//! Change to explicit cast, as soon as possible!
///*explicit */ operator bool() const { return reinterpret_cast<bool>(this->holder.get()); }
#endif

    opt<T>& operator=(const T& x)
    {
        this->holder_ = ul::make_unique<T>(x);
        return *this;
    }

    opt<T>& operator=(T&& x)
    {
        this->holder_ = ul::make_unique<T>(std::move(x));
        return *this;
    }

    opt<T>& operator=(const none_t& /*unused*/)
    {
        this->holder_.reset();
        return *this;
    }

    opt<T>& operator=(none_t&& /*unused*/)
    {
        this->holder_.reset();
        return *this;
    }

    void reset()
    {
        this->holder_.reset();
    }

private:
    std::unique_ptr<T> holder_;
};

template <typename T>
bool operator==(const opt<T>& lhs, const opt<T>& rhs)
{
    if (lhs && rhs)
        return *lhs == *rhs;
    if (!lhs && !rhs)
        return true;
    return false;
}

template <typename T>
bool operator!=(const opt<T>& lhs, const opt<T>& rhs)
{
    return !operator==(lhs, rhs);
}

//! Note: ul::none is interpreted to be the smallest.
template <typename T>
bool operator<(const opt<T>& lhs, const opt<T>& rhs)
{
    if (lhs && rhs)
        return *lhs < *rhs;
    if (!lhs && !rhs)
        return false;
    return !lhs && rhs; // decide none to be the smallest
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
} // namespace mb::ul

#endif
