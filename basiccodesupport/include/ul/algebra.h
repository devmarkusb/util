/** \file
    Inspired by 'From Mathematics to Generic Programming'*/

#ifndef ALGEBRA_H_25CAEB73847C4B97992F8BD3BE310F1A
#define ALGEBRA_H_25CAEB73847C4B97992F8BD3BE310F1A

#include "concepts.h"
#include "foundations.h"
#include "integer.h"
#include "ul/assert.h"
#include "ul/config.h"
#if __has_include(<concepts>)
#include <concepts>
#endif
#include <functional>
#include <limits>

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
/**
    Synonym of Regular suiting a mathematically oriented naming scheme.
    Roughly, you can think of the concept as the category of sets, e.g. int being a type fulfilling/implementing that,
    i.e. being a set. Any variable then being an element or object of that set.
    More precisely, Set is a concept (programming) is a theory (math, like also Group can be considered as a theory and
    not only an algebraic structure) is a genus (Aristotle),
    and e.g. int is a value/object type (programming) is a model (math) is a species (Aristotle),
    and e.g. 42 is a value/object/instance (programming) is an element (math) is an individual (Aristotle).
 */
template <typename S>
concept Set = Regular<S>;

namespace v1 {
template <typename Op, typename SetT>
concept SemigroupOperation =
    Set<SetT> && std::regular_invocable<Op, SetT, SetT> && std::is_same_v<SetT, std::invoke_result_t<Op, SetT, SetT>>
    && requires(Op op, SetT a, SetT b, SetT c) {
           { op(a, b) } -> std::convertible_to<SetT>;
           UL_SEMANTICS {
               op(op(a, b), c) == op(a, op(b, c));
           };
       };
} // namespace v1

inline namespace v2 {
template <typename Op>
concept SemigroupOperation = Set<Domain<Op>> && BinaryOperation<Op> && Associative<Op>;
} // namespace v2

template <typename Op>
concept Semigroup = SemigroupOperation<Op>;

template <typename SetT>
concept NoncommutativeAdditiveSemigroup = Semigroup<std::plus<SetT>>;

template <typename SetT>
concept MultiplicativeSemigroup = Semigroup<std::multiplies<SetT>>;

template <typename Op>
concept MonoidOperation = SemigroupOperation<Op> && requires(Op op, Domain<Op> a, Domain<Op> identity) {
                                                        UL_SEMANTICS {
                                                            op(identity, a) == identity;
                                                            op(a, identity) == identity;
                                                        };
                                                    };

template <typename Op>
concept CommutativeMonoidOperation =
    MonoidOperation<Op> && requires(Op op, Domain<Op> a, Domain<Op> b) {
                               UL_SEMANTICS {
                                   op(a, b) == op(b, a);
                               };
                           };

template <typename Op>
concept Monoid = MonoidOperation<Op>;

template <typename Op>
concept CommutativeMonoid = CommutativeMonoidOperation<Op>;

template <typename SetT>
concept NoncommutativeAdditiveMonoid = Monoid<std::plus<SetT>>;

template <typename SetT>
concept MultiplicativeMonoid = Monoid<std::multiplies<SetT>>;

template <typename Op>
concept GroupOperation = MonoidOperation<Op>;

template <typename Op>
concept GroupInverseOperation =
    Set<Domain<Op>> && UnaryOperation<Op> && requires(Op op, Domain<Op> a) {
                                                 { op(a) } -> std::convertible_to<Domain<Op>>;
                                                 UL_SEMANTICS {
                                                     op(op(a)) == a;
                                                 };
                                             };

template <typename Op, typename InverseOp>
concept Group = GroupOperation<Op> && GroupInverseOperation<InverseOp>
                && requires(Op op, InverseOp inverse_op, Domain<Op> a, Domain<Op> identity) {
                       UL_SEMANTICS {
                           op(a, inverse_op(a)) == identity;
                           op(inverse_op(a), a) == identity;
                       };
                   };

template <typename SetT>
concept NoncommutativeAdditiveGroup = Group<std::plus<SetT>, std::negate<SetT>>;

template <typename SetT>
concept AdditiveGroup = NoncommutativeAdditiveGroup<SetT> && requires(SetT a, SetT b) {
                                                                 UL_SEMANTICS {
                                                                     a + b == b + a;
                                                                 };
                                                             };

/** Borderline, works for arithmetic types but enforces general Regular to provide a constructor accepting integer 1
    to produce the multiplicative identity element.*/
template <Regular T>
T identity_element(std::multiplies<T>) {
    return T{1};
}

template <Regular T>
T identity_element(std::plus<T>) {
    return T{};
}

/** Borderline, works for arithmetic types but enforces general Regular to provide a constructor accepting integer 1
    to produce the multiplicative identity element.*/
template <Regular T>
T multiplicative_inverse(T a) {
    return identity_element(std::multiplies<T>{}) / a;
}

template <Regular T>
struct Reciprocal {
    T operator()(T a) const {
        return multiplicative_inverse(a);
    }
};

template <typename SetT>
concept MultiplicativeGroup = Group<std::multiplies<SetT>, Reciprocal<SetT>>;

template <typename OpCommutativeMonoid /*add*/, typename OpMonoid /*multiply*/>
concept SemiRing =
    CommutativeMonoid<OpCommutativeMonoid> && Monoid<OpMonoid>
    && std::is_convertible_v<Domain<OpCommutativeMonoid>, Domain<OpMonoid>>
    && std::is_convertible_v<Domain<OpMonoid>, Domain<OpCommutativeMonoid>>
    && requires(
        OpCommutativeMonoid op_commutative_monoid, OpMonoid op_monoid, Domain<OpMonoid> a, Domain<OpMonoid> b,
        Domain<OpMonoid> c, Domain<OpCommutativeMonoid> commutative_monoid_identity, Domain<OpMonoid> monoid_identity) {
           UL_SEMANTICS {
               commutative_monoid_identity != monoid_identity;
               op_monoid(commutative_monoid_identity, a) == commutative_monoid_identity;
               op_monoid(a, op_commutative_monoid(b, c)) == op_commutative_monoid(op_monoid(a, b), op_monoid(a, c));
           };
       };

template <typename SetT>
concept SemiRingAddMult = SemiRing<std::plus<SetT>, std::multiplies<SetT>>;

template <typename OpCommutativeGroup /*add*/, typename OpInvCommutativeGroup /*add*/, typename OpMonoid /*multiply*/>
concept Ring = SemiRing<OpCommutativeGroup, OpMonoid> && Group<OpCommutativeGroup, OpInvCommutativeGroup>;

template <
    typename OpCommutativeGroup /*add*/, typename OpInvCommutativeGroup /*add*/,
    typename OpCommutativeMonoid /*multiply*/>
concept CommutativeRing =
    Ring<OpCommutativeGroup, OpInvCommutativeGroup, OpCommutativeMonoid> && CommutativeMonoid<OpCommutativeMonoid>;

template <
    typename OpCommutativeGroup /*add*/, typename OpInvCommutativeGroup /*add*/,
    typename OpCommutativeMonoid /*multiply*/>
concept IntegralDomain =
    CommutativeRing<OpCommutativeGroup, OpInvCommutativeGroup, OpCommutativeMonoid>
    && requires(
        OpCommutativeMonoid op_commutative_monoid, Domain<OpCommutativeGroup> a, Domain<OpCommutativeGroup> b,
        Domain<OpCommutativeGroup> commutative_group_identity) {
           UL_SEMANTICS {
               // a * b = 0  => a = 0 || b = 0 (no other zero divisor besides 0)
               // 0: commutativeGroupIdentity, *: opCommutativeMonoid
               op_commutative_monoid(a, b) != commutative_group_identity
                   || (a == commutative_group_identity || b == commutative_group_identity);
           };
       };

template <
    typename OpCommutativeGroup /*add*/, typename OpInvCommutativeGroup /*add*/,
    typename OpCommutativeMonoid /*multiply*/, typename OpQuotient, typename OpRemainder, typename OpNorm>
concept EuclideanDomain =
    IntegralDomain<OpCommutativeGroup, OpInvCommutativeGroup, OpCommutativeMonoid> && BinaryOperation<OpQuotient>
    && BinaryOperation<OpRemainder> && std::regular_invocable<OpNorm, Domain<OpCommutativeGroup>>
    && NaturalNumber<std::invoke_result_t<OpNorm, Domain<OpCommutativeGroup>>>
    && requires(
        Domain<OpCommutativeGroup> a, Domain<OpCommutativeGroup> b,
        Domain<OpCommutativeGroup> commutative_group_identity, OpQuotient op_quotient, OpRemainder op_remainder,
        OpCommutativeMonoid op_commutative_monoid, OpCommutativeGroup op_commutative_group, OpNorm op_norm) {
           { op_quotient(a, b) } -> std::convertible_to<Domain<OpCommutativeGroup>>;
           { op_remainder(a, b) } -> std::convertible_to<Domain<OpCommutativeGroup>>;
           { op_norm(a) } -> NaturalNumber;
           UL_SEMANTICS {
               b != commutative_group_identity;
               a == op_commutative_group(op_commutative_monoid(op_quotient(a, b), b), op_remainder(a, b));
               (op_norm(a) == commutative_group_identity) == (a == commutative_group_identity);
               op_norm(op_commutative_monoid(a, b)) >= op_norm(a);
               op_norm(op_remainder(a, b)) <= op_norm(b);
           };
       };

template <Set S>
struct Abs {
    auto operator()(const S& elem) const {
        if constexpr (!std::is_unsigned_v<S>) {
            return std::abs(elem);
        } else {
            return elem;
        }
    }
};

template <typename SetT>
concept EuclideanDomainAddMult = EuclideanDomain<
    std::plus<SetT>, std::negate<SetT>, std::multiplies<SetT>, std::divides<SetT>, std::modulus<SetT>, Abs<SetT>>;

template <
    typename OpCommutativeGroup /*add*/, typename OpInvCommutativeGroup /*add*/,
    typename OpCommutativeGroupMul /*multiply*/, typename OpInvCommutativeGroupMul /*multiply*/>
concept Field = IntegralDomain<OpCommutativeGroup, OpInvCommutativeGroup, OpCommutativeGroupMul>
                && Group<OpCommutativeGroupMul, OpInvCommutativeGroupMul>;

template <AdditiveGroup T>
std::negate<T> inverse_operation(std::plus<T>) {
    return std::negate<T>{};
}

template <MultiplicativeGroup T>
Reciprocal<T> inverse_operation(std::multiplies<T>) {
    return Reciprocal<T>{};
}

template <NoncommutativeAdditiveMonoid A>
struct Min {
    A operator()(const A& a1, const A& a2) const {
        return std::min(a1, a2);
    }
};

struct Tropical {
    static constexpr auto inf{std::numeric_limits<double>::infinity()};

    constexpr Tropical() noexcept = default;

    constexpr /*implicit*/ Tropical(double d) noexcept
        : d_{d} {
    }

    constexpr /*implicit*/ operator double() const noexcept {
        return d_;
    }

private:
    double d_{inf};
};

template <MultiplicativeSemigroup A, Integral N>
A power_accumulate_semigroup(A r, A a, N n) {
    UL_EXPECT(n >= 0);
    if (n == 0)
        return r;
    while (true) {
        if (odd(n)) {
            r *= a;
            if (n == 1)
                return r;
        }
        n = half_nonnegative(n);
        a *= a;
    }
}

template <MultiplicativeSemigroup A, Integral N>
A power_semigroup(A a, N n) {
    UL_EXPECT(n > 0);
    while (!odd(n)) {
        a *= a;
        n = half_nonnegative(n);
    }
    if (n == 1)
        return a;
    return power_accumulate_semigroup(a, a * a, half_nonnegative(n - 1));
}

template <MultiplicativeMonoid A, Integral N>
A power_monoid(const A& a, N n) {
    UL_EXPECT(n >= 0);
    if (n == 0)
        return A{1};
    return power_semigroup(a, n);
}

template <Integral N, SemigroupOperation Op>
Domain<Op> power_accumulate_semigroup(Domain<Op> r, Domain<Op> a, N n, Op op) {
    UL_EXPECT(n >= 0);
    if (n == 0)
        return r;
    while (true) {
        if (odd(n)) {
            r = op(r, a);
            if (n == 1)
                return r;
        }
        n = half_nonnegative(n);
        a = op(a, a);
    }
}

//! For a multiply operation as op you achieve the canonical 'power'.
template <Integral N, SemigroupOperation Op>
Domain<Op> power_semigroup(Domain<Op> a, N n, Op op) {
    UL_EXPECT(n > 0);
    while (!odd(n)) {
        a = op(a, a);
        n = half_nonnegative(n);
    }
    if (n == 1)
        return a;
    return power_accumulate_semigroup(a, op(a, a), half_nonnegative(n - 1), op);
}

template <Integral N, MonoidOperation Op>
Domain<Op> power_monoid(Domain<Op> a, N n, Op op, const Domain<Op>& identity) {
    UL_EXPECT(n >= 0);
    if (n == 0)
        return identity;
    return power_semigroup(a, n, op);
}

template <Integral N, GroupOperation Op, GroupInverseOperation InvOp>
Domain<Op> power_group(Domain<Op> a, N n, Op op, InvOp invop, const Domain<Op>& identity) {
    if (n < 0) {
        n = -n;
        a = invop(a);
    }
    return power_monoid(a, n, op, identity);
}
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
