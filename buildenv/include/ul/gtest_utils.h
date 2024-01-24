#ifndef GTEST_UTILS_H_JKFNFYEYUFQWFGUYIE634YU
#define GTEST_UTILS_H_JKFNFYEYUFQWFGUYIE634YU

#include "ul/config.h"

#include "gtest/gtest.h"
#include <iostream>

namespace mb::ul {
inline void dump_test_name() {
    const auto& i{*testing::UnitTest::GetInstance()->current_test_info()};
    std::cout << i.test_suite_name() << '/' << i.name() << " test:\n";
}
} // namespace mb::ul

UL_HEADER_END

#endif
