#include "ul/type/listener.h"
#include <gtest/gtest.h>

namespace ul = mb::ul;

namespace
{
struct Model_listener : public ul::Listener
{
    ~Model_listener() override = default;

    virtual void on_prop_changed() = 0;
};

struct Model_notifier : public ul::ListenerRegister
{
    void on_prop_changed()
    {
        for (auto& l : this->registeredListeners_)
            dynamic_cast<Model_listener*>(l)->on_prop_changed();
    }
};

struct Model : public Model_notifier
{
    [[nodiscard]] int get_prop() const
    {
        return this->prop_;
    }

    void set_prop(int x)
    {
        this->prop_ = x;
    }

    void do1()
    {
        set_prop(get_prop() + 5);
        on_prop_changed();
    }

private:
    int prop_{};
};

struct View : private Model_listener
{
    View()
    {
        this->in_v_.registerListener(this);
    }

    ~View() override
    {
        this->in_v_.unregisterListener(this);
    }

    [[nodiscard]] int get_prop() const
    {
        return this->in_v_.get_prop();
    }

    void set_prop(int x)
    {
        if (x == this->in_v_.get_prop())
            return;
        this->in_v_.set_prop(x);
        prop_changed();
    }

    void prop_changed()
    {
    }

    void slot1()
    {
        this->in_v_.do1();
    }

private:
    Model in_v_;

    void on_prop_changed() override
    {
        prop_changed();
    }
};
} // namespace

TEST(ul_listenerTest, test)
{
    View v;
    v.slot1();
}
