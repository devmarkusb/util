//! \file

#ifndef ANY_H_sdfuiofx37tn3z47txn378xtfg43n8g3
#define ANY_H_sdfuiofx37tn3z47txn378xtfg43n8g3

#include "std/std_extensions.h"
#include "ul/macros.h"
#include <exception>
#include <memory>
#include <type_traits>
#include <typeinfo>

namespace mb::ul
{
/** Can be used like any built-in value type, like \code int \endcode.
    You can assign everything you want. If you want it back as a concrete type
    you need to use \code ul::any_cast<>() \endcode.
    Usage
    \code
    ul::any x = std::vector<long>{1,2,3,4,5};
    std::vector<long> back = ul::any_cast<std::vector<long>>(x);
    \endcode
    The type cann be especially useful for interfaces that shouldn't be touched
    again but remain flexible, example shown in any.test.cpp.
    So you can parametrize interface functions with (1.) an enum type as attribute
    description plus commenting the expected concrete type, and (2.) ul::any.

    Attention: use any_cast() on pointers to any by specifying the *non*-pointer
    concrete type as cast type within <>, but of course the pointer type as the cast
    return type.*/
class any;

//! Cf. any.
//!@{
template <typename Type>
Type any_cast(any& val);
template <typename Type>
Type any_cast(const any& val);
template <typename Type>
Type* any_cast(any* pval);
template <typename Type>
const Type* any_cast(const any* val);

//!@}

//! Applied to any type. Thrown if any is empty or casted to the wrong concrete type.
struct bad_any_cast : public std::bad_cast
{
} __attribute__((packed, aligned(1)));

//! Details.
/** If any is empty or casted to the wrong type, ul::bad_any_cast is thrown.*/
class any
{
public:
    any() = default;
    ~any() = default;

    any(const any& x)
    {
        if (x.holder_)
            holder_ = x.holder_->clone();
    }

#if UL_HAS_CPP11_DEFAULT_MOVES
    any(any&& x) = default;
    any& operator=(any&& rhs) = default;
#endif

    any& operator=(const any& rhs) // untested
    {
        // ptr = std::move(any(rhs).ptr); // faster?
        *this = any(rhs); // no code duplication, no move, as according to clang this could prevent copy elision
        return *this;
    }

    template <typename T>
    /*implicit*/ any(T&& x)
        : holder_(ul::make_unique<concrete<T>>(std::forward<T>(x)))
    {
    }

    template <typename T>
    /*implicit*/ any(const T& x)
        : holder_(ul::make_unique<concrete<T>>(x))
    {
    }

    template <typename T>
    any& operator=(T&& x)
    {
        holder_ = ul::make_unique<concrete<T>>(std::forward<T>(x));
        return *this;
    }

    template <typename T>
    any& operator=(const T& x)
    {
        holder_ = ul::make_unique<concrete<T>>(x);
        return *this;
    }

    void clear()
    {
        holder_ = nullptr;
    }

    //! Would be true on default construction.
    [[nodiscard]] bool empty() const
    {
        return !holder_;
    }

    [[nodiscard]] const std::type_info& type() const
    {
        return (!empty()) ? holder_->type() : typeid(void);
    }

private:
    struct ibase
    {
        [[nodiscard]] virtual std::unique_ptr<ibase> clone() const = 0;
        [[nodiscard]] virtual const std::type_info& type() const = 0;
        virtual ~ibase() = default;
        //            ibase(const ibase&) = default;
        //            ibase& operator=(const ibase&) = default;
        //#if UL_HAS_CPP11_DEFAULT_MOVES
        //            ibase(ibase&&) = default;
        //            ibase& operator=(ibase&&) = default;
        //#endif
    };

    template <typename T>
    struct concrete : public ibase
    {
        explicit concrete(T&& x)
            : value(std::forward<T>(x))
        {
        }

        explicit concrete(const T& x)
            : value(x)
        {
        }

        [[nodiscard]] std::unique_ptr<ibase> clone() const override
        {
            return ul::make_unique<concrete<T>>(value);
        }

        [[nodiscard]] const std::type_info& type() const override
        {
            return typeid(T);
        }

        T value;
    };

    std::unique_ptr<ibase> holder_;

    template <typename Type>
    friend Type any_cast(any&);

    template <typename Type>
    friend Type any_cast(const any&);

    template <typename Type>
    friend Type* any_cast(any*);

    template <typename Type>
    friend const Type* any_cast(const any*);
};

template <typename Type>
Type any_cast(any& val)
{
    if (val.empty() || val.holder_->type() != typeid(Type))
        throw bad_any_cast();
    return static_cast<any::concrete<Type>*>(val.holder_.get())->value;
}

template <typename Type>
Type any_cast(const any& val)
{
    if (val.empty() || val.holder_->type() != typeid(Type))
        throw bad_any_cast();
    return static_cast<any::concrete<Type>*>(any(val).holder_.get())->value;
}

template <typename Type>
Type* any_cast(any* pval)
{
    if (pval->empty() || pval->holder_->type() != typeid(Type))
        throw bad_any_cast();
    return &(static_cast<any::concrete<Type>*>(pval->holder_.get())->value);
}

template <typename Type>
const Type* any_cast(const any* pval)
{
    if (pval->empty() || pval->holder_->type() != typeid(Type))
        throw bad_any_cast();
    return &(static_cast<any::concrete<Type>*>(pval->holder_.get())->value);
}
} // namespace mb::ul

#endif
