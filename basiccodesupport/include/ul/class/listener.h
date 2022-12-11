// 2016-22

//! \file

#ifndef LISTENER_H_sjkdzth78tn2378xgh73fr
#define LISTENER_H_sjkdzth78tn2378xgh73fr

#include "ul/assert.h"
#include <algorithm>
#include <vector>


namespace mb::ul
{
//! Base class for every abstract listener interface.
/** Usage:
    \code
    struct Model_listener : public ul::Listener
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
            this->m_in_v.registerListener(this);
        }
        ~View()
        {
            this->m_in_v.unregisterListener(this);
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
inline Listener::~Listener() = default;

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
class ListenerRegister
{
public:
    virtual ~ListenerRegister() = default;

    /** \param l has to be non-nullptr and valid/alive until calling unregisterListener on it,
        which also has to be called before l's livetime ends.
        The same l also mustn't be registered more than once.*/
    virtual void registerListener(Listener* l)
    {
        UL_EXPECT(l);
        UL_EXPECT(
            std::find(std::begin(this->registeredListeners_), std::end(this->registeredListeners_), l)
            == std::end(this->registeredListeners_));

        this->registeredListeners_.push_back(l);
    }

    /** \param l has to be a non-nullptr, still valid, already via registerListener registered Listener.
        The same l also mustn't be unregistered more than once.*/
    virtual void unregisterListener(Listener* l)
    {
        UL_EXPECT(l);
        const auto it = std::find(std::begin(this->registeredListeners_), std::end(this->registeredListeners_), l);
        UL_EXPECT(it != std::end(this->registeredListeners_));

        this->registeredListeners_.erase(it);
    }

    //! \param l has to be non-nullptr.
    virtual bool isRegistered(Listener* l) const
    {
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
class ListenerStaticRegister
{
public:
    /** \param l has to be non-nullptr and valid/alive until calling unregisterListener on it,
        which also has to be called before l's livetime ends.
        The same l also mustn't be registered more than once.*/
    static void registerListener(Listener* l)
    {
        UL_EXPECT(l);
        UL_EXPECT(
            std::find(std::begin(registeredListeners()), std::end(registeredListeners()), l)
            == std::end(registeredListeners()));

        registeredListeners().push_back(l);
    }

    /** \param l has to be a non-nullptr, still valid, already via registerListener registered Listener.
        The same l also mustn't be unregistered more than once.*/
    static void unregisterListener(Listener* l)
    {
        UL_EXPECT(l);
        const auto it = std::find(std::begin(registeredListeners()), std::end(registeredListeners()), l);
        UL_EXPECT(it != std::end(registeredListeners()));

        registeredListeners().erase(it);
    }

    //! \param l has to be non-nullptr.
    static bool isRegistered(Listener* l)
    {
        UL_EXPECT(l);

        return std::find(std::begin(registeredListeners()), std::end(registeredListeners()), l)
               != std::end(registeredListeners());
    }

protected:
    using ListenerContainer = std::vector<Listener*>;
    static ListenerContainer& registeredListeners()
    {
        static ListenerContainer inst;
        return inst;
    }
};
} // namespace mb::ul

#endif
