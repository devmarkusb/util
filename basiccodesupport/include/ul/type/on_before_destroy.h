//! \file More than just a first example of using the ul::Listener/ul::ListenerRegister pair.

#ifndef ONBEFOREDESTROY_H_VOERUINH235GT3NGFY13F1
#define ONBEFOREDESTROY_H_VOERUINH235GT3NGFY13F1

#include "listener.h"

namespace mb::ul {
/** This could be a very useful listener/notifier pair in any situation where you
    want to manage lifetimes in dependency injection situations and don't want to
    use a shared pointer.
    If you inject an object pointer that won't live as long as the containing
    object (the one that contains the injected object), you just need to make
    the containing object listen to the injected object notifying about its own
    destructtion. That is you need to
        1 call on_before_destroy within the destructor of the injected object
        2 within the containing object make sure that you
            2.2 register the containing object as listener within the injected object
                right after/at injection
            2.1 set the pointer to the injected object to nullptr as soon as you get
                called via on_before_destroy (implement on_before_destroy that way).
*/
struct OnBeforeDestroyListener : public Listener {
    ~OnBeforeDestroyListener() override = default;

    virtual void on_before_destroy() = 0;
};
} // namespace mb::ul

#endif
