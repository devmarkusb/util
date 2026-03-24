//! \file

#ifndef ONBEFOREDESTROY_NOTIFIER_H_SDUIFGH5ST78247HG
#define ONBEFOREDESTROY_NOTIFIER_H_SDUIFGH5ST78247HG

#include "on_before_destroy.h"
#include "listener.h"

namespace mb::ul {
//! Cf. OnBeforeDestroyListener.
struct OnBeforeDestroyNotifier : public ListenerRegister {
    void on_before_destroy() {
        for (auto& l : this->registered_listeners_)
            dynamic_cast<OnBeforeDestroyListener*>(l)->on_before_destroy();
    }
};
} // namespace mb::ul

#endif
