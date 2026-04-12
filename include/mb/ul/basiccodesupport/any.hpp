//! \file

#ifndef ANY_H_SDFUIOFX37TN3Z47TXN378XTFG43N8G3
#define ANY_H_SDFUIOFX37TN3Z47TXN378XTFG43N8G3

#include "mb/ul/buildenv/macros.hpp"
#include "std/std-extensions.hpp"
#include <memory>
#include <typeinfo>
#include <utility>

namespace mb::ul {
/** Can be used like any built-in value type, like \code int \endcode.
    You can assign everything you want. If you want it back as a concrete type
    you need to use \code ul::any_cast<>() \endcode.
    Usage
    \code
    ul::Any x = std::vector<long>{1,2,3,4,5};
    std::vector<long> back = ul::any_cast<std::vector<long>>(x);
    \endcode
    The type can be especially useful for interfaces that shouldn't be touched
    again but remain flexible, example shown in any.test.cpp.
    So you can parametrize interface functions with (1.) an enum type as attribute
    description plus commenting the expected concrete type, and (2.) ul::Any.

    Attention: use any_cast() on pointers to Any by specifying the *non*-pointer
    concrete type as cast type within <>, but of course the pointer type as the cast
    return type.*/
class Any;

//! Cf. Any.
//!@{
template <typename Type>
Type any_cast(Any& val);
template <typename Type>
Type any_cast(const Any& val);
template <typename Type>
Type* any_cast(Any* pval);
template <typename Type>
const Type* any_cast(const Any* val);

//!@}

//! Applied to any type. Thrown if Any is empty or casted to the wrong concrete type.
struct BadAnyCast : public std::bad_cast {};

//! Details.
/** If Any is empty or casted to the wrong type, ul::bad_any_cast is thrown.*/
class Any {
public:
    Any() = default;
    ~Any() = default;

    Any(const Any& x) {
        if (x.holder_)
            holder_ = x.holder_->clone();
    }

#if UL_HAS_CPP11_DEFAULT_MOVES
    Any(Any&& x) = default;
    Any& operator=(Any&& rhs) = default;
#endif

    Any& operator=(const Any& rhs) // untested
    {
        // ptr = std::move(Any(rhs).ptr); // faster?
        *this = Any(rhs); // no code duplication, no move, as according to clang this could prevent copy elision
        return *this;
    }

    // NOLINTBEGIN
    template <typename T>
    /*implicit*/ Any(T&& x)
        : holder_(ul::make_unique<Concrete<T>>(std::forward<T>(x))) {
    }

    template <typename T>
    /*implicit*/ Any(const T& x)
        : holder_(ul::make_unique<Concrete<T>>(x)) {
    }

    // NOLINTEND

    template <typename T>
    Any& operator=(T&& x) {
        holder_ = ul::make_unique<Concrete<T>>(std::forward<T>(x));
        return *this;
    }

    template <typename T>
    Any& operator=(const T& x) {
        holder_ = ul::make_unique<Concrete<T>>(x);
        return *this;
    }

    void clear() {
        holder_ = nullptr;
    }

    //! Would be true on default construction.
    [[nodiscard]] bool empty() const {
        return !holder_;
    }

    [[nodiscard]] const std::type_info& type() const {
        return (!empty()) ? holder_->type() : typeid(void);
    }

private:
    struct Ibase {
        [[nodiscard]] virtual std::unique_ptr<Ibase> clone() const = 0;
        [[nodiscard]] virtual const std::type_info& type() const = 0;
        virtual ~Ibase() = default;
        //            ibase(const ibase&) = default;
        //            ibase& operator=(const ibase&) = default;
        //#if UL_HAS_CPP11_DEFAULT_MOVES
        //            ibase(ibase&&) = default;
        //            ibase& operator=(ibase&&) = default;
        //#endif
    };

    template <typename T>
    struct Concrete : Ibase {
        explicit Concrete(T&& x)
            : value(std::move(x)) {
        }

        explicit Concrete(const T& x)
            : value(x) {
        }

        [[nodiscard]] std::unique_ptr<Ibase> clone() const override { // NOLINT
            return ul::make_unique<Concrete<T>>(value);
        }

        [[nodiscard]] const std::type_info& type() const override { // NOLINT
            return typeid(T);
        }

        T value;
    };

    std::unique_ptr<Ibase> holder_;

    template <typename Type>
    friend Type any_cast(Any& val);

    template <typename Type>
    friend Type any_cast(const Any& val);

    template <typename Type>
    friend Type* any_cast(Any* pval);

    template <typename Type>
    friend const Type* any_cast(const Any* pval);
};

template <typename Type>
Type any_cast(Any& val) {
    if (val.empty() || val.holder_->type() != typeid(Type))
        throw BadAnyCast();
    return static_cast<Any::Concrete<Type>*>(val.holder_.get())->value;
}

template <typename Type>
Type any_cast(const Any& val) {
    if (val.empty() || val.holder_->type() != typeid(Type))
        throw BadAnyCast();
    return static_cast<Any::Concrete<Type>*>(Any(val).holder_.get())->value;
}

template <typename Type>
Type* any_cast(Any* pval) {
    if (pval->empty() || pval->holder_->type() != typeid(Type))
        throw BadAnyCast();
    return &(static_cast<Any::Concrete<Type>*>(pval->holder_.get())->value);
}

template <typename Type>
const Type* any_cast(const Any* pval) {
    if (pval->empty() || pval->holder_->type() != typeid(Type))
        throw BadAnyCast();
    return &(static_cast<Any::Concrete<Type>*>(pval->holder_.get())->value);
}
} // namespace mb::ul

#endif
