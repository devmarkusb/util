#include "mb/ul/basiccodesupport/type_name.hpp"
#include "gtest/gtest.h"
#include <tuple>

TEST(UL_COMPILETIME_TYPE_NAMETest, special) {
    [[maybe_unused]] auto what_type = std::make_tuple("bla", 42, 1.33); // NOLINT
    // uncomment for experimentation
    //UL_COMPILETIME_TYPE_NAME(what_type);
}
