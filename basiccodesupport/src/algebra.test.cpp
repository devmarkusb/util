#include "ul/algebra.h"
#include "ex-for-test.h"

#include "gtest/gtest.h"

namespace {
using UInt = unsigned int;
// constexpr Int some_int{42};
const std::string str42{"42"}; // NOLINT
const std::string str43{"43"}; // NOLINT
constexpr UInt uint42{42};
constexpr UInt uint43{43};
const std::string str_comma{","}; // NOLINT
} // namespace

TEST(semigroup, tests) {
    const auto op{[](ul::NoncommutativeAdditiveSemigroup auto a, ul::NoncommutativeAdditiveSemigroup auto b) {
        return a + b;
    }};
    EXPECT_EQ(op(str42, str43), "4243");
    EXPECT_EQ(op(str43, str42), "4342");
    EXPECT_NE(op(str42, str43), op(str43, str42));
    EXPECT_EQ(op(op(str42, str_comma), str43), op(str42, op(str_comma, str43)));

    const NoncommutativeAdditiveSemigroupEx a{"1"};
    const NoncommutativeAdditiveSemigroupEx b{"2"};
    const NoncommutativeAdditiveSemigroupEx c{"3"};
    EXPECT_EQ(op(op(a, b), c), op(a, op(b, c)));
}

TEST(monoid, tests) {
    const auto op{[](ul::NoncommutativeAdditiveMonoid auto a, ul::NoncommutativeAdditiveMonoid auto b) {
        return a + b;
    }};
    EXPECT_EQ(op(str42, str43), "4243");
    EXPECT_EQ(op(str43, str42), "4342");
    EXPECT_NE(op(str42, str43), op(str43, str42));
    EXPECT_EQ(op(op(str42, str_comma), str43), op(str42, op(str_comma, str43)));

    const NoncommutativeAdditiveMonoidEx a{"1"};
    const NoncommutativeAdditiveMonoidEx b{"2"};
    const NoncommutativeAdditiveMonoidEx c{"3"};
    const NoncommutativeAdditiveMonoidEx id{};
    EXPECT_EQ(op(op(a, b), c), op(a, op(b, c)));
    EXPECT_EQ(op(id, c), c);
}

TEST(group, tests) {
    const auto op{[](ul::NoncommutativeAdditiveGroup auto a, ul::NoncommutativeAdditiveGroup auto b) {
        return a + b;
    }};
    EXPECT_EQ(op(str42, str43), "4243");
    EXPECT_EQ(op(str43, str42), "4342");
    EXPECT_NE(op(str42, str43), op(str43, str42));
    EXPECT_EQ(op(op(str42, str_comma), str43), op(str42, op(str_comma, str43)));

    const NoncommutativeAdditiveGroupEx a{1};
    const NoncommutativeAdditiveGroupEx b{2};
    const NoncommutativeAdditiveGroupEx c{3};
    const NoncommutativeAdditiveGroupEx id{};
    const NoncommutativeAdditiveGroupEx inv_b{-2};
    EXPECT_EQ(op(op(a, b), c), op(a, op(b, c)));
    EXPECT_EQ(op(id, c), c);
    EXPECT_EQ(op(b, inv_b), id);
    EXPECT_EQ(op(inv_b, b), id);
}

TEST(semiring, tests) {
    const auto example_op{[](ul::SemiRingAddMult auto a, ul::SemiRingAddMult auto b, ul::SemiRingAddMult auto c) {
        return (a + b) * c;
    }};

    SemiRingOpCommMonoidEx semi_ring_op_comm_monoid_ex{};
    SemiRingOpMonoidEx semi_ring_op_monoid_ex{};

    EXPECT_EQ(semi_ring_op_comm_monoid_ex(uint42, uint43), 85);
    EXPECT_EQ(semi_ring_op_comm_monoid_ex(uint43, uint42), 85);
    EXPECT_EQ(semi_ring_op_comm_monoid_ex(uint42, uint43), semi_ring_op_comm_monoid_ex(uint43, uint42));
    EXPECT_EQ(
        semi_ring_op_comm_monoid_ex(semi_ring_op_comm_monoid_ex(uint42, 1), uint43),
        semi_ring_op_comm_monoid_ex(uint42, semi_ring_op_comm_monoid_ex(1, uint43)));

    EXPECT_EQ(semi_ring_op_monoid_ex(2, 3), 6);
    EXPECT_EQ(
        semi_ring_op_monoid_ex(semi_ring_op_monoid_ex(uint42, 2), uint43),
        semi_ring_op_monoid_ex(uint42, semi_ring_op_monoid_ex(2, uint43)));

    const SemiRingEx id_comm_monoid{};
    const SemiRingEx id_monoid{1};
    EXPECT_EQ(example_op(id_comm_monoid, 2, id_monoid), 2);
    EXPECT_EQ(example_op(2, id_comm_monoid, id_monoid), 2);
    EXPECT_EQ(example_op(2, 3, id_monoid), 5);
    EXPECT_EQ(example_op(2, 3, 2), 10);
}

TEST(power_accumulate_semigroup, tests) {
    EXPECT_EQ(ul::power_accumulate_semigroup(1, 2, 3), 8);
    EXPECT_EQ(ul::power_accumulate_semigroup(2, 2, 3), 16);
}

TEST(power_semigroup, tests) {
    // abuse as multiply
    EXPECT_EQ(ul::power_semigroup(2, 4, std::plus<int>{}), 8);
    EXPECT_EQ(ul::power_semigroup(2, 4, std::multiplies<int>{}), 16);
}

using MultiplicativeMonoidEx = unsigned int;
using MonoidEx = std::string;

TEST(power_monoid, tests) {
    EXPECT_EQ(ul::power_monoid(MultiplicativeMonoidEx{1}, 3), MultiplicativeMonoidEx{1});
    EXPECT_EQ(ul::power_monoid(MultiplicativeMonoidEx{2}, 3), MultiplicativeMonoidEx{8});
    EXPECT_EQ(ul::power_monoid(MultiplicativeMonoidEx{3}, 1), MultiplicativeMonoidEx{3});
    EXPECT_EQ(ul::power_monoid(MultiplicativeMonoidEx{3}, 2), MultiplicativeMonoidEx{9});

    EXPECT_EQ(ul::power_monoid(MonoidEx{"a"}, 3, std::plus<MonoidEx>{}, MonoidEx{""}), MonoidEx{"aaa"});
    EXPECT_EQ(
        ul::power_monoid(
            MonoidEx{"a"}, 3,
            [](const MonoidEx& a, const MonoidEx& b) {
                return a + b;
            },
            MonoidEx{""}),
        MonoidEx{"aaa"});
}

using CommutativeAdditiveGroupEx = int;

TEST(power_group, tests) {
    EXPECT_EQ(
        ul::power_group(
            CommutativeAdditiveGroupEx{1}, 3, std::plus<CommutativeAdditiveGroupEx>{},
            std::negate<CommutativeAdditiveGroupEx>{}, CommutativeAdditiveGroupEx{}),
        CommutativeAdditiveGroupEx{3});
    EXPECT_EQ(
        ul::power_group(
            CommutativeAdditiveGroupEx{2}, 3, std::plus<CommutativeAdditiveGroupEx>{},
            std::negate<CommutativeAdditiveGroupEx>{}, CommutativeAdditiveGroupEx{}),
        CommutativeAdditiveGroupEx{6});
    EXPECT_EQ(
        ul::power_group(
            CommutativeAdditiveGroupEx{3}, 1, std::plus<CommutativeAdditiveGroupEx>{},
            std::negate<CommutativeAdditiveGroupEx>{}, CommutativeAdditiveGroupEx{}),
        CommutativeAdditiveGroupEx{3});
    EXPECT_EQ(
        ul::power_group(
            CommutativeAdditiveGroupEx{3}, 2, std::plus<CommutativeAdditiveGroupEx>{},
            std::negate<CommutativeAdditiveGroupEx>{}, CommutativeAdditiveGroupEx{}),
        CommutativeAdditiveGroupEx{6});
}
