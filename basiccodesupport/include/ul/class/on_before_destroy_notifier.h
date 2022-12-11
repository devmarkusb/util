// 2017-22

//! \file

#ifndef ONBEFOREDESTROY_NOTIFIER_H_sduifgh5st78247hg
#define ONBEFOREDESTROY_NOTIFIER_H_sduifgh5st78247hg

#include "on_before_destroy.h"
#include "listener.h"


namespace mb::ul
{
//! Cf. OnBeforeDestroyListener.
struct OnBeforeDestroyNotifier : public ListenerRegister
{
    void onBeforeDestroy()
    {
        for (auto& l : this->registeredListeners_)
            dynamic_cast<OnBeforeDestroyListener*>(l)->onBeforeDestroy();
    }
};
} // namespace mb::ul

#endif
