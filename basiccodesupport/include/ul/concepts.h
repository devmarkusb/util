/** \file
    Most general ones and framework, sometimes also rough adhoc inventions.
    Otherwise concepts are spread to where to belong thematically, e.g. math.
    The proper starting point for general programming concepts and beyond is foundations.h.*/

#ifndef CONCEPTS_H_CFDDF4FE5F6D48D0BD50FCFBBAAE968A
#define CONCEPTS_H_CFDDF4FE5F6D48D0BD50FCFBBAAE968A

#include "ul/config.h"
#include <type_traits>
#if __has_include(<concepts>)
#include <concepts>
#endif
#if __has_include(<ranges>)
#include <ranges>
#endif

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
template <typename T, typename... U>
concept AnyOf = (std::same_as<T, U> || ...);

template <typename T, typename... U>
concept AllOf = (std::same_as<T, U> && ...);

template <typename T>
concept Dereferenceable = requires(T x) { *x; };

template <typename T>
concept EnumConcept = std::is_enum_v<T>;

template <typename From, typename To>
concept NonNarrowingConvertible = std::convertible_to<From, To> && requires(From f) { To{f}; };

#if __cpp_lib_ranges
template <typename T>
concept Range = std::ranges::range<T>;
#else
template <typename T>
concept Range = requires(T x) {
    { x.begin() } -> Dereferenceable;
    { x.end() } -> Dereferenceable;
};
#endif

template <typename T>
concept Container = Range<T> && requires(T x) {
    { x.size() } -> std::integral;
};

// not a concept, but no better place
template <typename>
inline constexpr bool always_false_v = false;
} // namespace mb::ul

/** It is not really functionally possible to specify semantic requirements within an requires expression of a concept.
    But this macro provides the slight improvement over just using a comment within the expression, that the compiler
    is at least able to check the syntax.
    Usage: \code requires { ...; UL_SEMANTICS { ... }; }; \endcode*/
#define UL_SEMANTICS requires requires

/** Using concepts while unfortunately being forced to keep compatibility with older compilers:
    Always prefer this instead of the requires clause - and prefer the requires clause in itself only, as it is not
    possible to define a macro for typename replacements, at least not for namespace scoped concepts.*/
#define UL_REQUIRES(x) requires x
#else
#define UL_REQUIRES(x)
#endif

UL_HEADER_END

#endif
