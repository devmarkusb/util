// Markus Borris, 2017
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef ONBEFOREDESTROY_NOTIFIER_H_sduifgh5st78247hg
#define ONBEFOREDESTROY_NOTIFIER_H_sduifgh5st78247hg

#include "listener.h"
#include "on_before_destroy.h"


namespace too
{
//! Cf. OnBeforeDestroyListener.
struct OnBeforeDestroyNotifier : public ListenerRegister
{
    void onBeforeDestroy()
    {
        for (auto& l : this->registered_listeners)
            dynamic_cast<OnBeforeDestroyListener*>(l)->onBeforeDestroy();
    }
};
} // too

#endif
