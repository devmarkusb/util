// Markus Borris, 2019
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef SHARED_INSTANCE_H_cjklht78g428ghxn3g34g
#define SHARED_INSTANCE_H_cjklht78g428ghxn3g34g

#include <memory>
#include <mutex>


namespace too
{
//! Thread-safe singleton creator and accessor with life time ending when last reference is gone.
/** Currently only implemented for objects constructible without parameters.
    You can expect the implementation to call make_shared in it's default value initialization flavor '{}', so
    even POD types get value initialized (typically 0). (Just for better understanding - chances are we won't use it,
    but C++20 might support a make_shared_default_init, which does default initialization which means *no*
    initialization for POD types - the more performant version in principle.)*/
template <class T>
std::shared_ptr<T> getSharedInstance()
{
    static std::mutex m;
    static std::weak_ptr<T> maybe_object;

    std::lock_guard<std::mutex> lk{m};

    std::shared_ptr<T> object = maybe_object.lock();
    if (maybe_object.expired())
    {
        object = std::make_shared<T>();
        maybe_object = object;
    }

    return object;
}
} // too
#endif
