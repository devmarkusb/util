#include "ul/map.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(ct_mapTest, test)
{
    // as the name says... this should be testable at compile time
    using ul::kv;
    using test_map = ul::ct_map<-1, kv<10, 11>, kv<20, 2002>, kv<30, 123>, kv<40, 546>>;

    static_assert(2002 == test_map::at<20>::value, "mmmhh... no!");
    static_assert(11 == test_map::at<10>::value, "mmmhh... no!");
    static_assert(123 == test_map::at<30>::value, "mmmhh... no!");
    static_assert(546 == test_map::at<40>::value, "mmmhh... no!");
    static_assert(-1 == test_map::at<347856>::value, "mmmhh... no!");
}
