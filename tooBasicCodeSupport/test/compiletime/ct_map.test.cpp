#include "gtest/gtest.h"
#include "Toolib/compiletime/ct_map.h"


TEST(too_ct_mapTest, test)
{
    using too::kv;
    using test_map = too::ct_map<-1, kv<10, 11>, kv<20, 2002>, kv<30, 123>, kv<40, 546>>;

    EXPECT_TRUE(2002 == test_map::at<20>::value);
    EXPECT_TRUE(11 == test_map::at<10>::value);
    EXPECT_TRUE(123 == test_map::at<30>::value);
    EXPECT_TRUE(546 == test_map::at<40>::value);

    EXPECT_TRUE(-1 == test_map::at<347856>::value);
}
