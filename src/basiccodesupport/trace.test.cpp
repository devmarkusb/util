#include "ul/basiccodesupport/trace.hpp"
#include "gtest/gtest.h"
#include <sstream>

namespace ul = mb::ul;

TEST(traceTest, test) {
    std::ostringstream os;
    os << "#TEST# This is just testing the debug output window trace messaging.\n";
    ul::deprecated::trace(os);
}
