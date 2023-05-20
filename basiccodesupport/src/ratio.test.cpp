#include "ul/ratio.h"
#include "gtest/gtest.h"

// NOLINTBEGIN
TEST(gcdTest, test) {
    EXPECT_EQ(5, ul::math::gcd(10, 5));
    EXPECT_EQ(1, ul::math::gcd(11, 5));
    EXPECT_EQ(2, ul::math::gcd(24, 22));
    EXPECT_EQ(8, ul::math::gcd(24, 32));
}

TEST(gcdTest, pathol) {
    EXPECT_EQ(42, ul::math::gcd(42, 0));
    EXPECT_EQ(42, ul::math::gcd(0, 42));
    EXPECT_EQ(1, ul::math::gcd(1, 1));
    EXPECT_EQ(1, ul::math::gcd(1, 2));
    EXPECT_EQ(1, ul::math::gcd(1, 23432));
    EXPECT_EQ(1, ul::math::gcd(23432, 1));
}

TEST(gcdTest, neg) {
    EXPECT_EQ(5, ul::math::gcd(-10, 5));
    EXPECT_EQ(1, ul::math::gcd(11, -5));
    EXPECT_EQ(2, ul::math::gcd(24, -22));

    // these don't work (and need not)
    // EXPECT_EQ(8, ul::math::gcd(-24, -32));
    // EXPECT_EQ(42, ul::math::gcd(-42, 0));
    // EXPECT_EQ(42, ul::math::gcd(0, -42));
}

TEST(lcmTest, test) {
    EXPECT_EQ(10, ul::math::lcm(10, 5));
    EXPECT_EQ(55, ul::math::lcm(11, 5));
    EXPECT_EQ(264, ul::math::lcm(24, 22));
    EXPECT_EQ(96, ul::math::lcm(24, 32));
}

TEST(RationalTest, constr) {
    const ul::math::Rational r1{1, 2};
    EXPECT_EQ(1, r1.num);
    EXPECT_EQ(2, r1.denom);

    const ul::math::Rational r2{4};
    EXPECT_EQ(4, r2.num);
    EXPECT_EQ(1, r2.denom);

    const ul::math::Rational r3{-1, 2};
    EXPECT_EQ(-1, r3.num);
    EXPECT_EQ(2, r3.denom);

    const ul::math::Rational r4{-4};
    EXPECT_EQ(-4, r4.num);
    EXPECT_EQ(1, r4.denom);

    const auto m = std::micro{};
    const ul::math::Rational r5{m};
    EXPECT_EQ(1, r5.num);
    EXPECT_EQ(1000000, r5.denom);

    const auto k = std::kilo{};
    const ul::math::Rational r6{k};
    EXPECT_EQ(1000, r6.num);
    EXPECT_EQ(1, r6.denom);
}

// NOLINTEND

TEST(RationalTest, constr_zero) {
    const ul::math::Rational r1{};
    EXPECT_EQ(0, r1.num);
    EXPECT_EQ(1, r1.denom);

    const ul::math::Rational r2;
    EXPECT_EQ(0, r2.num);
    EXPECT_EQ(1, r2.denom);

    const ul::math::Rational r3{0};
    EXPECT_EQ(0, r3.num);
    EXPECT_EQ(1, r3.denom);
}

TEST(RationalTest, assign) {
    ul::math::Rational r{};
    const ul::math::Rational r1{24, 32};

    r = r1;
    EXPECT_EQ(24, r.num);
    EXPECT_EQ(32, r.denom);
}

TEST(RationalTest, inv) {
    ul::math::Rational r1{1, 2};
    r1.inverse();
    EXPECT_EQ(2, r1.num);
    EXPECT_EQ(1, r1.denom);

    ul::math::Rational r2{-1, 2};
    r2.inverse();
    EXPECT_EQ(-2, r2.num);
    EXPECT_EQ(1, r2.denom);
}

TEST(RationalTest, asfloat) {
    const ul::math::Rational r1{1, 2};
    const double d{r1.as_floating_point<double>()};
    EXPECT_DOUBLE_EQ(0.5, d);
}

// NOLINTBEGIN
TEST(RationalTest, reduce) {
    ul::math::Rational r1{24, 32};
    r1.reduce();
    EXPECT_EQ(3, r1.num);
    EXPECT_EQ(4, r1.denom);

    ul::math::Rational r2{-24, 32};
    r2.reduce();
    EXPECT_EQ(-3, r2.num);
    EXPECT_EQ(4, r2.denom);
}

TEST(RationalTest, eq) {
    const ul::math::Rational r1{24, 32};
    const ul::math::Rational r1_{3, 4};
    EXPECT_TRUE(r1 == r1_);
    EXPECT_FALSE(r1 != r1_);
    EXPECT_FALSE(r1 < r1_);
    EXPECT_FALSE(r1 > r1_);
    EXPECT_TRUE(r1 <= r1_);
    EXPECT_TRUE(r1 >= r1_);

    const ul::math::Rational r2{-24, 32};
    const ul::math::Rational r2_{-3, 4};
    EXPECT_TRUE(r2 == r2_);

    const ul::math::Rational r3_{3, 4};
    EXPECT_FALSE(r2 == r3_);
    EXPECT_TRUE(r2 != r3_);

    const ul::math::Rational r4{3, 4};
    EXPECT_TRUE(r4 == r3_);

    const ul::math::Rational r5{3, 5};
    EXPECT_FALSE(r5 == r3_);

    const ul::math::Rational r6{4, 4};
    EXPECT_FALSE(r6 == r3_);
}

TEST(RationalTest, make_common_denom) {
    ul::math::Rational r1{1, 32};
    ul::math::Rational r2{1, 24};
    ul::math::make_common_denom(r1, r2);

    const ul::math::Rational r1_{3, 96};
    EXPECT_TRUE(r1 == r1_);
    const ul::math::Rational r2_{4, 96};
    EXPECT_TRUE(r2 == r2_);
}

TEST(RationalTest, make_common_denom_neg) {
    {
        ul::math::Rational r1{1, 32};
        ul::math::Rational r2{-1, 24};
        ul::math::make_common_denom(r1, r2);

        const ul::math::Rational r1_{3, 96};
        EXPECT_TRUE(r1 == r1_);
        const ul::math::Rational r2_{-4, 96};
        EXPECT_TRUE(r2 == r2_);
    }
    {
        ul::math::Rational r1{-1, 32};
        ul::math::Rational r2{1, 24};
        ul::math::make_common_denom(r1, r2);

        const ul::math::Rational r1_{-3, 96};
        EXPECT_TRUE(r1 == r1_);
        const ul::math::Rational r2_{4, 96};
        EXPECT_TRUE(r2 == r2_);
    }
    {
        ul::math::Rational r1{-1, 32};
        ul::math::Rational r2{-1, 24};
        ul::math::make_common_denom(r1, r2);

        const ul::math::Rational r1_{-3, 96};
        EXPECT_TRUE(r1 == r1_);
        const ul::math::Rational r2_{-4, 96};
        EXPECT_TRUE(r2 == r2_);
    }
}

TEST(RationalTest, lessthan) {
    {
        const ul::math::Rational r1{2, 4};
        const ul::math::Rational r2{3, 4};
        const ul::math::Rational r3{4, 4};

        EXPECT_TRUE(r1 < r2);
        EXPECT_TRUE(r2 < r3);
    }
    {
        const ul::math::Rational r1{3, 5};
        const ul::math::Rational r2{3, 4};
        const ul::math::Rational r3{3, 3};

        EXPECT_TRUE(r1 < r2);
        EXPECT_TRUE(r2 < r3);
    }
    {
        const ul::math::Rational r1{5, 8};
        const ul::math::Rational r2{3, 4};
        const ul::math::Rational r3{7, 8};

        EXPECT_TRUE(r1 < r2);
        EXPECT_TRUE(r2 < r3);
    }
}

TEST(RationalTest, lessthan_consts) {
    using namespace mb::ul::math; // NOLINT

    EXPECT_TRUE(atto < femto);
    EXPECT_TRUE(femto < pico);
    EXPECT_TRUE(pico < nano);
    EXPECT_TRUE(nano < micro);
    EXPECT_TRUE(micro < milli);
    EXPECT_TRUE(milli < centi);
    EXPECT_TRUE(centi < deci);
    EXPECT_TRUE(deci < one);
    EXPECT_TRUE(one < deca);
    EXPECT_TRUE(deca < hecto);
    EXPECT_TRUE(hecto < kilo);
    EXPECT_TRUE(kilo < mega);
    EXPECT_TRUE(mega < giga);
    EXPECT_TRUE(giga < tera);
    EXPECT_TRUE(tera < peta);
    EXPECT_TRUE(peta < exa);

    EXPECT_TRUE(peta <= exa);
    EXPECT_FALSE(peta >= exa);
    EXPECT_FALSE(peta > exa);

    EXPECT_TRUE(exa >= peta);
    EXPECT_FALSE(exa <= peta);
    EXPECT_FALSE(exa < peta);
}

TEST(RationalTest, booltest) {
    const ul::math::Rational r1{0, 32};
    if (!r1.is_null())
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    if (!r1)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);

    const ul::math::Rational r2{1, 24};
    if (!r2)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    if (!r2.is_null())
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    if (!!r2)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
}

TEST(RationalTest, unaryminus) {
    const ul::math::Rational exp_mr1{-1, 24};
    const ul::math::Rational r1{1, 24};
    const ul::math::Rational mr1{-r1};
    EXPECT_TRUE(exp_mr1 == mr1);
}

// NOLINTEND
