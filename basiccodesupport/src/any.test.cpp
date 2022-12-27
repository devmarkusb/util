#include "ul/any.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

namespace
{
namespace A
{
// doc real type in comments
enum class Attr
{
    a1, // int
    a2, // long
};
// doc real type in comments
enum class LargeData
{
    d1, // std::vector<int>
    d2, // std::vector<long>
};

struct IA
{
    virtual ~IA() = default;
    //...

    virtual void set(Attr, const ul::any&) = 0;
    virtual ul::any get(Attr) const = 0;
    virtual bool copy_to(LargeData, const ul::any&) = 0;
    virtual bool copy_from(LargeData, ul::any&) const = 0;
};

struct AFactory
{
    std::unique_ptr<IA> createA();
};

struct A : public IA
{
    explicit operator bool() const
    {
        return true; // untested
    }

    virtual void set(Attr a, const ul::any& val) override
    {
        switch (a)
        {
            case Attr::a1:
                this->a1 = ul::any_cast<int>(val);
                break;
            default:
                break;
        }
    }

    virtual ul::any get(Attr a) const override
    {
        switch (a)
        {
            case Attr::a1:
                return this->a1;
            default:
                return ul::any{};
        }
    }

    virtual bool copy_to(LargeData d, const ul::any& data) override
    {
        switch (d)
        {
            case LargeData::d1:
                this->d1 = ul::any_cast<std::vector<int>>(data);
                return true;
            default:
                return false;
        }
    }

    virtual bool copy_from(LargeData d, ul::any& data) const override
    {
        switch (d)
        {
            case LargeData::d1:
                data = this->d1;
                return true;
            default:
                return false;
        }
    }

private:
    int a1 = 0;
    std::vector<int> d1;
};

std::unique_ptr<IA> AFactory::createA()
{
    return ul::make_unique<A>();
}
} // namespace A
} // namespace

TEST(ul_anyTest, test)
{
    A::AFactory afactory;
    std::unique_ptr<A::IA> ia = afactory.createA();
    ul::any a1 = 5;
    ul::any test(5);
    ul::any test2{5};
    ia->set(A::Attr::a1, a1);
    ul::any a1get = ia->get(A::Attr::a1);
    EXPECT_EQ(5, ul::any_cast<int>(a1get));
    ul::any a1get_err = ia->get(A::Attr::a2);
    EXPECT_TRUE(a1get_err.empty());
    EXPECT_THROW(ul::any_cast<long>(a1get_err), ul::bad_any_cast);

    ul::any d1 = std::vector<int>{1, 2, 3, 4, 5};
    ia->copy_to(A::LargeData::d1, d1);
    ul::any d1get;
    ia->copy_from(A::LargeData::d1, d1get);
    std::vector<int> test_v{1, 2, 3, 4, 5};
    EXPECT_EQ(test_v, ul::any_cast<std::vector<int>>(d1get));
    EXPECT_THROW(ul::any_cast<std::vector<bool>>(d1get), ul::bad_any_cast);

    ul::any* pd1 = &d1;
    ia->copy_to(A::LargeData::d1, *pd1);
    ul::any* pd1get = &d1get;
    ia->copy_from(A::LargeData::d1, *pd1get);
    std::vector<int>* pvres = ul::any_cast<std::vector<int>>(pd1get);
    EXPECT_EQ(test_v, *pvres);

    const ul::any* cpd1get = pd1get;
    const std::vector<int>* cpvres = ul::any_cast<std::vector<int>>(cpd1get);
    EXPECT_EQ(test_v, *cpvres);
}
