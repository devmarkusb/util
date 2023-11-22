#include "ul/transformation.h"
#include "ul/assert.h"
#include "gtest/gtest.h"

namespace {
// definition space predicate for TransfEx
template <std::integral I>
struct TransfExPredicate {
    using N = ul::DistanceType<I>;
    I x_0{};
    N h{};
    N c{};

    TransfExPredicate(I x_0, N h, N c)
        : x_0{x_0}
        , h{h}
        , c{c} {
        UL_EXPECT(h >= 0 && c >= 0);
        UL_EXPECT(
            h < static_cast<N>(std::numeric_limits<I>::max()) && c < static_cast<N>(std::numeric_limits<I>::max()));
    }

    bool operator()(I x) const {
        return x_0 <= x && x < x_0 + static_cast<I>(h) + static_cast<I>(c);
    }
};

template <std::integral I>
struct TransfEx {
    using N = ul::DistanceType<I>;

    TransfExPredicate<I> p;

    TransfEx(I x_0, N h, N c)
        : p{x_0, h, c} {
        UL_EXPECT(h >= 0 && c >= 0);
        UL_EXPECT(
            h < static_cast<N>(std::numeric_limits<I>::max()) && c < static_cast<N>(std::numeric_limits<I>::max()));
    }

    I operator()(I x) const {
        UL_EXPECT(p(x));
        ++x;
        if (x == p.x_0 + static_cast<I>(p.h) + static_cast<I>(p.c))
            x = p.x_0 + static_cast<I>(p.h);
        return x;
    }
};

template <std::integral I>
struct ActionEx {
    using N = ul::DistanceType<I>;

    TransfExPredicate<I> p;

    ActionEx(I x_0, N h, N c)
        : p{x_0, h, c} {
        UL_EXPECT(h >= 0 && c >= 0);
        UL_EXPECT(
            h < static_cast<N>(std::numeric_limits<I>::max()) && c < static_cast<N>(std::numeric_limits<I>::max()));
    }

    void operator()(I& x) const {
        UL_EXPECT(p(x));
        ++x;
        if (x == p.x_0 + static_cast<I>(p.h) + static_cast<I>(p.c))
            x = p.x_0 + static_cast<I>(p.h);
    }
};
} // namespace

namespace mb::ul {
template <std::integral I>
struct DistanceTypeDecl<TransfEx<I>> {
    using Type = DistanceType<I>;
};
} // namespace mb::ul

class OrbitAlgTest
    : public testing::TestWithParam<std::tuple<int, std::make_unsigned_t<int>, std::make_unsigned_t<int>>> {};

// NOLINTBEGIN
INSTANTIATE_TEST_SUITE_P(
    OrbitAlgMisc, OrbitAlgTest,
    testing::Values(
        // cyclic
        std::make_tuple(0, 0u, 5u),
        // \rho-shaped
        std::make_tuple(0, 2u, 11u), std::make_tuple(7, 97u, 17u), std::make_tuple(0, 4u, 2u),
        // terminating
        std::make_tuple(0, 101u, 0u)));

// NOLINTEND

TEST_P(OrbitAlgTest, comprehensive) {
    using F = TransfEx<int>;
    // just two compilation checks
    using T = ul::Domain<F>;
    static_assert(std::same_as<T, int>);
    using N = ul::DistanceType<F>;
    static_assert(std::integral<N>);
    const auto x{std::get<0>(GetParam())};
    const auto h{std::get<1>(GetParam())};
    const auto c{std::get<2>(GetParam())};
    const F f{x, h, c};
    EXPECT_TRUE(!c == ul::terminating(x, f, f.p));
    if (!h && c) {
        EXPECT_TRUE(ul::circular(x, f, f.p));
        EXPECT_TRUE(ul::circular_nonterminating_orbit(x, f));
    } else if (h) {
        EXPECT_TRUE(!ul::circular(x, f, f.p));
        if (c) {
            EXPECT_TRUE(!ul::circular_nonterminating_orbit(x, f));
        }
    }
    const auto y{ul::connection_point(x, f, f.p)};
    EXPECT_TRUE(ul::power_unary<F>(x, h, f) == y);
    if (c) {
        EXPECT_TRUE(y == ul::connection_point_nonterminating_orbit(x, f));
    }
    auto [m0, m1, m2]{ul::orbit_structure(x, f, f.p)};
    if (!c) {
        // terminating
        EXPECT_TRUE(m0 == h);
        EXPECT_TRUE(!m1);
        EXPECT_TRUE(m2 == ul::collision_point(x, f, f.p));
        EXPECT_TRUE(m2 == y);
    } else if (!h) {
        // circular
        EXPECT_TRUE(!m0);
        EXPECT_TRUE(m1 == c - 1);
        EXPECT_TRUE(m2 == x);
    } else {
        // rho-shaped
        EXPECT_TRUE(m0 == h);
        EXPECT_TRUE(m1 == c - 1);
        EXPECT_TRUE(m2 == y);
    }
    if (c) {
        std::tie(m0, m1, m2) = ul::orbit_structure_nonterminating_orbit(x, f);
        if (!h) {
            // circular
            EXPECT_TRUE(!m0);
            EXPECT_TRUE(m1 == c - 1);
            EXPECT_TRUE(m2 == x);
        } else {
            // rho-shaped
            EXPECT_TRUE(m0 == h);
            EXPECT_TRUE(m1 == c - 1);
            EXPECT_TRUE(m2 == y);
        }
    }
}

TEST_P(OrbitAlgTest, action) {
    using Arg = int;
    using F = ActionEx<Arg>;
    const auto x{std::get<0>(GetParam())};
    const auto h{std::get<1>(GetParam())};
    const auto c{std::get<2>(GetParam())};
    const F f{x, h, c};
    EXPECT_TRUE(!c == ul::terminating_action<Arg>(x, f, f.p));
    const auto y{ul::connection_point_action<Arg>(x, f, f.p)};
    auto [m0, m1, m2]{ul::orbit_structure_action<Arg>(x, f, f.p)};
    if (!c) {
        // terminating
        EXPECT_TRUE(m0 == h);
        EXPECT_TRUE(!m1);
        EXPECT_TRUE(m2 == y);
    } else if (!h) {
        // circular
        EXPECT_TRUE(!m0);
        EXPECT_TRUE(m1 == c - 1);
        EXPECT_TRUE(m2 == x);
    } else {
        // rho-shaped
        EXPECT_TRUE(m0 == h);
        EXPECT_TRUE(m1 == c - 1);
        EXPECT_TRUE(m2 == y);
    }
}

TEST(distanceTest, misc) {
    const TransfEx<int> f_c{0, 0u, 5u};
    EXPECT_EQ(ul::distance(1, 3, f_c), 2u);
    EXPECT_EQ(ul::distance(2, 1, f_c), 4u);
    EXPECT_EQ(ul::distance(2, 2, f_c), 0u);
    EXPECT_EQ(ul::distance(0, 0, f_c), 0u);
    EXPECT_EQ(ul::distance(4, 4, f_c), 0u);
    EXPECT_EQ(ul::distance(4, 0, f_c), 1u);

    const TransfEx<int> f_r{0, 4u, 2u};
    EXPECT_EQ(ul::distance(0, 5, f_r), 5u);

    const TransfEx<int> f_t{0, 4u, 0u};
    EXPECT_EQ(ul::distance(0, 4, f_t), 4u);

    EXPECT_EQ(
        ul::distance(
            1, 5,
            [](int x) {
                return x + 1;
            }),
        4u);

    static_assert(ul::Action<
                  decltype([](int& x) {
                      ++x;
                  }),
                  int&>);
    EXPECT_EQ(
        ul::distance_action<int>(
            1, 5,
            [](int& x) {
                ++x;
            }),
        4u);
}
