//! \file

#ifndef OPTIONAL_H_LFJUOIJV5IJMZHC453457TNX387
#define OPTIONAL_H_LFJUOIJV5IJMZHC453457TNX387

#include "assert.h"
#include "std/std_extensions.h"

#include "ul/macros.h"
#include <cstddef>
#include <memory>
#include <utility>

namespace mb::ul {
using NoneT = std::nullptr_t;
//! Used to set some \code ul::Opt<T> x \endcode variable to empty.
/** Note that this unfortunately does not always work well when being passed as return value.
    It is recommended to use the much clearer `return {};` then.*/
const NoneT none = nullptr;

//! Optional type.
/** Known shortcomings:
        -doesn't implicitly convert well to complex types, e.g. std::string,
        when assigning or passing to function; construction is ok
        -streaming operators >> << would have to be implemented on demand yet

    And probably many more unknown ones. The implementation is kept extremely
    simple. It is just done in a way as if you would use bool* instead of
    ul::Opt<bool> and did everything manually. But for everyday live it should
    just be absolutely sufficient.
    Usage:
    \code
    void f(const ul::Opt<bool>& prop) // const& just to demonstrate where to put it
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
struct [[deprecated("use std::optional")]] Opt {
    Opt() = default;
    ~Opt() = default;

    Opt(const Opt<T>& other)
        : holder_(other.holder_ ? ul::make_unique<T>(*other.holder_) : nullptr) {
    }

    Opt<T>& operator=(const Opt<T>& other) {
        if (std::addressof(other) == this)
            return *this;
        holder_ = other.holder_ ? ul::make_unique<T>(*other.holder_) : nullptr;
        return *this;
    }

    explicit Opt(const NoneT& /*unused*/)
        : holder_(nullptr) {
    }

    explicit Opt(NoneT&& /*unused*/) // NOLINT
        : holder_(nullptr) {
    }

#if UL_HAS_CPP11_DEFAULT_MOVES
    Opt(Opt<T>&&) noexcept = default;
    Opt& operator=(Opt<T>&&) noexcept = default;
#endif

    /*implicit*/ Opt(const T& x)
        : holder_(ul::make_unique<T>(x)) {
    }

    /*implicit*/ Opt(T&& x)
        : holder_(ul::make_unique<T>(std::move(x))) {
    }

    /*implicit*/ operator T*() const {
        return this->holder_.get();
    }

// otherwise we get warnings of implicit conversions to bool (performance issue, theoretically)
#if UL_COMP_MS_VISUAL_STUDIO_CPP
//! Change to explicit cast, as soon as possible!
///*explicit */ operator bool() const { return reinterpret_cast<bool>(this->holder.get()); }
#endif

    Opt<T>& operator=(const T& x) {
        this->holder_ = ul::make_unique<T>(x);
        return *this;
    }

    Opt<T>& operator=(T&& x) {
        this->holder_ = ul::make_unique<T>(std::move(x));
        return *this;
    }

    Opt<T>& operator=(const NoneT& /*unused*/) { // NOLINT
        this->holder_.reset();
        return *this;
    }

    Opt<T>& operator=(NoneT&& /*unused*/) { // NOLINT
        this->holder_.reset();
        return *this;
    }

    void reset() {
        this->holder_.reset();
    }

private:
    std::unique_ptr<T> holder_;
};

template <typename T>
bool operator==(const Opt<T>& lhs, const Opt<T>& rhs) {
    if (lhs && rhs)
        return *lhs == *rhs;
    if (!lhs && !rhs)
        return true;
    return false;
}

template <typename T>
bool operator!=(const Opt<T>& lhs, const Opt<T>& rhs) {
    return !operator==(lhs, rhs);
}

//! Note: ul::none is interpreted to be the smallest.
template <typename T>
bool operator<(const Opt<T>& lhs, const Opt<T>& rhs) {
    if (lhs && rhs)
        return *lhs < *rhs;
    if (!lhs && !rhs)
        return false;
    return !lhs && rhs; // decide none to be the smallest
}

template <typename T>
bool operator>(const Opt<T>& lhs, const Opt<T>& rhs) {
    return operator<(rhs, lhs);
}

template <typename T>
bool operator<=(const Opt<T>& lhs, const Opt<T>& rhs) {
    return !operator>(lhs, rhs);
}

template <typename T>
bool operator>=(const Opt<T>& lhs, const Opt<T>& rhs) {
    return !operator<(lhs, rhs);
}
} // namespace mb::ul

#endif
