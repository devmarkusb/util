#include "Toolib/trace.h"
#include "gtest/gtest.h"


TEST(traceTest, test)
{
    std::ostringstream os;
    os << "#TEST# This is just testing the debug output window trace messaging.\n";
    too::deprecated::trace(os);
}
