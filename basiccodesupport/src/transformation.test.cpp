#include "ul/transformation.h"
#include "ul/assert.h"
#include "ul/gtest_utils.h"

#include "gtest/gtest.h"

namespace {
const auto pos_example_nr{42};
const auto neg_example_nr{-15};

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

//! Constants for the following transf_ex_orbit examples.
//!@{
const auto cycle_size_ex{5};
const auto orbit_terminator_ex{100};
const auto orbit_separator_ex{50};

//!@}

//! Expects x < orbit_terminator.
int transf_ex_terminating_orbit(int x) {
    UL_EXPECT(x < orbit_terminator_ex);
    if (x >= orbit_terminator_ex)
        throw std::overflow_error{"transf_ex_terminating_orbit"};
    return ++x;
}

bool transf_ex_terminating_orbit_pred(int x) {
    return x < orbit_terminator_ex;
}

/** Cycle size cycle_size. Handle size 0 for 0 <= x <= cycle_size-1, 1 for -1 and >= cycle_size, up to cycle_size
    for <= -2.*/
int transf_ex_non_terminating_orbit(int x) {
    return (x + 1) % cycle_size_ex;
}

int transf_ex_orbit(int x) {
    if (x >= orbit_separator_ex)
        return transf_ex_terminating_orbit(x);
    return transf_ex_non_terminating_orbit(x);
}

bool transf_ex_orbit_pred(int x) {
    return x < orbit_terminator_ex;
}

template <std::integral N>
N hf(const N& x) {
    return x / N{2};
}

bool hf_pred(int) {
    return true;
}

template <ul::Transformation F, ul::UnaryPredicate P>
    requires std::same_as<ul::Domain<F>, ul::Domain<P>>
std::ostream& orbit_dump(std::ostream& os, F f, P p, std::string_view loglabel) {
    const auto example_count{50};
    os << loglabel << " orbit_dump\n";
    ul::orbit_dump(os, f, p, -4, example_count);
    ul::orbit_dump(os, f, p, neg_example_nr - 1, example_count);
    ul::orbit_dump(os, f, p, neg_example_nr, example_count);
    ul::orbit_dump(os, f, p, neg_example_nr + 1, example_count);
    ul::orbit_dump(os, f, p, -1, example_count);
    ul::orbit_dump(os, f, p, 0, example_count);
    ul::orbit_dump(os, f, p, 1, example_count);
    ul::orbit_dump(os, f, p, pos_example_nr, example_count);
    ul::orbit_dump(os, f, p, pos_example_nr * 2, example_count);
    return os;
}
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

TEST(intersectTest, both_terminating) {
    EXPECT_TRUE(
        ul::intersect(orbit_terminator_ex / 2, 1, transf_ex_terminating_orbit, transf_ex_terminating_orbit_pred));
}

TEST(intersectTest, non_terminating_and_terminating) {
    EXPECT_FALSE(ul::intersect(orbit_separator_ex - 2, orbit_separator_ex + 2, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_FALSE(ul::intersect(orbit_separator_ex + 2, orbit_separator_ex - 2, transf_ex_orbit, transf_ex_orbit_pred));
}

TEST(intersectTest, both_non_terminating) {
    // both circular
    EXPECT_TRUE(ul::intersect(0, pos_example_nr, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_TRUE(ul::intersect(pos_example_nr, 0, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_TRUE(ul::intersect(0, 2, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_TRUE(ul::intersect(2, 0, transf_ex_orbit, transf_ex_orbit_pred));
    // circular and \rho-shaped
    EXPECT_TRUE(ul::intersect(pos_example_nr, 2, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_TRUE(ul::intersect(2, pos_example_nr, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_TRUE(ul::intersect(-1, 2, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_TRUE(ul::intersect(2, -1, transf_ex_orbit, transf_ex_orbit_pred));
    // both \rho-shaped
    EXPECT_TRUE(ul::intersect(pos_example_nr, -2, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_TRUE(ul::intersect(-2, pos_example_nr, transf_ex_orbit, transf_ex_orbit_pred));
    EXPECT_TRUE(ul::intersect(-2, -1, transf_ex_orbit, transf_ex_orbit_pred));
}

TEST(convergent_point_guardedTest, misc) {
    EXPECT_TRUE(ul::convergent_point_guarded(1024, 64, hf<int>, hf_pred) == 64);
    EXPECT_TRUE(ul::convergent_point_guarded(1025, 65, hf<int>, hf_pred) == 32);
    EXPECT_TRUE(ul::convergent_point_guarded(64, 1024, hf<int>, hf_pred) == 64);
    EXPECT_TRUE(ul::convergent_point_guarded(65, 1025, hf<int>, hf_pred) == 32);
    EXPECT_TRUE(ul::convergent_point_guarded(1024, 2047, hf<int>, hf_pred) == 1);
}

TEST(orbit_dumpTest, test) {
    std::stringstream ss;
    orbit_dump(ss, transf_ex_terminating_orbit, transf_ex_terminating_orbit_pred, "transf_ex_terminating_orbit");
    EXPECT_EQ(ss.str(), R"(transf_ex_terminating_orbit orbit_dump
-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,
-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,
-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,
-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,
84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,terminated
)");
}

TEST(orbit_dumpTest, dumps) {
    ul::dump_test_name();
    orbit_dump(std::cout, transf_ex_terminating_orbit, transf_ex_terminating_orbit_pred, "transf_ex_terminating_orbit");
    orbit_dump(
        std::cout, transf_ex_non_terminating_orbit,
        [](int) {
            return true;
        },
        "transf_ex_non_terminating_orbit");
    orbit_dump(std::cout, transf_ex_orbit, transf_ex_orbit_pred, "transf_ex_orbit");
    const auto o{TransfEx<int>{-20, 20, 5}};
    orbit_dump(std::cout, o, o.p, "TransfEx");
}
