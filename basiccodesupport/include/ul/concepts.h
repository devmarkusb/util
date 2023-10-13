/** \file
Most general ones. Otherwise they are spread to where to belong thematically, e.g. math.
*/

#ifndef CONCEPTS_H_CFDDF4FE5F6D48D0BD50FCFBBAAE968A
#define CONCEPTS_H_CFDDF4FE5F6D48D0BD50FCFBBAAE968A

#include "ul/config.h"
#if __has_include(<concepts>)
#include <concepts>
#endif

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

template <class T, class... U>
concept AnyOf = (std::same_as<T, U> || ...);

// not a concept, but no better place
template <class>
inline constexpr bool always_false_v = false;
} // namespace mb::ul
#endif

UL_HEADER_END

#endif
