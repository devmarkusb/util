// Markus Borris, 2016
// This file is part of tfl library.

//!
/**
 */
//! \file

#ifndef MAP_H_54982dn78txhwgf1236784g
#define MAP_H_54982dn78txhwgf1236784g


namespace too
{
//! Compile time map int->int.
/** But you shouldn't forget about the simpler runtime solution like
    \code std::map<int, char> m = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'}}; \endcode
    Not to mention that the compile time map is restricted to integral types.
    Nevertheless its usage is e.g.
    \code
    using some_map = too::ct_map<-1, kv<10, 11>, kv<20, 2002>, kv<30, 123>, kv<40, 546>>;
    static_assert(546 == test_map::at<40>::value, "mmmhh... no!");
    static_assert(-1 == test_map::at<347856>::value, "mmmhh... no!");
    \endcode
*/

//!@{

template <typename TypeForKey, typename TypeForValue, TypeForKey KEY, TypeForValue VALUE>
struct key_value
{
    static const TypeForKey key = KEY;
    static const TypeForValue value = VALUE;
};

// for convenience
template <int KEY, int VALUE>
using kv = key_value<int, int, KEY, VALUE>;

template <typename TypeForValue, TypeForValue NOT_FOUND_VALUE, typename...>
struct ct_mapTT;

template <int NOT_FOUND_VALUE, typename... Args>
using ct_map = ct_mapTT<int, NOT_FOUND_VALUE, Args...>;

template <typename TypeForValue, TypeForValue NOT_FOUND_VALUE>
struct ct_mapTT<TypeForValue, NOT_FOUND_VALUE>
{
    template <TypeForValue>
    struct at
    {
        static const TypeForValue value = NOT_FOUND_VALUE;
    };
};

template <
    typename TypeForKey, typename TypeForValue, TypeForValue NOT_FOUND_VALUE, TypeForKey KEY, TypeForValue VALUE,
    typename... FURTHER_KEY_VALUEs>
struct ct_mapTT<TypeForValue, NOT_FOUND_VALUE, key_value<TypeForKey, TypeForValue, KEY, VALUE>, FURTHER_KEY_VALUEs...>
{
    template <TypeForKey THIS_KEY>
    struct at
    {
        static const TypeForValue value =
            (THIS_KEY == KEY)
                ? VALUE
                : ct_mapTT<TypeForValue, NOT_FOUND_VALUE, FURTHER_KEY_VALUEs...>::template at<THIS_KEY>::value;
    };
};

//!@}
} // namespace too

#endif
