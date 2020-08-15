// Markus Borris, 2016-17
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef LISTENER_H_sjkdzth78tn2378xgh73fr
#define LISTENER_H_sjkdzth78tn2378xgh73fr

#include "toolib/assert.h"
#include <algorithm>
#include <vector>


namespace too
{
//! Base class for every abstract listener interface.
/** Usage:
    \code
    struct Model_listener : public too::Listener
    {
        virtual ~Model_listener() {}
        virtual void on_prop_changed() = 0;
    };
    \endcode
    which you then implement within your listening class, along with
    necessary register/unregister calls to the sender/notifier you
    want to listen to:
    \code
    struct View : private Model_listener
    {
        View()
        {
            this->m_in_v.register_listener(this);
        }
        ~View()
        {
            this->m_in_v.unregister_listener(this);
        }
    private:
        Model m_in_v;

        virtual void on_prop_changed() override
        {
            // do sth when getting this callback
        }
    };
    \endcode
    */
struct Listener
{
    virtual ~Listener() = 0;
};
inline Listener::~Listener()
{
}

//! Base class for every sender/notifier.
/** Usage:
    \code
    struct Model_notifier : public too::ListenerRegister
    {
        void on_prop_changed()
        {
            for (auto& l : this->registered_listeners)
                dynamic_cast<Model_listener*>(l)->on_prop_changed();
        }
    };
    \endcode
    which will mostly follow this scheme. In relation to a Model_listener
    which is shown in the usage example of Listener you are free to choose the
    signatures of the methods. E.g. a sender parameter
    \code Model_notifier* sender \endcode
    could be useful. Or you could provide additional functions controlling the
    enabling/disabling of certain method calls.

    However, your notifying class, your caller must inherit
    (indirectly) from ListenerRegister:
    \code
    struct Model : public Model_notifier
    {
        void do()
        {
            // do sth
            on_prop_changed();
        }

        // sth
    };
    \endcode
    That is the one listeners can register themselves in to be called.

    Implementation note: member functions are virtual for a reason, there are
    ues-cases. E.g. if you like to forward registrations to members.
*/
class ListenerRegister
{
public:
    virtual ~ListenerRegister() = default;

    /** \param l has to be non-nullptr and valid/alive until calling unregister_listener on it,
        which also has to be called before l's livetime ends.
        The same l also mustn't be registered more than once.*/
    virtual void register_listener(Listener* l)
    {
        TOO_EXPECT(l);
        TOO_EXPECT(std::find(std::begin(this->registered_listeners), std::end(this->registered_listeners), l) ==
                   std::end(this->registered_listeners));

        this->registered_listeners.push_back(l);
    }

    /** \param l has to be a non-nullptr, still valid, already via register_listener registered Listener.
        The same l also mustn't be unregistered more than once.*/
    virtual void unregister_listener(Listener* l)
    {
        TOO_EXPECT(l);
        const auto it = std::find(std::begin(this->registered_listeners), std::end(this->registered_listeners), l);
        TOO_EXPECT(it != std::end(this->registered_listeners));

        this->registered_listeners.erase(it);
    }

    //! \param l has to be non-nullptr.
    virtual bool is_registered(Listener* l) const
    {
        TOO_EXPECT(l);

        return std::find(std::begin(this->registered_listeners), std::end(this->registered_listeners), l) !=
               std::end(this->registered_listeners);
    }

protected:
    using listener_container = std::vector<Listener*>;
    listener_container registered_listeners;
};

//! Static version of ListenerRegister. That is, you can let your
//! class be a notfier statically and not per object.
template <class StaticNotifier>
class ListenerStaticRegister
{
public:
    /** \param l has to be non-nullptr and valid/alive until calling unregister_listener on it,
        which also has to be called before l's livetime ends.
        The same l also mustn't be registered more than once.*/
    static void register_listener(Listener* l)
    {
        TOO_EXPECT(l);
        TOO_EXPECT(std::find(std::begin(registered_listeners()), std::end(registered_listeners()), l) ==
                   std::end(registered_listeners()));

        registered_listeners().push_back(l);
    }

    /** \param l has to be a non-nullptr, still valid, already via register_listener registered Listener.
        The same l also mustn't be unregistered more than once.*/
    static void unregister_listener(Listener* l)
    {
        TOO_EXPECT(l);
        const auto it = std::find(std::begin(registered_listeners()), std::end(registered_listeners()), l);
        TOO_EXPECT(it != std::end(registered_listeners()));

        registered_listeners().erase(it);
    }

    //! \param l has to be non-nullptr.
    static bool is_registered(Listener* l)
    {
        TOO_EXPECT(l);

        return std::find(std::begin(registered_listeners()), std::end(registered_listeners()), l) !=
               std::end(registered_listeners());
    }

protected:
    using listener_container = std::vector<Listener*>;
    static listener_container& registered_listeners()
    {
        static listener_container inst;
        return inst;
    }
};
} // namespace too

#endif
