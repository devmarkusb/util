#include "toolib/class/listener.h"
#include <gtest/gtest.h>


namespace
{
struct Model_listener : public too::Listener
{
    virtual ~Model_listener()
    {
    }

    virtual void on_prop_changed() = 0;
};

struct Model_notifier : public too::ListenerRegister
{
    void on_prop_changed()
    {
        for (auto& l : this->registered_listeners)
            dynamic_cast<Model_listener*>(l)->on_prop_changed();
    }
};

struct Model : public Model_notifier
{
    int get_prop() const
    {
        return this->prop;
    }
    void set_prop(int x)
    {
        this->prop = x;
    }

    void do1()
    {
        set_prop(get_prop() + 5);
        on_prop_changed();
    }

private:
    int prop{};
};

struct View : private Model_listener
{
    View()
    {
        this->m_in_v.register_listener(this);
    }
    ~View() override
    {
        this->m_in_v.unregister_listener(this);
    }

    int get_prop() const
    {
        return this->m_in_v.get_prop();
    }
    void set_prop(int x)
    {
        if (x == this->m_in_v.get_prop())
            return;
        this->m_in_v.set_prop(x);
        prop_changed();
    }
    void prop_changed()
    {
    }

    void slot1()
    {
        this->m_in_v.do1();
    }

private:
    Model m_in_v;

    virtual void on_prop_changed() override
    {
        prop_changed();
    }
};
} // namespace

TEST(too_listenerTest, test)
{
    View v;
    v.slot1();
}
