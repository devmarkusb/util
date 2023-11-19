/** \file
    Inspired by 'Elements of Programming'*/

#ifndef FOUNDATIONS_H_85CAEB73747C4B9799298BD3EE310F1F
#define FOUNDATIONS_H_85CAEB73747C4B9799298BD3EE310F1F

#include "concepts.h"
#include "ul/config.h"
#include "ul/macros.h"
#if __has_include(<concepts>)
#include <concepts>
#endif
#include <functional>
#include <type_traits>
#include <utility>

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
inline namespace more_generic {
/** Opposed to std::regular this doesn't contain std::default_initializable.
    The intention of the latter might be to be able to write something like 'R x;' with possibly uninitialized x,
    perhaps modelling a mathematical 'let x be an arbitrary element of R'. But to be honest, that's not needed in
    programming. You are more likely to have a function implementation of 'f(R x)' (or 'f(Regular auto x)') getting in
    an arbitrary x. Also it is the preferable principle of generalization to impose less constraints. You can still
    have it for your specific type R if you want to. Above all it is just not helping to be forced to have a default
    constructor, quite on the contrary, so std::regular is not a good choice.

    Semantics:
    * T a{b} => (b==c => a==c)
    * a=b => (b==c => a==c)
    * f FunctionalProcedure and a==b => f(a)==f(b)
    Time/space complexity:
    * each operation on Regular is no worse than linear in the memory of the object

    Note: Right now it is highly improbable, that we also add total ordering and underlying type here, as proposed by
    the master (Stepanov) at some time. That's again too much restriction (which he himself at another place discourages
    us from rightfully. We will provide separate concepts.*/
template <typename R>
concept Regular = std::copyable<R> && std::equality_comparable<R> && requires(R a, R b, R c) {
                                                                         // would be too verbose / duplicated to have the following here (already part of the concepts built upon)
                                                                         // R{};
                                                                         // R{a};
                                                                         // a == a;
                                                                         // c = a;
                                                                         UL_SEMANTICS {
                                                                             c = a, c == a;
                                                                             !(a == b) || (!(b == c) || a == c);
                                                                         };
                                                                     };

template <typename R>
concept RegularTotallyOrdered = Regular<R> && std::totally_ordered<R>;

template <typename R>
concept SemiRegular = std::copyable<R>;
} // namespace more_generic

namespace generic {
/** Opposed to Regular from more_generic this here implies default initializable also. This allows for the equivalence
    of `T a; a = b;` and `T a{b};`. (`T a;` possibly uninitialized despite default constructed.) Sounds better at first,
    but read the comment at the actually chosen version of Regular.*/
template <typename R>
concept Regular = std::regular<R>;

template <typename R>
concept SemiRegular = std::semiregular<R>;
} // namespace generic

template <typename P>
concept Procedure = std::invocable<P>;

namespace most_generic {
template <typename>
concept FunctionalProcedure = true;
} // namespace most_generic

template <typename F, typename... Args>
concept FunctionalProcedure = std::regular_invocable<F, Args...> && !
std::is_same_v<void, std::invoke_result_t<F, Args...>> && (Regular<Args> && ...) && sizeof...(Args) >= 1;

// Remarks on the purpose and implementation of the following traits - where user code needs to define (specialize) the
// versions with postfix 'Decl', if not provided automatically.
// We want to be able to define algorithms like `template <Function F> Domain<F> do_sth(F f, Domain<F> x)` without
// the necessity of additionally passing the domain of `F` as another template parameter all the time. Ideally it
// should get deduced from `F` alone automatically - either by the maximum generic trait implementations below, or
// by handcrafted specializations on the application side whenever needed.
// Unfortunately, we need to use the most generic version of FunctionalProcedure here, otherwise we get (additional)
// constraint checks that seem syntactically impossible to fulfill. Specifically: the variadic arguments for the
// parameters of the underlying invocable concept.
namespace impl {
template <typename ArgTypes>
struct FirstArg {
    using Type = std::tuple_element_t<0, ArgTypes>;
};

template <>
struct FirstArg<std::tuple<>> {
    using Type = void;
};

template <typename>
struct Method {
    using Type = void;
};

template <typename Ret, typename Type, typename... Args>
struct Method<Ret (Type::*)(Args...) const> {
    using ReturnType = Ret;
    using ArgsTuple = std::tuple<Args...>;
    using FirstArgType = typename FirstArg<ArgsTuple>::Type;
};

template <typename F>
concept HasCallOperator = most_generic::FunctionalProcedure<F> && requires(F) { decltype(&F::operator()){}; };

template <most_generic::FunctionalProcedure F>
using CallOperator = Method<decltype(&F::operator())>;

template <typename>
struct Function {
    using Type = void;
};

template <typename Ret, typename... Args>
struct Function<Ret (*)(Args...)> {
    using ReturnType = Ret;
    using ArgsTuple = std::tuple<Args...>;
    using FirstArgType = typename FirstArg<ArgsTuple>::Type;
};

template <most_generic::FunctionalProcedure F>
using FunctionCall = Function<F>;

template <typename TypeForCheck>
struct AssertRegularityWeakSyntax {
    static_assert(
        (std::is_reference_v<TypeForCheck> && std::is_const_v<std::remove_reference_t<TypeForCheck>>)
            || (std::is_pointer_v<TypeForCheck> && std::is_const_v<std::remove_pointer_t<TypeForCheck>>)
            || (!std::is_pointer_v<TypeForCheck> && !std::is_reference_v<TypeForCheck>),
        "arguments/return have to by regular types, but we want to allow `const a*` and `const a&` as well");
};
} // namespace impl

template <int, most_generic::FunctionalProcedure>
struct InputTypeDecl;

template <int i, most_generic::FunctionalProcedure F>
using InputType = typename InputTypeDecl<i, F>::Type;

namespace most_generic {
template <typename>
concept HomogeneousFunction = true;
} // namespace most_generic

template <typename F, typename... Args>
concept HomogeneousFunction =
    FunctionalProcedure<F, Args...> && AllOf<std::tuple_element_t<0, std::tuple<Args...>>, Args...>;

template <most_generic::HomogeneousFunction F>
using Domain = InputType<0, F>;

template <most_generic::FunctionalProcedure F>
struct CodomainDecl {
    using TypeOrig = typename impl::Function<F>::ReturnType;
    using Type = std::remove_cvref_t<TypeOrig>;

private:
    using _ = impl::AssertRegularityWeakSyntax<TypeOrig>;
};

template <most_generic::FunctionalProcedure F>
using Codomain = typename CodomainDecl<F>::Type;

template <most_generic::FunctionalProcedure F>
struct InputTypeDecl<0, F> {
    static auto choose_type() {
        if constexpr (impl::HasCallOperator<F>) {
            return std::type_identity<typename impl::CallOperator<F>::FirstArgType>{};
        } else {
            return std::type_identity<typename impl::FunctionCall<F>::FirstArgType>{};
        }
    }

    using TypeOrig = typename decltype(choose_type())::type;
    using Type = std::remove_cvref_t<TypeOrig>;

private:
    using _ = impl::AssertRegularityWeakSyntax<TypeOrig>;
};

template <impl::HasCallOperator F>
struct CodomainDecl<F> {
    using TypeOrig = typename impl::CallOperator<F>::ReturnType;
    using Type = std::remove_cvref_t<TypeOrig>;

private:
    using _ = impl::AssertRegularityWeakSyntax<TypeOrig>;
};

template <typename F>
concept UnaryFunction = FunctionalProcedure<F, Domain<F>>;

template <typename P, typename... Args>
concept Predicate = FunctionalProcedure<P, Args...> && std::is_convertible_v<Codomain<P>, bool>;

template <typename P>
concept UnaryPredicate = UnaryFunction<P> && Predicate<P, Domain<P>>;

template <typename Op, typename... Args>
concept Operation = HomogeneousFunction<Op, Args...> && std::convertible_to<Domain<Op>, Codomain<Op>>;

template <typename Op>
concept UnaryOperation = Operation<Op, Domain<Op>>;

template <typename Op>
concept BinaryOperation = Operation<Op, Domain<Op>, Domain<Op>>;
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
