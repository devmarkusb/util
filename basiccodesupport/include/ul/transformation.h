#ifndef TRANSFORMATION_H_3B9BF5527F2149448A2F6A415A26D6D6
#define TRANSFORMATION_H_3B9BF5527F2149448A2F6A415A26D6D6

#include "concepts.h"
#include "foundations.h"
#include "ul/assert.h"
#include "ul/config.h"
#if __has_include(<concepts>)
#include <concepts>
#endif
#include <optional>
#include <ostream>
#include <tuple>

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
namespace most_generic {
template <typename>
concept Transformation = true;

template <typename T>
concept TransformationOrIntegral = Transformation<T> || std::integral<T>;
} // namespace most_generic

template <most_generic::TransformationOrIntegral T>
struct DistanceTypeDecl {
    static auto choose_type() {
        if constexpr (std::integral<T>) {
            return std::type_identity<T>{};
        } else if constexpr (impl::HasCallOperator<T>) {
            return std::type_identity<typename impl::CallOperator<T>::FirstArgType>{};
        } else {
            return std::type_identity<typename impl::FunctionCall<T>::FirstArgType>{};
        }
    }

    using TypeOrig = typename decltype(choose_type())::type;
    using Type = std::make_unsigned_t<std::remove_cvref_t<TypeOrig>>;

    static_assert(impl::is_regularity_weak_syntax<TypeOrig>(), UL_REGULARITY_WEAK_SYNTAX_ERR_STR);
};

template <most_generic::TransformationOrIntegral T>
using DistanceType = typename DistanceTypeDecl<T>::Type;

template <typename F>
concept Transformation = UnaryOperation<F> && requires(F f) { DistanceType<F>{}; };

/** For implementation simplification we opted for usage inconvenience here: no auto deduction of domain type. It needs
    to be explicitly specified everywhere.*/
template <typename F, typename Arg>
concept Action = std::invocable<F, Arg&> && std::is_same_v<void, std::invoke_result_t<F, Arg&>>;

//! And an implementation - if ever needed - would go: `Type transformation(Type x) { action(x); return x; }`
template <Regular Arg, Action<Arg> A>
struct TransformationFromAction {
    using Type = std::function<Arg(Arg)>;
};

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

template <Regular Arg, Action<Arg> A, Transformation F = typename TransformationFromAction<Arg, A>::Type>
DistanceType<F> distance_action(Domain<F> x, const Domain<F>& y, A f) {
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

template <
    Regular Arg, Action<Arg> A, UnaryPredicate P, Transformation F = typename TransformationFromAction<Arg, A>::Type>
    requires std::same_as<Domain<F>, Domain<P>>
Domain<F> collision_point_action(Domain<F> x, A f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    if (!p(x))
        return x;
    Domain<F> slow{x};
    f(x);
    Domain<F> fast{x};

    while (fast != slow) {
        f(slow);
        if (!p(fast))
            return fast;
        f(fast);
        if (!p(fast))
            return fast;
        f(fast);
    }
    return fast;
}

template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>> bool
terminating(const Domain<F>& x, F f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    return !p(collision_point(x, f, p));
}

template <
    Regular Arg, Action<Arg> A, UnaryPredicate P, Transformation F = typename TransformationFromAction<Arg, A>::Type>
    requires std::same_as<Domain<F>, Domain<P>> bool
terminating_action(Domain<F> x, A f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    return !p(collision_point_action<Arg>(x, f, p));
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

template <Regular Arg, Action<Arg> A, Transformation F = typename TransformationFromAction<Arg, A>::Type>
Domain<F> collision_point_nonterminating_orbit_action(const Domain<F>& x, A f) {
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

template <Regular Arg, Action<Arg> A, Transformation F = typename TransformationFromAction<Arg, A>::Type>
Domain<F> convergent_point_action(Domain<F> x0, Domain<F> x1, A f) {
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

template <Regular Arg, Action<Arg> A, Transformation F = typename TransformationFromAction<Arg, A>::Type>
Domain<F> connection_point_nonterminating_orbit(const Domain<F>& x, A f) {
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

template <
    Regular Arg, Action<Arg> A, UnaryPredicate P, Transformation F = typename TransformationFromAction<Arg, A>::Type>
    requires std::same_as<Domain<F>, Domain<P>>
Domain<F> connection_point_action(Domain<F> x, A f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    auto y{collision_point_action<Arg>(x, f, p)};
    if (!p(y))
        return y;
    f(y);
    return convergent_point_action<Arg>(x, y, f);
}

/** \return (0, c-1, x) for circular orbits and
    (h, c-1, connection point) for \\rho-shaped orbits, where h is the handle size and c the cycle size.*/
template <Transformation F>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure_nonterminating_orbit(const Domain<F>& x, F f) {
    const auto y{connection_point_nonterminating_orbit(x, f)};
    return std::make_tuple(distance(x, y, f), distance(f(y), y, f), y);
}

template <Regular Arg, Action<Arg> A, Transformation F = typename TransformationFromAction<Arg, A>::Type>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure_nonterminating_orbit_action(
    const Domain<F>& x, A f) {
    const auto y{connection_point_nonterminating_orbit(x, f)};
    auto f_y{y};
    f(f_y);
    return std::make_tuple(distance(x, y, f), distance(f_y, y, f), y);
}

/** \return (h, 0, terminal element) for terminating orbits (we define the handle for terminating orbits to be the
    number of elements minus 1, last one is the connection point, which is not the beginning of a cycle but the point
    on which the transformation isn't defined anymore), (0, c-1, x) for circular orbits and
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

template <
    Regular Arg, Action<Arg> A, UnaryPredicate P, Transformation F = typename TransformationFromAction<Arg, A>::Type>
    requires std::same_as<Domain<F>, Domain<P>>
std::tuple<DistanceType<F>, DistanceType<F>, Domain<F>> orbit_structure_action(const Domain<F>& x, A f, P p) {
    // UL_EXPECT(!p(x) || f(x) defined);
    using N = DistanceType<F>;
    const auto y{connection_point_action<Arg>(x, f, p)};
    const auto m{distance_action<Arg>(x, y, f)};
    N n{};
    if (p(y)) {
        auto f_y{y};
        f(f_y);
        n = distance_action<Arg>(f_y, y, f);
    }
    return std::make_tuple(m, n, y);
}

/** Returns an (not necessarily the earliest - use convergent_point_guarded instead) intersection point if there is
    one.*/
template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
std::optional<Domain<F>> intersect(const Domain<F>& x0, const Domain<F>& x1, F f, P p) {
    // expect for any x in Domain<F>: p(x) <=> f(x) defined
    const auto x_coll{collision_point(x0, f, p)};
    const auto y_coll{collision_point(x1, f, p)};

    // Since intersection implies having in common all cyclic elements, the contraposition that there
    // is a cyclic element (and therefore a cycle) for x (or y), but none for y (or x), i.e. y (or x)
    // is terminating, we can return false immediately.
    if (p(x_coll) != p(y_coll))
        return {};

    if (!p(x_coll)) {
        // Case of two terminating orbits.
        // They intersect, if they have the last element in common. Intersection means having at least
        // one element in common. But that implies having all the following ones - applying the
        // transformation, in common also.
        return x_coll == y_coll ? std::optional{x_coll} : std::nullopt;
    }

    // Case of two non-terminating orbits (\rho-shaped or cyclic).
    // If they intersect, they have in common all cyclic elements. But the converse is also true for
    // orbits with non-empty common cycles.
    // Intersection happens, when the connection point of one orbit is reachable by the other.
    const auto x_conn{convergent_point(x0, f(x_coll), f)};
    const auto y_conn{convergent_point(x1, f(y_coll), f)};
    if (x_conn == y_conn || f(x_conn) == y_conn)
        return y_conn;
    for (auto x_runner{f(x_conn)}; x_runner != x_conn; x_runner = f(x_runner)) {
        if (x_runner == y_conn)
            return y_conn;
    }
    return {};
}

// Additionally you have to know that the convergent doesn't necessarily have to appear after an equal amount of
// transformation steps applied to x0 and x1. But it is defined as the earliest/first intersection point.
template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
Domain<F> convergent_point_guarded(Domain<F> x0, Domain<F> x1, F f, P p) {
    auto xs{intersect(x0, x1, f, p)};
    UL_EXPECT(xs);
    auto d0{distance(x0, *xs, f)};
    auto d1{distance(x1, *xs, f)};
    if (d0 < d1)
        x1 = power_unary(x1, d1 - d0, f);
    else if (d1 < d0)
        x0 = power_unary(x0, d0 - d1, f);
    return convergent_point(x0, x1, f);
}

template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
std::optional<Domain<F>> power_unary_guarded(Domain<F> x, DistanceType<F> n, F f, P p) {
    UL_EXPECT(n >= 0);
    using N = DistanceType<F>;
    while (n != N{0}) {
        n = n - N{1};
        if (!p(x)) {
            return {};
        }
        x = f(x);
    }
    return x;
}

template <Transformation F, UnaryPredicate P>
    requires std::same_as<Domain<F>, Domain<P>>
std::ostream& orbit_dump(std::ostream& os, F f, P p, Domain<F> starting_point, DistanceType<F> example_count) {
    for (decltype(example_count) i{}; i <= example_count; ++i) {
        const auto x{power_unary_guarded(starting_point, i, f, p)};
        if (x)
            os << *x << ',';
        else {
            os << "terminated";
            break;
        }
    }
    os << '\n';
    return os;
}
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
