//! \file

#ifndef INTEGER_SEQUENCE_H_ERUIX4GH8452GH3G8XH3
#define INTEGER_SEQUENCE_H_ERUIX4GH8452GH3G8XH3

#include <utility>

namespace mb::ul::idx {
// I would have liked to use the documentation https://en.cppreference.com/w/cpp/utility/integer_sequence
// in order to use C++1x board tools to simplify things. But wtf do they expect one should do with the
// non-compiling snippet of make_integer_sequence?!
//
//template <std::size_t... Ints>
//using index_sequence = std::integer_sequence<std::size_t, Ints...>;
//
//template<class T, T N>
//using make_integer_sequence = std::integer_sequence<T, /* a sequence 0, 1, 2, ..., N-1 */ >;


/** Use case is e.g. when you want to pass an std::array to a function expecting a variadic template parameter
    list.
    So in order to call
        template <typename... T>
        void toBeCalled(T... t) {}
    with some example array
        std::array<int, 3> arr{1, 2, 3};
    you have to write an intermediate helper function like
        template <int... Is>
        void caller_impl(const std::array<int, 3>& theArray, ul::idx::seq<Is...>)
        {
            toBeCalled(theArray[Is]...);
        }
    which you call like
        void caller(const std::array<int, 3>& theArray)
        {
            caller_impl(theArray, ul::idx::GenSeq<theArray.size()>());
        }
    The use of ul::idx::GenSeq generates a sequence of a certain array length, which is then used to fill in a
    template parameter that allows fold-expanding the array by index - the function parameter ul::idx::seq itself
    isn't used directly.*/
template <int... is>
struct Seq {};

template <int n, int... is>
struct GenSeq : GenSeq<n - 1, n - 1, is...> {};

template <int... is>
struct GenSeq<0, is...> : Seq<is...> {};
} // namespace mb::ul::idx

#endif
