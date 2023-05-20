#include "ul/map.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(CtMapTest, test) {
    // as the name says... this should be testable at compile time
    using ul::KV;
    // NOLINTBEGIN
    using TestMap = ul::CtMap<-1, KV<10, 11>, KV<20, 2002>, KV<30, 123>, KV<40, 546>>;

    static_assert(2002 == TestMap::At<20>::v, "mmmhh... no!");
    static_assert(11 == TestMap::At<10>::v, "mmmhh... no!");
    static_assert(123 == TestMap::At<30>::v, "mmmhh... no!");
    static_assert(546 == TestMap::At<40>::v, "mmmhh... no!");
    static_assert(-1 == TestMap::At<347856>::v, "mmmhh... no!");
    // NOLINTEND
}
