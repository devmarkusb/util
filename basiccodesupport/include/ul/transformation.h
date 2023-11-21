#ifndef TRANSFORMATION_H_3B9BF5527F2149448A2F6A415A26D6D6
#define TRANSFORMATION_H_3B9BF5527F2149448A2F6A415A26D6D6

#include "concepts.h"
#include "foundations.h"
#include "ul/assert.h"
#include "ul/config.h"
#if __has_include(<concepts>)
#include <concepts>
#endif
#include <tuple>

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
namespace most_generic {
template <typename>
concept Transformation = true;

template <typename>
concept Action = true;

template <typename F>
concept TransformationOrAction = Transformation<F> || Action<F>;

template <typename T>
concept TransformationOrActionOrIntegral = Transformation<T> || Action<T> || std::integral<T>;
} // namespace most_generic

template <most_generic::TransformationOrActionOrIntegral T>
struct DistanceTypeDecl {
    using Type = std::make_unsigned_t<std::type_identity_t<T>>;
};

template <most_generic::TransformationOrActionOrIntegral T>
using DistanceType = typename DistanceTypeDecl<T>::Type;

template <most_generic::TransformationOrActionOrIntegral T>
using DistanceType = typename DistanceTypeDecl<T>::Type;

template <typename F>
concept Transformation = UnaryOperation<F> && requires(F f) { DistanceType<F>{}; };

/// Fulfills/models Transformation concept.
template <Regular T>
using TransformationFctByValue = T (*)(T);

template <Regular T>
using TransformationFctByConstRef = T (*)(const T&);

template <typename F>
concept Action = std::invocable<F, Domain<F>&> && std::is_same_v<void, std::invoke_result_t<F, Domain<F>&>>;

template <Transformation F, std::integral N>
Domain<F> power_unary(Domain<F> x, N n, F f) {
    UL_EXPECT(n >= 0);
    // expect f^i(x) defined for all 0<i<=n
    while (n) {
        --n;
        x = f(x);
    }
    return x;
}

template <std::integral T>
struct DistanceTypeDecl<TransformationFctByValue<T>> {
    using Type = std::make_unsigned_t<T>;
};

template <std::integral T>
struct DistanceTypeDecl<TransformationFctByConstRef<T>> {
    using Type = std::make_unsigned_t<T>;
};

template <Transformation F>
DistanceType<F> distance(Domain<F> x, const Domain<F>& y, F f) {
    // expect y reachable from x under f
    using N = DistanceType<F>;
    N n{};
    while (x != y) {
        x = f(x);
        ++n;
    }
    return n;
}

template <Action F>
DistanceType<F> distance(Domain<F> x, const Domain<F>& y, F f) {
    // expect y reachable from x under f
    using N = DistanceType<F>;
    N n{};
    auto xc{std::move(x)};
    while (xc != y) {
        f(xc);
        ++n;
    }
    return n;
}

template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
Domain<F> collision_point(const Domain<F>& x, F f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    if (!p(x))
        return x;
    Domain<F> slow{x};
    Domain<F> fast{f(x)};

    while (fast != slow) {
        slow = f(slow);
        if (!p(fast))
            return fast;
        fast = f(fast);
        if (!p(fast))
            return fast;
        fast = f(fast);
    }
    return fast;
}

template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>> bool
terminating(const Domain<F>& x, F f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    return !p(collision_point(x, f, p));
}

template <Transformation F>
Domain<F> collision_point_nonterminating_orbit(const Domain<F>& x, F f) {
    auto slow{x};
    auto fast{f(x)};

    while (fast != slow) {
        slow = f(slow);
        fast = f(fast);
        fast = f(fast);
    }
    return fast;
}

template <Action F>
Domain<F> collision_point_nonterminating_orbit(const Domain<F>& x, F f) {
    auto slow{x};
    auto fast{x};
    f(fast);

    while (fast != slow) {
        f(slow);
        f(fast);
        f(fast);
    }
    return fast;
}

template <Transformation F>
bool circular_nonterminating_orbit(const Domain<F>& x, F f) {
    return x == f(collision_point_nonterminating_orbit(x, f));
}

template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>> bool
circular(const Domain<F>& x, F f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    const auto y{collision_point(x, f, p)};
    return p(y) && x == f(y);
}

template <Transformation F>
Domain<F> convergent_point(Domain<F> x0, Domain<F> x1, F f) {
    // expect n existing in DistanceType<F>, such that n >= 0 and f^n(x0) = f^n(x1)
    while (x0 != x1) {
        x0 = f(x0);
        x1 = f(x1);
    }
    return x0;
}

template <Action F>
Domain<F> convergent_point(Domain<F> x0, Domain<F> x1, F f) {
    // expect n existing in DistanceType<F>, such that n >= 0 and f^n(x0) = f^n(x1)
    auto x0c{std::move(x0)};
    auto x1c{std::move(x1)};
    while (x0c != x1c) {
        f(x0c);
        f(x1c);
    }
    return x0c;
}

template <Transformation F>
Domain<F> connection_point_nonterminating_orbit(const Domain<F>& x, F f) {
    return convergent_point(x, f(collision_point_nonterminating_orbit(x, f)), f);
}

template <Action F>
Domain<F> connection_point_nonterminating_orbit(const Domain<F>& x, F f) {
    auto f_collision_point{collision_point_nonterminating_orbit(x, f)};
    f(f_collision_point);
    return convergent_point(x, f_collision_point, f);
}

template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
Domain<F> connection_point(const Domain<F>& x, F f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    const auto y{collision_point(x, f, p)};
    if (!p(y))
        return y;
    return convergent_point(x, f(y), f);
}

/** \return (0, c-1, x) for circular orbits and
    (h, c-1, connection point) for \\rho-shaped orbits, where h is the handle size and c the cycle size.*/
template <Transformation F>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure_nonterminating_orbit(const Domain<F>& x, F f) {
    const auto y{connection_point_nonterminating_orbit(x, f)};
    return std::make_tuple(distance(x, y, f), distance(f(y), y, f), y);
}

template <Action F>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure_nonterminating_orbit(const Domain<F>& x, F f) {
    const auto y{connection_point_nonterminating_orbit(x, f)};
    auto f_y{y};
    f(f_y);
    return std::make_tuple(distance(x, y, f), distance(f_y, y, f), y);
}

/** \return (h-1, 0, terminal element) for terminating orbits, (0, c-1, x) for circular orbits and
    (h, c-1, connection point) for \\rho-shaped orbits, where h is the handle size and c the cycle size.*/
template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure(const Domain<F>& x, F f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    using N = DistanceType<F>;
    const auto y{connection_point(x, f, p)};
    const auto m{distance(x, y, f)};
    N n{};
    if (p(y))
        n = distance(f(y), y, f);
    return std::make_tuple(m, n, y);
}

template <Action F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure(const Domain<F>& x, F f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    using N = DistanceType<F>;
    const auto y{connection_point(x, f, p)};
    const auto m{distance(x, y, f)};
    N n{};
    auto f_y{y};
    f(f_y);
    if (p(y))
        n = distance(f_y, y, f);
    return std::make_tuple(m, n, y);
}
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
