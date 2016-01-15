// Markus Borris, 2016
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef CT_MAP_H_54982dn78txhwgf1236784g
#define CT_MAP_H_54982dn78txhwgf1236784g


namespace too
{
//! Compile time map.
/** But you shouldn't forget about the simpler runtime solution like
    \code std::map<int, char> m = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'}}; \endcode
    Nevertheless its usage is e.g.
    \code
    using some_map = too::ct_map<-1, kv<10, 11>, kv<20, 2002>, kv<30, 123>, kv<40, 546>>;
    \endcode
*/

//!@{

template <int KEY, int VALUE>
struct key_value
{
    static const int key   = KEY;
    static const int value = VALUE;
};

// for convenience
template <int KEY, int VALUE>
using kv = key_value<KEY, VALUE>;

template <int NOT_FOUND_VALUE, typename...>
struct ct_map;

template <int NOT_FOUND_VALUE>
struct ct_map<NOT_FOUND_VALUE>
{
    template <int>
    struct at
    {
        static const int value = NOT_FOUND_VALUE;
    };
};

template <int NOT_FOUND_VALUE, int KEY, int VALUE, typename... FURTHER_KEY_VALUEs>
struct ct_map<NOT_FOUND_VALUE, key_value<KEY, VALUE>, FURTHER_KEY_VALUEs...>
{
    template <int THIS_KEY>
    struct at
    {
        static const int value =
            (THIS_KEY == KEY) ? VALUE : ct_map<NOT_FOUND_VALUE, FURTHER_KEY_VALUEs...>::template at<THIS_KEY>::value;
    };
};

//!@}
}

#endif
