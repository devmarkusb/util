/** \file
Most general ones. Otherwise they are spread to where to belong thematically, e.g. math.
*/

#ifndef CONCEPTS_H_CFDDF4FE5F6D48D0BD50FCFBBAAE968A
#define CONCEPTS_H_CFDDF4FE5F6D48D0BD50FCFBBAAE968A

#include "ul/config.h"
#if __has_include(<concepts>)
#include <concepts>
#endif
#include <type_traits>

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul {
template <typename T>
concept Dereferenceable = requires(T x) { *x; };

template <typename T>
concept Container = requires(T x) {
                        { x.begin() } -> Dereferenceable;
                        { x.end() } -> Dereferenceable;
                        { x.size() } -> std::integral;
                    };

template <typename T, typename... U>
concept AnyOf = (std::same_as<T, U> || ...);

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

// not a concept, but no better place
template <typename>
inline constexpr bool always_false_v = false;
} // namespace mb::ul

/** Using concepts while unfortunately being forced to keep compatibility with older compilers:
    Always prefer this instead of the requires clause - and prefer the requires clause in itself only, as it is not
    possible to define a macro for typename replacements, at least not for namespace scoped concepts.*/
#define UL_REQUIRES(x) requires x
#else
#define UL_REQUIRES(x)
#endif

UL_HEADER_END

#endif
