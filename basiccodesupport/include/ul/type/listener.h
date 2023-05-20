//! \file

#ifndef LISTENER_H_SJKDZTH78TN2378XGH73FR
#define LISTENER_H_SJKDZTH78TN2378XGH73FR

#include "../assert.h"
#include <algorithm>
#include <vector>

namespace mb::ul {
//! Base class for every abstract listener interface.
/** Usage:
    \code
    struct Model_listener : public ul::Listener
    {
        virtual ~Model_listener() = default;
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
        Model in_v_;

        virtual void on_prop_changed() override
        {
            // do sth when getting this callback
        }
    };
    \endcode
    */
struct Listener {
    virtual ~Listener() = default;
};

//! Base class for every sender/notifier.
/** Usage:
    \code
    struct Model_notifier : public ul::ListenerRegister
    {
        void on_prop_changed()
        {
            for (auto& l : this->registeredListeners)
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
class ListenerRegister {
public:
    virtual ~ListenerRegister() = default;

    /** l has to be non-nullptr and valid/alive until calling unregister_listener on it,
        which also has to be called before l's livetime ends.
        The same l also mustn't be registered more than once.*/
    virtual void register_listener(Listener* l) {
        UL_EXPECT(l);
        UL_EXPECT(
            std::find(std::begin(this->registeredListeners_), std::end(this->registeredListeners_), l)
            == std::end(this->registeredListeners_));

        this->registeredListeners_.push_back(l);
    }

    /** \param l has to be a non-nullptr, still valid, already via register_listener registered Listener.
        The same l also mustn't be unregistered more than once.*/
    virtual void unregister_listener(Listener* l) {
        UL_EXPECT(l);
        const auto it = std::find(std::begin(this->registeredListeners_), std::end(this->registeredListeners_), l);
        UL_EXPECT(it != std::end(this->registeredListeners_));

        this->registeredListeners_.erase(it);
    }

    //! \param l has to be non-nullptr.
    virtual bool is_registered(Listener* l) const {
        UL_EXPECT(l);

        return std::find(std::begin(this->registeredListeners_), std::end(this->registeredListeners_), l)
               != std::end(this->registeredListeners_);
    }

protected:
    using ListenerContainer = std::vector<Listener*>;
    ListenerContainer registeredListeners_;
};

/** Static version of ListenerRegister. That is, you can let your
    class be a notfier statically and not per object.*/
template <class StaticNotifier>
class ListenerStaticRegister {
public:
    /** \param l has to be non-nullptr and valid/alive until calling unregister_listener on it,
        which also has to be called before l's livetime ends.
        The same l also mustn't be registered more than once.*/
    static void register_listener(Listener* l) {
        UL_EXPECT(l);
        UL_EXPECT(
            std::find(std::begin(registered_listeners()), std::end(registered_listeners()), l)
            == std::end(registered_listeners()));

        registered_listeners().push_back(l);
    }

    /** \param l has to be a non-nullptr, still valid, already via register_listener registered Listener.
        The same l also mustn't be unregistered more than once.*/
    static void unregister_listener(Listener* l) {
        UL_EXPECT(l);
        const auto it = std::find(std::begin(registered_listeners()), std::end(registered_listeners()), l);
        UL_EXPECT(it != std::end(registered_listeners()));

        registered_listeners().erase(it);
    }

    //! \param l has to be non-nullptr.
    static bool is_registered(Listener* l) {
        UL_EXPECT(l);

        return std::find(std::begin(registered_listeners()), std::end(registered_listeners()), l)
               != std::end(registered_listeners());
    }

protected:
    using ListenerContainer = std::vector<Listener*>;

    static ListenerContainer& registered_listeners() {
        static ListenerContainer inst;
        return inst;
    }
};
} // namespace mb::ul

#endif
