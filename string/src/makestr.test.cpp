#include "ul/string/makestr.h"
#include "gtest/gtest.h"
#include <ios>
#include <string>

namespace ul = mb::ul;

TEST(makestrTest, test) {
    const std::string s = ul::str::Makestr() << "Hey, here is number one: " << 1 << ", and other stuff: " << 5.4 << 'X'
                                             << "\xc3\xa4 " << std::boolalpha << true;
    EXPECT_EQ("Hey, here is number one: 1, and other stuff: 5.4X\xc3\xa4 true", s);
}
