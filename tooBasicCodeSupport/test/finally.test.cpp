#include "gtest/gtest.h"
#include "Toolib/finally.h"
#include "Toolib/PPDEFS.h"


TEST(FinallyTest, Test)
{
	bool isChanged = false;
	{
		const auto TOO_ANONYMOUS_VARIABLE = too::finally([&](){ isChanged = true; });
		isChanged = false;
	}
	EXPECT_TRUE(isChanged);
}
