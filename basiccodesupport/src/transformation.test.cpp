#include "ul/transformation.h"
#include "ul/assert.h"
#include "gtest/gtest.h"

namespace {
// definition space predicate for transf_ex
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

    I operator()(I x) {
        UL_EXPECT(p(x));
        ++x;
        if (x == p.x_0 + static_cast<I>(p.h) + static_cast<I>(p.c))
            x = p.x_0 + static_cast<I>(p.h);
        return x;
    }
};
} // namespace

class OrbitAlgTest
    : public testing::TestWithParam<std::tuple<int, std::make_unsigned_t<int>, std::make_unsigned_t<int>>> {};

INSTANTIATE_TEST_SUITE_P(OrbitAlgMisc, OrbitAlgTest, testing::Values(std::make_tuple(0, 0u, 5u)));

TEST_P(OrbitAlgTest, misc) {
    //    using F = TransfEx<int>;
    //    const auto x{std::get<0>(GetParam())};
    //    const auto h{std::get<1>(GetParam())};
    //    const auto c{std::get<2>(GetParam())};
    //    using T = ul::Domain<F>;
    //    typedef DistanceType(F) N;
    //    F f(x, h, c);
    //    Assert(zero(c) == terminating(x, f, f.p));
    //    if (zero(h) && !zero(c)) {
    //        Assert(circular(x, f, f.p));
    //        Assert(circular_nonterminating_orbit(x, f));
    //    } else if (!zero(h)) {
    //        Assert(!circular(x, f, f.p));
    //        if (!zero(c))
    //            Assert(!circular_nonterminating_orbit(x, f));
    //    }
    //    T y = connection_point(x, f, f.p);
    //    Assert(power_unary<F>(x, h, f) == y);
    //    if (!zero(c))
    //        Assert(y == connection_point_nonterminating_orbit(x, f));
    //    triple<unsigned, unsigned, int> t = orbit_structure(x, f, f.p);
    //    if (zero(c)) { // terminating
    //        Assert(t.m0 == h);
    //        Assert(zero(t.m1));
    //        Assert(t.m2 == collision_point(x, f, f.p));
    //    } else if (zero(h)) { // circular
    //        Assert(zero(t.m0));
    //        Assert(t.m1 == predecessor(c));
    //        Assert(t.m2 == x);
    //    } else { // rho-shaped
    //        Assert(t.m0 == h);
    //        Assert(t.m1 == predecessor(c));
    //        Assert(t.m2 == y);
    //    }
    //    if (!zero(c)) {
    //        triple<N, N, T> t = orbit_structure_nonterminating_orbit(x, f);
    //        if (zero(h)) { // circular
    //            Assert(zero(t.m0));
    //            Assert(t.m1 == predecessor(c));
    //            Assert(t.m2 == x);
    //        } else { // rho-shaped
    //            Assert(t.m0 == h);
    //            Assert(t.m1 == predecessor(c));
    //            Assert(t.m2 == y);
    //        }
    //    }
}
