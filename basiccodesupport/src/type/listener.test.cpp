#include "ul/type/listener.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

namespace {
struct ModelListener : public ul::Listener {
    ~ModelListener() override = default;

    virtual void on_prop_changed() = 0;
};

struct ModelNotifier : public ul::ListenerRegister {
    void on_prop_changed() {
        for (auto& l : this->registered_listeners_)
            dynamic_cast<ModelListener*>(l)->on_prop_changed();
    }
};

struct Model : public ModelNotifier {
    [[nodiscard]] int get_prop() const {
        return this->prop_;
    }

    void set_prop(int x) {
        this->prop_ = x;
    }

    void do1() {
        set_prop(get_prop() + 3);
        on_prop_changed();
    }

private:
    int prop_{};
};

struct View : private ModelListener {
    View() {
        this->in_v_.register_listener(this);
    }

    ~View() override {
        this->in_v_.unregister_listener(this);
    }

    [[nodiscard]] int get_prop() const {
        return this->in_v_.get_prop();
    }

    void set_prop(int x) {
        if (x == this->in_v_.get_prop())
            return;
        this->in_v_.set_prop(x);
        prop_changed();
    }

    void prop_changed() {
    }

    void slot1() {
        this->in_v_.do1();
    }

private:
    Model in_v_;

    void on_prop_changed() override {
        prop_changed();
    }
};
} // namespace

TEST(ul_listenerTest, test) {
    View v;
    v.slot1();
}
