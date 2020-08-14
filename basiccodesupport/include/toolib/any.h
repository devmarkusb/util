// Markus Borris, 2016
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef ANY_H_sdfuiofx37tn3z47txn378xtfg43n8g3
#define ANY_H_sdfuiofx37tn3z47txn378xtfg43n8g3

#include "toolib/std/std_extensions.h"
#include <exception>
#include <memory>
#include <type_traits>
#include <typeinfo>

#include "toolib/macros.h"


namespace too
{
//! Can be used like any built-in value type, like \code int \endcode.
//! You can assign everything you want. If you want it back as a concrete type
//! you need to use \code too::any_cast<>() \endcode.
/** Usage
    \code
    too::any x = std::vector<long>{1,2,3,4,5};
    std::vector<long> back = too::any_cast<std::vector<long>>(x);
    \endcode
    The type cann be especially useful for interfaces that shouldn't be touched
    again but remain flexible, example shown in any.test.cpp.
    So you can parametrize interface functions with (1.) an enum type as attribute
    description plus commenting the expected concrete type, and (2.) too::any.

    Attention: use any_cast() on pointers to \ref any by specifying the *non*-pointer
    concrete type as cast type within <>, but of course the pointer type as the cast
    return type.*/
class any;

//! Cf. \ref any.
//!@{
template <typename Type>
Type any_cast(any&);
template <typename Type>
Type any_cast(const any&);
template <typename Type>
Type* any_cast(any*);
template <typename Type>
const Type* any_cast(const any*);
//!@}

//! Applied to \ref any type. Thrown if \ref any is empty or casted to the wrong concrete type.
struct bad_any_cast : public std::bad_cast
{
};

//! Details.
/** If \ref any is empty or casted to the wrong type, \ref too::bad_any_cast is thrown.*/
class any
{
public:
    any() = default;
    ~any() = default;

    any(const any& x)
    {
        if (x.holder)
            holder = x.holder->clone();
    }

#if TOO_HAS_CPP11_DEFAULT_MOVES
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
    any(T&& x)
        : holder(too::make_unique<concrete<T>>(std::forward<T>(x)))
    {
    }

    template <typename T>
    any(const T& x)
        : holder(too::make_unique<concrete<T>>(x))
    {
    }

    template <typename T>
    any& operator=(T&& x)
    {
        holder = too::make_unique<concrete<T>>(std::forward<T>(x));
        return *this;
    }

    template <typename T>
    any& operator=(const T& x)
    {
        holder = too::make_unique<concrete<T>>(x);
        return *this;
    }

    //!
    void clear()
    {
        holder = nullptr;
    }

    //! Would be true on default construction.
    bool empty() const
    {
        return !holder;
    }

    //!
    const std::type_info& type() const
    {
        return (!empty()) ? holder->type() : typeid(void);
    }

private:
    struct ibase
    {
        virtual std::unique_ptr<ibase> clone() const = 0;
        virtual const std::type_info& type() const = 0;
        virtual ~ibase()
        {
        }
        //            ibase(const ibase&) = default;
        //            ibase& operator=(const ibase&) = default;
        //#if TOO_HAS_CPP11_DEFAULT_MOVES
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

        virtual std::unique_ptr<ibase> clone() const override
        {
            return too::make_unique<concrete<T>>(value);
        }

        virtual const std::type_info& type() const override
        {
            return typeid(T);
        }

        T value;
    };

    std::unique_ptr<ibase> holder;

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
    if (val.empty() || val.holder->type() != typeid(Type))
        throw bad_any_cast();
    return static_cast<any::concrete<Type>*>(val.holder.get())->value;
}

template <typename Type>
Type any_cast(const any& val)
{
    if (val.empty() || val.holder->type() != typeid(Type))
        throw bad_any_cast();
    return static_cast<any::concrete<Type>*>(any(val).holder.get())->value;
}


template <typename Type>
Type* any_cast(any* pval)
{
    if (pval->empty() || pval->holder->type() != typeid(Type))
        throw bad_any_cast();
    return &(static_cast<any::concrete<Type>*>(pval->holder.get())->value);
}

template <typename Type>
const Type* any_cast(const any* pval)
{
    if (pval->empty() || pval->holder->type() != typeid(Type))
        throw bad_any_cast();
    return &(static_cast<any::concrete<Type>*>(pval->holder.get())->value);
}
} // namespace too

#include "toolib/macros_end.h"

#endif
