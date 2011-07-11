#include <gtest/gtest.h>
#include "woodcutter/woodcutter.h"

int main(int argc, char **argv)
{
	Woodcutter::minloglevel(ERROR);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
