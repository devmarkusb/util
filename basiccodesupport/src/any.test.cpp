#include "toolib/any.h"
#include "gtest/gtest.h"


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

    virtual void set(Attr, const too::any&) = 0;
    virtual too::any get(Attr) const = 0;
    virtual bool copy_to(LargeData, const too::any&) = 0;
    virtual bool copy_from(LargeData, too::any&) const = 0;
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
    virtual void set(Attr a, const too::any& val) override
    {
        switch (a)
        {
            case Attr::a1:
                this->a1 = too::any_cast<int>(val);
                break;
            default:
                break;
        }
    }
    virtual too::any get(Attr a) const override
    {
        switch (a)
        {
            case Attr::a1:
                return this->a1;
            default:
                return too::any{};
        }
    }
    virtual bool copy_to(LargeData d, const too::any& data) override
    {
        switch (d)
        {
            case LargeData::d1:
                this->d1 = too::any_cast<std::vector<int>>(data);
                return true;
            default:
                return false;
        }
    }
    virtual bool copy_from(LargeData d, too::any& data) const override
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
    return too::make_unique<A>();
}
} // namespace A
} // namespace

TEST(too_anyTest, test)
{
    A::AFactory afactory;
    std::unique_ptr<A::IA> ia = afactory.createA();
    too::any a1 = 5;
    too::any test(5);
    too::any test2{5};
    ia->set(A::Attr::a1, a1);
    too::any a1get = ia->get(A::Attr::a1);
    EXPECT_EQ(5, too::any_cast<int>(a1get));
    too::any a1get_err = ia->get(A::Attr::a2);
    EXPECT_TRUE(a1get_err.empty());
    EXPECT_THROW(too::any_cast<long>(a1get_err), too::bad_any_cast);

    too::any d1 = std::vector<int>{1, 2, 3, 4, 5};
    ia->copy_to(A::LargeData::d1, d1);
    too::any d1get;
    ia->copy_from(A::LargeData::d1, d1get);
    std::vector<int> test_v{1, 2, 3, 4, 5};
    EXPECT_EQ(test_v, too::any_cast<std::vector<int>>(d1get));
    EXPECT_THROW(too::any_cast<std::vector<bool>>(d1get), too::bad_any_cast);

    too::any* pd1 = &d1;
    ia->copy_to(A::LargeData::d1, *pd1);
    too::any* pd1get = &d1get;
    ia->copy_from(A::LargeData::d1, *pd1get);
    std::vector<int>* pvres = too::any_cast<std::vector<int>>(pd1get);
    EXPECT_EQ(test_v, *pvres);

    const too::any* cpd1get = pd1get;
    const std::vector<int>* cpvres = too::any_cast<std::vector<int>>(cpd1get);
    EXPECT_EQ(test_v, *cpvres);
}
