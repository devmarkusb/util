#ifndef TRANSFORMATION_H_3B9BF5527F2149448A2F6A415A26D6D6
#define TRANSFORMATION_H_3B9BF5527F2149448A2F6A415A26D6D6

#include "concepts.h"
#include "foundations.h"
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

template <typename F>
concept Action = std::invocable<F, Domain<F>&> && std::is_same_v<void, std::invoke_result_t<F, Domain<F>&>>;

template <std::integral T>
struct DistanceTypeDecl<TransformationFctByValue<T>> {
    using Type = std::make_unsigned_t<T>;
};

template <Transformation F>
DistanceType<F> distance(Domain<F> x, Domain<F> y, F f) {
    // expect y reachable from x under f
    using N = DistanceType<F>;
    N n{};
    while (x != y) {
        x = f(x);
        n = n + N{1};
    }
    return n;
}

template <Action F>
DistanceType<F> distance(Domain<F> x, Domain<F> y, F f) {
    // expect y reachable from x under f
    using N = DistanceType<F>;
    N n{};
    while (x != y) {
        f(x);
        n = n + N{1};
    }
    return n;
}

template <Transformation F>
Domain<F> collision_point_nonterminating_orbit(Domain<F> x, F f) {
    auto slow = x;
    auto fast = f(x);

    while (fast != slow) {
        slow = f(slow);
        fast = f(fast);
        fast = f(fast);
    }
    return fast;
}

template <Action F>
Domain<F> collision_point_nonterminating_orbit(Domain<F> x, F f) {
    auto slow = x;
    auto fast = x;
    f(fast);

    while (fast != slow) {
        f(slow);
        f(fast);
        f(fast);
    }
    return fast;
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
    while (x0 != x1) {
        f(x0);
        f(x1);
    }
    return x0;
}

template <Transformation F>
Domain<F> connection_point_nonterminating_orbit(Domain<F> x, F f) {
    return convergent_point(x, f(collision_point_nonterminating_orbit(x, f)), f);
}

template <Action F>
Domain<F> connection_point_nonterminating_orbit(Domain<F> x, F f) {
    auto f_collision_point{collision_point_nonterminating_orbit(x, f)};
    f(f_collision_point);
    return convergent_point(x, f_collision_point, f);
}

/** \return (0, c-1, x) for circular orbits and
    (h, c-1, connection point) for \\rho-shaped orbits, where h is the handle size and c the cycle size.*/
template <Transformation F>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure_nonterminating_orbit(Domain<F> x, F f) {
    const auto y{connection_point_nonterminating_orbit(x, f)};
    return std::make_tuple(distance(x, y, f), distance(f(y), y, f), y);
}

template <Action F>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure_nonterminating_orbit(Domain<F> x, F f) {
    const auto y{connection_point_nonterminating_orbit(x, f)};
    auto f_y{y};
    f(f_y);
    return std::make_tuple(distance(x, y, f), distance(f_y, y, f), y);
}

/** \return (h-1, 0, terminal element) for terminating orbits, (0, c-1, x) for circular orbits and
    (h, c-1, connection point) for \\rho-shaped orbits, where h is the handle size and c the cycle size.*/
template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure(Domain<F> x, F f, P p) {
    // expect p(x) implies f(x) defined
    using N = DistanceType<F>;
    const auto y{connection_point(x, f, p)};
    const auto m{distance(x, y, f)};
    const N n{};
    if (p(y))
        n = distance(f(y), y, f);
    return std::make_tuple(m, n, y);
}

template <Action F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure(Domain<F> x, F f, P p) {
    // expect p(x) implies f(x) defined
    using N = DistanceType<F>;
    const auto y{connection_point(x, f, p)};
    const auto m{distance(x, y, f)};
    const N n{};
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