#include "ul/trace.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(traceTest, test) {
    std::ostringstream os;
    os << "#TEST# This is just testing the debug output window trace messaging.\n";
    ul::deprecated::trace(os);
}
