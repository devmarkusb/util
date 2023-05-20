//! \file

#ifndef MAP_H_54982DN78TXHWGF1236784G
#define MAP_H_54982DN78TXHWGF1236784G

namespace mb::ul {
//! Compile time map int->int.
/** But you shouldn't forget about the simpler runtime solution like
    \code std::map<int, char> m = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'}}; \endcode
    Not to mention that the compile time map is restricted to integral types.
    Nevertheless its usage is e.g.
    \code
    using some_map = ul::ct_map<-1, kv<10, 11>, kv<20, 2002>, kv<30, 123>, kv<40, 546>>;
    static_assert(546 == test_map::at<40>::value, "mmmhh... no!");
    static_assert(-1 == test_map::at<347856>::value, "mmmhh... no!");
    \endcode
*/

//!@{

template <typename TypeForKey, typename TypeForValue, TypeForKey key, TypeForValue value>
struct KeyValue {
    static const TypeForKey key = key;
    static const TypeForValue value = value;
};

// for convenience
template <int key, int value>
using kv = KeyValue<int, int, key, value>;

template <typename TypeForValue, TypeForValue not_found_value, typename...>
struct ct_mapTT;

template <int not_found_value, typename... Args>
using ct_map = ct_mapTT<int, not_found_value, Args...>;

template <typename TypeForValue, TypeForValue not_found_value>
struct ct_mapTT<TypeForValue, not_found_value> {
    template <TypeForValue>
    struct At {
        static const TypeForValue value = not_found_value;
    };
};

template <
    typename TypeForKey, typename TypeForValue, TypeForValue not_found_value, TypeForKey key, TypeForValue value,
    typename... FurtherKeyValuEs>
struct ct_mapTT<TypeForValue, not_found_value, KeyValue<TypeForKey, TypeForValue, key, value>, FurtherKeyValuEs...> {
    template <TypeForKey this_key>
    struct At {
        static const TypeForValue value =
            (this_key == key)
                ? value
                : ct_mapTT<TypeForValue, not_found_value, FurtherKeyValuEs...>::template at<this_key>::value;
    };
};

//!@}
} // namespace mb::ul

#endif
